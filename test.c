#include "defer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    printf("Main Function:\n");
    int *arr[10];
    printf("\e[32m...allocating data...\e[m\n");
    for(unsigned i = 0; i < 10; i++){
        arr[i] = malloc(sizeof(int));
        defer(arr[i], free);
    }

    printf("\e[32m...cleaning data...\e[m\n");
    cleanup();
    return 0;
}
