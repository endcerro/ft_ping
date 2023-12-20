#include "header.h"
#include <netinet/ip_icmp.h>

static const char		*ip_header_str = \
"\e[32m IP HEADER\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m| IPv  %x | IHL %x |    TOS %hhx     |          Total Length    %hx   |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|         Identification    %04hx      |   Fragment Offset  %hx    |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|   TTL   %hhx    |   Protocol %hhx  |       Header Checksum %04hx    |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|                     Source Address    %08x                |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|                  Destination Address  %08x                |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
static const char       *icmp_header_str = \
"\e[32m ICMP HEADER\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|    Type %hhx     |    Code %hhx     |        Checksum %04hx          |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" \
"\e[32m|           Identifier %x       |        Sequence Number %x      |\n" \
"\e[34m+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\e[0m\n";
static const char       *timeval_str = \
"%ld.%06ld\n";

void	print_icmp_packet(void *packet)
{
	struct icmphdr *icmp = packet;

	printf(icmp_header_str, icmp->type, icmp->code, \
		icmp->checksum, ntohs(icmp->un.echo.id), ntohs(icmp->un.echo.sequence));
}

void print_ip_packet(void *packet)
{
    struct iphdr *ip = packet;

    printf(ip_header_str, ip->version, ip->ihl, ip->tos, ntohs(ip->tot_len), \
        ntohs(ip->id), ntohs(ip->frag_off), ip->ttl, ip->protocol, ip->check, \
        ip->saddr, ip->daddr);
}

void print_timeval(void *timeval)
{
  struct timeval *time = timeval;
  printf(timeval_str, time->tv_sec, time->tv_usec);
}

void print_packet(void *packet)
{
    print_ip_packet(packet);
    print_icmp_packet(packet + IP_HDR_SIZE);
    print_timeval(packet + IP_HDR_SIZE + ICMP_HDR_SIZE);
}

static const char *error_table[] = 
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


const char* get_icmp_error_str(int id)
{
    switch (id) {
        case ICMP_ECHOREPLY :
        case ICMP_DEST_UNREACH :
        case ICMP_SOURCE_QUENCH :
        case ICMP_REDIRECT :
        case ICMP_ECHO :
        case ICMP_TIME_EXCEEDED :
        case ICMP_PARAMETERPROB :
        case ICMP_TIMESTAMP :
        case ICMP_TIMESTAMPREPLY :
        case ICMP_INFO_REQUEST :
        case ICMP_INFO_REPLY :
        case ICMP_ADDRESS :
        case ICMP_ADDRESSREPLY :
        return error_table[id];
        default :
        return error_table[ICMP_ADDRESSREPLY + 1];
        break;
    }
}


void summary(void)
{

    float ratio = (1.0f - ping_data.stats.total_received / (float)ping_data.sequence) * 100.0f;
    // int ratio = (ping_data.stats.total_received / ping_data.sequence ) * 100; 
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d.%dms\n", 
    ping_data.sequence, ping_data.stats.total_received, (int)ratio,0 ,0 );
}