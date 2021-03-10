#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[]) {
    char *message;
    char *response;
    int sockfd;

    
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    
    // Ex 1.1: GET dummy from main server
    
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/dummy", NULL,
        NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 1.2: POST dummy and print response from main server
    char** body_data = malloc(1 * sizeof(char*));
    body_data[0] = calloc(LINELEN, sizeof(char));
    strcpy(body_data[0], "hello=world");

    message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/dummy",
        "application/x-www-form-urlencoded", body_data, 1, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Ex 2: Login into main server
    memset(body_data[0], 0, LINELEN);
    strcpy(body_data[0], "username=student&password=student");

    message = compute_post_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/auth/login",
        "application/x-www-form-urlencoded", body_data, 1, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // extragerea cookie-ului
    char cookie[LINELEN];
    memset(cookie, 0, LINELEN);

    char *start = strstr(response, "Set-Cookie: ");
    start += strlen("Set-Cookie: ");
    char *end = strstr(start, "\r\n");
    strncpy(cookie, start, end - start);
    printf("Isolated the cookie: <%s>\n\n\n", cookie);

    // Ex 3: GET key from main server
    memset(body_data[0], 0, LINELEN);
    strcpy(body_data[0], cookie);
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/weather/key",
        0, body_data, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    puts("");

    // Logout
    memset(body_data[0], 0, LINELEN);
    strcpy(body_data[0], cookie);
    message = compute_get_request("ec2-3-8-116-10.eu-west-2.compute.amazonaws.com", "/api/v1/auth/logout",
        NULL, body_data, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // Close the main server
    close_connection(sockfd);

    free(body_data[0]);
    free(body_data);
    return 0;
}
