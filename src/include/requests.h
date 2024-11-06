#ifndef REQUESTS_H
#define REQUESTS_H

#include <sys/_types/_ssize_t.h>
#include <unistd.h>

typedef struct {
  char*     data;
  size_t    len;
} String;

// temporary
typedef struct {
  String     field_name;
  String     field_content;
} Header;

typedef struct {
  String    method;
  String    request_uri;
  String    http_version;
 
  Header*   headers;
  size_t    headers_len;
} Request;

int handle_requests(int client_fd, const char* buffer, const ssize_t buffer_size);

#endif // REQUETS_H
