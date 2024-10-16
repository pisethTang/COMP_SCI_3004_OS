#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]){
/* The dup() all is useful when writing a UNIX shell, and performing operations like output redirection.
 * It allows a process to duplicate a file descriptor from a given old one and hence it can refer to the same underlying open file as an existing descriptor.
 *
 */
  char fileName[5] = "file1";
  printf("fileName: %s\n", fileName);
  int fd = open(fileName, O_RDONLY);
  printf("fd: %d\n", fd);
  assert(fd >= 0);
  int fd2 = dup(fd);
   // Now fd and fd2 can be used interchangeably
  printf("fd2: %d\n", fd2);


/*
Using fsync(int fd): when this is called by the process for particular file descriptor, the system responds by forcing all dirty data to disk, for the 
file referred to by the file descriptor. It returns once all of those writes are complete.
*/

int fd1 = open(fileName, O_CREAT | O_WRONLY | O_TRUNC | S_IRUSR | S_IWUSR);
assert(fd1 >= 0);
int size = 20;
char buffer[100] = "Hello World!";
int rc = write(fd, buffer, size);
assert(rc == size);
rc = fsync(fd1);
assert(rc == 0);
/*In some cases, we also need to fsync() the directory that contains the file fileName. Adding this step ensures that 
not only the file is on disk, but that the file, if newly created, also is durably also a part of the directory. This type of detail is often overlooked, leading to many application-level bugs.*/


/*
rename(char* old, char* new) provides an interesting guarantee -- it is atomic with respect to system crashes - if the system crashes during the renaming, the file with either be named the old file's name or the newly given name, there is no odd in-between state. It is crucial for 
supporting certain kinds of applications that require an atomic update to file state.
 struct stat {
 dev_t
 ino_t
 mode_t
 st_dev;
 st_ino;
 st_mode;
 /* ID of device containing file */
 /* inode number */
 /* protection */
//  nlink_t st_nlink; /* number of hard links */
//  uid_t
//  gid_t
//  dev_t
//  off_t
//  st_uid;
//  st_gid;
//  st_rdev;
//  st_size;
//  /* user ID of owner */
//  /* group ID of owner */
//  /* device ID (if special file) */
//  /* total size, in bytes */
//  blksize_t st_blksize; /* blocksize for filesystem I/O */
//  blkcnt_t st_blocks; /* number of blocks allocated */
//  time_t
//  time_t
//  time_t
//  };
//  st_atime; /* time of last access */
//  st_mtime; /* time of last modification */
//  st_ctime; /* time of last status change */

  



  return 0;
}
