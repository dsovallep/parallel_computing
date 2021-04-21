//%cflags:-lpthread -lm -D_DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define ITERATIONS 1e09
#define THREADS 2
#define PAD 8

double piTotal[THREADS*PAD];

void *calculatePi(void *arg)
{
  int initIteration, endIteration, threadId = *(int *)arg;

  initIteration = (ITERATIONS/THREADS) * threadId;
  endIteration = initIteration + ((ITERATIONS/THREADS) - 1);
  piTotal[threadId] = 0.0;
    do{
        piTotal[threadId] = piTotal[threadId] + (double)(4.0 / ((initIteration*2)+1));
        initIteration++;
        piTotal[threadId] = piTotal[threadId] - (double)(4.0 / ((initIteration*2)+1));
        initIteration++;
    }while(initIteration < endIteration);

    return 0;
}

int main()
{
    int threadId[THREADS*PAD], i, *retval;
    pthread_t thread[THREADS*PAD];
    struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);
    
    for(i = 0; i < THREADS; i++){
        threadId[i*PAD] = i;
        piTotal[i*PAD] = 0;
        pthread_create(&thread[i*PAD], NULL, (void *)calculatePi, &threadId[i*PAD]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i*PAD], (void **)&retval);
    }

    for(i = 1; i < THREADS; i++){
        piTotal[0] = piTotal[0] + piTotal[i*PAD];
    }
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    
    printf("\npi: %2.12f \n", piTotal[0]);
}
