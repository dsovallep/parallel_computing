/* Comando para correr codigo C++ con phtread en unix
g++ script1.c -o output -pthread
./output
*/    

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

//Definicion de espacios de nombre
#define THREADS 2
#define ROWS 10
#define COLS 10
int a[ROWS][COLS];
using namespace std;

//Funcion a paralelizar
void * fillOne(void *arg){
    int threadId = *(int *)arg;
    int yi = (ROWS / THREADS) * threadId;//si el id es 0, yi = 0. Si el id es 1, yi = 0
    int xi = (COLS / THREADS) * threadId;//si el id es 0, xi = 0. Si el id es 1, xi = COLS/2
    int yf = yi + (ROWS / THREADS);//si el id es 0,yf = ROWS/2, si es 1, yf = ROWS/2
    int xf = (COLS / THREADS) * threadId;
    /*for(int i = yi; i < yf; i++){
        for(int j = xi; j < xf; j++){
            a[i][j] = 1;
        }
    }*/
    for(int i = yi; i < yf; i++){
        for(int j = 0; j < COLS; j++){
            a[i][j] = 1;
        }
    }

    return 0;
}
//Funcion principal
int main(int argc,char* argv[]){
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
    int threadId[THREADS];//Declaracion del arreglo de los identificadores de los hilos
    pthread_t thread[THREADS];//Declaracion de un arreglo de hilos
    
    //if(THREADS == 2){
        threadId[0] = 0;
        yi = 0;
        xi = 0;
        yf = ROWS/2;
        xf = COLS;
        pthread_create(&thread[0], NULL, fillOne, &threadId[0]);
        threadId[1] = 1;
        yi = ROWS/2;//fila inicial
        xi = 0;//columna inicial                
        yf = ROWS;//fila final
        xf = COLS;//columna final
        pthread_create(&thread[1], NULL, fillOne, &threadId[1]);
    /*}else if(THREADS > 2){
        cout<<"HAY MAS DE DOS HILOS"<<endl;
    }else if(THREADS == 1){
        threadId[0] = 0;
        yi = 0;
        xi = 0;
        yf = ROWS;
        xf = COLS;
        pthread_create(&thread[0], NULL, fillOne, &threadId[0]);
    }
    for(int i = 0;i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }*/
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    return 0; // Finaliza el programa
}