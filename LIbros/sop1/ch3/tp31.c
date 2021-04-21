/*
   tp31.c programa que crea n threads indicado por teclado
   para compilar:
            gcc -Wall -o tp31 tp31.c -pthread   
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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
         rc = pthread_create(&threads[t], NULL, imprimo,  (void *) t);
         if (rc){ 
            printf("main(): Error, pthread_create() retorna %d\n", rc);
            exit(-1);
         }
      }
   } else printf("Debe ingresar un número mayor que cero\n");
   printf("main():fin!\n");
   pthread_exit(NULL);
}

//============================================================
//                FUNCIONES
//============================================================

void *imprimo(void *threadid)
{
   int tid;
   tid = (int ) threadid;
   sleep(3);
   printf("Mensaje de thread %d!\n", tid);
   pthread_exit(NULL);
}

void uso() {
   printf("Forma de Uso:\nthr1 [numero de threads]\n");
}
