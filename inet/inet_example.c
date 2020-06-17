#include <stdio.h>
#include <net/if.h>
#include <netinet/in.h>
#include "inet.h"

int main(int argc, char* argv[])
{
    char ip[INET_ADDRSTRLEN] = "192.168.2.1";
    char str[INET_ADDRSTRLEN]; 
    struct sockaddr_in sa, sa_if;
    uint16_t port = 6048;
    char dns_servers[DNS_SERVER_COUNT][INET_ADDRSTRLEN];
    char interfaces[INTERFACE_COUNT][IFNAMSIZ];
    int a;
    
    sa = sockaddrCreate(ip, port);
    printf("Converted to sockaddr_in\n");
    SockIPtoStr(sa, str);
    printf("Converted IP %s\n", str);
    port = SockPorttoStr(sa);
    printf("Converted Port %d\n", port);
    get_dns_servers(dns_servers);
    for (a = 0; a < DNS_SERVER_COUNT; a++) {
        printf("DNS server %s\n", dns_servers[a]);
    }
    get_interfaces(interfaces);
    for (a = 0; a < INTERFACE_COUNT; a++) {
        printf("Interface %s\n", interfaces[a]);
        sa_if = get_iface_ipaddr(interfaces[a]);
        SockIPtoStr(sa_if, str);
        printf("\tIP %s\n", str);
    }
    return 0;
}
