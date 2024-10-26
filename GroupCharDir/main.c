#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int main(int argc, char* argv[]){
    // Creating a Multi-threaded + multiple users group char applications. 
    // This is a simple client that connects to a server.

    // Create a socket
    // AF_INET - IPv4 protocol, SOCK_STREAM - TCP socket 0 - use default protocol.
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    // We can use this socket FD to connect with other function IO 
    
    char* ip = "235.3.42.1";
    struct sockaddr_in address; // this structure will be used for IPV4
    address.sin_family = AF_INET;
    address.sin_port = 2000;
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);


    int result = connect(socketFD, &address, sizeof(address));
    if (result == 0){
        printcf"Connected ();
    }



    return 0;
}