// #include <pthread.h>
#include <stdio.h>
#include "common_threads.h"




int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER; // condition variable


void thread_ext(){
    pthread_mutex_lock(&m);
    done = 1; // signal the child thread that it should stop spinning and exit
    // excluding the line above would make the parent thread spin forever! 
    // while the child thread returns from this function. 
    // Such a program will not halt. 
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}



void *child(void* arg){
    printf("child\n");
    thread_ext();
    return NULL;
}

void thread_join(){
    Pthread_mutex_lock(&m);
    while (done == 0) Pthread_cond_wait(&c, &m);
    Pthread_mutex_unlock(&m);
}



int main(int argc, char* argv[]){
    printf("parent begin:\n");
    pthread_t p;
    Pthread_create(&p, NULL, child, NULL);
    thread_join();
    printf("parent end:\n");

    return 0;
}