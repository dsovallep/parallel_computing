/*
   tp52.c Pipes. Programa que hace fork(), exec(), waitpid() del proceso tp51
   Actuara como padre de tp51 para luego hacer un pipe con su hijo
   para compilar:
            gcc -Wall -o tp52 tp52.c 
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include	<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc,char *argv[]) {
   int status;
   pid_t pid;
   printf("main(): hago fork()!\n");
   pid = fork();
   switch(pid) {
      case -1:
         printf("main():Error, en fork()!\n");
         break;
      case 0: // hijo
         printf("hijo!\n");
         break;
      default: // padre
printf("padre esperando hijo!\n");
            waitpid(pid, &status,WUNTRACED);
printf("fin padre esperando hijo!\n");
         break;
   }
   return 0;
}

