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
		fprintf(stderr, "\nusage error: argc = %d\tUDPClientProgName, Servername, port#, requestID, hostname1, hostname2, ..., hostnameX\n", argc);
		exit(1);
	}

	//Get the server name
	char* serverName = argv[1];

	//Get Port Number
	char* portNumber = argv[2];

	//get Request ID
	checkRequestIDRange(atoi(argv[3]));
	char* requestID = argv[3];

	//Get all the hostnames
	int amtOfHostnames = argc - 4;				//minus 4 bc command line takes 4 arguments before listing the hostnames
	char listOfHostNames[amtOfHostnames][32];
	//Get the sizes of each hostname
	char sizeOfEachHostName[amtOfHostnames][1];
	int host;
	for(host = 0; host < amtOfHostnames; host++)
	{
		//printf("\nHost %d: %s\tLength: %d\n", host, argv[host+4], strlen(argv[host+4]));
		listOfHostNames[host][0] = argv[host+4];
		sizeOfEachHostName[host][0] = strlen(argv[host+4]);
		//printf("\n\tSize check in array: %d\n\n", sizeOfEachHostName[host][0]);
	}


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

	freeaddrinfo(servinfo);
	/*End of some UDP stuff*/

	/*Obatin the hostnames from the command line arguments and get the 
	request ID. See assignment for details on how the message array should 
	be contructed
	*/











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







