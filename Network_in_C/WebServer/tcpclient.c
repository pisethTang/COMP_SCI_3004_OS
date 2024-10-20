#include <sys/socket.h> // basic socket definitions
#include <sys/types.h> 
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h> // for variadic function 
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>



// Standard HTTP port
#define SERVER_PORT 80 // server port: where we're connecting 

#define MAXLINE 4096 
#define SA struct sockaddr 

// For handling errors
void err_n_die(const char *fmt, ...);



int main(int argc, char **argv){
    // Local variables 
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in serveraddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    if (argc != 2){ // usage check, if the user is using it correctly.
        // Take the ip address
        err_n_die("usage: %s <server address>", argv[0]);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        // Create a socket (like an end-point, can write data into and out of).
        // Address family - Internet
        // Socket varieties: stream (send a stream of data and get a stream back) and datagram (like a single packagae chunk)
        // 0 == TCP 
        err_n_die("Error while creating the socket!");
    }
    // zeros out the address
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; // Specify the address family again 
    serveraddr.sin_port = htons(SERVER_PORT); // char server 
    // htons = host to network server, short 
    // convert the number from local byte order (big or little endians) to the network standard byte order (so two computers with different byte orders can still commmunicate with each other)
    
    // Convert string to binary representation of the address 
    if (inet_pton(AF_INET, argv[1], &serveraddr) <= 0) err_n_die("inet_pton error for %s ", argv[1]);

    // Try to connect to the server via the address 
    if (connect (sockfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0) err_n_die("connection failed!");

    // We're connected. Prepare the message.
    sprintf(sendline, "GET /HTTP/1.1\r\n\r\n"); // \r\n\r\n to denote end of request.
    sendbytes = strlen(sendline);

    // Send the request -- making sure you send it all
    if (write(sockfd, sendline, sendbytes) != sendbytes){
        err_n_die("wrtie errror");
    }


    // Now read the server's response 
    while ((n = read(sockfd, recvline, MAXLINE-1))  > 0){
        printf("%s", recvline);
        memset(recvline, 0, MAXLINE);
    }

    if (n < 0){
        err_n_die("read error");
    }

    exit(0); // end successfully.
}





void err_n_die(const char* fmt, ...){
    int errno_save;
    va_list ap;

    // any system or library call can set errno, so we need to save it now 
    errno_save = errno;

    // Print out the fmt + args to standard out 
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    // Print out error messages if errno was set 
    if (errno_save != 0){
        fprintf(stdout, "(errno = %d): %s\n", 
                                                errno_save, 
                                                strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    // This is the ... and_die part
    exit(1);

}
