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
#define ROUTEFILE "/proc/net/route"
#define MAC_BUF_SIZE 6
#define ATF_COM 0x02
#define STRING_LEN  1023
#define BUFFER_LEN  (STRING_LEN + 1)
#define ARP_ALL_LINE_FORMAT "%" xstr(STRING_LEN) "s %*s %*s %" xstr(STRING_LEN) "s %*s %" xstr(STRING_LEN) "s"
#define ARP_IP_LINE_FORMAT "%" xstr(STRING_LEN) "s %*s %*s %*s %*s %*s"
#define ARP_MAC_LINE_FORMAT "%*s %*s %*s %" xstr(STRING_LEN) "s %*s %*s"
#define ARP_IF_LINE_FORMAT "%*s %*s %*s %*s %*s %" xstr(STRING_LEN) "s"
#define ROUTE_ALL_LINE_FORMAT "%" xstr(STRING_LEN) "s %" xstr(STRING_LEN) "s %" xstr(STRING_LEN) "s %" xstr(STRING_LEN) "s %*s %*s %*s %" xstr(STRING_LEN) "s %*s %*s %*s"
#define ROUTE_IF_LINE_FORMAT "%" xstr(STRING_LEN) "s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s"
#define ROUTE_DST_LINE_FORMAT "%*s %" xstr(STRING_LEN) "s %*s %*s %*s %*s %*s %*s %*s %*s %*s"
#define ROUTE_GW_LINE_FORMAT "%*s %*s %" xstr(STRING_LEN) "s %*s %*s %*s %*s %*s %*s %*s %*s"
#define ROUTE_FL_LINE_FORMAT "%*s %*s %*s %" xstr(STRING_LEN) "s %*s %*s %*s %*s %*s %*s %*s"
#define ROUTE_NM_LINE_FORMAT "%*s %*s %*s %*s %*s %*s %*s %" xstr(STRING_LEN) "s %*s %*s %*s"

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

int count_lines(char* file);

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

struct sockaddr_in HexIPToSockIP(char* hexip, char* ipaddr);

void RTFlagsToStr(char* inflags, char* outflags);

/*
RETRIEVAL
These functions get information about interfaces or net config
*/

void getRouteTable(char entries[count_lines(ROUTEFILE)][BUFFER_LEN]);

void getDSTFromRoute(char* entry, char* dst);

void getIFFromRoute(char* entry, char* ifname);

void getGWFromRoute(char* entry, char* gw);

void getNMFromRoute(char* entry, char* netmask);

void getFLFromRoute(char* entry, char* flags);

void getArpTable(char entries[count_lines(ARPFILE)][BUFFER_LEN]);

void getArpFromHost(char* ipaddr, char* entry);

void getArpFromIfname(char* ifname, char entries[count_lines(ARPFILE)][BUFFER_LEN]);

void getMacFromArp(char* entry, char* mac);

void getIPFromArp(char* entry, char* ipaddr);

void getIFFromArp(char* entry, char* ifname);

void get_dns_servers(char dns_servers[dns_server_count()][INET_ADDRSTRLEN]);

void get_interfaces(char interfaces[interface_count()][IFNAMSIZ]);

struct sockaddr_in get_iface_ipaddr(char* iface);

int get_iface_mtu(char* iface);

void get_iface_mac(char* iface, char* mac);

short get_iface_flags(char* iface);

