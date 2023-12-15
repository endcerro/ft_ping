#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>


int getAddressFromHostname(const char* hostname, struct addrinfo **results) {
    // struct addrinfo hints;
    struct addrinfo *result;

    result = 0;

    int ret = getaddrinfo(hostname, 0, 0, &result);

    if (ret == 0) {
        *results = result;
        printf("Found info for %s !\n", hostname);
        return 0;
    }
    else {
        *results = result;
        printf("getaddrinfo error: %s\n", gai_strerror(ret));
        return 1;
    }
}