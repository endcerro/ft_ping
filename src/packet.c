#include "header.h"
#include <netinet/ip.h>
//https://sites.uclouvain.be/SystInfo/usr/include/netinet/ip_icmp.h.html
//https://en.wikipedia.org/wiki/Internet_Protocol_version_4#Header

unsigned short checksum(void* b, int len)
{
    unsigned short* buf = b;
    unsigned int sum = 0;
    unsigned short result;
 
    for (sum = 0; len > 1; len -= 2)
        sum += *(buf++);
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result =~ sum;
    return result;
}

void fill_ip_header(void* buffer, int dest_ip)
{
	struct iphdr *ip = buffer;
  	ip->version = 4; //ipv4
	ip->ihl = IP_HDR_SIZE / 4; //Number of 4 bytes block in the message;
	ip->tot_len = htons(PACKET_SIZE); //Size of the packet
	ip->ttl = PING_TTL;
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

void dump_body(void *buffer)
{
	struct iphdr *ip = buffer + IP_HDR_SIZE + ICMP_HDR_SIZE;
	struct icmphdr *icmp = buffer + IP_HDR_SIZE * 2 + ICMP_HDR_SIZE;
	uint8_t *bytes = (uint8_t *)ip;
	char str[INET_ADDRSTRLEN];

	printf("IP Hdr Dump:\n");
	for (size_t i = 0; i < sizeof(struct ip); i += 2) {
		printf(" %02x%02x", *bytes, *(bytes + 1));
		bytes += 2;
	}
	printf("\nVr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	"
	       "Dst	Data\n");
	printf(" %x  %x  %02x %04x %04x   %x %04x  %02x  %02x %04x ",
	       ip->version, ip->ihl, ip->tos, (ip->tot_len),
	       ntohs(ip->id), ntohs(ip->frag_off) >> 13,
	       ntohs(ip->frag_off) & 0x1FFF, ip->ttl, ip->protocol,
	       ntohs(ip->check));

	inet_ntop(AF_INET, &ip->saddr, str, sizeof(str));
	printf("%s  ", str);
	inet_ntop(AF_INET, &ip->daddr, str, sizeof(str));
	printf("%s\n", str);
	printf("ICMP: type %x, code %x, size %zu, id %#04x, seq 0x%04x\n",
	       icmp->type, icmp->code, ICMP_BODY_SIZE + sizeof(*icmp),
	       icmp->un.echo.id, ntohs(icmp->un.echo.sequence));
}

void process_pong(void *buffer)
{
	struct iphdr *ip = buffer;
	struct icmphdr *icmp = buffer + IP_HDR_SIZE;
	struct timeval *time = buffer + IP_HDR_SIZE + ICMP_HDR_SIZE;
		
	char			*ip_str = inet_ntoa(*(struct in_addr*)&ip->saddr);
	unsigned int	recvd_seq = htons(icmp->un.echo.sequence);
	// const char		*error_str;
	suseconds_t		rtt = time_diff(time);

	if (icmp->type != ICMP_ECHOREPLY)
	{
		char *strptr = error_table[icmp->type];
		printf("From %s icmp_seq=%hu %s\n", ip_str, recvd_seq, strptr);
		if (ping_data.verbose)
		{
			dump_body(buffer);
		}
	}
	else
	{
		++ping_data.stats.total_received; 
		// rtt = get_rtt(buffer + IP_HDR_SIZE + ICMP_HDR_SIZE);
		printf("%hu bytes from %s: icmp_seq=%hu ttl=%hhu time=%ld.%02ld ms\n", \
			(uint16_t)(ntohs(ip->tot_len) - IP_HDR_SIZE), \
			ip_str, recvd_seq, ip->ttl, rtt / 1000l, rtt % 1000l);

		// update_rtt_stats(rtt, seq);
	}

}