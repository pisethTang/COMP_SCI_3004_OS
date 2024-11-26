// Program to demonstrate the lock api porvided by the POSIX library

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




// Use this to keep your code clean but check for failures
// Only use if exiting prohram is OK upon failure
void Pthread_mutex_lock(pthread_mutex_t* mutex){
    int rc = pthread_mutex_lock(mutex);
    assert(rc == 0);
}


int main(int argc, char **argv){
    // int x = 0;
    // pthread_mutex_t lock;
    // pthread_mutex_lock(&lock);
    // x = x + 1; // or whatever your critical section is.
    // pthread_mutex_unlock(&lock);
     
    /*
    The above code is broken due to a lack of proper intialization.
    In POSIX threads, locks can be initialized in two ways:
    - PTHREAD_MUTEX_INITIALIZER
    */
    // Static initialization
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


    // Dynamic initialization (during runtime)
    int rc = pthread_mutex_init(&lock, NULL);
    assert(rc == 0);
    // arg[0]: address of the lock itself
    // arg[1]: the optional set of attributes (NULL for default attributes) 

    // We shoul also free up the lock after a thread has finished
    // pthread_cond_destroy(pthread_cond_t *cond)


    // 2nd error: it does not check for error codes when calling lock or unlock 
    // ===> can lead to silent errors where multiple threads can enter the critical section(s).
    // Solution: use wrapper to check for failure and do something appropriate when lock or unlock does not succeed.



    return 0;
}