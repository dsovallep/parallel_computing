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

typedef struct velocidad {
   char *nombre;
   int valor;
} velocidad;

void printVel(); // imprime tabla de velocidades
int findVel(char *strVel); // busca strVel en tabla de velocidades
void setParidad(struct termios *t,int par); // paridad=0,1,2 (0 no paridad, 1 paridad impar, 2 paridad par)
void setDataBits(struct termios *t,int bits); // bits=5,6,7,8 (nro. de bits por byte)
void setTimeOut(struct termios *t,int sec); // nro.de segundos de time out en input/output (0 indica no time out)
void setVel(struct termios *t,char *velocidad);  // <velocidad>=<baudios>
void setStopBits(struct termios *t,int bits); // <stop bits>=1,2 (nro. de bits de stop)
void usage(char *prog); // forma de uso
