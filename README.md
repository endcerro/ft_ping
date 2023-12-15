# ft_ping


Lets ping 163.172.250.16 or 42.fr


Allowd functions are :

gettimeofday : explicit
exit : explicit
inet_ntop / inet_pton : convert IP sstring format to dest and inverse
nthos / htons : convert host to network bytes and inverse
signal / alarm / usleep : explicit
socket / setsockopt / close 
sendto / recvmsg : sendto can be use to initiate connection as well it seems
use sendto + SOCK_RAW for the socket
getpid / getuid : to check if root 
getaddrinfo / getnameinfo / freeaddrinfo.
strerror / gai_strerror.

fcntl, poll et ppoll is strictly forbidden.