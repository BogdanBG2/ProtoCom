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

int main() {
    char command[100];            // comanda citita de la tastatura
    char session_cookie[120];     // cookie-ul de sesiune
    char url[100];                // URL
    char token[300];              // token JWT pentru accesul la librarie
    char* appl_type = "application/json";
    int check;                    // variabila de verificare
                                  // a corectitudinii datelor citite



    char* message;                // mesaj de la client la server
    char* response;               // raspuns de la server la client
    char* server = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    char* payload = calloc(1000, sizeof(char));
    char* line = calloc(400, sizeof(char));
    int sockfd; // socket file descriptor

    memset(command, 0, 100);
    while(69 % 3 == 420 % 3) {
    	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("Could not open the connection.\n");
        
        fgets(command, 100, stdin);
        command[strlen(command) - 1] = 0;

        memset(url, 0, sizeof(url));

        if(!strcmp(command, "exit"))
            break;

        else if(!strcmp(command, "register")) {
            // Sunt deja logat?
            if(strcmp(session_cookie, "")) {
                puts("ERROR: You are already logged in.");
                continue;
            }

            char name[20], pass[20];
            
            // Citirea datelor necesare
            printf("username=");
            fgets(name, 20, stdin);
            name[strlen(name) - 1] = 0;

            printf("password=");
            fgets(pass, 20, stdin);
            pass[strlen(pass) - 1] = 0;

            // Generarea payload-ului
            memset(payload, 0, 1000);
            compute_message(payload, "{");
            sprintf(line, "    \"username\": \"%s\",", name);
            compute_message(payload, line);
            sprintf(line, "    \"password\": \"%s\"", pass);
            compute_message(payload, line);
            compute_message(payload, "}");
            
            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/auth/register");
            message = compute_post_request(server, url, 
               appl_type, &payload, 1, NULL, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        else if(!strcmp(command, "login")) {
            // Sunt deja logat?
            if(strcmp(session_cookie, "")) {
                puts("ERROR: You are already logged in.");
                continue;
            }

            char name[20], pass[20];
            
            // Citirea datelor necesare
            printf("username=");
            fgets(name, 20, stdin);
            name[strlen(name) - 1] = 0;

            printf("password=");
            fgets(pass, 20, stdin);
            pass[strlen(pass) - 1] = 0;

            // Generarea payload-ului
            memset(payload, 0, 1000);
            compute_message(payload, "{");
            sprintf(line, "    \"username\": \"%s\",", name);
            compute_message(payload, line);
            sprintf(line, "    \"password\": \"%s\"", pass);
            compute_message(payload, line);
            compute_message(payload, "}");

            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/auth/login");
            message = compute_post_request(server, url,\
            	appl_type, &payload, 1, "", "");
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);


            // Extragerea cookie-ului ("prajiturei") din raspuns
            char *start = strstr(response, "Set-Cookie: ");
            if(start == NULL)
                continue;
            start += strlen("Set-Cookie: ");
            char *end = strstr(start, ";");
            strncpy(session_cookie, start, end - start);
            
            // Un utilizator proaspat conectat nu are acces la biblioteca
            if(token != NULL)
                memset(token, 0, 300);
        }

        else if(!strcmp(command, "enter_library")) {
            // Sutn logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }

            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/library/access");
            message = compute_get_request(server, url, "", session_cookie, "");
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            // extragerea token-ului ("monedei") JWT din raspuns
            char* start = strstr(response, "{\"token\":\"");
            if(start == NULL)
                continue;
            start += strlen("{\"token\":\"");
            char* end = strstr(start, "\"}");
            strncpy(token, start, end - start);
        }

        else if(!strcmp(command, "get_books")) {
            // Sunt logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }
            // Am acces la librarie?
            if(!strcmp(token, "")) {
                puts("ERROR: You are not in the library.");
                continue;
            }

            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/library/books");
            message = compute_get_request(server, url, NULL, session_cookie, token);
            
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        else if(!strcmp(command, "get_book")) {
            // Sunt logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }
            // Am acces la librarie?
            if(!strcmp(token, "")) {
                puts("ERROR: You are not in the library.");
                continue;
            }

            int book_id;

            // Citirea datelor necesare
            printf("id=");
            check = scanf("%d", &book_id);
            if(check < 0 || book_id < 0) {
                puts("ERROR: Invalid input.");
                continue;
            }

            // Trimiterea cererii
            sprintf(url, "/api/v1/tema/library/books/%d", book_id);
            message = compute_get_request(server, url, NULL, session_cookie, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        else if(!strcmp(command, "add_book")) {
            // Sunt logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }
            // Am acces la librarie?
            if(!strcmp(token, "")) {
                puts("ERROR: You are not in the library.");
                continue;
            }

            char title[50], author[30], genre[30], publisher[30];
            int page_count;

            // Citirea datelor necesare
            printf("title=");
            fgets(title, 50, stdin);
            title[strlen(title) - 1] = 0;
            
            printf("author=");
            fgets(author, 30, stdin);
            author[strlen(author) - 1] = 0;
            
            printf("genre=");
            fgets(genre, 30, stdin);
            genre[strlen(genre) - 1] = 0;
            
            printf("publisher=");
            fgets(publisher, 30, stdin);
            publisher[strlen(publisher) - 1] = 0;
            
            printf("page_count=");
            check = scanf("%d", &page_count);
            if(check < 0 || page_count < 0) {
                puts("ERROR : Could not read the number of pages.");
                continue;
            }

            // Generarea payload-ului
            memset(payload, 0, 1000);
            compute_message(payload, "{");
            sprintf(line, "    \"title\": \"%s\",", title);
            compute_message(payload, line);
            sprintf(line, "    \"author\": \"%s\",", author);
            compute_message(payload, line);
            sprintf(line, "    \"genre\": \"%s\",", genre);
            compute_message(payload, line);
            sprintf(line, "    \"page_count\": %d,", page_count);
            compute_message(payload, line);
            sprintf(line, "    \"publisher\": \"%s\"", publisher);
            compute_message(payload, line);
            compute_message(payload, "}");
            printf("%s", payload);

            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/library/books");
            message = compute_post_request(server, url, 
                appl_type, &payload, 1, NULL, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        else if(!strcmp(command, "delete_book")) {
			// Sunt logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }
            // Am acces la librarie?
            if(!strcmp(token, "")) {
                puts("ERROR: You are not in the library.");
                continue;
            }

            int book_id;

            // Citirea datelor necesare
            printf("id=");
            check = scanf("%d", &book_id);
            if(check < 0 || book_id < 0) {
                puts("ERROR: Invalid input.");
                continue;
            }
            
            // Trimiterea cererii
            sprintf(url, "/api/v1/tema/library/books/%d", book_id);
            message = compute_delete_request(server, url, NULL, session_cookie, token);

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        else if(!strcmp(command, "logout")) {
            // Sunt logat?
            if(!strcmp(session_cookie, "")) {
                puts("ERROR: You are not logged in.");
                continue;
            }
            
            // Trimiterea cererii
            strcpy(url, "/api/v1/tema/auth/logout");
            message = compute_get_request(server, url, NULL, session_cookie, NULL);
            
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            // Resetarea cookie-ului si a token-ului
            memset(token, 0, 300);
            memset(session_cookie, 0, 120);
        }
    }

    // eliberarea memoriei de pe Heap / tentativa de valgrind
    free(payload);
    free(message);
    free(line);

    // inchiderea conexiunii cu server-ul
    close(sockfd);

    // terminarea programului
    return 0;
}
