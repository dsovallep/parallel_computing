/*
   tp35.c programa que crea n threads con exclusión mutua
   para compilar:
            gcc -Wall -o tp35 tp35.c -pthread   

   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <pthread.h>
#include <unistd.h>  // funciones unix (sleep,read(), write(), etc)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_SUC 5

// datos de ventas de las sucursales
double mivta[NUM_OF_SUC][31]; // se asume un maximo de 31 dias para el mes en curso
double totalgral=0.0;
double totalsuc[NUM_OF_SUC];

// prototipo de funciones
void *sumoVentas(void *);
void *cargoVentas(void *);

// mutex para implementar exclusión mutua entre carga y suma
pthread_mutex_t cargada_mutex;

int main (int argc, char *argv[])
{
      int rc, t, status;
      memset(&mivta,0,sizeof(double)*NUM_OF_SUC*31);
      memset(&totalsuc,0,sizeof(double)*NUM_OF_SUC);

      pthread_t threads[NUM_OF_SUC];
      pthread_t thread_cargo;
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      pthread_mutex_init(&cargada_mutex, NULL);

      printf("main(): creando thread para carga de ventas\n");
      rc = pthread_create(&thread_cargo,NULL,cargoVentas,NULL);
         if (rc){ 
            printf("main(): Error, pthread_create() retorna %d\n", rc);
            exit(-2);
         }

      for(t=0; t<NUM_OF_SUC; t++){
         printf("main(): creando thread %d\n", t);
         rc = pthread_create(&threads[t], &attr, sumoVentas, (void *)  t);
         if (rc){ 
            printf("main(): Error, pthread_create() retorna %d\n", rc);
            exit(-1);
         }
      }
      
      // libero recurso que ya no necesito
      pthread_attr_destroy(&attr);
      
      // hago joins
      for(t=0; t<NUM_OF_SUC; t++){
         printf("main(): haciendo join con thread %d\n", t);
         rc = pthread_join(threads[t],(void **) &status);
         if (rc){ 
            printf("main(): Error, pthread_join() retorna %d\n", rc);
            exit(-3);
         }
         printf("main(): termino join con thread %d status= %d\n",t, status);
      }

      // hago total gral
      for(t=0; t<NUM_OF_SUC; t++) totalgral+=totalsuc[t];
      printf("main():Total Gral $ %12.2f\n",totalgral);

      // libero recurso que ya no necesito
      pthread_mutex_destroy(&cargada_mutex);
      
      printf("fin main():1\n");
   pthread_exit(NULL);
}

//============================================================
//                FUNCIONES
//============================================================
void *sumoVentas(void *sucursalid) {
   int sucid;
   sucid = (int) sucursalid;
   printf("thread %d esperando carga de sucursal %d \n", sucid,sucid);
   pthread_mutex_lock(&cargada_mutex);
   printf("thread %d sumoVentas sucursal %d \n", sucid,sucid);
   totalsuc[sucid]=0.0;
   int d;
   for(d=0;d<31;d++) {
      totalsuc[sucid]+=mivta[sucid][d];
   }
   printf("fin thread %d sumatoria de sucursal %d  $%10.2f\n",sucid,sucid,totalsuc[sucid]);
   pthread_mutex_unlock(&cargada_mutex);
   pthread_exit((void *) (sucid+1));
}

void *cargoVentas(void *p) {
   int s,d;
   printf("thread cargoVentas\n");
   pthread_mutex_lock(&cargada_mutex);
   for(s=0;s<NUM_OF_SUC;s++) {
      for(d=0;d<31;d++) {
         mivta[s][d] = ((double) random())/10000000.0;
      }
      sleep(4);
   }
   pthread_mutex_unlock(&cargada_mutex);
   printf("fin thread cargoVentas\n");
   pthread_exit(NULL);
}

