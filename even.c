#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    // Check for invalid input 
    if (argc < 2){
        printf("Please enter a number.\n");
        exit(1);
    }
    // Takes the input n from the command line
    int number = atoi(argv[1]); // use the atoi() function from the standard library stdlib.h to convert the string representation of a number to an integer
    // printf("argv[0]: name of the program = %s\n", argv[0]);
    // printf("Entered number: %d and %d\n", number, atoi(argv[2]));    
    // Prints the first n even numbers 
    for (int i=0; i<number; i++){
        if (i % 2 == 0) 
        {
            printf("%d", i);
            sleep(3);
        }
        scanf("");
        printf("scanned %d\n", i);
        // printf("bye %d", i);
    }
    printf("\n");


    return 0;
}
