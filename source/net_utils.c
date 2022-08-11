/*
    @file net_utils.c
    Utilities to handle network interfaces at system level

 */

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <netinet/in.h>
#include "libemutils.h"

typedef struct ifaddrs ifaddrs_t;
typedef struct _if_addrs_local {
    ifaddrs_t *ifaddr;
} ifaddrsptr_t;

static int _updateInterfaceList(ifaddrsptr_t *if_list)
{
    int ret = OK;

	if (getifaddrs(&if_list->ifaddr) == -1) {
		printf("ERROR fetching interface list\n");
        ret = ERROR;
	}

    return OK;
}

static ifaddrs_t* _findInterfaceInList(ifaddrsptr_t *if_list, const char *interface)
{
    ifaddrs_t *if_entry = NULL;
    if (_updateInterfaceList(if_list) == OK) {
        /* Cycle through ifaddr linked list */
        for (if_entry = if_list->ifaddr; if_entry != NULL; if_entry = if_entry->ifa_next) {
            if(strncmp(interface, if_entry->ifa_name, strlen(interface)) == 0) {
                // Found
                break;
            }
        }
    }

    return if_entry;
}

int getInterfaceIp(const char* interface, char *ipaddr)
{
	int ret = ERROR;
    ifaddrsptr_t if_list;
	ifaddrs_t *ifa;

    if (!interface || !ipaddr)
        return ret;

    ipaddr[0] = 0;

    ifa = _findInterfaceInList(&if_list, interface);
    if (ifa) {
    	char host[NI_MAXHOST];

        if (ifa->ifa_addr == NULL) {
            printf("Interface %s, NO IP address assigned\n", interface);
        } else {
            int family, s;
            family = ifa->ifa_addr->sa_family;

            if (family == AF_INET || family == AF_INET6) {
                s = getnameinfo(ifa->ifa_addr,
                        (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                if (s != 0) {
                    printf("ERROR getting interface info %s\n", gai_strerror(s));
                } else {
                    ret = OK;
                    strncpy(ipaddr, host, strlen(host));
                }

                printf("Interface %s, IP address: %s\n", interface, ipaddr);
            }
        }
	}

    if (if_list.ifaddr != NULL) {
        freeifaddrs(if_list.ifaddr);
    }

	return ret;
}

int getInterfaceStats(const char* interface)
{
	int ret = ERROR;
    ifaddrsptr_t if_list;
	ifaddrs_t *ifa;

    if (!interface)
        return ret;

    ifa = _findInterfaceInList(&if_list, interface);
    if (ifa) {
    	int family, s;
    	char host[NI_MAXHOST];

        struct rtnl_link_stats* stats = (struct rtnl_link_stats*)ifa->ifa_data;

        printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                stats->tx_packets,
                stats->rx_packets,
                stats->tx_bytes,
                stats->rx_bytes);
	}

    if (if_list.ifaddr != NULL) {
        freeifaddrs(if_list.ifaddr);
    }

    return ret;
}

//TODO:
//getdefaultRoute
//getInterfaceStatus(char *if_name)
//isSocketValid(int fd)
//numIpToStr(u8 *ipaddr)
//strIptoNum(char *ipaddr)
//numMacToStr(u8 *mac)
//strMacToNum(char *mac)
