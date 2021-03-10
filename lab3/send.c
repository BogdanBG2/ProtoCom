#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[])
{
	msg t;
	msg* win = calloc(WINDOW_SIZE, sizeof(msg));
	int win_len = 0;
	int i, res;
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	/* printf("[SENDER]: BDP=%d\n", atoi(argv[1])); */
	for (i = 0; i < COUNT; ++i) {
		/* cleanup msg */
		memset(&t, 0, sizeof(msg));

		/* gonna send an empty msg */
		t.len = MSGSIZE;
		/* send msg */
		sprintf(t.payload, "Package #%d", i+1);
		if(win_len < WINDOW_SIZE) {
			strcpy(win[win_len].payload, t.payload);
			win_len++;

			printf("[SENDER] Sent: %s\n", t.payload);
			res = send_message(win);
			if (res < 0) {
				perror("[SENDER] Send error. Exiting.\n");
				return -1;
			}
			continue;
		}

		/*
		res = send_message(win);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
		printf("[SENDER] Sent: %s\n", win[0].payload);
		*/

		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
		printf("[SENDER] Received back: %s\n", t.payload);
		for(int j = 0; j < WINDOW_SIZE - 1; ++j)
			strcpy(win[j].payload, win[j+1].payload);
		win_len--;
	}
	printf("[SENDER] Job done, all sent.\n");

	free(win);
	return 0;
}