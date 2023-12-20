#include "header.h"
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

void handler(int code){
  exit(0);
}

void fatal(const char* str)
{
  if (str)
    printf(str);
  exit(1);
}

suseconds_t get_time_ms(void)
{
  struct timeval tmp;
  gettimeofday(&tmp,0);
  return tmp.tv_sec * 1000000 + tmp.tv_usec;
}

suseconds_t time_diff(struct timeval* then)
{
  suseconds_t	now = get_time_ms();
	return (now - then->tv_sec * 1000000 - then->tv_usec);
}