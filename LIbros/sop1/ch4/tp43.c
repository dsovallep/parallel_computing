/*
   tp43.c programa que captura las dos señales del usuario, termina ejecucion con SIGUSR2, 
   emito los totales de ventas actuales de las sucursales cada vez que se recibe la señal SIGUSR1
   Para simplificar se asumen 10 sucursales con valores aleatorios double para cada una

   para compilar:
            gcc -Wall -o tp43 tp43.c 
            
   Guillermo Cherencio
   SO
   UTN FRD
*/
#include <stdio.h>
#include <stdlib.h>
#include	<signal.h>
#include	<unistd.h>
#include <string.h>

#define NUM_SUC 10
void sig_usr(int signo);
void print_ventas(int p);
int salir=0;
double ventas[NUM_SUC];

int main(void) {
   // inicializo ventas de sucursales
   memset(&ventas[0],0,sizeof(double)*NUM_SUC);
   print_ventas(0);
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGUSR1\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) fprintf(stderr,"main():imposible capturar SIGUSR2\n");

	for ( ;!salir; ) {
		pause();
   }
   return 0;
}

void sig_usr(int signo)	{	/* signo=nro de señal */
	if (signo == SIGUSR1)           print_ventas(1);
	else if (signo == SIGUSR2)  salir=1;
	else printf("sig_usr():recibi señal %d\n", signo);
	return;
}

void print_ventas(int imprimo) {
   int s;
   for(s=0;s<NUM_SUC;s++) ventas[s]+=((double) random())/10000000.0;
   if (imprimo) for(s=0;s<NUM_SUC;s++) printf("suc[%d]=%lf\n",s,ventas[s]);
}

