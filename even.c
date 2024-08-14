#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Handler for SIGHUP 
void handle_sighup(int sig){
    printf("Ouch!\n");
}

// Handler for SIGINT 
void handle_sigint(int sig){
    printf("Yeah!\n");
}

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     printf("Usage: %s <number of even numbers>\n", argv[0]);
    //     return 1;
    // }

    int n = atoi(argv[1]); // Number of even numbers to print
    // if (n <= 0) {
    //     printf("Please provide a positive integer.\n");
    //     return 1;
    // }

    // printf("PID: %d\n", getpid());

    // Register signal handlers using sigaction
    struct sigaction sa;
    sa.sa_handler = handle_sighup;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);

    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    int count = 0;
    for (int i = 0; count < n; i++) {
        if (i % 2 == 0) {
            printf("%d ", i);
            fflush(stdout);
            sleep(5); // Sleep for 5 seconds

            // Prompt for user to press Enter to continue
            // printf("\nPress Enter to continue...");
            getchar(); // Wait for Enter key
            count++; // Increment count of even numbers printed
        }
    }

    return 0;
}
