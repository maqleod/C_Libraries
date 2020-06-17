/*
Inet utility functions
Written by Jared Epstein
*/

#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <resolv.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netinet/in.h>
#include "inet.h"

//to do: add addr family as an input to allow both AF_INET and AF_INET6 functionality

//checks if given string is a valid IP address
bool isIpAddr(char *ipaddr)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
    return result;
}

//converts string based IP into an sockaddr_in struct with IP
struct sockaddr_in StrtoSockIP(char ipaddr[INET_ADDRSTRLEN])
{
    struct sockaddr_in sa;
    inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
    return sa;
}

//given an IP string and a port number, returns a completed sockaddr_in structure
struct sockaddr_in sockaddrCreate(char ipaddr[INET_ADDRSTRLEN], uint16_t port)
{
    struct sockaddr_in sa;
    sa = StrtoSockIP(ipaddr);
    sa.sin_port = htons(port);
    return sa;
}

//converts sockaddr_in struct IP into a human readable string
void SockIPtoStr(struct sockaddr_in sa, char* ipaddr)
{
    inet_ntop(AF_INET, &(sa.sin_addr), ipaddr, INET_ADDRSTRLEN);
}

//converts sockaddr_in struct port into a human readable string
uint16_t SockPorttoStr(struct sockaddr_in sa)
{
    uint16_t port;
    port = htons(sa.sin_port);
    return port;
}

//gets a list of DNS servers configured on the system
void get_dns_servers(char dns_servers[DNS_SERVER_COUNT][INET_ADDRSTRLEN])
{
    int i;
    char str[INET_ADDRSTRLEN];
    res_init();
    
    for(i = 0; i < _res.nscount; i++) {
        SockIPtoStr(_res.nsaddr_list[i], str);
        strcpy(dns_servers[i], str);
    }
}

void get_interfaces(char interfaces[INTERFACE_COUNT][IFNAMSIZ])
{
    int i = 0;
    struct if_nameindex *if_nidxs, *intf;
    
    if_nidxs = if_nameindex();
    if ( if_nidxs != NULL ) {
        for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++) {
            strcpy(interfaces[i], intf->if_name);
            i++;
        }
        if_freenameindex(if_nidxs);
    }
}

struct sockaddr_in get_iface_ipaddr(char* iface)
{
    int s;
    struct ifreq ifr;
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ioctl(s, SIOCGIFADDR, &ifr);
    struct sockaddr_in sa = *(struct sockaddr_in *)&ifr.ifr_addr;
    close(s);

    return sa;
}
