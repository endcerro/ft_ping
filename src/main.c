#include "../incl/header.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

s_data ping_data;

void handler(int code){
  exit(0);
}

void fatal(const char* str)
{
  if (str)
    printf(str);
  exit(1);
}
void printAnnounce()
{
  printf("FT_PING %s (%s): 56 data bytes\n", ping_data.hostname_str, ping_data.ip_str);
}

int getAddressFromHostname() {
    char   ip[INET_ADDRSTRLEN];
    struct addrinfo *result = 0;
    int ret = 0;
    struct addrinfo hint;
    ft_bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;

    ret = getaddrinfo(ping_data.hostname_str, 0, &hint, &result);

    if (ret == 0) 
    {
      ping_data.results = result;
      ping_data.address_ptr = (struct in_addr*)(&((struct sockaddr_in*)result->ai_addr)->sin_addr);
      ping_data.target.sin_addr.s_addr = ping_data.address_ptr->s_addr;
      inet_ntop(ping_data.results->ai_family, ping_data.address_ptr, ip, INET_ADDRSTRLEN);
      ping_data.ip_str = ft_strdup(ip);
    }
    else 
    {
      printf("getaddrinfo error: %s\n", gai_strerror(ret));
      fatal(NULL);
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
  //Fill header
  //Note timestamps
  //Send

  fill_icmp_header();

  int nb_bytes = sendto(ping_data.sock, (const char*)(&(ping_data.ping_pkt)), sizeof(ping_data.ping_pkt), 0,
			  (const struct sockaddr *)&ping_data.target,
			  sizeof(ping_data.target));
  if (nb_bytes < 0)
    fatal("sendto failed\n");
  printf("Succes send !\n");
  alarm(1);
}

void receive_pong()
{
  //receive
  //Stop timestamp
  ping_data.run = true;
  while(ping_data.run)
  {
    //
    uint8_t buf[PACKET_SIZE * 2] = {};
    char buff[512] = {};
	  ssize_t nb_bytes;
    struct icmphdr *icmph;
	struct iovec	io =
	{
		.iov_base = buf,
		.iov_len = PACKET_SIZE
	};
	struct msghdr	msg =
	{
		.msg_name = &ping_data.target,
		.msg_namelen = sizeof(ping_data.target),
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buf,
		.msg_controllen = sizeof(buf),
		.msg_flags = 0
	};
  	nb_bytes = recvmsg(ping_data.sock, &msg, 0);
    if (errno != EAGAIN && errno != EWOULDBLOCK && nb_bytes == -1) {
		  // printf("recvmsg err: %s\n", strerror(errno));
		  // return -1;
	  }
    else if (nb_bytes == -1) {
      printf("nothing to read");

		// return 0;
    }
    else
      printf("nbbytes recived : %d : %s\n", nb_bytes);

    // usleep(10);
  }

}

int initSocket()
{
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  if (sock < 0)
		fatal("failed opening ICMP socket");
  
  int ttl_val = 64;
  struct timeval tv_out;
  tv_out.tv_sec = 5;
  tv_out.tv_usec = 0;

  if (setsockopt(sock, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val)) != 0 || 
      setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(const char*)&tv_out, sizeof tv_out) != 0) {
    fatal("failed to set socket options");
  }
  ping_data.sock = sock;
  return sock;
}

int main(int argc, char **argv){

  if (parse_args(argc, argv))
    return 1;
  signal(SIGABRT, handler);
  signal(SIGALRM, send_ping);

  // struct addrinfo *result = 0;
  getAddressFromHostname(argv[1]);
  initSocket();
  printAnnounce();
  fill_icmp_header();

  bool run = true;
  bool send = true;
  send_ping(0);
  receive_pong();
  // while (run)
  // {
  //   if (send)
  //   {
  //     //Send
  //     send = false;

  //   }
  // }

  // freeaddrinfo(result);

  return 0;
}