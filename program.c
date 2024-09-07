#include <stdio.h>


int main(){
  int arr[5] = {10, 20, 30, 40, 50};
  int* ptr = &arr[2]; // Pointer to the 3rd element in the array 

  printf("Value at arr[2]: %d\n", *ptr);
  arr[2] = 100; // Writing at arr[2]
  printf("New value at arr[2]: %d\n", arr[2]);

  return 0;
}
