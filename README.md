# ft_ping


Lets ping 163.172.250.16 or 42.fr
https://manpages.debian.org/bullseye/inetutils-ping/ping.1.en.html

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


IP Hdr Dump: 
 45 00 54 00 10 bb 00 40 09 01 0c cd 0a 00 02 0f a3 ac fa 10
Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data
 4  5  00 0054 10bb   0 0040  09  01 0ccd 10.0.2.15  163.172.250.16