#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum repl {
  Rand,
  Clock,
  Lru
};
struct result{
    int numberAccesses;
    int numFrames_;
    double pageFaultRate;
    int replAlgorithm;
    char* traceFile;
};



int main(){
  struct result* R = malloc(sizeof(struct result));
  R -> traceFile = "Hello, world!";
  printf("%s\n", R -> traceFile);
  // // printf("%s", s);

  // char* arr[] = {
  //   strcat(s, "1"), 
  //   strcat(s, "2"),
  //   strcat(s, "3"),
  //   strcat(s, "4"),
  //   strcat(s, "5"),
  // };

  // for (int i=0; i<5; i++){
  //   printf("%s\n", arr[i]);
  // }
  free(R);
  return 0;
}
