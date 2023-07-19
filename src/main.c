#include "../incl/header.h"

int main(int argc, char **argv)
{

  targetInfo tinfo;
  serverInfo sinfo;

  struct sockaddr_in  target_addr;
  target_addr.sin_family = AF_INET;

  char* ip_addr;
  target_addr.sin_addr.s_addr = inet_addr(ip_addr);


  return 0;
}