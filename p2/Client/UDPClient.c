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

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	/*Check for correct terminal usage for this program. 
	  It should be UDPClient Servername Port# requestID hostname1 hostname2 ... hostnameX
	  See assignment paper for details
	*/
	if(!(argc >= 5))
	{
		fprintf(stderr, "\nusage error: argc = %d\tUDPClient, Servername, port#, requestID, hostname1, hostname2, ..., hostnameX\n", argc);
		exit(1);
	}

	//Initialize struct
	msg_t message;
	message.magicNumber = 0x1234;
	message.GID = 9;

	//Get the server name
	char* serverName = argv[1];

	//Get Port Number
	char* portNumber = argv[2];

	//get Request ID
	checkRequestIDRange(atoi(argv[3]));
	message.requestID = atoi(argv[3]);

	//Get all the hostnames
	int amtOfHostNames = argc - 4;
	char *listOfHostNames[amtOfHostNames];
	char sizeOfHostNames[amtOfHostNames];
	
	int host;
	for(host = 0; host < amtOfHostNames; host++)
	{
		char *hostName = argv[host+4];
		sizeOfHostNames[host] = (char) strlen(hostName);
		printf("Added host name %s\t\t\t\tSize of Hostname: %d\n", hostName, sizeOfHostNames[host]);
		listOfHostNames[host] = hostName;
		
	}

	
	int idx = 0;
	for(host = 0; host <amtOfHostNames; host++)
	{
		printf("Index at %d\n", idx);
		message.data[idx] = (char)sizeOfHostNames[host];
		idx++;
		int i;
		for(i = 0; i < sizeOfHostNames[host]; i++)
		{
			message.data[idx+i] = listOfHostNames[host][i]; 
		}	 	
		idx += sizeOfHostNames[host];
	}
	
	printf("\n");
	host = 0;
	while(message.data[host] != '\0')
	{
		printf("ASCHII NUM: %d \tCharAt %d: %c\n", message.data[host], host, message.data[host]);
		host++;
	}
	printf("\n");

	printf("Size of Struct: %lu\n", sizeof(message));
	
	/*Calculate the Total message length TML*/
	int totalHostNameLength = 0;
	for(host = 0; host < amtOfHostNames; host++)
	{
		totalHostNameLength += sizeOfHostNames[host];
		totalHostNameLength++;
	}

	printf("Total of all host names length: %d\n", totalHostNameLength);

	totalHostNameLength += 7;
	message.TML = totalHostNameLength;

	
	/*Calculate the checksume*/
	char *ptr = (char*)&message;
	int index;
	short sum;
	for(index = 0; index < totalHostNameLength; index++)
	{
		//printf("Thing: %d\t", (short) *(ptr + index));
		sum = sum + (short) *(ptr + index);
		sum = (sum & 0xFF) + (sum >>8);
		//printf("Sum at index %d: %d\n", index, sum);
	}
	message.checksum = ~sum;
	printf("\n\nThe checksum is: %d", message.checksum);


	/*Some UDP set up stuff*/
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(serverName, portNumber, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
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

	/*End of some UDP stuff*/

	// Put magicNumber and TML into network byte order (Big Endian)
	message.magicNumber = htons(message.magicNumber);
	message.TML = htons(message.TML);


	/*Send the message to the Server*/
	if ((numbytes = sendto(sockfd, &message, message.TML, 0,
		p->ai_addr, p->ai_addrlen)) == -1)
	{
		perror("talker: sendto");
		exit(1);
	}
	freeaddrinfo(servinfo);


	char buf[1024];
	struct sockaddr_storage sender;
	socklen_t sendsize = sizeof(sender);

	int response;
	response = recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr *)&sender, &sendsize);
	




	/*Upon recieveing the message from the server see if it's
	a "VALID" message. See assignment for details.
	*/








	/*With a "valid" essage print out the hostname and IP address
	as follows:
	hostname1 \t IP Address \n
	hostname2 \t IP Address \n
	.
	.
	.
	hostname3 \t IP Address \n
	*/




 }

 int checkRequestIDRange(int ID)
{
	if(ID < 0)
	{
		fprintf(stderr, "\nRequest ID must be in range of 0 to 127 inclusive\n");
		exit(1);
	}
	if(ID > 127)
	{
		fprintf(stderr, "\nRequest ID must be in range of 0 to 127 inclusive\n");
		exit(1);
	}
	return ID;
}








