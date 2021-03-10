/*
 * Protocoale de comunicatii
 * Laborator 11 - E-mail
 * send_mail.c
 */

#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define SMTP_PORT 25
#define MAXLEN 500
#define FROM "bogdandrei04@gmail.com"
#define TO "radu.ciobanu@gmail.com"

#define USER_BASE64     "MzI3ZjcyZGRiNjYwZmI="
#define PASS_BASE64     "MTllYzkxMjFmYmE0NDc="

#define DIE(condition, description) \
    if(condition) { \
        perror(description); \
        exit(EXIT_FAILURE); \
    }

/**
 * Citeste maxim maxlen octeti de pe socket-ul sockfd in
 * buffer-ul vptr. Intoarce numarul de octeti cititi.
 */
ssize_t read_line(int sockd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *buffer;

    buffer = vptr;

    for (n = 1; n < maxlen; n++) {
        if ((rc = read(sockd, &c, 1)) == 1) {
            *buffer++ = c;

            if (c == '\n') {
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0;
            } else {
                break;
            }
        } else {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }
    }

    *buffer = 0;
    return n;
}

/**
 * Trimite o comanda SMTP si asteapta raspuns de la server. Comanda
 * trebuie sa fie in buffer-ul sendbuf. Sirul expected contine
 * inceputul raspunsului pe care trebuie sa-l trimita serverul
 * in caz de succes (de exemplu, codul 250). Daca raspunsul
 * semnaleaza o eroare, se iese din program.
 */
void send_command(int sockfd, const char sendbuf[], char *expected)
{
    char recvbuf[MAXLEN];
    int nbytes;
    char CRLF[2] = {13, 10};

    printf("Trimit: %s\n", sendbuf);
    write(sockfd, sendbuf, strlen(sendbuf));
    write(sockfd, CRLF, sizeof(CRLF));

    nbytes = read_line(sockfd, recvbuf, MAXLEN - 1);
    recvbuf[nbytes] = 0;
    printf("Am primit: %s", recvbuf);

    if (strstr(recvbuf, expected) != recvbuf) {
        printf("Am primit mesaj de eroare de la server!\n");
        exit(-1);
    }
}

int main(int argc, char **argv) {
    int sockfd, ret;
    int port = SMTP_PORT;
    struct sockaddr_in servaddr;
    char server_ip[INET_ADDRSTRLEN];
    char sendbuf[MAXLEN]; 
    char recvbuf[MAXLEN];

    if (argc != 3) {
        printf("Utilizare: ./send_msg adresa_server nume_fisier\n");
        exit(-1);
    }

    strncpy(server_ip, argv[1], INET_ADDRSTRLEN);

     // TODO 1: creati socket-ul TCP client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    // TODO 2: completati structura sockaddr_in cu
    // portul si adresa IP ale serverului SMTP
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(port);

    // TODO 3: conectati-va la server
    DIE(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0, "Cannot connect to the server.\n");

    // se primeste mesajul de conectare de la server
    memset(recvbuf, 0, MAXLEN);
    read_line(sockfd, recvbuf, MAXLEN - 1);
    printf("Am primit: %s\n", recvbuf);
    
    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "HELO localhost");
    send_command(sockfd, sendbuf, "250");
        
    sprintf(sendbuf, USER_BASE64);
    send_command(sockfd, sendbuf, "334");

    sprintf(sendbuf, PASS_BASE64);
    send_command(sockfd, sendbuf, "235");

    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "MAIL FROM: <%s>", FROM);
    send_command(sockfd, sendbuf, "250");

    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "RCPT TO: <%s>", TO);
    send_command(sockfd, sendbuf, "250");

    memset(sendbuf, 0, MAXLEN);
    strcpy(sendbuf, "DATA");
    send_command(sockfd, sendbuf, "354");

    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf,
        "MIME-Version: 1.0\n"
        "From: %s\n"
        "To: %s\n"
        "Subject: Testare E-Mail\n"
        "Content-Type: multipart/mixed; boundary=bogdan\n\n"
        "--bogdan\n"
        "Content-Type: text/plain\n\n"
        "Merge?\n\n"
        "--bogdan\n"
        "Content-Type: text/plain\n"
        "Content-Disposition: attachment; filename=\"file.txt\"\n\n"
        "TEST 29042020\n\n"
        "--bogdan\r\n.", FROM, TO);

    // trimiteti e-mail-ul (antete + corp + atasament)
    send_command(sockfd, sendbuf, "250");

    // trimiteti comanda de QUIT
    memset(sendbuf, 0, MAXLEN);
    strcpy(sendbuf, "QUIT");
    send_command(sockfd, sendbuf, "221");

    // inchideti socket-ul TCP client
    close(sockfd);

    return 0;
}
