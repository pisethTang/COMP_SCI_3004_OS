#include <stdio.h>
#include <pthread.h>

/*
This program below aims to illustrate how threads interact with one another when sharing data.
*/

// #include "mythread.h"
static volatile int counter = 0; // what is volatile? 



void* my_thread(void* arg){
  printf("%s: begin\n", (char*) arg);

  for(int i=0; i<1e7; i++){
    counter = counter + 1;
  }
  printf("%s: done\n", (char*) arg);
  return NULL;
}




int main(int argc, char* argv[]){
  pthread_t p1, p2;
  printf("main: begin (counter = %d)\n",counter);
  pthread_create(&p1, NULL, my_thread, "A");
  pthread_create(&p2, NULL, my_thread, "B");

  // Join() waits for the threads to finish 
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("main: done with both counter  = %d\n", counter);

  return 0;
}
