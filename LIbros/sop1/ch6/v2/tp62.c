/*
   tp62.c Pipes. FIFO. Programa cliente que envia peticion a traves de un FIFO
   previamente creado por el servidor y conocido por todos los clientes
   MAX_BUFFER debe ser menor que PIPE_BUF para que los envios no se intercalen
   con los de otros clientes, puesto que dejarian de ser operaciones atomicas
   
   para compilar:
            gcc -Wall -o tp62 tp62.c 
            
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

      int fdo = open(MY_FIFO,O_WRONLY,0777);
      char buffer[MAX_BUFFER+1];
      int suc;
      for(suc=0;suc<10;suc++) { // solicito total ventas de sucursal suc
         snprintf(buffer,MAX_BUFFER,"<%d/%d>",(int) getpid(),suc); 
         int nwrite = write(fdo,buffer,strlen(buffer));
         printf("main():envie %d bytes a servidor [%s]\n",nwrite,buffer);
      }
      close(fdo);
      printf("main():fin cliente FIFO!\n");
      return 0;
}

static void sig_pipe(int signo) {
   fprintf(stderr,"main():Error en PIPE, señal SIGPIPE capturada!\n");
   exit(2);
}
