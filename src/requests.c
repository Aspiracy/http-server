#include "include/requests.h"

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>


Request* allocate_request() {
  Request* request = malloc(sizeof(Request));

  request->method.data = NULL;
  request->request_uri.data = NULL;
  request->http_version.data = NULL;
  request->headers = NULL;

  return request;
}

void free_request(Request* request) {
  if (request) {
    free(request->method.data);
    free(request->request_uri.data);
    free(request->http_version.data);

    for (int i = 0; i < request->headers_len; i++) {
      free(request->headers[i].field_name.data);
      free(request->headers[i].field_content.data);
    }
    free(request->headers);
    
    free(request);
  }
}

// Return -1 when Request-Line is invalid
int parse_request_line(char* buffer, Request* request) {
  if (request == NULL) {
    return -1;
  }
  
  char* request_line = strtok(buffer, "\r\n");  
  if (request_line == NULL) {
    return -1;
  }
  printf("Request-Line: %s\n", request_line);  
  
  char* method = strtok(request_line, " "); 
  if (method == NULL) {
    return -1;
  }
 
  request->method.len = strlen(method);
  request->method.data = strdup(method);
  if (request->method.data == NULL) {
    return -1;
  }
  
  char* uri = strtok(NULL, " "); 
  if (uri == NULL) {
    return -1;
  }

  request->request_uri.len = strlen(uri);
  request->request_uri.data = strdup(uri);
  if (request->request_uri.data == NULL) {
    return -1;
  }
  
  char* http_version = strtok(NULL, " ");
  if (http_version == NULL) {
    return -1;
  }

  request->http_version.len = strlen(http_version);
  request->http_version.data = strdup(http_version);
  if (request->http_version.data == NULL) {
    return -1;
  }
   
  return 1;
}

int validate_http_version(const String* http_version) {
  if (http_version->data == NULL) {
    return -1;
  }
  
  if (strcmp(http_version->data, "HTTP/1.1") == 0) {
      return 1;  
  } else {
    printf("Got %s\n", http_version->data); 
    return -1;
  }
}

// Only return -1 when Request-Line is invalid
int validate_request_line(Request* request) { 
  if (request == NULL) {
    return -1;
  }
  
  // Check HTTP Version
  if (validate_http_version(&request->http_version) < 0) {
    return -1;
  }
  // Check Method


  // Check URI
  

  return 1;
}

// Return -1 only when client closes connection 
int handle_requests(int client_fd, const char* buffer, const ssize_t buffer_size) {   
    Request* request = allocate_request(); 
    
    printf("Received message from client: \n%s\n", buffer);
      
    if (parse_request_line((char*)buffer, request) < 0) {
      fprintf(stderr, "Invalid Request-Line"); 
      return -1;
    }
    validate_request_line(request);  
    char response[] =
      "HTTP/1.1 200 OK\r\n"
      "Content-Length: 13\r\n"
      "Connection: close\r\n"  // Tell client to close connection
      "Content-Type: text/plain\r\n\r\n"
      "response";
     
    write(client_fd, response, strlen(response));  
    
    free_request(request);
    
    // for now
    return -1;
}
