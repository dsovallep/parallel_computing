/*
   tp82.c IPC. Memoria compartida. Proceso que lee de memoria compartida el total de
   ventas de las sucursales. 
         
   para compilar:
            gcc -Wall -o tp82 tp82.c myipc.c
            
   Guillermo Cherencio
   SO
   UTN FRD
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <ctype.h> 
#include	<fcntl.h>
#include	<unistd.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <signal.h> 

#include "myipc.h"

// prototipo de funciones

int main(int argc, char *argv[])  {
   int ret=0;
   key_t key; 
   int idmem; 
   double *pventas;

   printf("main():inicio proceso consumidor!\n");
   
   /* Create unique key via call to ftok() */ 
   printf("main():solicito clave ipc\n");
   key = obtener_clave(IPC_PATH, IPC_MKEY); 

   /* Open the queue -create if necessary */ 
   int modo = SHM_R;
   printf("main():obtengo memoria compartida\n");
   if ((idmem = obtener_memoria(key,modo)) != -1) { 
      printf("main():atachar memoria local a memoria compartida %d\n",idmem);
      if ((pventas = (double *) conectar_memoria(idmem,modo)) != (void *) -1) {
         printf("main():leo memoria compartida\n");
         mostrar_ventas(pventas);
         printf("main():Desconecto memoria compartida\n");
         desconectar_memoria(pventas);
      } else {
         printf("main():Error atachando memoria compartida\n");
         ret=2;
      }
   } else {
      fprintf(stderr,"main():Error obteniendo memoria compartida\n"); 
      ret=1;
   } 
   printf("main():fin proceso consumidor! retorno=%d\n",ret);
   return ret;
} 

