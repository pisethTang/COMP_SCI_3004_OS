#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <string.h> // for memset
#include <errno.h> // for EINPROGRESS


#define BUFFSIZE 25000


struct aiocb * async_read(FILE* fp, char* buf, int bytes){
    struct aiocb* aio = malloc(sizeof(struct aiocb));
    if (aio == NULL) return NULL;

    memset(aio, 0, sizeof(*aio));
    aio -> aio_buf = buf;
    aio -> aio_fildes = fileno(fp); // grab dile descriptor from the file pointer struct 
    aio -> aio_nbytes = bytes;
    aio -> aio_offset = 0; // start reading from the beginning of the file, it could also be at the middle or at the end or anywhere within the file.

    int result = aio_read(aio);
    if (result < 0) {
        free(aio);
        return NULL; 
    }

    // At this point, the read was accepted and we can start reading from the file.
    return aio;
}



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


    // ret = fread(buffer, sizeof(char), BUFFSIZE, fp); // read bytes from a file-like thing.
    struct aiocb* aio = async_read(fp, buffer, BUFFSIZE); // asynchronouse -io control block 
    // wait for it to finish.
    uint64_t counter = 0;
    while (aio_error(aio) == EINPROGRESS){
        counter++;
    }
    ret = aio_return(aio);

    fclose(fp);

    printf("While we were reading, we counted up %lu times\n", counter);

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