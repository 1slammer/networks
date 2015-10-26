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


	/*Check for correct terminal usage for this program. 
	  It should