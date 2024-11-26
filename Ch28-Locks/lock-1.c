/*A simple lock implementation using loads and stores (failed attempt)*/
#include <stdio.h>
#include <stdbool.h>
// #include <pthread.h>
#include <stdlib.h>

#include "/home/sething2002/2024_UoA/s2/COMP_SCI_3004_OS/Ch27-Threads-API/HWK/common_threads.h"


typedef struct __lock_t{
    int flag;
}lock_t;

void init(lock_t *lock){
    // 0 indicates that the lock is available
    // 1 indicates that the lock is held
    lock -> flag = 0;
}

void unlock(lock_t* lock){
    lock -> flag = 0;
}


int TestAndSet(lock_t* lock, int val){
    if (lock -> flag == 1) return 1;
    else {
        // lock is released
        // lock -> flag = 0;
        unlock(lock);
        return 0;
    }
}

void lock(lock_t* lock){
    // wait until the lock is available
    while(TestAndSet(lock, 1) == 1); // spinwait (do nothing);
    // acquire the lock
    // lock -> flag = 1;
}






int main(){



    return 0;
}