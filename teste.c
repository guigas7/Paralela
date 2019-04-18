#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define BSIZE 10

int main(void) {
    unsigned char data[BSIZE];
    FILE *file = fopen("tes.txt", "r");
    int a = 31, b = 3;
    double res;
    fseek(file, 0, SEEK_SET);
    fread(data, BSIZE, 1, file);
    printf("%s\n", data);
    res = (double) a/b;
    a = (int) ceil(res);
    printf("%f\n", res);
    printf("%d\n", a);
    return 0;
}

