#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char* argv[]){
    // Creating a Multi-threaded + multiple users group char applications. 
    // This is a simple client that connects to a server.

    // Create a socket
    // AF_INET - IPv4 protocol, SOCK_STREAM - TCP socket 0 - use default protocol.
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // We can use this socket FD to connect with other function IO 
    struct sockaddr_in address; 
    address.sin_family = AF_INET;
    address.sin_port = 2000; // port of the server that listens from the other side 
    


    connect(socketFD, &address, sizeof(address));

    return 0;
}