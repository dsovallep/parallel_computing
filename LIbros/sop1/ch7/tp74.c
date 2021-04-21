/*
   tp72.c IPC. Cola de mensajes. Message Queue. Programa cliente que envia peticiones de sucursales
   al servidor.  Las peticiones son mensajes de tipo 1.
   El mensaje contiene el id de proceso del cliente y el nro de sucursal de la cual desea saber
   el total de ventas. Formato del mensaje: <id proceso/nro.sucursal>
   El servidor responde a los clientes con un mensaje de tipo=id de proceso del cliente y el 
   formato del mensaje de respuesta es: <nro.sucursal/total ventas>
         
   para compilar:
            gcc -Wall -o tp72 tp72.c myipc.c
            
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
#include <sys/msg.h> 
#include <signal.h> 

#include "myipc.h"

#define SUC_MSG_LEN 20 

struct sucmsg {
   long mtype; 
   char mtext[SUC_MSG_LEN]; 
};

int main(int argc, char *argv[])  {
   int ret=0;
   key_t key; 
   int idcola; 
   struct sucmsg qbuf; 

   printf("main():inicio cliente Cola de Mensajes!\n");
   /* Create unique key via call to ftok() */ 
   printf("main():solicito clave ipc\n");
   key = obtener_clave(IPC_PATH, IPC_KEY); 

   /* Open the queue -create if necessary */ 
   printf("main():obtengo cola de mensajes con clave ipc\n");
   if ((idcola = obtener_cola(key)) != -1) { 
      int suc;
      int pid = (int) getpid();
      for(suc=0;suc<10;suc++) {
         snprintf(qbuf.mtext,SUC_MSG_LEN,"<%d/%d>",pid,suc);
         // solicito sucursal suc, mensajes de tipo 1
         if ( enviar_msg(idcola,&qbuf,SUC_MSG_LEN,1L) != -1 ) {
            printf("main():envie [%s] Ok, ahora espero respuesta\n",qbuf.mtext);
            if ( recibir_msg(idcola,&qbuf,SUC_MSG_LEN,(long) pid) != -1 ) {
               printf("main():recibi [%s] del servidor de cola de mensajes\n",qbuf.mtext);
            }
         }
      }
      printf("main():fin envio de mensajes a cola [%d]\n",idcola);
   } else {
      fprintf(stderr,"main():Error obteniendo cola de mensajes\n"); 
      ret=1;
   } 
   printf("main():fin cliente Cola de Mensajes! retorno=%d\n",ret);
   return ret;
}

