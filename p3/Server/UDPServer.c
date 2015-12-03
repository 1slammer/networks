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
#include <stdbool.h>

#define SERVERPORT "10019"  // the port users will be connecting to
#define MAXBUFFLEN 256
#define GID_C 9

//Function Prototypes

bool hasMagicNumber( unsigned char bufIn[] );
bool isCorrectLength( unsigned char bufIn[], int numBytesIn) ;
bool hasClient(unsigned long ip_in_wait);
bool portIsInRange( unsigned char bufIn[]) ;
bool sendClientWaitingMessage(unsigned char bufIn[], unsigned long ip_in, unsigned short port, int sockfd, struct sockaddr_in their_addr);
bool sendNoClientMessage(unsigned char bufin[], unsigned short port, int sockfd, struct sockaddr_in their_addr);
void sendErrorMessage(unsigned char bufIn[], int sockfd, struct addrinfo *, int numbytesIn);
void sendBadNumMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p);
void sendBadLengthMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p);
void sendBadPortMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p);




struct msg
{
	unsigned short magicNumber;
    unsigned long ip_address;
    unsigned short port;
	char GID;
} __attribute__((__packed__));
typedef struct msg msg_t;

struct msg_w
{
    unsigned short magicNumber;
    unsigned short port;
    char GID;
} __attribute__((__packed__));
typedef struct msg_w msg_wt;

struct error_msg
{
    unsigned short magicNumber;
    char GID;
    unsigned short err;
} __attribute__((__packed__));
typedef struct error_msg error_msg_t;


int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
    int numbytes;
	struct sockaddr_in their_addr;
	char buf[MAXBUFFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
    unsigned long ip_in_wait = 0;
    unsigned short wait_port;

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


while (1) 
{

	memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, serverPort, &hints, &servinfo)) != 0) 
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) 
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
    {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if (p == NULL) 
  {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 2;
  }

  freeaddrinfo(servinfo);

  printf("listener: waiting to recvfrom...\n");
    //while (1) {
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) 
        {
            perror("recvfrom");
            exit(1);
        }
        unsigned long ip_address = their_addr.sin_addr.s_addr;
        if (hasMagicNumber(buf) && isCorrectLength(buf, numbytes) && portIsInRange(buf)) 
        {
            if(hasClient(ip_in_wait)) 
            {
                if(sendClientWaitingMessage(buf, ip_in_wait, their_addr.sin_port, sockfd, their_addr)){
					printf("test1\n");
                    //freeaddrinfo(servinfo);
                    ip_in_wait = 0;
                }
                else 
                {
					printf("test2\n");
                    perror("listener: sendto");
                    exit(1);

                }
            }
            else 
            {
					printf("test3\n");
                if (sendNoClientMessage(buf, their_addr.sin_port, sockfd, their_addr)) {
                    //freeaddrinfo(servinfo);
					ip_in_wait = 1;
                }
                else 
                {
					printf("test4\n");
                    perror("listener: sendto");
                    exit(1);
                    
                }

				printf("test5\n");
                ip_in_wait = ip_address;
                wait_port = their_addr.sin_port;
            }
        }
        else {
					printf("test6\n");
            sendErrorMessage(buf, sockfd, p, numbytes);
        }

		close(sockfd);
    }

  



	return 0;
 }

bool hasMagicNumber(unsigned char bufIn[]) 
{
	//printf("bufIn[0] %x\n", bufIn[0]);
	//printf("true? %d\n", bufIn[0] == 0xa5); 
	//printf("test: %d\n", sizeof bufIn[0]);
	if(bufIn[0] == 0xa5 && bufIn[1] == 0xa5) return true;
    else return false;
}

bool isCorrectLength( unsigned char bufIn[], int numBytesIn) 
{
	//printf("length %d", numBytesIn);
    if(numBytesIn == 5) return true;
    else return false;
}

bool hasClient(unsigned long ip_in_wait) 
{
	printf("hasClient\n");
	printf("%d", ip_in_wait>0);
    if(ip_in_wait > 0) return true;
    else return false;
}
bool portIsInRange(unsigned char bufIn[]) 
{
	printf("port %x\n", bufIn[2]);
	printf("port %x\n", bufIn[3]);
    int num = (bufIn[2] << 8) + bufIn[3];
	printf("num: %d\n", num);
    //num = num * 5;
    //num = num + 10010;
    if(num >= 10055 && num <= 10059) 
    {
		printf("good port num\n");
		return true;
	}
    else return false;
    
}

bool sendClientWaitingMessage(unsigned char bufIn[], unsigned long ip_in, unsigned short port, int sockfd, struct sockaddr_in their_addr) 
{
	printf("sendClientWaiting\n");
    msg_t msg_out;
    int numbytes;
    msg_out.magicNumber = 0xa5a5;
    msg_out.ip_address = htonl(ip_in);
    msg_out.port = htons(port);
    msg_out.GID = GID_C;
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }
    return true;
    
}
bool sendNoClientMessage(unsigned char bufIn[], unsigned short port, int sockfd, struct sockaddr_in their_addr) {
    //port = (bufIn[2] << 8) + bufIn[3];
	printf("send port: %d", port);
    msg_wt msg_out;
    int numbytes;
    msg_out.magicNumber = 0xa5a5;
    msg_out.port = htons(port);
    msg_out.GID = GID_C;
	printf("size of msg out: %d", sizeof(msg_out));
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           (struct sockaddr *)&their_addr, sizeof their_addr)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }
    return true;

}

void sendErrorMessage(unsigned char bufIn[], int sockfd, struct addrinfo *p, int numbytesIn) 
{
    if(!hasMagicNumber(bufIn)) 
    {
        sendBadNumMsg(bufIn, sockfd, p);
    }
    else if(!isCorrectLength(bufIn, numbytesIn))
    {
        sendBadLengthMsg(bufIn, sockfd, p);
    }
    else if(!portIsInRange(bufIn)) 
    {
        sendBadPortMsg(bufIn, sockfd, p);
    }
}

void sendBadNumMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p) 
{
    error_msg_t msg_out;
    msg_out.magicNumber = 0xa5a5;
    msg_out.GID = GID_C;
    int numbytes;
    msg_out.err = 0x0001;
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           p->ai_addr, p->ai_addrlen)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }

}

void sendBadLengthMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p) 
{
    error_msg_t msg_out;
    msg_out.magicNumber = 0xa5a5;
    msg_out.GID = GID_C;
    int numbytes;
    msg_out.err = 0x0002;
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           p->ai_addr, p->ai_addrlen)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }
    
}

void sendBadPortMsg(unsigned char bufIn[], int sockfd, struct addrinfo *p) 
{
    error_msg_t msg_out;
    msg_out.magicNumber = 0xa5a5;
    msg_out.GID = GID_C;
    int numbytes;
    msg_out.err = 0x0004;
    if ((numbytes = sendto(sockfd, &msg_out, sizeof(msg_out), 0,
                           p->ai_addr, p->ai_addrlen)) == -1)
    {
        perror("listener: sendto");
        exit(1);
    }
    
}




