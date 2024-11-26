#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>


int initialized = 0;

void Pthread_mutex_lock(pthread_mutex_t* mutex){
    int rc = pthread_mutex_lock(mutex);
    assert(rc == 0);
}




// 27.4: Conditional variables
// Puts the calling thread to sleep, and waits for some other threads
// to singal it, usually when soemthing in the program has changed that the now-sleeping thread might care about.


int main(int argc, char* argv[]){
    // int initialized = 0;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t init = PTHREAD_COND_INITIALIZER;

    Pthread_mutex_lock(&lock);
    while(initialized == 0){ // 
        pthread_cond_wait(&init, &lock);
    }
    pthread_mutex_unlock(&lock);






    return 0;
}