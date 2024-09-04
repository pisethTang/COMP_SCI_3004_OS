#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
/*
 * Threads allow concurrency within the same process, without isoaltion.Thread has different call stacks but in the same memory space ==> one thread can write into the memory of another thread which improves communication and this can either be a clessing or curse.  
 * Thread APIs that we can have a look at:  Window thread and POSIX Thread 
 *
*/
void* myTurn(void* arg){
  int* iptr = (int*) malloc(sizeof(int));
  *iptr = 5;
  for(int i = 0; i < 8; i++){
    sleep(1);
    printf("My turn! %d --> %d\n", i, *iptr);
    (*iptr)++;
  }
  return iptr;
}

void yourTurn(){
  for(int i=0; i<3; i++){
    sleep(2);
    printf("Your turn! %d\n", i);
  }
}

int main(int argc, char* argv[]){
  pthread_t newThread;
  void* result;



  pthread_create(&newThread, NULL, myTurn, NULL);
 // myTurn();
  yourTurn();
  pthread_join(newThread, &result);
  printf("Thread's done: *result=%d\n", *((int*)result));

  return 0;
}
