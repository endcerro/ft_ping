#ifndef HEADER_H
#define HEADER_H

#include "../libft/libft.h"
#include "define.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/time.h>

#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#include <signal.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

typedef enum { false, true } bool;


typedef struct t_data
{
	struct sockaddr_in	target;
	struct addrinfo 	*results; /*Return from getaddrinfo*/
	struct in_addr    	*address_ptr; /*IPv4 address in above struct*/

	char *hostname_str; //Original input
	char *ip_str; //String representation of the IP
	int sock;	//Socket on which we will communicate
	unsigned int sequence; //Current sequence index

	uint run; //Used for signaling
	
	unsigned int port; // Unused ?
	uint verbose; // Todo
} s_data;


void	fill_icmp_header(void *buffer);
void fill_ip_header(void*buffer, int dest);

void fatal(const char* str);

void send_ping(int sig);
void receive_pong();
void fatal(const char* str);
void handler(int code);
int getAddressFromHostname();
void print_packet(void * packet);
void print_timeval(void * packet);
void fill_timestamp(void *buffer);

void init_socket();

extern s_data ping_data;
#endif