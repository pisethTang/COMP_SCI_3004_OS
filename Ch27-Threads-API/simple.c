#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>


void* mythread(void* arg){
    int m  = (int)(intptr_t) arg;
    printf("%d\n", m);
    return (void*)(arg+1);
}




int main(int argc, char **argv){
    pthread_t p;
    int rc, m;
    rc = pthread_create(&p, NULL, mythread, (void*) 100);
    assert(rc == 0);
    pthread_join(p, (void**) &m);
    printf("Returned: %d\n", m);



    return 0;
}