#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
  printf("Hello world (pid: %d)", getpid());
  int rc = fork();

  if (rc < 0){
    // fork failed; exit 
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  else if (rc == 0){
    // Child (new process)
    printf("Hello, I am a child (pid:%d)", getpid());
  }
  else{
    // parent/original goes down this path
    printf("Hello, I am parent of %d (pid: %d)\n", rc, getpid());
  }



  return 0;
}
