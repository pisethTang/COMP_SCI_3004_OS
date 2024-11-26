/*
This program uses condition variables for producer-consumer synchronization.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>



#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;


pthread_mutex_t lock;
pthread_cond_t not_empty;
pthread_cond_t not_full;



void* producer(){
    int item = 0;
    while(1){
        pthread_mutex_lock(&lock);

        // Wait if the buffer is still full
        while (count == BUFFER_SIZE){
            pthread_cond_wait(&not_full, &lock);
        }

        // count < BUFFER_SIZE ==> still space in the buffer, producer increments item or produce a new item and put it into buffer at the current *count* index, then increment count to avoid adding to the same index the next round.
        // Produce item and add to buffer
        buffer[count++] = item++; // 
        printf("Produced: %d\n", item);

        // After adding an item or producing an item to the buferm the producer signals not_empty to the consumer
        pthread_cond_signal(&not_empty); // signals to the consumer that buffer is not empty, to wake it up.
        pthread_mutex_unlock(&lock); // then it releases the lock to allow the consumer to access the shared resources - buffer and counter. 

        sleep(1); // Simulating production time to make it seem to the user that production takes time before the next production cycle.
        // If the sleep(1) were commented out, it would be difficult to discern the behavior of the producer thread
    } 
}



void* consumer(void* arg){
    while(1){
        pthread_mutex_lock(&lock);

        // Wait if buffer is empty
        while(count == 0){
            pthread_cond_wait(&not_empty, &lock);
        }

        // Consume item from buffer
        int item = buffer[--count];
        printf("Consumed: %d\n", item);

        pthread_cond_signal(&not_full); // Signal producer that the bffer is not full
        pthread_mutex_unlock(&lock);

        sleep(1); // Simulate consumption time delay.
    }
    return NULL;
}


int main(int argc, char* argv[]){
    pthread_t prod, cons;

    // Initialize mutex and condition variables
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);


    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Join threads (this program runs indefinitely, so we won't actually reach here)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);


    return 0;
}