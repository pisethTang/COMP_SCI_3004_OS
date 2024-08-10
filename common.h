#ifndef __common_h__
#define __common_h__

#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <assert.h>


// double GetTime();
// void Spin(int);
/*
Including the inline keyword to supress the ODR (a translation unit such as a function can have many declaration but only one definition, hence ODR - One Definition Rule) warning message from the compiler.
Note to myself: the inline keyword allows for multiple definitions of a particular function, but it implies 
that I am sure all are identical and the linker may use one of those and ignore the rest. 



Another way to circumvent this warning message is to declare functions in the header files and their definitions in the source files (i.e., .c extension).
*/
inline double GetTime(){
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}



inline void Spin(int howlong){
    double t = GetTime();
    while ((GetTime() - t) < (double) howlong)
    ; // do nothing in loop 
}


#endif 

