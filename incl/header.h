#ifndef HEADER_H
#define HEADER_H

#include "../libft/libft.h"
#include "define.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/time.h>

#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#include <signal.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

typedef enum { false, true } bool;


/* Netwok.c */
void resolve(void);
void init_socket(void);

/* Packet.c*/

void	fill_icmp_header(void *buffer);
void fill_ip_header(void*buffer, int dest);

void fatal(const char* str);

void send_ping(int sig);
void receive_pong();
void fatal(const char* str);
void handler(int code);
void print_packet(void * packet);
void print_timeval(void * packet);
void fill_timestamp(void *buffer);
suseconds_t time_diff(struct timeval* then);


extern s_data ping_data;
#endif