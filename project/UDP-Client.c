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

  char* progName = argv[0];
  char* clientName = argv[1];
  char* serverName = argv[2];
  char* portNumber = argv[3];
  char* operation = argv[4];
  char* msgString = argv[5];

  printf("progName: %s\n", progName);
  printf("clientName: %s\n", clientName);
  printf("serverName: %s\n", serverName);
  printf("portNumber: %s\n", portNumber);
  printf("operation: %s\n", operation);
  printf("msgString: %s\n", msgString);

  // Convert character of integer from command line parameter to a
  // hex value in char*
  *operation = (char)atoi(operation);
  printf("operation: %x\n", *operation);
  printf("afterwards\n");

  //sprintf(operation, "%x", test);
  //int test = atoi(operation);
  //*operation = (char)test;
  //*operation = 0x50;


  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(serverName, portNumber, &hints, &servinfo)) != 0) {
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
  message.request_id = 1;
  message.operation = *operation;
  int i;
  for (i=0; i<strlen(msgString); i++)
      message.data[i] = msgString[i];
  message.size = sizeof(message.request_id) + sizeof(message.operation)
      + strlen(msgString) + sizeof(message.size);

  printf("sizeof(message): %d\n", (int)sizeof(message));
  printf("sizeof(message.request_id): %d\n", (int)sizeof(message.request_id));
  printf("sizeof(message.operation): %d\n", (int)sizeof(message.operation));
  printf("message.operation: %x\n", message.operation);
  printf("sizeof(msgString): %d\n", (int)strlen(msgString));
  printf("sizeof(message.data): %d\n", (int)sizeof(message.data));
  printf("sizeof(message): %d\n", (int)message.size);

  if ((numbytes = sendto(sockfd, &message, message.size, 0,
       p->ai_addr, p->ai_addrlen)) == -1) {
    perror("talker: sendto");
    exit(1);
  }

  freeaddrinfo(servinfo);

  printf("talker: sent %d bytes to %s\n", numbytes, argv[2]);

  char *buf[6];
  struct sockaddr_storage sender;
  socklen_t sendsize = sizeof(sender);

  int response;
  response = recvfrom(sockfd, &buf, sizeof(buf), 0, 
          (struct sockaddr *)&sender, &sendsize);
  char* yes = (char*)buf;
  for (i=0; i<response; i++)
      printf("response: %x\n", yes[i]);
  printf("Recieved Response:\n");
  printf("Request ID: %d\n", response);
  printf("Response: %d\n", buf[0]);
  printf("sizeof(buf[0]): %d\n", sizeof(buf[0]));

  close(sockfd);
  return 0;
}
