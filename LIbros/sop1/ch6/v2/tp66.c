/*
   tp66.c Pipes. FIFO. Programa servidor que lee aquello que los clientes
   graban en un FIFO abierto a tal efecto
   Se agrega al servidor la posibilidad de hace poll con la lectura de las peticiones de
   los clientes, para permitir plantear un time out para la operacion read() y permitir
   una salida normal del proceso.
   Se agrega el manejo de las señales SIGPIPE para que el servidor reaccione ante un error en 
   el pipe y la señal SIGUSR2 para permitir una salida no anormal del servidor, poder cerrar
   el pipe y removerlo del sistema de archivos.
   Se agrega simulacion de total de ventas, al estilo de tp51.c
   Se agrega envio de respuesta a clientes a traves de un nuevo pipe cuyo nombre se forma
   con el id de proceso del proceso cliente que realizo la peticion
   Se re-estructura el servidor para una mejor codificacion: se crea libreria comun myfifo.c,
   alli pueden compartirse -entre otras- funciones tales como crearfifo()
   Se re-estrucutra funcion parse() y se mueve a biblioteca para ser compartida con cliente
   
      
   para compilar:
            gcc -Wall -o tp66 tp66.c myfifo.c
            
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
#include <time.h>

// requerido por poll para dar time out a operacion bloqueante de I/O
#include <stropts.h>
#include <poll.h>

#include "myfifo.h"

// datos globales
int salir=0;
double ventas[10];

// prototipo de funciones
static void sig_pipe(int);
static void sig_salir(int);
void init();
void proceso(char *sid,char *ssuc);
//void capturo

//
int main(int argc,char *argv[]) {
      int ret=0;
      printf("main():inicio servidor FIFO!\n");
      printf("main():para salir envie señal SIGUSR2 a proceso %d\n",(int) getpid());
      printf("main():inicializo total de ventas de sucursales\n");
      init();
      
      // capturar señal SIGPIPE
      if (signal(SIGPIPE,sig_pipe) == SIG_ERR) fprintf(stderr,"main():Error capturando señal SIGPIPE\n");

      // capturar señal SIGUSR2
      if (signal(SIGUSR2,sig_salir) == SIG_ERR) fprintf(stderr,"main():Error capturando señal SIGUSR2\n");
   
      if ( crearfifo(MY_FIFO) ) {
         printf("main():Tamaño maximo de operaciones atomicas sobre PIPES: %d\n",PIPE_BUF);
         printf("main():Tamaño maximo de buffer de servidor: %d\n",MAX_BUFFER);

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
                  parse(buffer,proceso);
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
         
         printf("main():cierro FIFO server [%s]\n",MY_FIFO);
         close(fdi);
         
         // elimino FIFO server
         printf("main():elimino FIFO server [%s]\n",MY_FIFO);
         remove(MY_FIFO);
         
      } else {
         fprintf(stderr,"main():Error creando FIFO %s\n",MY_FIFO);
         ret=1;
      }
      printf("main():fin servidor FIFO! retorno=%d\n",ret);
      return ret;
}

static void sig_pipe(int signo) {
   fprintf(stderr,"main():Error en PIPE, señal SIGPIPE capturada!\n");
   exit(2);
}

static void sig_salir(int signo) {
   salir=1;
   printf("main():recibi señal de salida\n");
}

void init() {
   srandom(time(0));
   int i;
   for(i=0;i<10;i++) ventas[i]=((double) random())/10000000.0;
}

// luego de parsing, proceso id de proceso, id de sucursal
void proceso(char *sid,char *ssuc) {
   int id=atoi(sid),suc=atoi(ssuc);
   char fifo[MAX_BUFFER],buffer[MAX_BUFFER];
   snprintf(fifo,MAX_BUFFER,"%s.%d",MY_FIFO,id);
   printf("proceso(): id proceso [%d] sucursal [%d] total $  %lf fifo:[%s]\n",id,suc,ventas[suc],fifo);
   snprintf(buffer,MAX_BUFFER,"<%d/%lf>",suc,ventas[suc]);
   int fdo = open(fifo,O_RDWR,0777);
   write(fdo,buffer,strlen(buffer));
   close(fdo);
}

