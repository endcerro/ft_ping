#include "header.h"
#include <netinet/ip_icmp.h>

void dump_ip(void *buffer)
{
	struct iphdr *ip = buffer + IP_HDR_SIZE + ICMP_HDR_SIZE;
	char str[INET_ADDRSTRLEN];

	printf("IP Hdr Dump:\n");
    ft_hexdump(ip, sizeof(struct iphdr),2);
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	"
	       "Dst	Data\n");
	printf(" %x  %x  %02x %04x %04x   %x %04x  %02x  %02x %04x ",
	       ip->version, //Version
		   ip->ihl,    //header size
		   ip->tos,     //Type of service
		   (ip->tot_len), //total lenght
	       ntohs(ip->id),  //ID
		   ntohs(ip->frag_off) >> 13, //Flag is on the first bit, so we shift
	       ntohs(ip->frag_off) & 0x1FFF, //Rest is the actual fragment value
		   ip->ttl, ip->protocol, //Explicit
	       ntohs(ip->check)); //Checksum

	inet_ntop(AF_INET, &ip->saddr, str, sizeof(str));
	printf("%s  ", str);
	inet_ntop(AF_INET, &ip->daddr, str, sizeof(str));
	printf("%s\n", str);
}

void summary(void)
{

    float ratio = (1.0f - ping_data.stats.total_received / (float)ping_data.sequence) * 100.0f;
    printf("---Ping stats %s--- \n%d packets transmitted, %d received, %d%% packet loss, time %d.%dms\n", 
    ping_data.hostname_str, 
    ping_data.sequence, ping_data.stats.total_received, (int)ratio,0 ,0);

    if (ping_data.stats.total_received == 0)
        return;
    suseconds_t	mdev;
    mdev = ping_data.stats.total / ping_data.sequence;
    if (ping_data.stats.total_received > 0)
        ping_data.stats.sq_total /= ping_data.stats.total_received;
    else
        ping_data.stats.sq_total = 0;
    mdev = sqrt(ping_data.stats.sq_total - mdev * mdev);

    printf("round-trip min/avg/max/stddev %.3f/%.3f/%.3f/%.3f ms\n", 
    ping_data.stats.min / 1000.0f,
    ping_data.stats.total / (float) ((ping_data.stats.total_received > 0) ?  ping_data.stats.total_received  : 1) / 1000.0f, 
    ping_data.stats.max / 1000.0f,
    mdev / 1000.0f);


}