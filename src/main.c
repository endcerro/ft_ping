#include "../incl/header.h"

s_data ping_data;

void parse_args(int ac, char **av)
{
  if (ac != 2 && ac !=3)
    fatal("usage : ft_ping (-v) example.com\n");
  if (getuid() != 0)
    fatal("this program needs privileges to run\n");

  ping_data.hostname_str = av[1];
  if (ft_strncmp(av[1], "-v", 3) == 0)
  {
    ping_data.verbose = true;
    ping_data.hostname_str = av[2];
  }
}

void init_struct()
{
  ft_bzero(&ping_data, sizeof(ping_data));
  ping_data.target.sin_family = AF_INET;
  ping_data.run = 1;
}



void fill_timestamp(void *buffer);

int main(int argc, char **argv){

  init_struct();
  parse_args(argc, argv);
  signal(SIGABRT, handler);
  signal(SIGALRM, send_ping);

  // struct addrinfo *result = 0;
  getAddressFromHostname(argv[1]);
  init_socket();
  printf("FT_PING %s (%s): 56 data bytes\n", ping_data.hostname_str, ping_data.ip_str);
  // char buff[PACKET_SIZE];
  // ft_bzero(buff, PACKET_SIZE);
  // fill_ip_header(buff, *ping_data.address_ptr);
  // fill_icmp_header(buff, 0);
  // print_packet(buff);
  
  send_ping(0);

  while(true)
  {
    receive_pong();
  }
  // char b2[PACKET_SIZE] = {};
  // struct timeval now;
  // gettimeofday(&now, 0);
  // print_timeval(&now);
  // int diff = (now.tv_sec-prev->tv_sec);
  // printf("DIFF %d\n",diff);

  // int diff2 = now.tv_usec - prev->tv_usec;
  // printf("DIFF %d\n",diff2);
  return 0;
}