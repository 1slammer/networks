/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "10019"  // the port users will be connecting to

struct msg
{
    unsigned char size;
    unsigned char request_id;
    unsigned char operation;
    unsigned char data[256];
} __attribute__((__packed__));

 typedef struct msg msg_t;


int main(int argc, char *argv[])
{
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;

  if (argc != 6) {
    fprintf(stderr,"usage: client servername PortNumber Operation String\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(argv[2], argv[3], &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and make a socket
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    return 2;
  }

  msg_t message;
  printf("sizeof(message): %d\n", (int)sizeof(message));
  message.request_id = 1;
  printf("sizeof(message.request_id): %d\n", (int)sizeof(message.request_id));
  message.operation = 5;
  printf("sizeof(message.operation): %d\n", (int)sizeof(message.operation));
  unsigned char t[5] = {'a', 'w', 'd', 'x', 'w'};
  printf("sizeof(t): %d\n", (int)sizeof(t));
  int i;
  for (i=0; i<sizeof(t); i++)
      message.data[i] = t[i];
  //message.data = &t;
  printf("sizeof(message.data): %d\n", (int)sizeof(message.data));
  message.size = sizeof(message.request_id) + sizeof(message.operation)
      + sizeof(t) + sizeof(message.size);
  printf("sizeof(message): %d\n", (int)message.size);
  if ((numbytes = sendto(sockfd, &message, message.size, 0,
       p->ai_addr, p->ai_addrlen)) == -1) {
    perror("talker: sendto");
    exit(1);
  }

  freeaddrinfo(servinfo);

  printf("talker: sent %d bytes to %s\n", numbytes, argv[2]);

  char *buf[20];
  struct sockaddr_storage sender;
  socklen_t sendsize = sizeof(sender);

  int response;
  response = recvfrom(sockfd, &buf, sizeof(buf), 0, 
          (struct sockaddr *)&sender, &sendsize);

  close(sockfd);
  return 0;
}
