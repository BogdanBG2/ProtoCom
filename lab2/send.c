#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

void read_message(msg *t) 
{
  int src, copy;
  char buf[1024];
  src = open("testA.txt", O_RDONLY);
  if(src < 0)
  {
    perror("read_message : Invalid input file\n");
    exit(-2);
  }
  lseek(src, 0, SEEK_SET);

  while((copy = read(src, buf, sizeof(buf))))
  {
    if(copy < 0)
    {
      perror("read_message : Data reading error\n");
      exit(-2);
    }
    strcpy((*t).payload, buf);

    (*t).len = strlen((*t).payload);
  }
  strcat((*t).payload, "\0");
  (*t).len = strlen((*t).payload);
  close(src);
}

int main(int argc,char** argv)
{
  init(HOST,PORT);
  msg t;

  //Send dummy message:
  printf("[send] Sending dummy...\n");
  //sprintf(t.payload, "%s", "This is definitely a dummy.");
  //t.len = strlen(t.payload) + 1;
  read_message(&t);
  send_message(&t);
  
  // Check response:
  if (recv_message(&t) < 0)
  {
    perror("Receive error ...");
    return -1;
  }
  else
    printf("[send] Got reply with payload: %s\n", t.payload);

  return 0;
}
