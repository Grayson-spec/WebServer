#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <sys/socket.h>

// Structure to represent an HTTP request
typedef struct {
    char method[10];
    char url[100];
} http_request_t;

// Structure to represent an HTTP response
typedef struct {
    int status_code;
    char headers[1000];
    char body[1000];
} http_response_t;

// Function to parse incoming HTTP requests
http_request_t* parse_http_request(char* request);

// Function to handle HTTP GET requests
http_response_t* handle_get_request(http_request_t* request);

// Function to handle HTTP POST requests
http_response_t* handle_post_request(http_request_t* request);

// Function to send HTTP responses
void send_response(int client_fd, http_response_t* response);

// Function to handle client requests
void handle_client(int client_fd);

#endif  // CLIENT_HANDLER_H