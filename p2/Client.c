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

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;


	/*Check for correct terminal usage for this program. 
	  It should UDPClient Servername Port# request_id hostname1 hostname2 ... hostnameX
	 */





	//UDP Stuff
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(serverName, portNumber, &hints, &servinfo)) != 0) 
	{
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
	//End of some UDP Stuff


	/*Form the message in an array that contains the request ID and
	list of hostnames. See assignment page for specs
	*/






	/*After you have sent the message, wait for a response and then 
	display it on the command line screen as follows:
	hostname1 \t IP address \n
	hostname2 \t IP address \n
	.
	.
	.
	hostnameX \t IP address \n
























