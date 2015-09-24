#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define BACKLOG 10	 // how many pending connections queue will hold

#define MAXDATASIZE 256 // max number of bytes we can get at once

char *handlePacket(char *Buffer, int length); // _M3

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// _M1 , now we need arguments int main(void)
int main(int argc, char *argv[]) // _M1
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;

	char s[INET6_ADDRSTRLEN];
	int rv;

	int numbytes; // _M2
	char buf[MAXDATASIZE]; // _M2


	/* _M1 Begin */
	if (argc != 2)
	{
		fprintf(stderr,"usage: TCPServerDisplay Port# \n");
		exit(1);
	}
	/* _M1 End*/

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, argv[1] /* _M1 PORT*/, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
			== -1)
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) 
			== -1)
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1)
	{  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, get_in_addr(
			(struct sockaddr *)&their_addr), s, sizeof s);
		//printf("server: got connection from %s\n", s);

		if (!fork())
		{ // this is the child process
			close(sockfd); // child doesn't need the listener

			//Receive the message from the Client
			if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1)
			{
				perror("recv");
				exit(1);
			}

			unsigned char request = buf[1];

			//Get consonant count
			if(buf[2] == 0x05)
			{
				int numConsonants = 0;
				int x;
				for(x = 3; x < numbytes; x++)
				{
					unsigned char ch = tolower(buf[x]);

					//Check if vowel
					if (!(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' 
						|| ch == 'u'))
					{
						//Check if in lowercase aschii character range
						if((ch >= 98) && (ch <= 122))
						{
							numConsonants++;
						}
					}
					else
					{
						//do nothing
					}

				}//end for-loop

				unsigned char sendData[3];
				sendData[0] = 0x03;
				sendData[1] = request;
				sendData[2] = (unsigned char) numConsonants;
				if (send(new_fd, sendData, sizeof(sendData), 0) == -1) {
					perror("send");
				}
			}
			//Disemvoweling
			else if(buf[2] == 0x50)
			{
				unsigned char an_buf[256];
				int x;
				int y = 0;
				for(x = 3; x < numbytes; x++)
				{
					unsigned char ch = buf[x];

					if (!(tolower(ch) == 'a' || tolower(ch) == 'e' 
						|| tolower(ch) == 'i' || tolower(ch) == 'o' 
						|| tolower(ch) == 'u'))
					{
						an_buf[y] = ch;
						y++;
					}
					else
					{
						//do nothing
					}
				}//end-for-loop

				char response[y+2];
				memset(response, 0, sizeof(response));
				response[0] = sizeof(response);
				response[1] = request;
				int z;
				for(z = 2; z < sizeof(response); z++)
				{
					response[z] = an_buf[z-2];
				}
				if (send(new_fd, response, sizeof(response), 0) == -1) {
					perror("send");
				}

			}
			else if(buf[2] == 0x0A)
			{
				unsigned char response[buf[0] - 1];
				memset(response, 0, sizeof(response));
				response[0] = sizeof(response);
				response[1] = request;
				int x;
				for(x = 2; x < sizeof(response); x++)
				{
					char ch = (char) toupper(buf[x + 1]);
					response[x] = ch;
				}

				if (send(new_fd, response, sizeof(response), 0) == -1) {
					perror("send");
				}
			}



			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}
