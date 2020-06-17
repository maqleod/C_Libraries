/*
Inet utility functions
Written by Jared Epstein
*/
#include <stdbool.h>
#include <resolv.h>
#include <arpa/nameser.h>
#include <net/if.h>

#define DNS_SERVER_COUNT dns_server_count()
#define INTERFACE_COUNT interface_count()

//populate the dns server limit with the actual number of entries
static inline int dns_server_count()
{
    res_init();
    return _res.nscount;
}

//populate the interface limit with the actual number of interfaces
static inline int interface_count()
{
    int i = 0;
    struct if_nameindex *if_nidxs, *intf;
    if_nidxs = if_nameindex();
    if ( if_nidxs != NULL ) {
        for (intf = if_nidxs; intf->if_index != 0 || intf->if_name != NULL; intf++) {
            i++;
        }
    }
    if_freenameindex(if_nidxs);
    return i;
}

bool isIpAddr(char *ipaddr);

struct sockaddr_in StrtoSockIP(char ipaddr[INET_ADDRSTRLEN]);

struct sockaddr_in sockaddrCreate(char ipaddr[INET_ADDRSTRLEN], uint16_t port);

void SockIPtoStr(struct sockaddr_in sa, char* ipaddr);

uint16_t SockPorttoStr(struct sockaddr_in sa);

void get_dns_servers(char dns_servers[DNS_SERVER_COUNT][INET_ADDRSTRLEN]);

void get_interfaces(char interfaces[INTERFACE_COUNT][IFNAMSIZ]);

struct sockaddr_in get_iface_ipaddr(char* iface);
