#ifndef HEADER_H
#define HEADER_H

#include "../libft/libft.h"
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <sys/time.h>

#define PING_TTL			64
#define PING_DELAY			1

# define ICMP_HDR_SIZE			ICMP_MINLEN
# define ICMP_PAYLOAD_SIZE		56
#define PACKET_SIZE (ICMP_HDR_SIZE + ICMP_PAYLOAD_SIZE)
// #define PACKET_SIZE 64
#define FT_PING_TTL 4
typedef enum { false, true } bool;

typedef struct t_ping_pkt {
    struct icmphdr hdr;
    char msg[PACKET_SIZE - sizeof(struct icmphdr)];
} s_ping_pkt;

typedef struct t_data
{
	struct sockaddr_in	target;
	struct addrinfo 	*results;
	struct in_addr    	*address_ptr;

	char *hostname_str;
	char *ip_str;
	unsigned int port;
	int sock;
	unsigned int sequence;
	bool verbose;
	s_ping_pkt ping_pkt;
	bool run;

} s_data;

void	fill_icmp_header();
void fatal(const char* str);

extern s_data ping_data;
#endif