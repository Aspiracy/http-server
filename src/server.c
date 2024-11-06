#include "include/server.h"

#include <stdio.h>
#include <strings.h>
#include <netdb.h>

int initialize_server(const char* host, const char* port) {
    struct addrinfo hints;
    struct addrinfo* server;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // IPV4 or IPV6, doesn't matter
    hints.ai_socktype = SOCK_STREAM; // Stream socket
   
    int status = 0;
    if ((status = getaddrinfo(host, port, &hints, &server)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
   
    // Create socket and client file descriptor holders
    int server_fd = 0;

    // Loop over every possible result and bind to first one
    struct addrinfo* p;
    for (p = server; p != NULL; p = p->ai_next) {
        // try creating socket
        if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket failed"); 
            continue;
        }
        
        // try binding to socket
        if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("bind failed");
            continue;
        }
     
        break;
    }

    freeaddrinfo(server);
    
    if (p == NULL) {
        fprintf(stderr, "Failed to bind to any socket\n");
        return -1;
    }

    // Begin listening
    int backlog = 1;
    if (listen(server_fd, backlog) == -1) {
        perror("listen failed");
        return -1;
    }

    return server_fd;
}
