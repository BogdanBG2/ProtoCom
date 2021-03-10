/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
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
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	int sock;
	int len = 0;
	struct sockaddr_in my_sockaddr, from_station;
	char buf[BUFLEN];
	memset(buf, 0, BUFLEN);

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
		perror("Socket creation failed");
	
	from_station.sin_family = AF_INET;
	from_station.sin_port = htons(12345);
	from_station.sin_addr.s_addr = INADDR_ANY;
	
	bind(sock, (struct sockaddr *)&from_station, sizeof(from_station));

	recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&from_station, len);

	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");

	while(1){
		recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&from_station, len);
		if (!strcmp(buf, "END_OF_PACKET")) // sfarsitul pachetului
			break;
		
		write(fd, buf, strlen(buf));
	}

	close(sock);
	close(fd);

	return 0;
}
