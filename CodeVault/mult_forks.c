#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>




int main(int argc, char** argv){
    int id1 = fork();
    int id2 = fork();   
    // Now we have 4 processes. 
    // But there is still only one parent process and the remaining 3 are children



    if (id1 == 0){
        if (id2 == 0) printf("We are process y.\n");
        else printf("We are process x.\n");
    }
    else{
        if (id2 == 0){
            printf("We are process z.\n");
        }
        else printf("We are the parent process.\n");
    }
    /*
    Expected: Should print out one of the above messages/ all conditions must be met. 
    */



    /*
        Now, how do we wait for all of the processes to finish? Not trivial... 
    */



   

    return 0;
}