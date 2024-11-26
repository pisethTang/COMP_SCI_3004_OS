#include <stdio.h>
#include <stdlib.h>
#include "common_threads.h"

volatile int done = 0;


void* child(void* arg){
    printf("child\n");
    done = 1;
    return NULL;    
}


int main(int argc, char **argv){
    printf("parent: begin\n");
    pthread_t c;
    Pthread_create(&c, NULL, child, NULL); // create child thread
    while(done == 0) ; // wait for child to finish while spinning
    printf("parent: done\n");
    // However, this approach is very error-prone! 
    // Solution is to use condition variables.

    return 0;
}
