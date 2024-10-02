#include <stdio.h>
#include <assert.h>
#include <pthread.h>
/* This program is useful in illustrating the order in which 
 * the CPU schedules each the excution streams of each thread.
 * But it doesn't show you the interactions between threads
 * when sharing the same address space/data.
 * */
void *mythread(void *arg) {
  printf("%s\n", (char *) arg);
  return NULL;
 }

 int main(int argc, char *argv[]) {
  pthread_t p1, p2;
  int rc;
  printf("main: begin\n");
  rc = pthread_create(&p1, NULL, mythread, "A");
  rc = pthread_create(&p2, NULL, mythread, "B");
  // join waits for the threads to finish
  rc = pthread_join(p1, NULL); 
  assert(rc == 0);
  
  rc = pthread_join(p2, NULL);
  assert(rc == 0);
  
  printf("main: end\n");
  return 0;
 }
