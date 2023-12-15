#include "header.h"

unsigned short checksum(void* b, int len)
{
    unsigned short* buf = b;
    unsigned int sum = 0;
    unsigned short result;
 
    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result =~ sum;
    return result;
}
 
void	fill_icmp_header()
{
    static int seq = 0;
    memset(&(ping_data.ping_pkt), 0, sizeof(s_ping_pkt));
    // struct icmphdr *ping = (struct icmphdr *)buff;
    // memset(ping, 0, sizeof(struct icmphdr));
    if (gettimeofday((struct timeval *)ping_data.ping_pkt.msg, NULL) == -1) {
		// printf("gettimeofday err: %s\n", strerror(errno));
		// return -1;
        fatal("Gettimeofday failed");
	}
	ping_data.ping_pkt.hdr.type = ICMP_ECHO;
    ping_data.ping_pkt.hdr.un.echo.id = getpid();
	ping_data.ping_pkt.hdr.un.echo.sequence = seq++;
	ping_data.ping_pkt.hdr.checksum = checksum(&ping_data.ping_pkt, sizeof(ping_data.ping_pkt));

}