#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>



int main(int argc, char* argv[]){
  // This program takes one command line argument, the number of megabytes of memory it will use. 
  if (argc < 2){
    printf("Usage: %s <megabytes> [<seconds>]\n", argv[0]);
    return 1;
  } 

  int megabytes = atoi(argv[1]);
  int bytes = megabytes * 1024 * 1024; // Convert MB to B 
                                       //
  // Optional second argument: runtime in seconds 
  int run_time = 0;
  if (argc == 3){
    run_time = atoi(argv[2]);
  }

  // ALlocate memory 
  char* memory = malloc(bytes);
  if (memory == NULL){
    fprintf(stderr, "Failed to allocate mrmoy.\n");
    return 1;
  }
  
  // Initialize memory to ensure it is being allocated 
  memset(memory, 0, bytes);

  // Time tracking 
  time_t start_time - time(NULL);
  
  // Access memory in a loop 
  while(1){
    for (int i=0; i<bytes; i++){
      memory[i] += 1; // Access each byte 

    }

    if (runtime > 0 && time(NULL) - start_time >= run_time){
      break; // exit the loop after run_time seconds 
    }
  }

  free(memory);

  return 0;
}
