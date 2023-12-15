#ifndef HEADER_H
#define HEADER_H

#include "../libft/libft.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>

#define PING_TTL			64
#define PING_DELAY			1
#define IP_HEADER_SIZE		20
typedef enum { false, true } bool;
typedef struct t_data
{
	struct sockaddr_in	target;
	struct addrinfo 	*results;
	struct in_addr    *address_ptr;

	char *hostname_str;
	char *ip_str;
	unsigned int port;
	int sock;
	unsigned int sequence;
	bool verbose;

} s_data;


#endif