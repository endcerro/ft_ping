#include "header.h"
#include <netinet/ip_icmp.h>
#include <math.h>


void summary(void)
{

    float ratio = (1.0f - ping_data.stats.total_received / (float)ping_data.sequence) * 100.0f;
    // int ratio = (ping_data.stats.total_received / ping_data.sequence ) * 100; 
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d.%dms\n", 
    ping_data.sequence, ping_data.stats.total_received, (int)ratio,0 ,0);

    suseconds_t	mdev;
    mdev = ping_data.stats.total / ping_data.sequence;
    ping_data.stats.sq_total /= ping_data.stats.total_received;
    mdev = sqrtl(ping_data.stats.sq_total - mdev * mdev);

    printf("round-trip min/avg/max/stddev %.3f/%.3f/%.3f/%.3f ms\n", 
    ping_data.stats.min / 1000.0f,
    (ping_data.stats.total / (float) ping_data.stats.total_received) / 1000.0f, 
    ping_data.stats.max / 1000.0f,
    mdev / 1000.0f);


}