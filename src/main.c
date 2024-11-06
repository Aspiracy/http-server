#include "include/server.h"
#include "include/requests.h"

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFSIZE 4096

void* get_in_address(struct sockaddr* sa); 
void handle_client(int client_fd); 

int main(int argc, char* argv[]) {
    char* host;
    char* port;

    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    } else {
        fprintf(stderr, "Invalid number of arguments\n");
        return -1;
    }
   
    int server_fd = initialize_server(host, port);
    if (server_fd == -1) {
        fprintf(stderr, "Server failed to initialize");
        return -1;
    }

    printf("Listening on %s:%s\n", host, port);
     
    // Store client's ip address here  
    char ip_str[INET_ADDRSTRLEN];
    memset(&ip_str, '\0', sizeof ip_str);
   
    struct sockaddr_storage client;
    socklen_t client_size = sizeof client;

    while (1) {   
        int client_fd = accept(server_fd, (struct sockaddr*)&client, &client_size); 
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
         
        // Print incoming connection's IP address  
        inet_ntop(client.ss_family, get_in_address((struct sockaddr*)&client), ip_str, sizeof ip_str);
        printf("got connection from: %s\n", ip_str); 

        pid_t pid = fork();
        if (pid == 0) { // Child
            // Child process no longer needs server socket
            close(server_fd);
           
            // infinite loop running in child process handling client requests until disconnection or error
            handle_client(client_fd);  
             
            // After client closes connection or something happens, close client and exit child process
            close(client_fd);
            exit(0);
        } else if (pid > 0) { // Parent
            close(client_fd);
        } else {
            perror("fork failed");
        }
    }
    
    close(server_fd);
    return 0;
}

void* get_in_address(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
};

void handle_client(int client_fd) {
    char buffer[BUFSIZE];
    ssize_t buffer_size = 0;
    
    while ((buffer_size = read(client_fd, &buffer, BUFSIZE - 1)) > 0) {
        buffer[buffer_size] = '\0';

        if (handle_requests(client_fd, buffer, buffer_size) < 0)
            break;
    }
    
    if (buffer_size < 0) {
        perror("Error reading from client");
    }
    
    printf("Client closed connection: %ld\n", buffer_size);
}

