#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct __myarg_t{
    int a;
    int b;
}myarg_t;


typedef struct __myret_t{
    int x;
    int y;
}myret_t;


void* mythread(void* arg){
    myarg_t* m = (myarg_t*) arg;
    printf("%d %d\n", m -> a, m-> b);
    myret_t* r =malloc(sizeof(myret_t)); 
    r -> x = 1;
    r -> y = 2; 
    return (void*) r;
}   



void* mythread_dangerous(void* arg){
    myarg_t* m = (myarg_t *) arg;
    printf("%d, %d\n", m->a, m-> b);
    myret_t r; //allocated r on the stack!
    r.x = 1;
    r.y = 2;
    return (void*) &r;
}






int main(int argc, char **argv){
    pthread_t p;
    // int arg = 100;
    pthread_create(&p, NULL, mythread_dangerous, (void*)100);
    // pthread_create(&p, NULL, mythread_dangerous, 100);

    return 0;
}