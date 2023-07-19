#ifndef HEADER_H
#define HEADER_H

#include "../libft/libft.h"
#include <stdio.h>
#include <arpa/inet.h>

typedef struct _targetInfo
{
  int sockfd;
  struct sockaddr_in  target_addr;
  socklen_t addrlen;
  char *address;

} targetInfo;

typedef struct _serverInfo
{


} serverInfo;

#endif