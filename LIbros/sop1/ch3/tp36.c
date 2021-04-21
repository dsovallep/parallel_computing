/*
   tp36.c programa que crea n threads con exclusión mutua y variables de condicion
   para compilar:
            gcc -Wall -o tp36 tp36.c -pthread   

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
//

double mivta[NUM_OF_SUC][31];
double totalgral=0.0;
double totalsuc[NUM_OF_SUC];
int cargadas[NUM_OF_SUC];

// prototipo de funciones
void *sumoVentas(void *);
void *cargoVentas(void *);

pthread_mutex_t cargada_mutex[NUM_OF_SUC];
pthread_cond_t cargada_cv[NUM_OF_SUC];

int main (int argc, char *argv[])
{
      int rc, t, status;
      memset(&mivta,0,sizeof(double)*NUM_OF_SUC*31);
      memset(&totalsuc,0,sizeof(double)*NUM_OF_SUC);
      memset(&cargadas,0,sizeof(int)*NUM_OF_SUC);

      pthread_t threads[NUM_OF_SUC];
      pthread_t thread_cargo;
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      for(t=0; t<NUM_OF_SUC; t++){
         pthread_mutex_init(&cargada_mutex[t], NULL);
         pthread_cond_init (&cargada_cv[t], NULL);
      }

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

      printf("main(): haciendo join con carga de ventas\n");
      rc = pthread_join(thread_cargo,(void **) &status);
      if (rc){ 
         printf("main(): Error, pthread_join() retorna %d\n", rc);
         exit(-4);
      }
      printf("main(): termino join con carga de ventas status= %d\n", status);
      
      // hago total gral
      for(t=0; t<NUM_OF_SUC; t++) totalgral+=totalsuc[t];
      printf("main():Total Gral $ %12.2f\n",totalgral);

      // libero recurso que ya no necesito
      for(t=0; t<NUM_OF_SUC; t++){
         pthread_mutex_destroy(&cargada_mutex[t]);
         pthread_cond_destroy(&cargada_cv[t]);
      }
      
      printf("fin main():1\n");
   pthread_exit(NULL);
}

//============================================================
//                FUNCIONES
//============================================================

void *sumoVentas(void *threadid)
{
   int tid;
   tid = (int) threadid;
   printf("thread %d esperando carga de sucursal %d \n", tid,tid);
   pthread_mutex_lock(&cargada_mutex[tid]);
   if ( !cargadas[tid] ) {
      printf("thread %d esperando condicion de cargada para sucursal %d \n", tid,tid);
      pthread_cond_wait(&cargada_cv[tid], &cargada_mutex[tid]);
   }
   printf("thread %d sumo Ventas sucursal %d \n", tid,tid);
   totalsuc[tid]=0.0;
   int d;
   for(d=0;d<31;d++) {
      totalsuc[tid]+=mivta[tid][d];
   }
   printf("fin thread %d sumatoria de sucursal %d  $%10.2f\n",tid,tid,totalsuc[tid]);
   pthread_mutex_unlock(&cargada_mutex[tid]);
   pthread_exit((void *) (tid+1));
}

void *cargoVentas(void *p) {
   int s,d;
   printf("thread cargoVentas\n");
   for(s=0;s<NUM_OF_SUC;s++) {
      pthread_mutex_lock(&cargada_mutex[s]);
      for(d=0;d<31;d++) {
         mivta[s][d] = ((double) random())/10000000.0;
      }
      cargadas[s]=1;
      pthread_cond_signal(&cargada_cv[s]);
      pthread_mutex_unlock(&cargada_mutex[s]);
      sleep(4);
   }
   printf("fin thread cargoVentas\n");
   pthread_exit(NULL);
}

