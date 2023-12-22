#include "header.h"

void handler(int code){
  (void)code;
  summary();
  freeaddrinfo(ping_data.results);
  close(ping_data.sock);
  exit(0);
}

void fatal(const char* str)
{
  if (str)
    printf(str);
  exit(1);
}

suseconds_t get_time_ms(void)
{
  struct timeval tmp;
  gettimeofday(&tmp,0);
  return tmp.tv_sec * 1000000 + tmp.tv_usec;
}

suseconds_t time_diff(struct timeval* then)
{
  suseconds_t	now = get_time_ms();
	return (now - then->tv_sec * 1000000 - then->tv_usec);
}

unsigned short checksum(void* b, int len)
{
    unsigned short* buf = b;
    unsigned int sum = 0;
    unsigned short result;
 
    for (sum = 0; len > 1; len -= 2)
        sum += *(buf++);
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result =~ sum;
    return result;
}

double sqrt(double n){
  // Max and min are used to take into account numbers less than 1
  double lo = (1 > n) ? n : 1;
  double hi = (1 < n) ? n : 1;
  double mid;

  // Update the bounds to be off the target by a factor of 10
  while(100 * lo * lo < n) lo *= 10;
  while(0.01 * hi * hi > n) hi *= 0.1;

  for(int i = 0 ; i < 100 ; i++){
      mid = (lo+hi)/2;
      if(mid*mid == n) return mid;
      if(mid*mid > n) hi = mid;
      else lo = mid;
  }
  return mid;
}