/*
   tp67.c Pipes. FIFO. Programa cliente que envia peticion a traves de un FIFO
   previamente creado por el servidor y conocido por todos los clientes
   MAX_BUFFER debe ser menor que PIPE_BUF para que los envios no se intercalen
   con los de otros clientes, puesto que dejarian de ser operaciones atomicas
   Leo respuesta de servidor a traves de pipes, previamente creado usando el id de proceso cliente
   Se re-estructura el cliente para una mejor codificacion: se crea libreria comun myfifo.c,
   alli pueden compartirse -entre otras- funciones tales como crearfifo()
   
   para compilar:
            gcc -Wall -o tp67 tp67.c myfifo.c
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include	<fcntl.h>
#include	<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

#include "myfifo.h"

static void sig_pipe(int);

int main(int argc,char *argv[]) {
      printf("main():inicio cliente FIFO!\n");
      
      // capturar señal SIGPIPE
      if (signal(SIGPIPE,sig_pipe) == SIG_ERR) fprintf(stderr,"main():Error capturando señal SIGPIPE\n");

      int pid=(int) getpid();
      char buffer[MAX_BUFFER+1],fifo[MAX_BUFFER];
      snprintf(fifo,MAX_BUFFER,"%s.%d",MY_FIFO,pid);
      printf("main():creando FIFO [%s]\n",fifo);
      if ( crearfifo(fifo) ) {
         printf("main():abro FIFO [%s] para grabar\n",MY_FIFO);
         int fdo = open(MY_FIFO,O_WRONLY,0777);
         int suc,nwrite,nread;
         printf("main():abro FIFO [%s] para leer y grabar\n",fifo);
         int fdi = open(fifo,O_RDWR,0777);   // para no recibir EOF cuando server cierra archivo
         for(suc=0;suc<10;suc++) { // solicito total ventas de sucursal suc
            snprintf(buffer,MAX_BUFFER,"<%d/%d>",pid,suc); 
            nwrite = write(fdo,buffer,strlen(buffer));
            printf("main():envie %d bytes a servidor [%s]\n",nwrite,buffer);
            nread = read(fdi,buffer,MAX_BUFFER);
            buffer[nread]='\0';
            printf("main():recibi %d bytes de servidor [%s]\n",nread,buffer);
         }
         printf("main():cierro FIFO's\n");
         close(fdo);
         close(fdi);

         // elimino FIFO cliente
         printf("main():elimino FIFO cliente [%s]\n",fifo);
         remove(fifo);
         
      } else {
         fprintf(stderr,"main():Error creando FIFO [%s]\n",fifo);
      }
      printf("main():fin cliente FIFO!\n");
      return 0;
}

static void sig_pipe(int signo) {
   fprintf(stderr,"main():Error en PIPE, señal SIGPIPE capturada!\n");
   exit(2);
}
