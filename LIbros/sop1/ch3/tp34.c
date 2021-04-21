/*
   tp34.c programa que crea n threads indicado por teclado
   para compilar:
            gcc -Wall -o tp34 tp34.c -pthread   

   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <pthread.h>
#include <unistd.h>  // funciones unix (sleep,read(), write(), etc)
#include <stdio.h>
#include <stdlib.h>

// prototipo de funciones
void *imprimo(void *threadid);
void uso();

int main (int argc, char *argv[])
{
   if ( argc != 2 ) { uso();exit(-2); }
   int nthreads = atoi(argv[1]);
   if (nthreads > 0) {
      pthread_t threads[nthreads];
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
      int rc, t, status;
      for(t=0; t<nthreads; t++){
         printf("main(): creando thread %d\n", t);
         rc = pthread_create(&threads[t], &attr, imprimo, (void *)  t);
         if (rc){ 
            printf("main(): Error, pthread_create() retorna %d\n", rc);
            exit(-1);
         }
      }
      
      // libero recurso que ya no necesito
      pthread_attr_destroy(&attr);
      
      // hago joins
      for(t=0; t<nthreads; t++){
         printf("main(): haciendo join con thread %d\n", t);
         rc = pthread_join(threads[t],(void **) &status);
         if (rc){ 
            printf("main(): Error, pthread_join() retorna %d\n", rc);
            exit(-3);
         }
         printf("main(): termino join con thread %d status= %d\n",t, status);
      }

   } else printf("Debe ingresar un número mayor que cero\n");
   printf("fin main():1\n");
   pthread_exit(NULL);
   printf("fin main():2\n");
}

//============================================================
//                FUNCIONES
//============================================================

void *imprimo(void *threadid)
{
   int tid;
   tid = (int) threadid;
   printf("Mensaje de thread %d!\n", tid);
   sleep(3);
   printf("fin Mensaje de thread %d!\n",tid);
   pthread_exit((void *) (tid+1));
}

void uso() {
   printf("Forma de Uso:\nthr1 [numero de threads]");
}
