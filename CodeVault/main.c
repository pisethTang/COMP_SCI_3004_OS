#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void* routine(){
    printf("Test from threads\n");
    sleep(3);
    printf("Ending thread\n");
    return 0;
}


int main(int argc, char **argv){
    // Declare a thread
    pthread_t t1; // thread1 
    pthread_t t2; // thread2

    // Initialize a thread 
    if (pthread_create(&t1, NULL, &routine, NULL) != 0) {
        return 1; // Error 
    }
    if (pthread_create(&t2, NULL, &routine, NULL)){
        return 2;
    }


    if (pthread_join(t1, NULL)){
        return 3;
    }
    if (pthread_join(t2, NULL)){
        return 4;
    }


    return 0;
}