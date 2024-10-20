#include <netdb.h> // the same header files as in server except for this first one. 
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>



void error(char *msg)
{
    perror(msg);
    exit(0);
} // identical to that in server.c



int main(int argc, char *argv[])
{
    // Identical to server.c
    int sockfd, portno, n;
    struct sockaddr_in serv_addr; // will contain the address of the server to which we want to connect. 

    // a pointer to a structure of type hostent, defined in netdb.h
    struct hostent *server; 
    /*
    Defines a host computer on the Internet.
    struct  hostent {
        char    *h_name;            official name of host */
        // char    **h_aliases;    /* alias list: a zero terminated array of alternate names for the host */
        // int     h_addrtype;     /* host address type: current always AF_INET*/
        // int     h_length;       /* length of address in bytes*/
        // char    **h_addr_list;  /* list of addresses from name server, a pointer to a list of network addresses for the named host. Host addresses are returned in network byte order*/
        // #define h_addr  h_addr_list[0]/*  /* address, for backward compatiblity */
    // Note: h_addr is an alias for the first address in the array of network addresses.

    // The same as that as in server.c
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");


    server = gethostbyname(argv[1]); // contains the nae of a host on the Internet
    // gethostname() takes such a name as an argument and returns a pointer to a hostend containing information
    // abt that host.
    // The field char* h_addr contains the IP address of the host.
    // The function's mechanism is complex: as it needs to query large databases all around the country. 
    if (server == NULL) {
        // If the structure is NULL, the sys. could not locate a host with this name
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    // sets the field in serv_addr. Similar to that in server.c, however, because the field server-> h_addr
    // is a character string, we use the function bcopy()
    // which copies h_length bytes from s1 to s2 

    // The connect() function is called by the client to establish a connection to the server 
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // It takes 3 arguments
    // arg[0]: the socket fd
    // arg[1]: the address of the host to whcih it wants to connect (including the port number)
    // arg[2]: the size of the address.

    
    // Note: the client needs to know the port number of the server, but it does not need to know its own port number, which is typically assigned by the system when connect() is called.
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin); // reads messages from stdin
    n = write(sockfd,buffer,strlen(buffer)); // writes the message back to the socket 
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255); // reads the reply back from the socket 
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer); // displays this reply on the screen. 
    return 0;
}