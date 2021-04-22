#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

#define N_HILOS 8

Mat src, dst;
float Kernel[3][3] = {
                          {1 / 9.0, 1 / 9.0, 1 / 9.0},
                          {1 / 9.0, 1 / 9.0, 1 / 9.0},
                          {1 / 9.0, 1 / 9.0, 1 / 9.0}
    };
int m=0, n=0;

void llenado (int id_hilo){
	
	//int longitud = sizeof(matriz) / sizeof(matriz[0]);//Devuelve la cantidad de filas
	/*
	int total_celdas = FILAS*COLUMNAS;//4*2=8
	int inicio = (total_celdas/N_HILOS)*id_hilo;//0
	int fin = ((total_celdas/N_HILOS)-1)+inicio;
	int contador;
	*/
	//int yi = (src.rows / N_HILOS) * id_hilo;//si el id es 0, yi = 0. Si el id es 1, yi = 0
    //int xi = (COLS / THREADS) * threadId;//si el id es 0, xi = 0. Si el id es 1, xi = COLS/2
    //int yf = yi + (src.rows / N_HILOS);//si el id es 0,yf = ROWS/2, si es 1, yf = ROWS/2
    //int xf = (COLS / THREADS) * threadId;
    int yi = ((id_hilo-1)/(N_HILOS))*src.rows;
    int yf = ((id_hilo)/(N_HILOS))*src.rows;
        float sum;
        printf("%s","Hello\n");
		for( m = yi ; m < yf; m++ ){
			for ( n = 0; n < src.cols; n++){
				   sum = 0.0;
                   for(int k = -1; k <= 1;k++){
                       for(int j = -1; j <=1; j++){
                           sum = sum + Kernel[j+1][k+1]*src.at<uchar>(m - j, n - k);
                        }
                    }
            dst.at<uchar>(m,n) = sum;		
			
			}	
		}

	return ;
	
}

void* intermedia(void* arg){

	int id_hilo = *((int*)arg); 
	llenado(id_hilo);

	return NULL;

}

int main()
{


    /// Load an image
    src = imread("zorro.jpg", IMREAD_GRAYSCALE);

    if (!src.data)
    {
        return -1;
    }


    dst = src.clone();
    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            dst.at<uchar>(y, x) = 0.0;
    //-------------------------SECCION DE LOS HILOS--------------------------------------------------
    int idHilos[N_HILOS + 1];
	pthread_t objeto_hilo[N_HILOS + 1];
    
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
	

	for (int i = 1; i <= N_HILOS;i++){
		idHilos [i] = i;
		pthread_create(&objeto_hilo[i], NULL, intermedia, &idHilos[i]);
	}
	for(int i = 1; i <= N_HILOS; i++){
        		pthread_join(objeto_hilo[i],NULL);
   	}

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    //-----------------------------------------------IMAGEN------------------------------------------

    //circularIndexing(src, dst, Kernel);
    namedWindow("final");
    imshow("final", dst);

    namedWindow("initial");
    imshow("initial", src);

    waitKey();


    return 0;
}