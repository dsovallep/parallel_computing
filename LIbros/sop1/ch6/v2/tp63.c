/*
   tp63.c Pipes. FIFO. Programa servidor que lee aquello que los clientes
   graban en un FIFO abierto a tal efecto
   Se agrega al servidor la posibilidad de hace poll con la lectura de las peticiones de
   los clientes, para permitir plantear un time out para la operacion read() y permitir
   una salida normal del proceso.
   
   para compilar:
            gcc -Wall -o tp63 tp63.c 
            
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
// requerido por poll para dar time out a operacion bloqueante de I/O
#include <stropts.h>
#include <poll.h>

#include "myfifo.h"
int salir=0;

int main(int argc,char *argv[]) {
      int ret=0;
      printf("main():inicio servidor FIFO!\n");
      
      int mf = mkfifo(MY_FIFO,O_CREAT);
      if ( mf == 0 || errno == EEXIST ) {
         chmod(MY_FIFO,0777);
         printf("main():Tama�o maximo de operaciones atomicas sobre PIPES: %d\n",PIPE_BUF);
         printf("main():Tama�o maximo de buffer de servidor: %d\n",MAX_BUFFER);

         int fdi = open(MY_FIFO,O_RDWR,0777);  // para no recibir EOF cuando se pasa de 1 cliente a 0 cliente

         // polling
         struct pollfd fdpoll[1];
         fdpoll[0].fd = fdi;
         fdpoll[0].events = POLLIN | POLLPRI;
         
         char buffer[MAX_BUFFER+1];
         int rpoll;
         while(!salir) {
            // printf("main():ingreso en poll()\n");  // trace
            rpoll = poll(fdpoll,1L,10000);  // time out de 10 segundos para read() de servidor FIFO
            // printf("main():fin poll()=%d\n",rpoll); // trace
            if ( rpoll == 1 ) { // ready to read
               //printf("main(): loop salir=%d entro en read()\n",salir);  // trace
               int nread = read(fdi,buffer,MAX_BUFFER);
               //printf("main():fin read()=%d errno=%d\n",nread,errno); // trace
               if ( nread != -1 ) {
                  buffer[nread]='\0';
                  printf("main():lei: [%s] de FIFO [%s]\n",buffer,MY_FIFO);
               } else {
                  printf("main():Error leyendo de FIFO [%s]\n",MY_FIFO);
               }
            } else { // timeout o error en poll
               if ( rpoll == -1 &&  !salir ) { // poll dio error y parece no haber sido interrumpido por SIGUSR2
                  fprintf(stderr,"main():Error esperando leer de FIFO!\n");
                  salir=1; 
               }
            }
            
         } // while (!salir)
         
         close(fdi);
         
         // elimino FIFO
         remove(MY_FIFO);
         
      } else {
         fprintf(stderr,"main():Error creando FIFO %s\n",MY_FIFO);
         ret=1;
      }
      printf("main():fin servidor FIFO! retorno=%d\n",ret);
      return ret;
}


