#include <stdio.h>
#include <unistd.h>
// #include <sys/stat.h>
#include <fcntl.h>
/*
fcntl.h is a header in the C POSIX standard library that contains constructs that refer to file control,
e.g., opening a file, retrieving and changing the permissions of file, locking a file for edit, etc.
*/
// #include <sys/types.h>
#include <assert.h>
#include <sys/wait.h>





int main(int argc, char* argv[]){
  int fd = open("foo", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR|S_IWUSR);
  assert(fd >= 0);
  int rc = fork();
  if (rc == 0){
        rc= lseek(fd, 10, SEEK_SET);
        printf("child: offset %d \n", rc);
  }else if(rc >0){
        (void) wait(NULL);
        printf("parent: offset %d \n", (int)lseek(fd, 0, SEEK_CUR));
  }
  /*
  p[0] := name of the file 
  p[1] := flags
          - O_CREAT := creates the file
          - O_WRONLY := ensures that the file can only be written to.
          - O_TRUNC := if the file exists, truncates it to the size of 0 bytes, which basically wipes out any existing content..
  p[2] := permissions.
          - S_IRUSR := makes the file readable by the owner
          - S_IWUSR := makes the file writeable by the owner
  Output: a file descriptor 

  File descriptor: an integer, private per process, 
  
  */

  /*
  Sometimes, it maybe useful to read or write to a specific offet within a file. 
  */  
  // off_t lseek(int fildes, off_t offset, int whence);
  /*arg
  0: a file descriptor
  1: positions the file's offset to a particular location within a file
  2: called "whence" for historical reasons, determines exactly how the seek is performed.
  If whence is SEEK_SET, the offser is set to offset bytes.
  If whence is SEEK_CUR, the offset is set to its current value plus offset bytes.
  If whence is SEEK_END, the offset is set to the end of the file plus offset bytes.
  */

//   int fd = opne();

  return 0;
}
