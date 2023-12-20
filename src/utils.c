#include "header.h"

void handler(int code){
  summary();

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