#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    printf("before\n");
    usleep(5000000);
    printf("after\n");

    // usleep(x) ==> sleeps for x microseconds.


    return 0;
}