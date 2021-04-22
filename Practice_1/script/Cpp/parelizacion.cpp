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

#define THREADS 16

double piTotal[THREADS];

void noBorderProcessing(Mat src, Mat dst, float Kernel[][3])
{

    float sum;
    for (int y = 1; y < src.rows - 1; y++) {
        for (int x = 1; x < src.cols - 1; x++) {
            sum = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y - j, x - k);
                }
            }
            dst.at<uchar>(y, x) = sum;
        }
    }
}


int main()
{

    Mat src, dst;


    /// Load an image
    src = imread("zorro.jpg", IMREAD_GRAYSCALE);
    //src = imread("zorro.jpg");

    if (!src.data)
    {
        return -1;
    }

      
        //ENFOQUE
    float Kernel[3][3] = {
                          {0, -1, 0},
                          {-1, 5, -1},
                          {0, -1, 0}
    };

    dst = src.clone();
    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            dst.at<uchar>(y, x) = 0.0;

    
    noBorderProcessing(src,dst,Kernel);
    

    namedWindow("final");
    imshow("final", dst);
 

    namedWindow("initial");
    imshow("initial", src);

    waitKey();


    return 0;
}
