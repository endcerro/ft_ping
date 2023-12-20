#ifndef DEFINE_H
#define DEFINE_H
#include <netinet/ip_icmp.h>

#define IP_HDR_SIZE 20
#define ICMP_HDR_SIZE 8
#define ICMP_BODY_SIZE 56
#define PACKET_SIZE 84

#define PING_TTL			64
#define PING_DELAY			1

typedef struct t_stats
{
	unsigned int total_sent;
	unsigned int total_received;
	long long total_time;
} s_stats;

typedef struct t_data
{
	struct sockaddr_in	target;
	struct addrinfo 	*results; /*Return from getaddrinfo*/
	struct in_addr    	*address_ptr; /*IPv4 address in above struct*/

    struct t_stats stats;

	char *hostname_str; //Original input
	char *ip_str; //String representation of the IP
	int sock;	//Socket on which we will communicate
	unsigned int sequence; //Current sequence index

	uint run; //Used for signaling
	
	unsigned int port; // Unused ?
	uint verbose; // Todo
} s_data;

#endif