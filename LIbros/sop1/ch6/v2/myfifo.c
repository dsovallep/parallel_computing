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
   //int iid,isuc;
   char *p;
   while(*buffer) {
      memset(id,0,MAX_BUFFER);
      memset(suc,0,MAX_BUFFER);
      while( *buffer && *buffer != '<') buffer++;
      if (  *buffer == '<' ) buffer++;
      p=id;
      while( *buffer && *buffer != '/') *p++=*buffer++;
      if (  *buffer == '/' ) buffer++;
      p=suc;
      while( *buffer && *buffer != '>') *p++=*buffer++;
      if (  *buffer == '>' ) buffer++;
      // final de parsing, ¿que obtuve?
      printf("parse(): campo1 [%s] campo2 [%s]\n",id,suc);
      //iid=atoi(id);
      //isuc=atoi(suc);
      //printf("parse(): id proceso [%d] sucursal [%d]\n",iid,isuc);
      (*proceso)(id,suc);
   }
  
}

