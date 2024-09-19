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
  int Trace1[4] = {};

  for (int i = 0; i < 4; i++){
    Trace1[i] = 0;
  }
  
  for (int i = 0; i < 4; i++){
    // Trace1[i] = 0;
    printf("%d\n", &i);
  }




  return 0;
}
