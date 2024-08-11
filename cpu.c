#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "common.h"
#include <unistd.h>

int main(int argc, char** argv){
    if (argc != 2){
        fprintf(stderr, "usage: cpu<string>\n");
        exit(1);
    };
    char* str = argv[1];
    while (1){
        Spin(1);
        printf("PID = %d, str = %s\n", getpid(), str);
    }
    
    return 0;
}


