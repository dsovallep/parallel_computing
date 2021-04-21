/*
   tp44.c programa que captura la señal SIGALRM utilizada con la funcion alarm() para enviar
   señal a otro proceso (tp43) para obtener el calculo de los totales de las sucursales cada 1 minuto.
   
   para compilar:
            gcc -Wall -o tp44 tp44.c 
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include	<signal.h>
#include	<unistd.h>
#include <string.h>

void usage();
void sig_usr(int signo);
void sig_alrm(int signo);
int salir=0;
int idproc=0;

int main(int argc,char *argv[]) {
   
   if ( argc != 2 ) {
      usage();
      exit(1);
   }
   idproc=atoi(argv[1]);
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGALRM\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGUSR2\n");

   alarm(5);
	for ( ;!salir; ) {
		pause();
   }
   return 0;
}

void sig_usr(int signo)	{	/* signo=nro de señal */
	if (signo == SIGUSR2)  salir=1;
	else printf("sig_usr():recibi señal %d\n", signo);
	return;
}

void sig_alrm(int signo)	{	/* signo=nro de señal */
   alarm(60);
   kill((pid_t) idproc,SIGUSR1); // envio señal para obtener totales de ventas
	return;
}

void usage() {
   printf("tp44 <tp43 proc.id>\n");
   printf("Donde:\n");
   printf("<tp43 proc.id>=nro. de proceso de tp43 que debe estar previamente en ejecucion\n");
}
