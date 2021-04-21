/*
   tp42.c programa que captura las dos señales del usuario, termina ejecucion con SIGUSR2
   para compilar:
            gcc -Wall -o tp42 tp42.c 
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include	<signal.h>
#include	<unistd.h>

void sig_usr(int signo);
int salir=0;

int main(void) {
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGUSR1\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGUSR2\n");

	for ( ;!salir; ) {
		pause();
   }
   return 0;
}

void sig_usr(int signo)	{	/* signo=nro de señal */
	if (signo == SIGUSR1) printf("sig_usr():recibi SIGUSR1 (%d)\n",signo);
	else if (signo == SIGUSR2)  salir=1;
	else printf("sig_usr():recibi señal %d\n", signo);
	return;
}

