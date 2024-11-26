/*************************************************************
This program desmontrates using a mutex to control access to a critical section where
a shared counter is incremented. The mutex lock ensures that only one thread can access the counter at any given time, precenting race conditions. 
***************************************************************/


#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int counter = 0; // Shared resource in the global segment
pthread_mutex_t lock; // Mutex for synchoonization


void* increment_counter(void* arg){
    for(int i=0; i<1e7;i++){
        pthread_mutex_lock(&lock);    // Lock before accessing shared resource
        counter++;                          // Critical section
        pthread_mutex_unlock(&lock); // Unlock after accessing shared resource
    }
    return NULL;
}




int main(int argc, char* argv[]){
    // Declare threads
    pthread_t t1,t2;

    // Intitialize muteex
    pthread_mutex_init(&lock, NULL);


    // Create threads
    pthread_create(&t1, NULL, increment_counter, NULL);
    pthread_create(&t2, NULL, increment_counter, NULL);

    // Join threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter: %d\n", counter);
    
    // Destroy mutex
    pthread_mutex_destroy(&lock);
    

    return 0;
}