#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

/*
int reflect(int M, int x)
{
    if (x < 0)
    {
        return -x - 1;
    }
    if (x >= M)
    {
        return 2 * M - x - 1;
    }
    return x;
}

int circular(int M, int x)
{
    if (x < 0)
        return x + M;
    if (x >= M)
        return x - M;
    return x;
}
*/

   //los píxeles en el borde simplemente se descuidan
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
/*

void refletedIndexing(Mat src, Mat dst, float Kernel[][3])
{
    float sum, x1, y1;
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            sum = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    x1 = reflect(src.cols, x - j);
                    y1 = reflect(src.rows, y - k);
                    sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y1, x1);
                }
            }
            dst.at<uchar>(y, x) = sum;
        }
    }
}


//Las coordenadas que exeden el borde de la imagen se envuelven en el lado opuesto
void circularIndexing(Mat src, Mat dst, float Kernel[][3])
{
    float sum, x1, y1;
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            sum = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    x1 = circular(src.cols, x - j);
                    y1 = circular(src.rows, y - k);
                    sum = sum + Kernel[j + 1][k + 1] * src.at<uchar>(y1, x1);
                }
            }
            dst.at<uchar>(y, x) = sum;
        }
    }
}
*/


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

      //Blur
    /*float Kernel[3][3] = {
                          {1 / 9.0, 1 / 9.0, 1 / 9.0},
                          {1 / 9.0, 1 / 9.0, 1 / 9.0},
                          {1 / 9.0, 1 / 9.0, 1 / 9.0}
    };
    */
/*        
        //DETECCION DE BORDES
      float Kernel[3][3] = {
                          {-1, -1, -1},
                          {-1, 8, -1},
                          {-1, -1, -1}
    };
*/ 
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

    //circularIndexing(src, dst, Kernel);
    noBorderProcessing(src,dst,Kernel);
    //refletedIndexing(src, dst,Kernel);

    namedWindow("final");
    imshow("final", dst);
 

    namedWindow("initial");
    imshow("initial", src);

    waitKey();


    return 0;
}
