/*
Inet utility functions
Written by Jared Epstein
*/
#include <stdbool.h>
#include <resolv.h>
#include <sys/ioctl.h>
#include <arpa/nameser.h>
#include <net/if.h>

#define xstr(s) str(s)
#define str(s) #s
#define ARPFILE "/proc/net/arp"
#define MAC_BUF_SIZE 6
#define ATF_COM 0x02
#define ARP_STRING_LEN  1023
#define ARP_BUFFER_LEN  (ARP_STRING_LEN + 1)
#define ARP_IP_LINE_FORMAT "%" xstr(ARP_STRING_LEN) "s %*s %*s %*s %*s %*s"
#define ARP_MAC_LINE_FORMAT "%*s %*s %*s %" xstr(ARP_STRING_LEN) "s %*s %*s"
#define ARP_IF_LINE_FORMAT "%*s %*s %*s %*s %*s %" xstr(ARP_STRING_LEN) "s"

struct intf
{
    char name[IFNAMSIZ];
    char mac[ETH_ALEN];
    int mtu;
    bool admin_status;
    bool link_status;
    int flags;
    //inaddr ips
};


/*
UTILITY
These functions are used by other functions, but can be used outside the library as well
*/

int arp_count();

int interface_count();

int dns_server_count();

/*
CHECKS
These functions are generally booleans
*/

bool isIpAddr(char *ipaddr);

bool isIfaceUp(char *iface);

bool isIfaceLinked(char *iface);

bool isIfaceUpandLinked(char *iface);

bool hasArpEntry(char *ipaddr);

/*
CONVERSIONS
These functions convert between different data types
*/

struct sockaddr_in StrtoSockIP(char ipaddr[INET_ADDRSTRLEN]);

struct sockaddr_in sockaddrCreate(char ipaddr[INET_ADDRSTRLEN], uint16_t port);

void SockIPtoStr(struct sockaddr_in sa, char* ipaddr);

uint16_t SockPorttoStr(struct sockaddr_in sa);

/*
RETRIEVAL
These functions get information about interfaces or net config
*/

void getArpTable(char entries[arp_count()][ARP_BUFFER_LEN]);

void getArpFromHost(char* ipaddr, char* entry);

void getArpFromIfname(char* ifname, char entries[arp_count()][ARP_BUFFER_LEN]);

void getMacFromArp(char* entry, char* mac);

void getIPFromArp(char* entry, char* ipaddr);

void getIFFromArp(char* entry, char* ifname);

void get_dns_servers(char dns_servers[dns_server_count()][INET_ADDRSTRLEN]);

void get_interfaces(char interfaces[interface_count()][IFNAMSIZ]);

struct sockaddr_in get_iface_ipaddr(char* iface);

int get_iface_mtu(char* iface);

void get_iface_mac(char* iface, char* mac);

short get_iface_flags(char* iface);

