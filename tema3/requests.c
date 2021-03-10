#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char* compute_get_request(char* host, char* url, char* query_params,
        char* cookie, char* token) {
    char* message = calloc(BUFLEN, sizeof(char));
    char* line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL)
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    else
        sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    if(token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    compute_message(message, "");
    free(line);
    
    return message;
}

char* compute_post_request(char* host, char* url, char* content_type,
		char** body_data, int body_data_fields_count,
		char* cookie, char* token) {
    char* message = calloc(BUFLEN, sizeof(char));
    char* line = calloc(LINELEN, sizeof(char));
    char* body_data_buffer = calloc(LINELEN, sizeof(char));

    int total_len = 0;

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    if(token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    if(body_data_fields_count != 0) {
        sprintf(line, "%s", body_data[0]);
        total_len += strlen(body_data[0]);
        strcat(body_data_buffer, line);

        for(int i =1; i < body_data_fields_count; ++i) {
            sprintf(line, "&%s", body_data[i]);
            total_len += 1 + strlen(body_data[i]);
            strcat(body_data_buffer, line);
            i++;
        }
    }

    sprintf(line, "Content-Length: %d", total_len);
    compute_message(message, line);

    

    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }
    compute_message(message, "");

    memset(line, 0, LINELEN);
    compute_message(message, body_data_buffer);

    free(line);
    
    return message;
}

char* compute_delete_request(char* host, char* url, char* query_params,
        char* cookie, char* token) {
    char* message = calloc(BUFLEN, sizeof(char));
    char* line = calloc(LINELEN, sizeof(char));

    if (query_params != NULL)
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    else
        sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    if(token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    if (cookie != NULL) {
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    compute_message(message, "");
    free(line);
    
    return message;
}