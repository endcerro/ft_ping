#include "../incl/header.h"

s_data ping_data;
int tryarg(char *arg)
{
  if (arg == 0)
    return 1;
  int arglen = ft_strlen(arg);
  if (arglen == 2 && ft_strncmp("-?", arg, arglen) == 0)
  {
    printf("Available flags\n");
    printf("-v        verbose in case of error\n");
    printf("--ttl=    packets ttl\n");
    return 1;
  }
  else if (arglen == 2 && ft_strncmp("-v", arg, arglen) == 0)
    ping_data.verbose = 1;
  else if(arglen > 6 && ft_strncmp("--ttl=", arg, arglen)==0)
  {
    ping_data.ttl = ft_atoi(arg + 6);
    if (ping_data.ttl == 0)
      return 1;
  }
  else if (ping_data.hostname_str == 0)
    ping_data.hostname_str = arg;
  else
    return 1;
  return 0;
}
void init(int ac, char **av)
{
  ft_bzero(&ping_data, sizeof(ping_data));

  if (getuid() != 0)
    fatal("this program needs superuser to run\n");
  ping_data.ttl = PING_TTL;
  ping_data.target.sin_family = AF_INET;
  ping_data.run = 1;
  for (int i = 1; i < ac; i++)
  {
    if (tryarg(av[i]))
      fatal("usage : ft_ping (-v) (--ttl=) example.com\n");
  }
  if (ping_data.hostname_str == 0)
    fatal("usage : ft_ping (-v) (--ttl=) example.com\n");
}

int main(int argc, char **argv)
{
  signal(SIGINT, &handler);
  signal(SIGALRM, &send_ping);
  init(argc, argv);

  resolve();
  init_socket();
  printf("FT_Ping %s (%s): %d(%d) data bytes", ping_data.hostname_str, ping_data.ip_str, ICMP_BODY_SIZE, PACKET_SIZE);
  if (ping_data.verbose)
    printf(", id 0x%04x = %d\n", getpid(), getpid());
  else
    printf("\n");
  send_ping(0);

  while(ping_data.run)
    receive_pong();

  return 0;
}