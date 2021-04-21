/*
   tp55.c Pipes. Programa que hace fork(), exec(), waitpid(), pipe() del proceso tp51
   Actuara como padre de tp51 para luego hacer un pipe con su hijo
   Agrega loop para solicitar total de ventas de 10 sucursales
   para compilar:
            gcc -Wall -o tp54 tp54.c 
            
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

#define MAX_BUFFER 255
void ejecutoHijo(int fd1[],int fd2[],char *prog);
void pidoVentas(char *buffer,int suc,int input,int output,pid_t pid);
static void sig_pipe(int);

int main(int argc,char *argv[]) {
   int fd1[2],fd2[2],nsuc;
   char buffer[MAX_BUFFER];
   pid_t pid;
   // capturar se�al SIGPIPE
   if (signal(SIGPIPE,sig_pipe) == SIG_ERR) fprintf(stderr,"main():Error capturando se�al SIGPIPE\n");
   
   for(nsuc=0;nsuc<10;nsuc++) {
      // crear pipes
      if ( pipe(fd1) < 0 || pipe(fd2) < 0 ) {
         fprintf(stderr,"main():Error creando pipes!\n");
         exit(1);
      }
      //printf("main(): hago fork()!\n");
      pid = fork();
      switch(pid) {
         case -1:
            printf("main():Error, en fork()!\n");
            break;
         case 0: // hijo
            //printf("hijo!\n");
            ejecutoHijo(fd1,fd2,"./tp51");
            break;
         default: // padre
            close(fd1[0]);
            close(fd2[1]);
            pidoVentas(buffer,nsuc,fd1[1],fd2[0],pid);
            close(fd1[1]);
            close(fd2[0]);
            // emito salida con el dato requerido
            write(STDOUT_FILENO,buffer,strlen(buffer));
            break;
      }
   }
   return 0;
}

void ejecutoHijo(int fd1[],int fd2[],char *prg) {
   close(fd1[1]);
   close(fd2[0]);
   if (fd1[0] != STDIN_FILENO) {
      if ( dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) fprintf(stderr,"ejecutoHijo():dup2 error en stdin\n");
      close(fd1[0]);
   }
   if (fd2[1] != STDOUT_FILENO) {
      if ( dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) fprintf(stderr,"ejecutoHijo():dup2 error en stdout\n");
      close(fd2[1]);
   }
   char *prog[] = {prg,prg,NULL};
   if (execvp(prog[0], prog) == -1) {
      printf("ejecutoHijo():Error en execvp()\n"); 
   }
}

void pidoVentas(char *buffer,int suc,int input,int output,pid_t pid) {
   snprintf(buffer,MAX_BUFFER,"%d\n",suc);
   int status;
   int l = strlen(buffer);
   int n = write(input,buffer,l);
   if ( n != l ) printf("pidoVentas:grabe %d y esperaba grabar %d en suc=%d\n",n,l,suc);
   n = read(output,buffer,MAX_BUFFER);
   buffer[n]='\0';
   //printf("main():lei de prog. hijo: [%s]",buffer);
   //printf("padre esperando hijo!\n");
   waitpid(pid, &status,WUNTRACED);
   //printf("fin padre esperando hijo!\n");
}

static void sig_pipe(int signo) {
   fprintf(stderr,"main():Error en PIPE, se�al SIGPIPE capturada!\n");
   exit(2);
}

