//References:
//http://www.cs.montana.edu/~halla/csci466/lectures/lec2.html
//man socket

#include <sys/types.h>	//Structs
#include <sys/socket.h>	//Sockets
#include <netdb.h>	//Get host by name

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SA  struct sockaddr

//#define SERVER_PORT  33333
#define MAX_LINE  256

int main(int argc, char* argv[]) {
	struct hostent *hp;
	int sfd; //Socket file descriptor
	struct sockaddr_in sin; //Socket binding IP
	char *host;
	unsigned int port;
	char buf[MAX_LINE+1];
	int len;

	printf("Creating a socket\n");

	//int socket(int domain, int type, int protocol);
	//Domain = AF_INET for IPv4 protocol	
	//
	//Type = SOCK_STREAM ("TCP"), SOCK_DGRAM ("UDP")
	//
	//From man:
	//Since Linux 2.6.27, the type argument serves a second  purpose:  in  addition  to  specifying  a
	//socket  type, it may include the bitwise OR of any of the following values, to modify the behav‐
	//ior of socket():
	//
	//SOCK_NONBLOCK   Set the O_NONBLOCK file status flag on the new  open  file  description.   Using
	//this flag saves extra calls to fcntl(2) to achieve the same result.
	//
	//SOCK_CLOEXEC    Set  the  close-on-exec  (FD_CLOEXEC)  flag on the new file descriptor.  See the
	//description of the O_CLOEXEC flag in open(2) for reasons why this may be useful.
	//


	/* create the socket */
	if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	/* translate host name into peer's IP address */
	if (argc == 3) {
		host = argv[1];
		port = atoi(argv[2]);
	} else {
		fprintf(stderr, "usage: simplex-talk host port\n");
		exit(1);
	}
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}


	//Init address structure
	/* build address data structure */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
       	sin.sin_port = htons(port);



	/* active open */
	if (connect(sfd, (SA *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(sfd);
		exit(1);
	}

	/* main loop: get and send lines of text */
	while (fgets(buf, MAX_LINE, stdin)) {
		len = strlen(buf) + 1;
		send(sfd, buf, len, 0);
	}

	//Create a connection
	//connect()
	//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

	//read()
	//write()
	
	//close()
	
	//Out-of-band data may also be transmitted as described in send(2) and received as described in recv(2).
}
