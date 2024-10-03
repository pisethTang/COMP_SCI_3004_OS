#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define BIG 1000000000UL
uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;



void* count_to_big(void* arg){
  for (uint32_t i=0; i<BIG; i++){
    // We want to make this atomic --- using a mutex or lock variable.
    // Locks: Mutex Locks := computing abstraction that allows one thread to exclue other threads from accessing shared resources.
    // 
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}





int main(){
  pthread_t t;
  pthread_create(&t, NULL, count_to_big, NULL);

  count_to_big(NULL);
  pthread_join(t, NULL);

  printf("Done: Counter = %u\n", counter);
  return 0;
}
