#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void fatal(char* err_msg)
{
	perror(err_msg);
	exit(-1);
}


int main()
{
/*
	//int src, copy;
	char buffer[1024];
	char text[1024][1024];
	int lines = 0;

	
	FILE* src = fopen("src1.txt", "r");
	if(!src)
		fatal("Nu putem deschide acest fisier");
	
	
    while(fgets(buffer, 1024, src) > 0)
        strcpy(text[lines++], buffer);
    for(int i = lines - 1; i >= 0; --i)
        printf("%s", text[i]);

    fclose(src);
*/

	int src, dest;
	int copy;
	//char buffer;
	char character;
	char text[1024][1024];

	src = open("src1.txt", O_RDONLY);
	if(src < 0)
		fatal("Nu putem deschide acest fisier");
	int fsize = lseek(src, -1, SEEK_END);

	/*
	while(copy = read(src, &buffer, 1))
	{
		if(copy < 0)
			fatal("Eroare la citire");
		copy = write(STDOUT_FILENO, &buffer, copy);
		if(copy < 0)
			fatal("Eroare la scriere");
	}
	*/

	for(int i = fsize - 1; i >= 0; --i)
	{
		lseek(src, i, SEEK_SET); 
		if(read(src, &character, 1) < 0)
			fatal("Eroare la citirea <--");
		
		if(character == '\n')
		{
			int crt_pos = lseek(src, i + 1, SEEK_SET);
			
			if(read(src, &character, 1) < 0)
				fatal("Eroare la citirea de start");
			
			printf("Endline la %d.\n", crt_pos);
			printf("Primul caracter: %c\n", character);
			
			
			while(character != '\0' || character != '\n')
			{
				printf("%d ", character);
				if(write(STDOUT_FILENO, &character, 1) < 0)
					fatal("Eroare la scriere");
				
				//lseek(src, j - 1, SEEK_CUR);		
				if(read(src, &character, 1) < 0)
					fatal("Eroare la citirea -->");
			}
			//lseek(src, crt_pos - 2, SEEK_SET);
			
			lseek(src, crt_pos - 1, SEEK_SET);
		}
		
		/*
		if(write(STDOUT_FILENO, &character, 1) < 0)
			fatal("Eroare la scriere");
		*/
	}
			
	printf("\n");
	close(src);
	return 0;
}
