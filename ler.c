#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#define NTHREADS 6
#define NCHARS 26

pthread_mutex_t *flag_mutex;
pthread_t thread[NTHREADS];
long int *hist;

void *count(void *);

int main(void){
    unsigned char ascii = 97;
    flag_mutex = malloc(NCHARS * sizeof(pthread_mutex_t));
    // Initialize mutexes
    for (int i = 0; i < NCHARS; i++) {
        flag_mutex[i] = PTHREAD_MUTEX_INITIALIZER;
    }
    hist = malloc (NCHARS * sizeof(int));
    // Create NTHREADS threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&thread[i], NULL, count, (void *)i);
    }
    // Join all threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(thread[i], NULL);
    }
    // Print result
    for (int i = 0; 0 < NCHARS; i++) {
        printf("%c - %ld\n", ascii, (hist[i] + hist[i+32]));
    }
    return 0;
}

/*  Acha o tamanho total do arquivo, divide em NTHREADS áreas
    cada thread conta a sua área. */
void *count(void *region_ptr)
{
    unsigned char *data;
    int size, stop, area, block;
    int start = (int) region_ptr;
    static char filename[] = "file.txt";
    FILE *file = fopen (filename, "r");
    // Acha tamanho total
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    area = ceil(size / NTHREADS);
    // calcula e incia ponteiro no início
    start = start * area;
    rewind(file);
    seek(file, start, SEEK_SET);
    // calcula fim
    stop = (start + area - 1 < size ? start + area - 1 : size);
    block = stop - start + 1;
    data = malloc(block*sizeof(char));
    // read block
    if (read(file, data, block) < 0) {
        write(2, "An error occurred in the read.\n", 31);
    }
    for (int i; i < block; i++) {
       hist[data[i] - 65]++;
    }
}
