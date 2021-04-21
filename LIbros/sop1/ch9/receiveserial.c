/*
     receiveserial.c  Non-Canonical File transfer por puerto serial
            configuracion dinamica de puerto serie, open(), close(), lectura buffered fread(), fclose(), etc.
            TP integracion con asignatura Comunicacion
      compilacion:
         gcc -Wall -o receiveserial receiveserial.c serial.c
     UTN FRD
     SO
     Guillermo Cherencio
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include "serial.h"

//#define _POSIX_SOURCE 1 /* POSIX compliant source */

extern velocidad vel[];

int main(int argc,char *argv[]) {

   if ( argc != 9 ) {
      usage(argv[0]);
      exit(1);
   }

   int fd;
   size_t nread,nwrite=0;
   struct termios oldtio,newtio;
   FILE *fpo = fopen(argv[1],"wb");
   if ( fpo == NULL ) { 
      perror(argv[1]);
      exit(-2); 
   }
   char buf[255];
   fd = open(argv[2], O_RDONLY | O_NOCTTY );
   if (fd <0) { 
      perror(argv[2]); 
      exit(-1); 
   }
   // guardar estado actual del puerto
   tcgetattr(fd,&oldtio); /* save current port settings */
   //bzero(&newtio, sizeof(newtio));
   tcgetattr(fd,&newtio); /* save current port settings */
   newtio.c_cflag = CLOCAL | CREAD;

   // seteo paridad
   setParidad(&newtio,atoi(argv[5]));

   // bits por byte
   setDataBits(&newtio,atoi(argv[6]));
   
   /* set input mode (non-canonical, no echo,...) */
   newtio.c_iflag = INPCK | PARMRK | ISTRIP;
   //newtio.c_oflag = 0;
   newtio.c_lflag &= ~(ICANON|ECHO);
   //newtio.c_lflag = 0;

   // set time out
   setTimeOut(&newtio,atoi(argv[4]));
   
   // seteo velocidad
   setVel(&newtio,argv[3]);

   // seteo bits de stop
   setStopBits(&newtio,atoi(argv[7]));
   
   newtio.c_cc[VMIN] = 0; /* blocking read until 5 chars received */

   // paso los nuevos parametros al file descriptor del puerto
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);

   // demora
   int sdelay = atoi(argv[8]);
   if ( sdelay < 0 ) sdelay =0;
   
   // leo puerto y grabo archivo
   while((nread=read(fd,buf,255))) { nwrite+=fwrite(buf,1,nread,fpo);sleep(sdelay); }

   // restablezco el seteo original del puerto
   tcsetattr(fd,TCSANOW,&oldtio);
   
   // cierro archivo y puerto
   close(fd);
   fclose(fpo);
   
   printf("Se recibieron %d bytes por dispositivo %s\ngrabados en %s",nwrite,argv[2],argv[1]);
   return 0;
}

