#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int num){
    write(STDOUT_FILENO, "I won't die\n", 13);
}

int main(){
    signal(SIGINT, handler); // allows handler() to run when SIGINT is received 
    
    while(1){
        printf("Wasting your cycles. %d\n", getpid());
        sleep(1);
    }

    return 0;
}