#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>



int main(int argc, char* argv[]){
  sem_t s;
  sem_init(&s, 0, 1); // Initialize the semaphore object with a value of 1 and if there are other processes that we want to share the value with, we can let the 2nd argument be 1. 
                      //
                      //
// arg[1] = 1 ==> the semaphore is shared between thread in the same process.
//
  
  /* sem_wait(sem_t* s): decrements the value of s by 1 and if s < 0: wait
   * sem_post(sem_t* s): increments the value of s by 1 and if there
   * are one or more threads waiting, wake one.
   * 
   * There is an inherent critical section between sem_wait() and sem_post(). 
   *
   * */


  // Using semaphore as a lock. 
  









  return 0;
}
