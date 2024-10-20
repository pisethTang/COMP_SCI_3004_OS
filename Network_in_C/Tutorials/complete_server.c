/* 
A simple server in the internet domain using TCP. 
The port number is passed as an argument.
This version runs forever, forking off a separate process for each connection.
*/

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>  // data type definitions in system calls (used in socket and netinet/in.h libraries)
#include <sys/socket.h> // needed for socket struct definitions
#include <netinet/in.h> // needed for internet domain addresses
#include <stdlib.h>
#include <unistd.h>


// Function prototypes.
void doStuff(int); 
// The function is called when a system call failed. It displays a message about the rror on stderr and then 
// aborts the program.
void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     // sockfd and newsockfd are used to index into the file descriptor table.
     // portno: stores the port number on which the server accepts connection, 


     unsigned int clilen; // stores the size of the address of the client, needed for the accept system call. 
     char buffer[256]; // the server reads characters from the socket connection into this buffer.
     struct sockaddr_in serv_addr, cli_addr; // sockaddr_in: a structure defiend in <netinet/in.h> containing an internet address.
     /*
     struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
     };
      */

      // serv_addr: will contain the address of the server  
      // cli_addr: will contain the address of the client which connects to the server.
     int n; // the return values for the read(0 and write() calls, contains the number of chars read or written.)
     // User needs to pass in the port number on which the server will accept connections as an argument. 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }


     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     // Creates a new socket
     //  arg[0]: address domain (could be unix: AF_UNIX or internet: AF_INET) 
     //  arg[1]: type of socket (stream socket: SOCK_STREAM and datagram socket: SOCK_DGRAM)
     //  arg[2]: the protocol, if this arg is 0 (as it should always be except for unsual circumstances), the OS will choose the most appropriate protocol. 
     // This system call is unlikely to fail.
     if (sockfd < 0) 
        error("ERROR opening socket");
    
     // Sets all value in the vuffer to 0. 
     bzero((char *) &serv_addr, sizeof(serv_addr));
     // arg[0]: pointer to the buffer
     // arg[1]: size of the buffer
     // Result: this will initialize serv_addr to zeros.
     
     portno = atoi(argv[1]); // pass in the port number on which the server will listen for connection (use atoi() to convert from chars to integer)
     serv_addr.sin_family = AF_INET; // should always be AD_INET
     serv_addr.sin_addr.s_addr = INADDR_ANY; // contains only 1 field: unsigned long s_addr, whcih contains the IP address of the host. 
     // For server code, this will always be the IP address of the machine on which the server is running,
     // and thereis a symbolic constant INADDR_ANY which gets this address.
     serv_addr.sin_port = htons(portno); // use htons() to convert portno to a port number in network byte order instead of just passing in the port number.
     
     
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     // binds a socket to an address
     // arg[0]: socket descriptor
     // arg[1]: pointer to the address of the socket to be bound
     // arg[2]: size of the address structure
     // Returns: 0 if successful, -1 if an error occurred.
     // The server is now listening for incoming connections.

    //  close(sockfd); // Close the listening socket (sockfd) after binding it to an address.


     listen(sockfd,5);
     // allows the process to listen on the socket for connections
     // arg[0]: socket file descriptor 
     // arg[1]: the size of the backlog queue, in this case, is 5: the number of connections that can be waiting while the process is handling a particular connection. 
     // 5 is the maximum size permitted by most systems. 
     // If the first argument is a valid socket, this call cannot fail so the code doesn't check for errors.

     clilen = sizeof(cli_addr);
     int pid;
     while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        pid  = fork();
        if (pid < 0) error("ERROR on fork()");
        if (pid == 0){
            close(sockfd);
            doStuff(newsockfd);
            exit(0);
        }
     }
     

}



/*********************
There is a separate instance of this function for each connection. 
It handles all communication once a connection has been established.
************** */

void doStuff(int sock){
    int n;
    char buffer[256];

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
     n = write(sock,"I got your message",18); // 18 is the size of the message that the server writes to the client.
     // Once a connection has been established, both ends can read and write to the connection. 
     // Naturally, everything written by the client will be read by the serverm and everythign written by the server will be read by th client. 
     if (n < 0) error("ERROR writing to socket");
}


/*
==================Alternative types of sockets====================

This example showed a stream socket in the Internet domain. This is the most common type of connection. A second type of connection is a datagram socket. You might want to use a datagram socket in cases where there is only one message being sent from the client to the server, and only one message being sent back. There are several differences between a datagram socket and a stream socket.
Datagrams are unreliable, which means that if a packet of information gets lost somewhere in the Internet, the sender is not told (and of course the receiver does not know about the existence of the message). In contrast, with a stream socket, the underlying TCP protocol will detect that a message was lost because it was not acknowledged, and it will be retransmitted without the process at either end knowing about this.
Message boundaries are preserved in datagram sockets. If the sender sends a datagram of 100 bytes, the receiver must read all 100 bytes at once. This can be contrasted with a stream socket, where if the sender wrote a 100 byte message, the receiver could read it in two chunks of 50 bytes or 100 chunks of one byte.
The communication is done using special system calls sendto() and receivefrom() rather than the more generic read() and write().
There is a lot less overhead associated with a datagram socket because connections do not need to be established and broken down, and packets do not need to be acknowledged. This is why datagram sockets are often used when the service to be provided is short, such as a time-of-day service.
Server code with a datagram socket

 */