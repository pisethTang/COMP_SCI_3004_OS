#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int x = 1;


void* routine(){
  printf("Test from threads\n");
  sleep(3); // pause for 3 seconds
  printf("Ending threads\n");
  return NULL;
}

void* f1(void* arg){
  // Do something
  printf("Doing sth in f1");
  x = x * 2;
  printf("%d", x);
  return NULL;
}


void* f2(void* arg){
  // read 4 values
  x = x * 4;
  return NULL;
}



int main(int argc, char* argv[]){
  void* x_ptr =  (int*) &x;
  pthread_t t1, t2;
  pthread_create(&t1, NULL, &f1, x_ptr);
  pthread_create(&t2, NULL, &f1, x_ptr);
  pthread_join(t1, &x_ptr);
  pthread_join(t2, &x_ptr);
  printf("Thread's done running: %d\n", *((int*)x_ptr));
  // Not thread safe. 

  
  // if (pthread_create(&t1, NULL, &routine, NULL) != 0){
  //   return 1;
  // }
  // if (pthread_create(&t2, NULL, &routine, NULL)){
  //   return 1;
  // }
  // // pthread T1;
  // if (pthread_join(t1, NULL) != 0){return 3;}
  // if (pthread_join(t2, NULL) != 0) {return 4;}


  return 0;
}
