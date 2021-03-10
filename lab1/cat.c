#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void fatal(char* err_msg)
{
	perror(err_msg);
	exit(-1);
}


int main()
{
	int src, dest;
	int copy;
	char buffer[1024];

	src = open("src1.txt", O_RDONLY);
	if(src < 0)
		fatal("Nu putem deschide acest fisier");
	lseek(src, 0, SEEK_SET);


	while((copy = read(src, buffer, sizeof(buffer))))
	{
		if(copy < 0)
			fatal("Eroare la citire");
		copy = write(STDOUT_FILENO, buffer, copy);
		if(copy < 0)
			fatal("eroare la scriere");
	}
	close(src);
	return 0;
}

/*
E exact acceasi idee ca la exemplul din laborator.
Singura diferenta e ca datele nu se scriu intr-un fisier dat,
ci in STDOUT.
*/