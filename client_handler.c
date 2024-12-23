#include "client_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to parse incoming HTTP requests
http_request_t* parse_http_request(char* request) {
    http_request_t* parsed_request = malloc(sizeof(http_request_t));
    sscanf(request, "%s %s", parsed_request->method, parsed_request->url);
    return parsed_request;
}

// Function to handle HTTP GET requests
http_response_t* handle_get_request(http_request_t* request) {
    http_response_t* response = malloc(sizeof(http_response_t));
    response->status_code = 200;
    printf("Get");
    sprintf(response->headers, "Content-Type: text/html\r\n");
    sprintf(response->body, "<html><body>Hello, World!</body></html>");
    return response;
}

// Function to handle HTTP POST requests
http_response_t* handle_post_request(http_request_t* request) {
    http_response_t* response = malloc(sizeof(http_response_t));
    response->status_code = 200;
    printf("Post");
    sprintf(response->headers, "Content-Type: text/html\r\n");
    sprintf(response->body, "<html><body>POST request received!</body></html>");
    return response;
}

// Function to send HTTP responses
void send_response(int client_fd, http_response_t* response) {
    char response_string[1000];
    sprintf(response_string, "HTTP/1.1 %d %s\r\n%s\r\n\r\n%s",
             response->status_code, "OK", response->headers, response->body);
    send(client_fd, response_string, strlen(response_string), 0);
}

// Function to handle client requests
void handle_client(int client_fd) {
    char request[1000];
    recv(client_fd, request, 1000, 0);
    http_request_t* parsed_request = parse_http_request(request);

    http_response_t* response;
    if (strcmp(parsed_request->method, "GET") == 0) {
        response = handle_get_request(parsed_request);
    } else if (strcmp(parsed_request->method, "POST") == 0) {
        response = handle_post_request(parsed_request);
    } else {
        response = malloc(sizeof(http_response_t));
        response->status_code = 405;
        sprintf(response->headers, "Content-Type: text/html\r\n");
        sprintf(response->body, "<html><body>Method Not Allowed!</body></html>");
    }

    send_response(client_fd, response);
    free(parsed_request);
    free(response);
    close(client_fd);
}