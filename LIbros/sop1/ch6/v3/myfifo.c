/*
   myfifo.c Pipes. FIFO. Libreria comun utilizada por cliente y servidor FIFO
   
            
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

// crea pipe, devuelve true si pudo hacerlo, caso contrario devuelve false
int crearfifo(char *pipe) {
      int mf = mkfifo(pipe,O_CREAT);
      if ( mf == 0 || errno == EEXIST ) {
         chmod(pipe,0777);
         return 1;
      } else return 0;
}

// analiza buffer recibido de los clientes para determinar id de proceso cliente y nro de sucursal solicitada
// formato de buffer: "<campo1/campo2><campo1/campo2>....<campo1/campo2>"
void parse(char *buffer,void (*proceso)(char *campo1,char *campo2)) {
   char id[MAX_BUFFER];
   char suc[MAX_BUFFER];
   while(*buffer) {
      buffer=strcpyentre(id,buffer,'<','/');
      buffer=strcpyentre(suc,buffer,'/','>');
      if ( *id && *suc ) { // algo obtuve
         // final de parsing, ¿que obtuve?
         //printf("parse(): campo1 [%s] campo2 [%s]\n",id,suc);  // trace
         (*proceso)(id,suc);
      }
   }
}

// to es un cadena que se encuentra dentro de from y esta delimitada por dos caracteres: desde y hasta
// si no encuentra la subcadena, devuelve una cadena vacia (inicializada a NULL
// to es una cadena que tiene previamente asignado -al menos- strlen(from)+1 caracteres.
// devuelve el desplazamiento realizado en from durante el parsing 
char *strcpyentre(char *to,char *from,char desde,char hasta) {
   memset(to,0,strlen(from)+1);
   while( *from && *from != desde) from++;
   if (  *from == desde ) from++;
   while( *from && *from != hasta) *to++=*from++;
   return from;
}

