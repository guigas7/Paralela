#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define NTHREADS 1
#define NCHARS 26

pthread_mutex_t *flag_mutex;
pthread_t thread[NTHREADS];
long int *hist;

void *count(void *);

int main(void) {
    unsigned char ascii = 97;
    flag_mutex = malloc(130 * sizeof(pthread_mutex_t));
    hist = calloc(130 * sizeof(long int), sizeof(long int));
    // Initialize mutexes
    for (int i = 0; i < 130; i++) {
        pthread_mutex_init(&flag_mutex[i], NULL);
    }
    // Create NTHREADS threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&thread[i], NULL, count, (void *)i);
    }
    // Join all threads
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(thread[i], NULL);
    }
    // printf("threads joined\n");
    // Print result
    for (int i = 65; i < 91; i++) {
        printf("%c - %ld\n", ascii++, hist[i] + hist[i+32]);
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
    char filename[] = "1.txt";
    FILE *file = fopen(filename, "r");
    int indice;
    int ret;
    // Acha tamanho total
    fseek(file, 0, SEEK_END);
    size = ftell(file) - 1;
    area = (int) ceil((double) size / NTHREADS);
    // printf("    size: %d, area: %d, in thread: %d\n", size, area, (int) region_ptr);
    // calcula e incia ponteiro no início
    start = start * area;
    rewind(file);
    fseek(file, start, SEEK_SET);
    // printf("    start: %d, in thread: %d\n", start, (int) region_ptr);
    // calcula fim
    stop = (start + area - 1 < size - 1 ? start + area - 1 : size - 1);
    block = stop - start + 1;
    data = malloc(block*sizeof(unsigned char));
    // printf("    stop: %d, block: %d, in thread: %d\n", stop, block, (int) region_ptr);
    // read block
    ret = fread(data, 1, block, file);
    // printf("read %d blocks, in thread: %d\n", ret, (int) region_ptr);
    for (int i; i < block; i++) {
        if (data[i] == 'EOF') {
            printf("        EOF found, in thread: %d\n", (int) region_ptr);
            break;
        }
        indice = data[i];
        // printf("char found: %d, in thread: %d\n", data[i], (int) region_ptr);
        ret = pthread_mutex_lock(&flag_mutex[indice]);
        // printf("Lret: %d, in thread> %d\n", ret, (int) region_ptr);
        hist[indice]++;
        // printf("hist for char %d is %ld, in thread: %d\n", data[i], hist[indice], (int) region_ptr);
        ret = pthread_mutex_unlock(&flag_mutex[indice]);
        // printf("Uret: %d, in thread> %d\n", ret, (int) region_ptr);
    }
    // printf("        Returning, in thread: %d\n", (int) region_ptr);
    return NULL;
}
