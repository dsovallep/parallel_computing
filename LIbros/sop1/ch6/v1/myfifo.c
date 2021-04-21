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

