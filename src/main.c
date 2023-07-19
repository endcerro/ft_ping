#include "../incl/header.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void handler(int code){
  exit(0);
}


/*char *host_to_ip(char *addr_host, struct sockaddr_in *addr_con)
{
    struct hostent *host = NULL;
    char *ip = NULL;



    if ((ip = malloc(NI_MAXHOST*sizeof(char))) == NULL)
      return NULL;
 
    if ((host_entity = gethostbyname(addr_host)) == NULL)
    {
      free(ip)
      return NULL;
    }
   
   getaddrinfo(addr_host, NULL,  )  
    //filling up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)
                          host->h_addr));
 
    (*addr_con).sin_family = host->h_addrtype;
    (*addr_con).sin_port = htons (0);
    (*addr_con).sin_addr.s_addr  = *(long*)host->h_addr;
 
    return ip;

}*/

/*char *ip_to_host(char *ip)
{
  char *ret;

  return ret;
}*/

int main(int argc, char **argv){

  signal(SIGABRT, handler);
  targetInfo tinfo;
  serverInfo sinfo;

  // struct sockaddr_in  target_addr;
  struct addrinfo hints;
  struct addrinfo *res;

  ft_bzero((void*)&hints, sizeof(hints));
  ft_bzero((void*)res, sizeof(res));

  // target_addr.sin_family = AF_INET;

  char *ip_addr = "192.168.1.1";
  char *port = "0";
  getaddrinfo(ip_addr, port, &hints, &res);
  


  return 0;
}