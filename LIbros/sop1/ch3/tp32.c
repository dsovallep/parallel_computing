/*
   tp32.c programa que crea n threads indicado por teclado
   para compilar:
            gcc -Wall -o tp32 tp32.c -pthread   

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
      int rc, t;
      for(t=0; t<nthreads; t++){
         printf("main(): creando thread %d\n", t);
         rc = pthread_create(&threads[t], NULL, imprimo, (void *)  t);
         if (rc){ 
            printf("main(): Error, pthread_create() retorna %d\n", rc);
            exit(-1);
         }
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
   pthread_exit(NULL);
}

void uso() {
   printf("Forma de Uso:\nthr1 [numero de threads]");
}
