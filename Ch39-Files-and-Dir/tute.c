#include <stdio.h>
// #include <unistd.h>
// #include <sys/stat.h>
#include <fcntl.h>
/*
fcntl.h is a header in the C POSIX standard library that contains constructs that refer to file control,
e.g., opening a file, retrieving and changing the permissions of file, locking a file for edit, etc.
*/
// #include <sys/types.h>






int main(int argc, char* argv[]){
  int fd = open("foo", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR|S_IWUSR);
  /*
  p[0] := name of the file 
  p[1] := flags
          - O_CREAT := creates the file
          - O_WRONLY := ensures that the file can only be written to.
          - O_TRUNC := if the file exists, truncates it to the size of 0 bytes, which basically wipes out any existing content..
  p[2] := permissions.
          - S_IRUSR := makes the file readable by the owner
          - S_IWUSR := makes the file writeble by the owner
  Output: a file descriptor 
  
  */  

  return 0;
}
