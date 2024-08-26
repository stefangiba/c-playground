#include <stdio.h>

#define MAX_PERSONS 1000
#define DEBUG

int main() {
    #ifdef DEBUG
        printf("WE ARE IN DEBUG MODE: %s:%d.\n", __FILE__, __LINE__);
    #endif

    printf("Hello, World!\n");
    printf("MAX_PERSONS: %d\n", MAX_PERSONS);

    return 0;
}