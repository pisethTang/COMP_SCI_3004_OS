#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

// Handler for SIGHUP 
void handle_sighup(int sig){
    printf("Ouch!\n");
}

// Handler for SIGINT 
void handle_sigint(int sig){
    printf("Yeah!\n");
}



int main(int argc, char *argv[]){
    printf("PID: %d", getpid());
    int n = 0;
    // if (argc < 2){
    //     // printf("Please enter an argument.\n");
    //     exit(1);
    // }
    // else{// argc >= 2 (although we want it to be exactly 2)
        
        // Register signal handlers 
        signal(SIGHUP, handle_sigint); 
        signal(SIGINT, handle_sigint);


        n = atoi(argv[1]); // number of even numbers to print out 
        int i = 0; // Count number of even numbers 
        
        for (int j = 0; i < n; j++){ // BEGIN LOOP 
            if (j % 2 == 0){
                printf("%d ", j);
                sleep(5);
                i++; // we have i even numbers so far in this iteration (Loop invariant correctness proof)
            }
            // else printf("not even ");
        } // END LOOP  



    // }


    return 0;
}

