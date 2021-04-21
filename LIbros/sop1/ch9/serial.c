/*
   Libreria serial.c

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

// dato global de biblioteca
velocidad vel[] = { {"0",B0},{"50",B50},{"75",B75},{"110",B110},{"134",B134},{"150",B150},{"200",B200},{"300",B300},
                    {"600",B600},{"1200",B1200},{"1800",B1800},{"2400",B2400},{"4800",B4800},{"9600",B9600},
                    {"19200",B19200},{"38400",B38400},{"57600",B57600},{"115200",B115200},
                    {"230400",B230400}, { '\0',0 } };


// imprime tabla de velocidades
void printVel() {
   velocidad *v = &vel[0];
   printf("Velocidades Soportadas:\n");
   while(v->nombre) {
      printf("%s \t\tbaudios\n",v->nombre);
      v++;
   }
}

// busca strVel en tabla de velocidades, 
//    si no esta devuelve -1
//    si esta devuelve su posicion
int findVel(char *strVel) {
   velocidad *v = &vel[0];
   while(v->nombre) {
      if (strcmp(v->nombre,strVel) == 0) return v - &vel[0];
      v++;
   }
   return -1;
}

// paridad=0,1,2 (0 no paridad, 1 paridad impar, 2 paridad par)
void setParidad(struct termios *t,int paridad) {
   // seteo paridad
   if (!paridad) {
      //PARENB significa habilidar bit de paridad
      //entonces, esto deshabilita el bit de paridad:
      t->c_cflag &= ~PARENB;
   } else { // habilito paridad
      t->c_cflag |= PARENB;
   }
   if ( paridad == 1 ) { // paridad impar
      //PARODD habilita paridad impar
      t->c_cflag |= PARODD;
   } else { // paridad par, implica no paridad impar
      t->c_cflag &= ~PARODD;
   }
}

// bits=5,6,7,8 (nro. de bits por byte)
void setDataBits(struct termios *t,int bits) {
   //CSIZE es una mascara para todos los tamaños de datos en bits,
   //entonces, haciendo un and con su negacion, elimina el
   //seteo actual en cuanto al tamaño de datos en bits
   t->c_cflag &= ~CSIZE;
   switch(bits) {
      case 5:
         t->c_cflag |= CS5;
         break;
      case 6:
         t->c_cflag |= CS6;
         break;
      case 7:
         t->c_cflag |= CS7;
         break;
      case 8:
         t->c_cflag |= CS8;
         break;
      default:
         t->c_cflag |= CS8;
   }
}

// nro.de segundos de time out en input/output (0 indica no time out)
void setTimeOut(struct termios *t,int sec) {
      t->c_cc[VTIME] = sec*10; /* inter-character timer unused */
}

// <velocidad>=<baudios>
void setVel(struct termios *t,char *velocidad) {
   int vpos = findVel(velocidad);
   int ret;
   if ( vpos != -1) {
      t->c_cflag |= vel[vpos].valor;
      ret=cfsetispeed(t, vel[vpos].valor);
      ret=cfsetospeed(t, vel[vpos].valor);
   } else printf("%s baudios no es una velocidad correcta!\n",velocidad);
}

// <stop bits>=1,2 (nro. de bits de stop)
void setStopBits(struct termios *t,int stopbits) {
   //CSTOPB indica 2 bits de stop, caso contrario es solo 1 bit de stop
   if ( stopbits == 2 ) t->c_cflag |= CSTOPB;
   else                       t->c_cflag &= ~CSTOPB;
}

void usage(char *prog) {
   //                   0            1             2            3                    4                 5                    6                     7
   printf("%s <archivo> <disp> <velocidad> <time out> <paridad> <bits por byte> <stop bits> <sec delay>\n",prog);
   printf("Donde:\n");
   printf("<archivo>=ubicacion y nombre de archivo a enviar (./mifile.txt)\n");
   printf("<disp>=/dev/ttyS0 (com1) /dev/ttyS1 (com2) /dev/ttyUSB0 (conversor usb serie-1)\n");
   printf("<velocidad>=<baudios>\n");
   printVel();
   printf("<time out>=nro.de segundos de time out en input/output (0 indica no time out)\n");
   printf("<paridad>=0,1,2 (0 no paridad, 1 paridad impar, 2 paridad par)\n");
   printf("<bits por byte>=5,6,7,8 (nro. de bits por byte)\n");
   printf("<stop bits>=1,2 (nro. de bits de stop)\n");
   printf("<sec delay>=0,1,2... (segundos de demora por envio/recepcion)\n");
}

