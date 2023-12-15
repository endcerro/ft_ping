#include "../incl/header.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

void handler(int code){
  exit(0);
}

int getAddressFromHostname(const char* hostname , struct addrinfo **results);

char *processAddrinfo(struct addrinfo *address){

  if (address->ai_family == AF_INET) 
  {
            struct sockaddr_in *psai = (struct sockaddr_in*)address->ai_addr;
        char ip[INET_ADDRSTRLEN];
        if (inet_ntop(address->ai_family, &(psai->sin_addr), ip, INET_ADDRSTRLEN) != NULL) {
            printf("IP: %s\n", ip);
        }
  }
  else if (address->ai_family == AF_INET6) {
        struct sockaddr_in6 *psai = (struct sockaddr_in6*)address->ai_addr;
        char ip[INET6_ADDRSTRLEN];
        if (inet_ntop(address->ai_family, &(psai->sin6_addr), ip, INET6_ADDRSTRLEN) != NULL) {
            printf("IP: %s\n", ip);
        }
        }
  return 0;
}

int main(int argc, char **argv){

  signal(SIGABRT, handler);
  const char *ip_addr = "163.172.250.16";
  const char *hostname = "42.fr";

  struct addrinfo *result = 0;

  getAddressFromHostname("google.com", &result);
  processAddrinfo(result);

  freeaddrinfo(result);
  // getAddressFromHostname("43.azerty",&result);
  // freeaddrinfo(result);


  return 0;
}