#ifndef __common_threads_h
#define __common_threads_h

#include <pthread.h>
#include <assert.h>
#include <sched.h>

#ifndef __linux
#include <semaphore.h>
#endif 

#ifndef __linux__
#define Sem_init(sem, value) assert(sem)init(sem, 0, value) == 0);
#endif // __linux__ 


// https://github.com/remzi-arpacidusseau/ostep-code/blob/master/intro/common_threads.h

#endif 