#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 6
#define VECSIZE 32

int *r, *a, *b;
pthread_t thread[NTHREADS];

void *somaVet(void *region_ptr)
{
    int start = (int) region_ptr;
    start = start * NTHREADS;
    int stop = (start + NTHREADS < VECSIZE - 1 ? start + NTHREADS : VECSIZE - 1);
    for (int i = start; i <= stop; i++) {
        r[i] = a[i] + b[i]; 
    }
    return NULL;
}

int main
{
    int soma;
    r = malloc(VECSIZE * sizeof(int));
    a = malloc(VECSIZE * sizeof(int));
    b = malloc(VECSIZE * sizeof(int));
    // Initialize a and b
    for (int i = 0; i < VECSIZE; i++) {
        a[i] = i;
        b[i] = i;
    }
    // Create NTHREADS threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&thread[i], NULL, somaVet, (void *)i);
    }
    soma=0;
    // Join all threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(thread[i], NULL);
    }
    // print r
    for (int i = 0; i < VECSIZE; i++) {
        printf("%d\n", r[i]);
    }
}



