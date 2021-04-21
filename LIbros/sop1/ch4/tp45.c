/*
   tp45.c programa que captura la señal SIGALRM utilizada con la funcion alarm() para enviar
   señal a otro proceso (tp43) para obtener el calculo de los totales de las sucursales cada 1 minuto.
   Realiza un mejor control de errores que tp44, chequeando el valor de retorno de kill y utilizando la 
   señal nula (null signal, signo=0) para chequear la existencia del proceso.
   
   para compilar:
            gcc -Wall -o tp45 tp45.c 
            
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
   if ( idproc <= 1 ) {
      fprintf(stderr,"main():%d no es un id de proceso valido\n",idproc);
      usage();
      exit(3);
   }
   if ( kill((pid_t) idproc,0) == -1 ) {
      fprintf(stderr,"main():%d no es un id de proceso valido\n",idproc);
      usage();
      exit(2);
   }
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
      fprintf(stderr,"main():imposible capturar SIGALRM\n");
      exit(4);
   }
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
      fprintf(stderr,"main():imposible capturar SIGUSR2\n");
      exit(5);
   }

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
   if (kill((pid_t) idproc,SIGUSR1) != 0) {
      fprintf(stderr,"sig_alrm():Error enviando señal SIGUSR1 a %d\n",idproc);
      raise(SIGUSR2); // envio señal para obtener totales de ventas
   }
	return;
}

void usage() {
   printf("tp44 <tp43 proc.id>\n");
   printf("Donde:\n");
   printf("<tp43 proc.id>=nro. de proceso de tp43 que debe estar previamente en ejecucion\n");
}
