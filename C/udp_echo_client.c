/**
 * Copyright Lodewijk Sikkel <l.n.c.sikkel@tudelft.nl>
 * @file udp_echo_client.c
 * @brief Echos incoming UDP packet
 *
 * BUILD INSTRUCTIONS: gcc -o udp_echo_client udp_echo_client.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_addr
#include <unistd.h> // for close

#define OUTGOING_PORT 8888
#define INCOMING_PORT 8889
#define BUFFER_SIZE 4096 // bytes

int main(void)
{
	struct sockaddr_in myaddr, remaddr;
	socklen_t addrlen = sizeof(remaddr);
	int fd;
	char *server = "10.0.2.15";	// change this to your local IP address
	char buf[BUFFER_SIZE];

    // Create the socket
	if ( ( fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) == -1 ) {
        perror("Cannot create socket\n");
        return 0;
    }

	// Bind the socket to any valid IP address
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr("10.0.2.15");
	myaddr.sin_port = htons(INCOMING_PORT);
	if ( bind( fd, (struct sockaddr *)&myaddr, sizeof(myaddr) ) < 0 ) {
		perror("Bind failed");
		return 0;
	}       

	// Define the remote address
	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(OUTGOING_PORT);
	if ( inet_aton( server, &remaddr.sin_addr ) == 0 ) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	/* now let's send the messages */
	int length;
	while (1) {
		printf("Waiting for data...\n");
		length = recvfrom( fd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&remaddr, &addrlen );
		printf("Received %d bytes\n", length);
		if ( sendto( fd, buf, length, 0, (struct sockaddr *)&remaddr, sizeof(remaddr) ) == -1 ) {
			perror("sendto");
		}
	}
	close(fd);
	return 0;
}