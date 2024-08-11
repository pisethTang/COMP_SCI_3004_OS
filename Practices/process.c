#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv){
    // int pid_clone = fork();
    // fork();
    // to distinguish between a child or a parent process,
    // we look at the return value of fork()
    if (fork() == 0){ // it's a child
        printf("I am a child!\n");
    }
    else{
        // it is a parent (complicated) as the parent process needs to keep track of the child PID
        // in fact, if fork() != 0, then the returned pid is actually the child's PID but now the parent has it! 
        printf("Parent here!!\n");
        // printf("Hello world: %d\n", getpid());
    }
    // printf("Your program has been cloned!\n");
    // printf("Original pid: %d\n", getpid());
    // printf("Cloned pid: %d\n", pid_clone);
    // printf("===================ENDS HERE=============\n");

    return 0;
}