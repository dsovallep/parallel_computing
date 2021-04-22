/*
tamaño de la matriz: 4x2 = 8 celdas
La matriz se llena de izquierda a derecha
tenemos 2 hilos
el hilo 1 llenaria desde la celda 1 hasta la celda 4
el hilo 2 llenaria desde la celda 5 hasta la celda 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

using namespace std;


#define N_HILOS 4
#define FILAS 4
#define COLUMNAS 5

int matriz[FILAS][COLUMNAS];
int m, n,fila = 0;



void llenado (int id_hilo){
	
	//int longitud = sizeof(matriz) / sizeof(matriz[0]);//Devuelve la cantidad de filas
	/*
	int total_celdas = FILAS*COLUMNAS;//4*2=8
	int inicio = (total_celdas/N_HILOS)*id_hilo;//0
	int fin = ((total_celdas/N_HILOS)-1)+inicio;
	int contador;
	*/
	int yi = (FILAS / N_HILOS) * id_hilo;//si el id es 0, yi = 0. Si el id es 1, yi = 0
    //int xi = (COLS / THREADS) * threadId;//si el id es 0, xi = 0. Si el id es 1, xi = COLS/2
    int yf = yi + (FILAS / N_HILOS);//si el id es 0,yf = ROWS/2, si es 1, yf = ROWS/2
    //int xf = (COLS / THREADS) * threadId;

		for( m = yi ; m < yf; m++ ){
			
			for ( n = 0; n < COLUMNAS; n++){
				matriz[m][n] = 1;		
				
					
			}
				
				
		}

	return ;
	
}

void* intermedia(void* arg){

	int id_hilo = *((int*)arg); 
	llenado(id_hilo);

	return NULL;

}

int main(){

	int idHilos[N_HILOS];
	pthread_t objeto_hilo[N_HILOS];	

	for (int i = 0; i < N_HILOS;i++){
		idHilos [i] = i;
		pthread_create(&objeto_hilo[i], NULL, intermedia, &idHilos[i]);
	}
	for(int i = 0; i < N_HILOS; i++){
        		pthread_join(objeto_hilo[i],NULL);
   	}
		
	
	

	for(int i = 0; i < FILAS; i++){
        for(int j = 0; j < COLUMNAS; j++){
            cout<<matriz[i][j]<<" ";
        }
        cout<<endl;
    }
	
	return 0;
 
}




