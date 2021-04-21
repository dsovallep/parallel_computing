
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

void *hilo1();
void *hilo2();
/*
void *hilo3();
void *hilo4();
void *hilo5();
void *hilo6();
void *hilo7();
void *hilo8();
void *hilo9();
void *hilo10();
void *hilo11();
void *hilo12();
void *hilo13();
void *hilo14();
void *hilo15();
void *hilo16();
*/

/*
int calculatePi(double *piTotal){   
    int i = 0;
    do{
        *piTotal = *piTotal + (double)(4.0 / ((i*2)+1));
        i++;
        *piTotal = *piTotal - (double)(4.0 / ((i*2)+1));
        i++;
    }while(i < ITERATIONS);
    return 0;
}
*/

int main(int argc,char **argv){
    
    //CODIGO TEMPORIZADOR
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
    
    //pthread_t h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,h11,h12,h13,h14,h15,h16;
    pthread_t h1,h2;

    
    pthread_create(&h1,NULL,hilo1,NULL);
    pthread_create(&h2,NULL,hilo2,NULL);
    
    /*
    pthread_create(&h3,NULL,hilo3,NULL);
    pthread_create(&h4,NULL,hilo4,NULL);
    pthread_create(&h5,NULL,hilo5,NULL);
    pthread_create(&h6,NULL,hilo6,NULL);
    pthread_create(&h7,NULL,hilo7,NULL);
    pthread_create(&h8,NULL,hilo8,NULL);
    pthread_create(&h9,NULL,hilo9,NULL);
    pthread_create(&h10,NULL,hilo10,NULL);
    pthread_create(&h11,NULL,hilo11,NULL);
    pthread_create(&h12,NULL,hilo12,NULL);
    pthread_create(&h13,NULL,hilo13,NULL);
    pthread_create(&h14,NULL,hilo14,NULL);
    pthread_create(&h15,NULL,hilo15,NULL);
    pthread_create(&h16,NULL,hilo16,NULL);
    */
    
    pthread_join(h1,NULL);
    pthread_join(h2,NULL);
    
    /*
    pthread_join(h3,NULL);
    pthread_join(h4,NULL);
    pthread_join(h5,NULL);
    pthread_join(h6,NULL);
    pthread_join(h7,NULL);
    pthread_join(h8,NULL);
    pthread_join(h9,NULL);
    pthread_join(h10,NULL);
    pthread_join(h11,NULL);
    pthread_join(h12,NULL);
    pthread_join(h13,NULL);
    pthread_join(h14,NULL);
    pthread_join(h15,NULL);
    pthread_join(h16,NULL);
    */
    
    
    /*double pi;
     pi = 0;
     calculatePi(&pi);
    */
    
    
    
    
    
    
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    //printf("\npi: %2.12f \n", pi);
    
    return 0;
    
}

void *hilo1() {
    int i;
    for(i=0;i<1000;i++) {
        printf("El valor del contador del hilo 1 va en: %d\n", i);
    }
    pthread_exit(NULL);//Termina la ejecucion del hilo
}
void *hilo2() {
    int i;
    for(i=0;i<1000;i++) {
        printf("El valor del contador del hilo 2 va en: %d\n", i);
    }
    pthread_exit(NULL);
}

