#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Handler for SIGHUP 
void handle_sighup(int sig) {
    printf("Ouch!\n");
    // Re-register the signal handler
    // signal(SIGHUP, handle_sighup);
}

// Handler for SIGINT 
void handle_sigint(int sig) {
    printf("Yeah!\n");
    // Re-register the signal handler
    // signal(SIGINT, handle_sigint);
}

int main(int argc, char *argv[]) {
    // Register signal handlers using sigaction
    struct sigaction sa_hup, sa_int;
    
    sa_hup.sa_handler = handle_sighup;
    sigemptyset(&sa_hup.sa_mask);
    sa_hup.sa_flags = 0;
    
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    
    // Register signal handlers
    sigaction(SIGHUP, &sa_hup, NULL);
    sigaction(SIGINT, &sa_int, NULL);

    // Print the process ID
    // printf("PID: %d\n", getpid());

    // if (argc < 2) {
    //     printf("Please enter an argument.\n");
    //     return 1;
    // }

    int n = atoi(argv[1]); // Number of even numbers to print out
    int i = 0; // Count number of even numbers

    for (int j = 0; i < n; j++) { // BEGIN LOOP
        if (j % 2 == 0) {
            printf("%d\n", j);
            sleep(5);
            i++; // We have i even numbers so far in this iteration
        }
    } // END LOOP

    return 0;
}
