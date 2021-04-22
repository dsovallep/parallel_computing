#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

#define N_HILOS 3
int m, n = 0;

Mat src, dst;
float Kernel[3][3] = {
                          {0, -1, 0},
                          {-1, 5, -1},
                          {0, -1, 0}
};

void* noBorderProcessing(void* arg)
{
    int id_hilo = *((int*)arg);
    float sum;
    int yi = (src.rows/ N_HILOS) * id_hilo; 
    int yf = yi + ((src.rows / N_HILOS)-1);
	
    cout<<  "filas: " << src.rows << endl;
    cout<<  "columnas: " << src.cols << endl;
    cout<<  "yi: " << yi << endl;
    cout<<  "yf: " << yf << endl;
    
    for (m = yi; m < yf -1; m++) {
        for (n = 0; n < src.cols -1 ; n++) {
            sum = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(m - j, n - k);
                }
            }
            dst.at<uchar>(m, n) = sum;
        }
    }
    
    return;
}


int main(int argc,char* argv[])
{

    

    int IdHilos[N_HILOS], i;
    pthread_t objeto_hilos[N_HILOS];

    src = imread("zorro.jpg", IMREAD_GRAYSCALE);
    //src = imread("zorro.jpg");

    if (!src.data)
    {
        return -1;
    }
      
    //ENFOQUE
 

    dst = src.clone();

    //int matriz[src.rows][src.cols];

    //Lleva a cero la matriz clonada
    for (int y = 0; y < src.rows; y++)    
        for (int x = 0; x < src.cols; x++)
            dst.at<uchar>(y, x) = 0.0;

    //HILOS

    int idHilos[N_HILOS];
	pthread_t objeto_hilo[N_HILOS];	

	for (int i = 0; i < N_HILOS;i++){
		idHilos [i] = i;
		pthread_create(&objeto_hilo[i], NULL, noBorderProcessing, &idHilos[i]);
	}
	for(int i = 0; i < N_HILOS; i++){
        pthread_join(objeto_hilo[i],NULL);
   	}
    

    namedWindow("final");
    imshow("final", dst);
 

    namedWindow("initial");
    imshow("initial", src);

    waitKey();


    return 0;
}
