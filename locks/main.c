/* Race condition := a programming phenomenon that occuers when a program depends on the timing of one or more events to function correctly.
 * It normally occurs when multiple threads access a shared vairable (or state) at the same time.
 *
 * Mutex := a synchronization primitive that enforces limits on access to a shared resource when we have multiple threads of execution.
 *
 */


#include <stdio.h>
#include <pthread.h>
#include "bank.h"



void* deposit(void* amount);
pthread_mutex_t mutex;

int main(){
  int before = read_balance();
  printf("Before:  %d\n", before);

  pthread_t thread1;
  pthread_t thread2;

  // Initializing the mutex using the POSIX library.
  // mutex: receives a pointer to the mutex variable that we declared above.
  pthread_mutex_init(&mutex, NULL);
  
  int deposit1 = 300;
  int deposit2 = 200;

  pthread_create(&thread1, NULL, deposit, (void*) &deposit1);
  pthread_create(&thread2, NULL, deposit, (void*) &deposit2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  pthread_mutex_destroy(&mutex);

  int after = read_balance();

  // We should have 500 after the work is done.
  printf("After: %d\n", after);
  //// But, we got this instead:
  // ething2002 Tutorials ./main 
  // Before:  0
  // After: 200



  return 0;
}



void* deposit(void* amount)
{
  // Create lock
  pthread_mutex_lock(&mutex);  
  int account_balance = read_balance();
  account_balance += *((int*) amount);
  write_balance(account_balance);

  // Release lock 
  pthread_mutex_unlock(&mutex);

  return NULL;
}
















