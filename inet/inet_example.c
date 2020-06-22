#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include "inet.h"

int main(int argc, char* argv[])
{
    char ip[INET_ADDRSTRLEN] = "192.168.2.1";
    char str[INET_ADDRSTRLEN];
    char mac[22];
    short flags;
    struct sockaddr_in sa, sa_if;
    bool entry;
    char hwaddr[BUFFER_LEN];
    uint16_t port = 6048;
    int routecount = count_lines(ROUTEFILE);
    int arpcount = count_lines(ARPFILE);
    int interfacecount = interface_count();
    int dnsservercount = dns_server_count();
    char dns_servers[dnsservercount][INET_ADDRSTRLEN];
    char interfaces[interfacecount][IFNAMSIZ];
    char entries[arpcount][BUFFER_LEN];
    char routes[routecount][BUFFER_LEN];
    char arp_entry[BUFFER_LEN];
    int a;
    int mtu;
    printf("arp file has %d entries\n", arpcount);
    printf("route file has %d entries\n", routecount);
    
    sa = sockaddrCreate(ip, port);
    printf("Converted to sockaddr_in\n");
    SockIPtoStr(sa, str);
    printf("Converted IP %s\n", str);
    port = SockPorttoStr(sa);
    printf("Converted Port %d\n", port);
    getArpFromHost(ip, arp_entry);
    printf("Found Entry %s", arp_entry);
    entry = hasArpEntry(ip);
    if (entry) {
        getMacFromArp(arp_entry, hwaddr);
        printf("%s found in arp cache\n", ip);
        printf("hwaddr is %s\n", hwaddr);
    } else {
        printf("%s not found in arp cache\n", ip);
    }
    get_dns_servers(dns_servers);
    for (a = 0; a < dnsservercount; a++) {
        printf("DNS server %s\n", dns_servers[a]);
    }
    getArpTable(entries);
    for (a = 0; a < arpcount; a++) {
        printf("ARP entry %s", entries[a]);
    }
    getRouteTable(routes);
    for (a = 0; a < routecount; a++) {
        printf("Route entry %s", routes[a]);
    }
    get_interfaces(interfaces);
    for (a = 0; a < interfacecount; a++) {
        printf("Interface %s", interfaces[a]);
        if (isIfaceUpandLinked(interfaces[a])) {
            printf(" UP\n");
        } else {
            printf(" DOWN\n");
        }
        flags = get_iface_flags(interfaces[a]);
        printf("\tflags %d\n", flags);
        get_iface_mac(interfaces[a], mac);
        printf("\tmac %02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx\n",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        sa_if = get_iface_ipaddr(interfaces[a]);
        SockIPtoStr(sa_if, str);
        printf("\tIP %s\n", str);
        //printf("\tmac %s\n", mac);
        mtu = get_iface_mtu(interfaces[a]);
        printf("\tmtu %d\n", mtu);
    }
    return 0;
}
