#include <stdio.h>
#include "common_threads.h"

// READ: A simple race condition

int balance = 0;

void* worker(void* arg){
    balance++; // unprotected access ==> critical section
    return NULL;
}


int main(int argc, char* argv[]){
    printf("Address of baalnce: %p\n", (void*)&balance);
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
    balance++; // unprotected access ==> critical section
    Pthread_join(p, NULL);


    return 0;
}