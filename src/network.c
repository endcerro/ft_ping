#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include "header.h"

void init_socket()
{
  //Raw socket 
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); 

  if (sock < 0)
		fatal("failed opening ICMP socket");
  
  //No ip header, need to build it by hand;
  int opt = 1;
  
  if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt)) != 0)
    fatal("failed to set socket options");
  ping_data.sock = sock;
}

int getAddressFromHostname() {
    char   ip[INET_ADDRSTRLEN]; /*Buffer to store ip string */
    struct addrinfo *result = 0; /*pointer to allocated struct from function return*/
    int ret = 0;
    struct addrinfo hint; /*used as function parameter*/
    ft_bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET; /*Only interested in IPv4*/

    ret = getaddrinfo(ping_data.hostname_str, 0, &hint, &result);

    if (ret == 0)
    {
      ping_data.results = result; //store the pointer
      ping_data.address_ptr = (struct in_addr*)(&((struct sockaddr_in*)result->ai_addr)->sin_addr); //store ip address ptr
      ping_data.target.sin_addr.s_addr = ping_data.address_ptr->s_addr; //Copy into target
      // printf("Address of struct start : %p / and data %p ", (void*)ping_data.address_ptr,(void*)&(ping_data.address_ptr->s_addr));
      inet_ntop(ping_data.results->ai_family, ping_data.address_ptr, ip, INET_ADDRSTRLEN); //Convert into string format;
      ping_data.ip_str = ft_strdup(ip);
    }
    else 
    {
      printf("getaddrinfo error: %s\n", gai_strerror(ret));
      fatal(NULL);
    }
    return ret;
}

void send_ping(int sig)
{
  if (!ping_data.run)
    return;

  char send_buffer[IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_BODY_SIZE] = {};
  ft_bzero(send_buffer, PACKET_SIZE);

  fill_ip_header(send_buffer, ping_data.address_ptr->s_addr);
  fill_icmp_header(send_buffer);

  int nb_bytes = sendto(ping_data.sock, send_buffer, sizeof(send_buffer), 0,
			  (const struct sockaddr *)&ping_data.target,
			  sizeof(ping_data.target));
  if (nb_bytes < 0)
    fatal("sendto failed\n");
  alarm(1);

  // printf("Packet sent !\n");
  if (ping_data.verbose)
  {
    print_packet(send_buffer);
  }
}


void receive_pong()
{
  struct msghdr message = {};
  struct iovec io= {} ;
  char receive_buffer[IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_BODY_SIZE] = {};
  char workBuffer[IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_BODY_SIZE]= {};
  // ft_bzero(receive_buffer, PACKET_SIZE);

  io.iov_base = receive_buffer;
  io.iov_len = IP_HDR_SIZE + ICMP_HDR_SIZE + ICMP_BODY_SIZE;
  message.msg_name = &ping_data.target;
  message.msg_namelen = sizeof(ping_data.target);
  message.msg_iov = &io;
  message.msg_iovlen = 1;
  message.msg_control = workBuffer;
  message.msg_controllen = sizeof(workBuffer);
  message.msg_flags = 0;


  int ret = recvmsg(ping_data.sock, &message, 0);


  // int ret = recv(ping_data.sock,receive_buffer, PACKET_SIZE,0);
	if (ret == -1)
  {
		printf("recvmsg failed\n");
    printf("getaddrinfo error: %s\n", strerror(errno));

  }
  // printf("recvms ret %d:\n", ret);

  if (ping_data.verbose)
	{
    printf("packet received :\n");
    print_packet(receive_buffer);
	}
  // print_icmp_packet(receive_buffer + IP_HDR_SIZE);
  process_pong(receive_buffer);
  usleep(10);
}