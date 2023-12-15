#include "../incl/header.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

static s_data ping_data;

void handler(int code){
  exit(0);
}

int getAddressFromHostname(const char* hostname) {
    char   ip[INET_ADDRSTRLEN];
    struct addrinfo *result = 0;
    int ret = 0;
    struct addrinfo hint;
    ft_bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;

    ret = getaddrinfo(hostname, 0, &hint, &result);

    if (ret == 0) 
    {
      ping_data.results = result;
      ping_data.address_ptr = (struct in_addr*)(&((struct sockaddr_in*)result->ai_addr)->sin_addr);
      ping_data.target.sin_addr.s_addr = ping_data.address_ptr->s_addr;
      inet_ntop(ping_data.results->ai_family, ping_data.address_ptr, ip, INET_ADDRSTRLEN);
      ping_data.ip_str = ft_strdup(ip);
      printf("FT_PING %s (%s): 56 data bytes\n", ping_data.hostname_str, ping_data.ip_str);
    }
    else 
    {
      printf("getaddrinfo error: %s\n", gai_strerror(ret));
    }
    return ret;
}

int parse_args(int ac, char **av)
{
  if (ac != 2 && ac !=3)
  {
    printf("usage : ft_ping (-v) example.com %d\n", ac);
    return 1;
  }
  if (getuid() != 0)
  {
    printf("this program needs privileges to run\n");
    return 1;
  }

  ping_data.hostname_str = av[1];
  if (ft_strncmp(av[1], "-v", 3) == 0)
  {
    ping_data.verbose = true;
    ping_data.hostname_str = av[2];
  }
  return 0;
}

void init_struct()
{
  ft_bzero(&ping_data, sizeof(ping_data));
  ping_data.target.sin_family = AF_INET;
}

void send_ping(int sig)
{

}

void receive_ping()
{
  
}

int main(int argc, char **argv){

  if (parse_args(argc, argv))
    return 1;
  signal(SIGABRT, handler);
  signal(SIGALRM, send_ping);
  const char *ip_addr = "163.172.250.16";
  const char *hostname = "42.fr";

  // struct addrinfo *result = 0;
  if (getAddressFromHostname(argv[1]) != 0)
  {
    return 1;
  }
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  if (sock < 0)
		printf("failed opening ICMP socket");
  
  int ret = 0;
  int value = 1;
	if ((ret = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &value, sizeof(int))) == -1)
		printf("failed to set socket option, might be running on restricted environment :%s\n",strerror(errno));
  printf("Socket is %d\n", sock);


  // freeaddrinfo(result);

  return 0;
}