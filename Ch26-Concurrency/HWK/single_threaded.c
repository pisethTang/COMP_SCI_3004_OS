#include <stdio.h>
#include <assert.h>
#include <pthread.h>



void* my_thread(void* arg){
  printf("%s\n", (char*)arg);
  return NULL;
}

int main(int argc, char* argv[]){
  pthread_t p1, p2;
  int rc;
  printf("main: begin\n");
  rc = pthread_create(&p1, NULL, my_thread, "A"); assert(rc==0);
  rc = pthread_create(&p2, NULL, my_thread, "B"); assert(rc==0);
  printf("main: end\n");
  

  return 0;
}
