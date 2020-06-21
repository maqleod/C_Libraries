/*
Inet utility functions
Written by Jared Epstein
*/

#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <resolv.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include "inet.h"

//to do: 
//add addr family as an input to allow both AF_INET and AF_INET6 functionality
//add error handling
//isReachable - check if a specific IP responds to ping
//get iface routes - list routes specific to a given iface
//get iface arp entries - list arp specific to given iface
//get gateways - list all gateways
//get routes - list all routes


/*
UTILITY
These functions are used by other functions, but can be used outside the library as well
*/

//get total number of arp entries
int arp_count()
{
    FILE *fp = fopen(ARPFILE,"r");
    int ch = 0;
    int lines = 0;

    lines++;
    while (!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n') {
            lines++;
        }
    }
    fclose(fp);
    return lines - 1;
}

//get total number of configured dns servers
int dns_server_count()
{
    res_init();
    return _res.nscount;
}

//get total number of installed interfaces
int interface_count()
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


/*
CHECKS
These functions are generally booleans
*/

//checks if given string is a valid IP address
bool isIpAddr(char *ipaddr)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipaddr, &(sa.sin_addr));
    return result;
}

//checks if given interface has the UP flag
bool isIfaceUp(char *iface)
{
    short flags;
    flags = get_iface_flags(iface);
    return !!(flags & IFF_UP);
}

//checks if given interface has the RUNNING flag
bool isIfaceLinked(char *iface)
{
    short flags;
    flags = get_iface_flags(iface);
    return !!(flags & IFF_RUNNING);
}

//checks if given interfaces has both UP and RUNNING flags, cuts down on ioctls if you want both
bool isIfaceUpandLinked(char *iface)
{
    short flags;
    flags = get_iface_flags(iface);
    return !!(flags & IFF_RUNNING) && (flags & IFF_UP);
}

bool hasArpEntry(char *ipaddr)
{
    int arpcount = arp_count(), a;
    char entries[arpcount][ARP_BUFFER_LEN];
    bool found = false;
    getArpTable(entries);

    for (a = 0; a < arpcount; a++) {
        if(strstr(entries[a], ipaddr) != NULL) {
            found = true;
        }
    }
    return found;
}


/*
CONVERSIONS
These functions convert between different data types
*/

//converts string based IP into a sockaddr_in struct with IP
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


/*
RETRIEVAL
These functions get information about interfaces or net config
*/

//reads arp table from /proc/net/arp
//NB: because this is pulled from a file, each line already contains a newline
void getArpTable(char entries[arp_count()][ARP_BUFFER_LEN])
{
    FILE *fp = fopen(ARPFILE, "r");
    char str[ARP_BUFFER_LEN];
    int i = 0;

    /* Ignore the first line, which contains the header */
    char header[ARP_BUFFER_LEN];
    fgets(header, sizeof(header), fp);

    while (fgets(str, ARP_BUFFER_LEN, fp)) {
        strcpy(entries[i], str);
        i++;
    }
    fclose(fp);
}

//gets an ARP entry from host IP
void getArpFromHost(char* ipaddr, char* entry)
{
    int arpcount = arp_count(), a;
    char entries[arpcount][ARP_BUFFER_LEN];
    getArpTable(entries);

    for (a = 0; a < arpcount; a++) {
        if(strstr(entries[a], ipaddr) != NULL) {
            strcpy(entry, entries[a]);
        }
    }
}

//gets array of ARP entries attached to a specified interface
void getArpFromIfname(char* ifname, char entries[arp_count()][ARP_BUFFER_LEN]) //arp_count() won't work quite right here because the number may get reduced
{
    int arpcount = arp_count(), a;
    char table_entries[arpcount][ARP_BUFFER_LEN];
    getArpTable(table_entries);

    for (a = 0; a < arpcount; a++) {
        if(strstr(table_entries[a], ifname) != NULL) {
            strcpy(entries[a], table_entries[a]);
        }
    }
}

//gets the mac address from a given ARP entry
void getMacFromArp(char* entry, char* mac)
{
    sscanf(entry, ARP_MAC_LINE_FORMAT, mac);
}

//gets the ip address from a given ARP entry
void getIPFromArp(char* entry, char* ipaddr)
{
    sscanf(entry, ARP_IP_LINE_FORMAT, ipaddr);
}

//gets the interface address from a ARP given entry
void getIFFromArp(char* entry, char* ifname)
{
    sscanf(entry, ARP_IF_LINE_FORMAT, ifname);
}

//gets a list of DNS servers configured on the system
void get_dns_servers(char dns_servers[dns_server_count()][INET_ADDRSTRLEN])
{
    int i;
    char str[INET_ADDRSTRLEN];
    res_init();

    for(i = 0; i < _res.nscount; i++) {
        SockIPtoStr(_res.nsaddr_list[i], str);
        strcpy(dns_servers[i], str);
    }
}

//gets a list of interfaces as strings
void get_interfaces(char interfaces[interface_count()][IFNAMSIZ])
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

//takes an interface name as string and returns a sockaddr_in IP
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

//takes an interface name as string and returns the mtu
int get_iface_mtu(char* iface)
{
    int s;
    int mtu;
    struct ifreq ifr;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    //ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ioctl(s, SIOCGIFMTU, &ifr);
    mtu = ifr.ifr_mtu;
    close(s);

    return mtu;
}

//retreives the mac address for a given iface name
void get_iface_mac(char* iface, char* mac)
{
    int s;
    struct ifreq ifr;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ioctl(s, SIOCGIFHWADDR, &ifr);
    bcopy(ifr.ifr_hwaddr.sa_data, mac, 6);
    close(s);
}

//retrieves flags for given iface name
short get_iface_flags(char* iface)
{
    int s;
    struct ifreq ifr;
    s = socket(AF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ioctl(s, SIOCGIFFLAGS, &ifr);
    close(s);

    return ifr.ifr_flags;
}
