#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


/*
wait()

useful for order the execution of processes (since the default behavior is that the execution either interleaves or it goes back and forth).

*/

int main(int argc, char* argv[]){
    // printf("Only once\n");
    int is_child = 0;
    // child := 0
    // parent := 1

    int id = fork();
    // printf("Hello, world!\n");
    // int id = fork();
    // printf("Hello, World from %d\n", id);
    // if (id == 0){
    //     printf("Hello, from child process %d\n", id);
    // }
    // else{
    //     printf("Hello, from the main process\n");
    // }
    int n;
    // child process: starts at 0 and ends at 5
    if (id == 0){
        n = 1;
        // printf("Child: %d\n", getpid());
    }
    else{
        // main process: starts at 6 and ends at 10
        n = 6;
        // printf("Parent: %d\n", getpid());
        is_child = 1;
    }
    if (id != 0) wait(NULL);
    // if we are not the child process, or if we are the main, let's wait till the child finishes (and all of its resources deallocated)

    for(int i = n; i < n + 5; i++){
        if (is_child == 0){ // child here
            printf("c_");
            printf("%d ", i);
            fflush(stdout);
        }
        else{ // parent here 
            printf("p_");
            printf("%d ", i);
            fflush(stdout);
            
        }
       // to stop the batch-processing behavior 
        // and make sure that 
        // every time we print a number, it is printed on the terminal 
    }

    if (id != 0) printf("\n");




    return 0;
}