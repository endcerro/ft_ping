#include "header.h"
#include <netinet/ip_icmp.h>



void summary(void)
{

    float ratio = (1.0f - ping_data.stats.total_received / (float)ping_data.sequence) * 100.0f;
    // int ratio = (ping_data.stats.total_received / ping_data.sequence ) * 100; 
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d.%dms\n", 
    ping_data.sequence, ping_data.stats.total_received, (int)ratio,0 ,0);

    printf("round-trip min/avg/max/stddev %d.%d/%d.%d/%d.%d/%d.%d ms\n", 
    ping_data.min.tv_sec * 1000000, ping_data.stats.total_received, (int)ratio,0 ,0);

}