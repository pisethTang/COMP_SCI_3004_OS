#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"



int main(int argc, char* argv[]){
    int* p = malloc(sizeof(int)); // a1
    assert(p != NULL);
    printf("(%d) address of p: %p\n",getpid(), (void *) p); // a2
    *p = 0;                                    // a3
    while(1){
        Spin(1);
        *p = *p + 1;
        printf("(%d) p at: %d\n", getpid(), *p); // a4
        /*@brief
        Delay for 1 second, increment value at address p, then print out 
        Process Identifier (PID, unique per running process) and the value p 
        */
    }
    /*
    By default, the kernel will leave the ASLR (Address Space Layout Randomization) on for precautionary reasons. 
    It is a secutiry feature that randomizes the memory addresses used by systems and application processes, making it more difficult for attackers to predict the location of specific 
    functions or buffer. To disable it , use the following command: 

    cat /proc/sys/kernel/randomize_va_space
    The output will be:
    2 indicates full randomization (default).
    1 indicates partial randomization.
    0 indicates that ASLR is disabled.


    It is recommended to turn it on again because the previous command affects the entire Linux System (since parts of its kernel have been modified),

    sudo sh -c 'echo 0 > /proc/sys/kernel/randomize_va_space'

    Re-enabling it: 
    
    sudo sh -c 'echo 2 > /proc/sys/kernel/randomize_va_space'

    
    */


    return 0;
}
