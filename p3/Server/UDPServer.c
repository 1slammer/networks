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
#define GID 9


int checkRequestIDRange(int ID);
struct msg
{
	unsigned short magicNumber;
    unsigned long ip;
    unsigned short port;
	char GID;
} __attribute__((__packed__));
typedef struct msg msg_t;

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
    int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
    unsigned long ip_in_wait;

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
        unsigned long ip_address = their_addr.sin_addr.s_addr;
        if (hasMagicNumber(buf) && isCorrectLength(buf)) {
            if(hasClient()) {
                if(sendClientWaitingMessage(buf, ip_in_wait, their_addr.sin_port, sockfd, p)) freeaddrinfo(servinfo);
                else {
                    perror("listener: sendto");
                    exit(1);

                }
            }
            else {
                sendNoClientMessage(buf);
                ip_in_wait = ip_address;
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

bool hasClient() {
    if(sizeof(ip_in_wait > 0)) return true;
    else return false;
}

bool sendClientWaitingMessage(char[] bufIn, unsigned long ip_in, unsigned short port, int sockfd, struct p) {
    msg_t msg_out;
    int numbytes;
    msg_out.magicNumber = 0xa5a5;
    msg_out.ip_address = htonl(ip_in);
    msg_out.port = htons(port);
    msg_out.GID = htons(GID);
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           p->ai_addr, p->ai_addrlen)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }
    return true;
    
}


