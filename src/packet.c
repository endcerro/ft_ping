#include "header.h"
#include <netinet/ip.h>
//https://sites.uclouvain.be/SystInfo/usr/include/netinet/ip_icmp.h.html
//https://en.wikipedia.org/wiki/Internet_Protocol_version_4#Header

const char *error_table[] = 
{
    [ICMP_ECHOREPLY] ="Echo Reply",
    [ICMP_SOURCE_QUENCH]= "Source Quench",
    [ICMP_DEST_UNREACH]="Destination Unreachable",
    [ICMP_REDIRECT] = "Redirect (change route)",
    [ICMP_ECHO] = "Echo Request",
    [ICMP_TIME_EXCEEDED]="Time Exceeded",
    [ICMP_PARAMETERPROB]="Parameter Problem",
    [ICMP_TIMESTAMP]="Timestamp Request",
    [ICMP_TIMESTAMPREPLY] ="Timestamp Reply",
    [ICMP_INFO_REQUEST] ="Information Request ",
    [ICMP_INFO_REPLY] ="Information Reply ",
    [ICMP_ADDRESS]="Address Mask Request",
    [ICMP_ADDRESSREPLY] ="Address Mask Reply",
    [ICMP_ADDRESSREPLY + 1] ="Unknown"
};

void fill_ip_header(void* buffer, int dest_ip)
{
	struct iphdr *ip = buffer;
  	ip->version = 4; //ipv4
	ip->ihl = IP_HDR_SIZE / 4; //Number of 4 bytes block in the message;
	ip->tot_len = htons(PACKET_SIZE); //Size of the packet
	ip->ttl = ping_data.ttl;
	ip->id = htons(0);
	ip->frag_off = htons(0);
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = INADDR_ANY;
	ip->daddr = dest_ip;

	ip->check = checksum(ip, IP_HDR_SIZE);
}

void fill_icmp_header(void *buffer)
{
	struct icmphdr *icmp = buffer + IP_HDR_SIZE;
	fill_timestamp(buffer);
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = htons(42);
	icmp->un.echo.sequence = htons(ping_data.sequence++);
	icmp->checksum = checksum(buffer, ICMP_HDR_SIZE + ICMP_BODY_SIZE);
}

void fill_timestamp(void *buffer)
{
	struct timeval* t = buffer + IP_HDR_SIZE + ICMP_HDR_SIZE;
	gettimeofday(t, 0);
}


static void update_stats(suseconds_t time_delta)
{
	static int start = 0;

	struct t_stats *stt = &(ping_data.stats);
	if (start == 0)
	{
		stt->start = time_delta;
		stt->min = time_delta;
		stt->max = time_delta;
		stt->total = time_delta;
		stt->sq_total = time_delta * time_delta;
		++start;
	}
	else 
	{
		stt->min = (stt->min > time_delta) ? time_delta : stt->min;
		stt->max = (stt->max < time_delta) ? time_delta : stt->max  ;
		stt->total += time_delta;
		stt->sq_total += time_delta * time_delta;
	}
}

void process_pong(void *buffer)
{
	struct iphdr *ip = buffer;
	struct icmphdr *icmp = buffer + IP_HDR_SIZE;
	struct timeval *time = buffer + IP_HDR_SIZE + ICMP_HDR_SIZE;
	struct icmphdr *icmp_error = buffer + IP_HDR_SIZE * 2 + ICMP_HDR_SIZE;
		
	char			*ip_str = inet_ntoa(*(struct in_addr*)&ip->saddr);
	unsigned int	recvd_seq = htons(icmp->un.echo.sequence);
	suseconds_t		delta = time_diff(time);

	if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type == ICMP_ECHO)
			return;
		recvd_seq = htons(icmp_error->un.echo.sequence);
		const char *strptr = error_table[icmp->type];
		printf("From %s icmp_seq=%hu %s\n", ip_str, recvd_seq, strptr);
		if (ping_data.verbose)
			dump_ip(buffer);
	}
	else
	{
		++ping_data.stats.total_received; 
		update_stats(delta);
		printf("%hu bytes from %s: icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms\n", \
			(uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE), \
			ip_str, recvd_seq, ip->ttl, delta / 1000l, delta % 1000l);
	}
}
