#include "../incl/header.h"

s_data ping_data;

void init(int ac, char **av)
{
  ft_bzero(&ping_data, sizeof(ping_data));

  if (ac != 2 && ac !=3)
    fatal("usage : ft_ping (-v) example.com\n");
  if (getuid() != 0)
    fatal("this program needs superuser to run\n");

  ping_data.hostname_str = av[1];
  if (ft_strncmp(av[1], "-v", 3) == 0)
  {
    ping_data.verbose = true;
    ping_data.hostname_str = av[2];
  }

  ping_data.target.sin_family = AF_INET;
  ping_data.run = 1;
}

void fill_timestamp(void *buffer);

int main(int argc, char **argv)
{
  // signal(SIGABRT, handler);
  signal(SIGINT, &handler);
  signal(SIGALRM, &send_ping);
  init(argc, argv);

  resolve();
  init_socket();
  printf("FT_PING %s (%s): %d data bytes\n", ping_data.hostname_str, ping_data.ip_str, ICMP_BODY_SIZE);
   
  send_ping(0);

  // printf("%s\n", print_icmp_error(4));
  while(ping_data.run)
    receive_pong();
  write(1, "Finished !\n",11);

  return 0;
}