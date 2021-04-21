/*
     sendserial.c  Non-Canonical File transfer por puerto serial
            configuracion dinamica de puerto serie, open(), close(), lectura buffered fread(), fclose(), etc.
            TP integracion con asignatura Comunicacion
      compilacion:
         gcc -Wall -o sendserial sendserial.c serial.c
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
   FILE *fpi = fopen(argv[1],"rb");
   if ( fpi == NULL ) { 
      perror(argv[1]);
      exit(-2); 
   }
   char buf[255];
   fd = open(argv[2], O_WRONLY | O_NOCTTY );
   if (fd <0) { 
      perror(argv[2]); 
      exit(-1); 
   }
   // guardar estado actual del puerto
   tcgetattr(fd,&oldtio); 
   //bzero(&newtio, sizeof(newtio));
   tcgetattr(fd,&newtio); 
   newtio.c_cflag = CLOCAL | CREAD; // CRTSCTS

   // seteo paridad
   setParidad(&newtio,atoi(argv[5]));

   // bits por byte
   setDataBits(&newtio,atoi(argv[6]));

   // set input mode (non-canonical, no echo,...)
   newtio.c_oflag = 0;
   //newtio.c_lflag = 0;
   newtio.c_lflag &= ~(ICANON|ECHO);

   
   // set time out
   setTimeOut(&newtio,atoi(argv[4]));
   
   // seteo velocidad
   setVel(&newtio,argv[3]);

   // seteo bits de stop
   setStopBits(&newtio,atoi(argv[7]));

   // paso los nuevos parametros al file descriptor del puerto
   tcflush(fd, TCOFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);

   // demora
   int sdelay = atoi(argv[8]);
   if ( sdelay < 0 ) sdelay =0;

   // leo archivo y envio a puerto serie
   while((nread=fread(buf,1,255,fpi))) {nwrite+=write(fd,buf,nread);sleep(sdelay);}

   tcflush(fd, TCOFLUSH);

   // restablezco el seteo original del puerto
   // atencion no usar TCSANOW, afecta a la recepcion de los ultimos caracteres
   //tcsetattr(fd,TCSADRAIN,&oldtio);  
   
   // cierro archivo y puerto
   close(fd);
   fclose(fpi);
   
   printf("Se enviaron %d bytes por dispositivo %s\n",nwrite,argv[2]);
   return 0;
}

