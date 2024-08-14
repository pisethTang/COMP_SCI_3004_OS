#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>



int main(int argc, char **argv){
    int id = fork();

    if (id == 0){
        // printf("Child: ");
        // printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());
        sleep(1);
    }
    // else{
    //     wait(NULL);
    //     printf("Parent: ");
    //     printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());

    // }

    printf("Current ID:  %d, parent ID: %d\n", getpid(), getppid());
    // Always wait for the child process to terminate before we terminate the main process 
    // if (id != 0){ // let parent wait 
    //     wait(NULL);
    // }
    
    // Even without the condition for the parent to wait...
    // This code would also work
    int res = wait(NULL);
    if (res == -1){
        printf("No children to wait for...child process is terminating.\n");
    }
    else{
        // if there are still children to wait for 
        printf("%d finished execution\n", res);   
        // the process that we waited for had terminated successfully. 
    }
    // Because the wait function checks whether the current process even has a a child to wait for
    // so when our child process starts wants to wait, the wait() method immediately returns -1, effectively telling it 
    // not to wait and thus, the execution continues normally
    // with the child process terminating and then
    // the wait() method in the parent process will return the PID of the just-terminated child
    // then, the parent process execution continues and terminates normally, freeing up all the resources. 

    // if (id != 0){
    //     printf("Child process terminated successfully. Parent process is terminating.\n");
    //     printf("Proof: Parent process ID is now the child process ID: %d", getpid());
    // }
    return 0;
}