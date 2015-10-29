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

static struct timeval t1;

static inline void start() {
	gettimeofday(&t1, NULL);
}

static inline void stop() {
	struct timeval t2;
	gettimeofday(&t2, NULL);

	unsigned long long t = 1000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
	printf("Round Trip Time: %llu microseconds\n", t);
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	if (argc != 5) {
		fprintf(stderr,"usage: ./prog servername PortNumber Operation String\n");
		exit(1);
	}

	char* progName = argv[0];
	char* serverName = argv[1];
	char* portNumber = argv[2];
	char* operation = argv[3];
	char* msgString = argv[4];

	// Convert character of integer from command line parameter to a
	// hex value in char*
	*operation = (char)atoi(operation);

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
			p->ai_protocol)) == -1) 
		{
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
	for (i=0; i<strlen(msgString); i++) {
		message.data[i] = msgString[i];
	}

	message.size = sizeof(message.request_id) + sizeof(message.operation)
		+ strlen(msgString) + sizeof(message.size);

	if ((numbytes = sendto(sockfd, &message, message.size, 0,
		p->ai_addr, p->ai_addrlen)) == -1)
	{
		perror("talker: sendto");
		exit(1);
	}
	start();

	freeaddrinfo(servinfo);

	char buf[1024];
	struct sockaddr_storage sender;
	socklen_t sendsize = sizeof(sender);

	int response;
	response = recvfrom(sockfd, &buf, sizeof(buf), 0, 
		(struct sockaddr *)&sender, &sendsize);
	stop();
	char* yes = (char*)buf;
	printf("Response ID: %d\n", buf[1]);
	printf("Response: ");
	if (message.operation == 0x5) {
		printf("%d", buf[2]);
	}
	else {
		for (i=2; i<buf[0]; i++)
			printf("%c", (char) buf[i]);
	}
	printf("\n");

	close(sockfd);
	return 0;
}

