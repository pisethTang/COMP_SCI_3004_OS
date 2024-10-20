#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 25000



int main(int argc, char* argv[]){
    int ret;
    FILE* fp;

    if (argc < 2){
        printf("usage: %s \n<filename>n", argv[0]);
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL){
        perror("fopen");
        return EXIT_FAILURE;
    }

    char buffer[BUFFSIZE];
    ret = fread(buffer, sizeof(char), BUFFSIZE, fp); // read bytes from a file-like thing.
    fclose(fp);

    if (ret > 0){
        printf("Got %d bytes:\n", ret);
    }
    else{
        printf("READ FAILED\n");
    }

    /*
    synchronouse i0:
    start
    wait for finish
    return result
    adv: easier to read and debug
    caveat: put everything on hold when we wait, slow, depending on the file system by computer standard. 
    */




    return 0;
}