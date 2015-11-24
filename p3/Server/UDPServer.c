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
#define MAXBUFFLEN 256


int checkRequestIDRange(int ID);
struct msg
{
	unsigned short magicNumber;
	unsigned short TML;
	char checksum;
	char GID;
	char requestID;
	unsigned char data[1024];
} __attribute__((__packed__));
typedef struct msg msg_t;

typedef struct {
    int size;      // slots used so far
    int capacity;  // total available slots
    int *data;     // array of integers we're storing
} Vector;

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
    Vector ip_addresses;
    vector_init(&ip_addresses);
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	/*Check for correct terminal usage for this program. 
	  It should be UDPClient Servername Port# requestID hostname1 hostname2 ... hostnameX
	  See assignment paper for details
	*/
	if(argc > 2)
	{
		fprintf(stderr, "\nusage error: argc = %d\tUDPServer ServerPort#\n", argc);
		exit(1);
	}

	char* serverPort = argv[1];



	memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, serverPort, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 2;
  }

  freeaddrinfo(servinfo);

  printf("listener: waiting to recvfrom...\n");
    while (1) {
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }
        if (hasMagicNumber(buf) && isCorrectLength(buf)) {
            if(hasClient(ip_addresses)) {
                sendClientWaitingMessage(buf);
            }
            else {
                sendNoClientMessage(buf);
            }
        }
        else {
            sendErrorMessage(buf);
        }

    }

  


	return 0;
 }

bool hasMagicNumber(char[] bufIn) {
    if(bufIn[0] == 0xa5 && bufIn[1] == 0xa5) return true;
    else return false;
}

bool isCorrectLength(char[] bufIn) {
    int size = sizeof(bufIn);
    if(size == 5) return true;
    else return false;
}

bool hasClient(Vector ip_addresses_in) {
    if(ip_addresses_in->size > 0) return true;
    else return false;
}


void vector_init(Vector *vector) {
    // initialize size and capacity
    vector->size = 0;
    vector->capacity = VECTOR_INITIAL_CAPACITY;
    
    // allocate memory for vector->data
    vector->data = malloc(sizeof(int) * vector->capacity);
}

void vector_append(Vector *vector, int value) {
    // make sure there's room to expand into
    vector_double_capacity_if_full(vector);
    
    // append the value and increment vector->size
    vector->data[vector->size++] = value;
}

int vector_get(Vector *vector, int index) {
    if (index >= vector->size || index < 0) {
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    return vector->data[index];
}

void vector_set(Vector *vector, int index, int value) {
    // zero fill the vector up to the desired index
    while (index >= vector->size) {
        vector_append(vector, 0);
    }
    
    // set the value at the desired index
    vector->data[index] = value;
}

void vector_double_capacity_if_full(Vector *vector) {
    if (vector->size >= vector->capacity) {
        // double vector->capacity and resize the allocated memory accordingly
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(int) * vector->capacity);
    }
}

void vector_free(Vector *vector) {
    free(vector->data);
}

