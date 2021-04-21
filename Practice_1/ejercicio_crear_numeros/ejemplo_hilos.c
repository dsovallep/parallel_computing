//%cflags:-fopenmp -lm -D_DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

double producirNumero(int desde,int hasta,int decimales);
void *hilo1();
void *hilo2();

int main(int argc,char **argv) {
    
    pthread_t h1,h2;//Referencias de los hilos
    
    /**HASTA ACA EL UNICO HILO QUE ESTA EN EJECUCION ES EL HILO main()
    	
    	*Libreria para crear hilos pthread_create() con parametros: 
    	(puntero de referencia del hilo, atributos asociados al hilo,funcion puntero que devuelve void* con argumento void*, el argumento de la funcion puntero )
    	
	*NULL son atributos por defecto
	
	*Cuando se crea un hilo queda a decision del planificador de la libreria de hilos para que el hilo sea ejecutado.
    	es decir, no significa que apenas se crea el hilo se ejecuta.
    */
    
    pthread_create(&h1,NULL,hilo1,NULL);
    printf("main(): he creado hilo1\n");
    
    pthread_create(&h2,NULL,hilo2,NULL);
    printf("main(): he creado hilo2\n");
        
    //ACA YA SE TENDRIAN EN EJECUCION CONCURRENTE TRES HILOS; main(), hilo1(), hilo2()
    	
    	printf("main(): espero por la finalizacion de hilo1\n");
	pthread_join(h1,NULL);//pthread_join() Espera hasta que el h1 (hilo1) haya terminado su ejecución para finalizar

	printf("main(): espero por la finalizacion de hilo2\n");
	pthread_join(h2,NULL);

	
	printf("main(): fin!\n");
	return 0;
}

void *hilo1() {
	int i;
	for(i=0;i<10;i++) {
		//sleep(1);
		printf("hilo1(): %10.3lf\n",producirNumero(1,1000,2));
	}
	pthread_exit(NULL);//Termina la ejecucion del hilo
}
void *hilo2() {
	int i;
	for(i=0;i<10;i++) {
		printf("hilo2(): %10.3lf\n",producirNumero(1,1000,3));
	}
	pthread_exit(NULL);
}

/**
 * Produce numeros aleatorios en un contexto de hilos A NIVEL DE USUARIO (el planificador de la libreria es el que plinifica el uso de los hilos, no el sistema operativo)
 * (usa version re-entrante de funcion rand(), 
 * srand(unsigned int) ) para evitar problemas de ejecucion
 * simultanea de mas de un hilo. rand() no esta preparada
 * para eso.
 * desde = numero desde
 * hasta = numero hasta
 * decimales = cantidad de decimales
 * Ejemplo de uso: si quiero producir un numero double con
 * 2 decimales entre 1 y 1000:
 * double numero = producirNumero(1,1000,2);
 */
double producirNumero(int desde,int hasta,int decimales) {
	unsigned int ahora = (unsigned int) time(NULL);
	static int offset = 0;
	ahora+=offset++;
	int num=desde+rand_r(&ahora)%((hasta+1)-desde);
	int dec=0;
	if ( decimales > 0 ) {
		dec = (int) (producirNumero(1,pow(10,decimales),0)-1.0);
	}
	double ret = (double) num;
	if ( decimales > 0 ) ret+= ((double) dec / pow(10,decimales));
	return ret;
}
