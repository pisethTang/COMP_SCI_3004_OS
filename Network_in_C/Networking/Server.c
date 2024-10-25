#include "Server.h"
#include <netinet/in.h>



struct Server server_constructor(int domain, int service, int protocol, 
    u_long interface, int port, int backlog, void(*launch)(void)){
    
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    // server.launch = launch;

    // Create our address
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);// convert integer port to bytes that point to network port 
    server.address.sin_addr.s_addr = htonl(interface); // does the same thing for a long


    server.socket = socket(domain, );

    return server;
}