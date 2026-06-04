//Mariana Ramirez
//A01787819

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_COUNT 10000000

typedef struct {
    int id;
    int start;
    int end;
    unsigned long long * result;
    pthread_mutex_t*mutex;
} data_t;

unsigned long long factorial(int n);
unsigned long long parallelFactorial(int n, int threads);
void * multiplyRange(void * data);

int main(int argc, char * argv[])
{

    int n = 5;
    int threads = 1;

    if (argc == 3) {
        threads = atoi(argv[2]);
    }
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    unsigned long long resultParallel = parallelFactorial(n, threads);
    printf("%llu is the Parallel Factorial of %d\n", resultParallel, n);

    unsigned long long result = factorial(n);
    printf("%llu is the Factorial of %d\n", result, n);

    return 0;
}

unsigned long long factorial(int n)
{
    unsigned long long result = 1;

    for (int i=1; i<=n; i++) {
        result *= i;
    }

    return result;
}

unsigned long long parallelFactorial(int n, int threads)
{
    int rangeSize = n / threads;
    int remainder = n % threads;

    data_t thread_data[threads];

    pthread_t tid[threads];

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int status = 0;

    unsigned long long resultParallel = 1;

    for (int i=0; i<threads; i++) {
        thread_data[i].id = i;
        thread_data[i].start = i * rangeSize + 1;;
        thread_data[i].end = (i + 1) * rangeSize;
        thread_data[i].result = &resultParallel;
        thread_data[i].mutex = &mutex;
        if(i == threads -1){
            thread_data[i].end += remainder;
        }
        printf("ID: %d [%d, %d]\n", thread_data[i].id, thread_data[i].start, thread_data[i].end);

        
        int status = pthread_create(&tid[i], NULL, multiplyRange, &thread_data[i]);
        if (status == -1)
        {
            perror("ERROR: pthread_create");
        }
    }

    printf("Going to pthread_join\n");

    for (int i=0; i<threads; i++){
        status = pthread_join(tid[i], NULL);
        if (status == -1)
        {
            perror("ERROR: pthread_join");
        }
    }

    return resultParallel;
}

void * multiplyRange(void * data)
{
    data_t * localData = (data_t *) data;
    unsigned long long result = 1;

    for (int i=localData->start; i<=localData->end; i++) {
        result *= i;
    }
    pthread_mutex_lock(localData->mutex);
    (*localData->result)*=result;
    pthread_mutex_unlock(localData->mutex);

    return 0;
}