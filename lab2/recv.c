#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

void recieve_message(msg t)
{
  int dest;
  dest = open("testB.txt", O_TRUNC | O_WRONLY);
  if(dest < 0)
  {
    perror("recieve_message : Invalid output file\n");
    exit(-3);
  }
  /*
  for(int i = 0; t.payload[i] != 0; ++i)
    if(write(dest, t.payload + i, 1) < 0)
    {
      perror("recieve_message : Data writing error\n");
      exit(-3);
    }
  */
  if(write(dest, t.payload, t.len) < 0)
  {
    perror("recieve_message : Data writing error\n");
    exit(-3);
  }
  close(dest);
}

int main(int argc,char** argv){
  msg r;
  init(HOST,PORT);

  if (recv_message(&r) < 0) {
    perror("Receive message");
    return -1;
  }
  printf("[recv] Got msg with payload: <%s>, sending ACK...\n", r.payload);
  recieve_message(r);

  // Send ACK:
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");

  return 0;
}