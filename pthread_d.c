#include <stdio.h>
#include <pthread.h>

void* computation(void* add);

int main(int argc, char* argv[]){
    pthread_t thread1;
    pthread_t thread2;

    long value1 = 1;
    long value2 = 2;

    /* Single-threaded (one after the other) , time : 4s
    computation((void*)& value1);
    computation((void*)& value2);
    */
    // Multi-threaded (concurrently, pseudo-parallelism): 2 seconds 
    // If 
    // Joined thread1 before thread2 ==> like a single-threaded program: 4 seconds

    pthread_create(&thread1, NULL, 
               computation, (void*) &value1);
    pthread_join(thread1, NULL);

    pthread_create(&thread2, NULL, 
                        computation, (void*) &value2);

    pthread_join(thread2, NULL);


    return 0;
}

void* computation(void* add1){
    long sum = 0;
    long* add_num = (long*) add1;
    for (long i=0; i<1000000000; i++){
        sum += *add_num;
    }

    printf("Add: %ld\n", *add_num);
    return NULL;
}