#include <stdio.h>
#include "common_threads.h"



// A simple child-parent signaling example

int done = 0;


void* worker(void* arg){
    printf("This should print first\n");
    done = 1;
    return NULL;
}


int main(int argc, char* argv[]){
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
    while(done == 0) ;
    printf("This should print last\n");

    return 0;
}