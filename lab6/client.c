/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd, sock, num_bytes;
	struct sockaddr_in to_station;
	char buf[BUFLEN];
	memset(buf, 0, BUFLEN);


	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
		perror("Socket creation failed");

	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(12345);
	inet_aton("127.0.0.1", &(to_station.sin_addr));

	DIE((fd=open(argv[3],O_RDONLY))==-1, "open file");

	sprintf(buf, "%s", argv[3]);
	sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&to_station, sizeof(to_station));

	while((num_bytes = read(fd, buf, BUFLEN - 1))) {
		sendto(sock, buf, num_bytes, 0, (struct sockaddr *)&to_station, sizeof(to_station));
	}

	sprintf(buf, "END_OF_PACKET"); // aici se termina trimiterea catre server 
	sendto(sock, buf, 14, 0, (struct sockaddr *)&to_station, sizeof(to_station));
	
	close(sock);
	close(fd);

	return 0;
}