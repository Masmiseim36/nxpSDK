/*
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/*! \file wm_net.h
 *  \brief Network Abstraction Layer
 *
 * This provides the calls related to the network layer. The SDK uses lwIP as
 * the network stack.
 *
 * Here we document the network utility functions provided by the SDK. The
 * detailed lwIP API documentation can be found at:
 * http://lwip.wikia.com/wiki/Application_API_layers
 *
 */

#ifndef _WM_NET_H_
#define _WM_NET_H_

#include <string.h>

#include <lwip/opt.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/stats.h>
#include <lwip/icmp.h>
#include <lwip/ip.h>
#include <lwip/inet_chksum.h>
#include <lwip/pbuf.h>
#include <netif/etharp.h>

#include <wm_os.h>
#include <wmtypes.h>

#if CONFIG_IPV6 && !LWIP_IPV6
#error "CONFIG_IPV6 is enabled, but LWIP_IPV6 is not, enable it from lwipopts.h"
#elif LWIP_IPV6 && !CONFIG_IPV6
#error "LWIP_IPV6 is enabled, but CONFIG_IPV6 is not, enable it from wifi_config.h"
#endif

#if CONFIG_IPV6 && LWIP_IPV6
#ifndef CONFIG_MAX_IPV6_ADDRESSES
#error "Define CONFIG_MAX_IPV6_ADDRESSES same as LWIP_IPV6_NUM_ADDRESSES in wifi_config.h"
#else
#if CONFIG_MAX_IPV6_ADDRESSES != LWIP_IPV6_NUM_ADDRESSES
#error "CONFIG_MAX_IPV6_ADDRESSES must be equal to LWIP_IPV6_NUM_ADDRESSES"
#endif
#endif
#endif

#if (!defined(LWIP_NETIF_EXT_STATUS_CALLBACK) || (LWIP_NETIF_EXT_STATUS_CALLBACK == 0))
#error "Define LWIP_NETIF_EXT_STATUS_CALLBACK as 1 in lwipopts.h"
#endif

/*
 * fixme: This dependancy of wm_net on wlc manager header should be
 * removed. This is the lowest level file used to access lwip
 * functionality and should not contain higher layer dependancies.
 */
#include <wlan.h>

#define NET_SUCCESS WM_SUCCESS
#define NET_ERROR   (-WM_FAIL)
#define NET_ENOBUFS ENOBUFS

#define NET_BLOCKING_OFF 1
#define NET_BLOCKING_ON  0

/* Error Codes
 * lwIP provides all standard errnos defined in arch.h, hence no need to
 * redefine them here.
 * */

/* To be consistent with naming convention */
#define net_socket(domain, type, protocol)            socket(domain, type, protocol)
#define net_select(nfd, read, write, except, timeout) select(nfd, read, write, except, timeout)
#define net_bind(sock, addr, len)                     bind(sock, addr, len)
#define net_listen(sock, backlog)                     listen(sock, backlog)
#define net_close(c)                                  close((c))
#define net_accept(sock, addr, len)                   accept(sock, addr, len)
#define net_shutdown(c, b)                            shutdown(c, b)
#define net_connect(sock, addr, len)                  connect(sock, addr, len)
#define net_read(sock, data, len)                     read(sock, data, len)
#define net_write(sock, data, len)                    write(sock, data, len)

/** Set hostname for network interface
 *
 * \param[in] hostname Hostname to be set.
 *
 * \note NULL is a valid value for hostname.
 *
 * \return WM_SUCESS
 */
int net_dhcp_hostname_set(char *hostname);

/** Deactivate the dhcp timer
 *
 */
void net_stop_dhcp_timer(void);

/** Set socket blocking option as on or off
 *
 * \param[in] sock socket number to be set for blocking option.
 * \param[in] state set blocking on or off
 *
 * \return WM_SUCESS otherwise standard LWIP error codes.
 */
static inline int net_socket_blocking(int sock, int state)
{
    return ioctlsocket(sock, FIONBIO, &state);
}

/** Get error number from provided socket
 *
 * \param[in] sock socket number to get error number.
 *
 * \return error number.
 */
static inline int net_get_sock_error(int sock)
{
    int ret = 0;
    switch (errno)
    {
        case EWOULDBLOCK:
            ret = -WM_E_AGAIN;
            break;
        case EBADF:
            ret = -WM_E_BADF;
            break;
        case ENOBUFS:
            ret = -WM_E_NOMEM;
            break;
        default:
            ret = errno;
            break;
    }
    return ret;
}

/** Converts Internet host address from the IPv4 dotted-decimal notation into
 * binary form (in network byte order)
 *
 * \param[in] cp IPv4 host address in dotted-decimal notation.
 *
 * \return IPv4 address in binary form
 */
static inline uint32_t net_inet_aton(const char *cp)
{
    struct in_addr addr;
    addr.s_addr = 0;
    (void)inet_aton(cp, ((void *)&addr));
    return addr.s_addr;
}

/**
 * Get network host entry
 *
 * \param[in] cp Hostname or an IPv4 address in the standard dot notation.
 * \param[in] hentry Pointer to pointer of host entry structure.
 *
 * \note This function is not thread safe. If thread safety is required
 * please use lwip_getaddrinfo() - lwip_freeaddrinfo() combination.
 *
 * \return WM_SUCESS if operation successful.
 * \return -WM_FAIL if operation fails.
 */
static inline int net_gethostbyname(const char *cp, struct hostent **hentry)
{
    struct hostent *he;
    if ((he = gethostbyname(cp)) == NULL)
    {
        return -WM_FAIL;
    }

    *hentry = he;
    return WM_SUCCESS;
}

/** Converts Internet host address in network byte order to a string in IPv4
 * dotted-decimal notation
 *
 * \param[in] addr IP address in network byte order.
 * \param[out] cp buffer in which IPv4 dotted-decimal string is returned.
 *
 */
static inline void net_inet_ntoa(unsigned long addr, char *cp)
{
    struct ip4_addr saddr;
    saddr.addr = addr;
    /* No length, sigh! */
    (void)strcpy(cp, inet_ntoa(saddr));
}

/** Check whether buffer is IPv4 or IPV6 packet type
 *
 * \param[in] buffer pointer to buffer where packet to be checked located.
 *
 * \return true if buffer packet type matches with IPv4 or IPv6, false otherwise.
 *
 */
static inline bool net_is_ip_or_ipv6(const uint8_t *buffer)
{
    if (((const struct eth_hdr *)(const void *)buffer)->type == PP_HTONS(ETHTYPE_IP) ||
        ((const struct eth_hdr *)(const void *)buffer)->type == PP_HTONS(ETHTYPE_IPV6))
    {
        return true;
    }
    return false;
}

/** Get interface handle from socket descriptor
 *
 * Given a socket descriptor this API returns which interface it is bound with.
 *
 * \param[in] sock socket descriptor
 *
 * \return[out] interface handle
 */
void *net_sock_to_interface(int sock);

/** Initialize TCP/IP networking stack
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wlan_init(void);

/** DiInitialize TCP/IP networking stack
 *
 * \return WM_SUCCESS on success
 * \return -WM_FAIL otherwise
 */
int net_wlan_deinit(void);

/** Get station interface handle
 *
 * Some APIs require the interface handle to be passed to them. The handle can
 * be retrieved using this API.
 *
 * \return station interface handle
 */
void *net_get_sta_handle(void);
#define net_get_mlan_handle() net_get_sta_handle()

/** Get micro-AP interface handle
 *
 * Some APIs require the interface handle to be passed to them. The handle can
 * be retrieved using this API.
 *
 * \return micro-AP interface handle
 */
void *net_get_uap_handle(void);

/** Take interface up
 *
 * Change interface state to up. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_up(void *intrfc_handle);

/** Take interface down
 *
 * Change interface state to down. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_down(void *intrfc_handle);

/** Stop DHCP client on given interface
 *
 * Stop the DHCP client on given interface state. Use net_get_sta_handle(),
 * net_get_uap_handle() to get interface handle.
 *
 * \param[in] intrfc_handle interface handle
 *
 * \return void
 */
void net_interface_dhcp_stop(void *intrfc_handle);

/** Configure IP address for interface
 *
 * \param[in] addr Address that needs to be configured.
 * \param[in] intrfc_handle Handle for network interface to be configured.
 *
 * \return WM_SUCCESS on success or an error code.
 */
int net_configure_address(struct wlan_ip_config *addr, void *intrfc_handle);

/** Configure DNS server address
 *
 * \param[in] ip IP address of the DNS server to set
 * \param[in] role Network wireless BSS Role
 *
 */
void net_configure_dns(struct wlan_ip_config *ip, enum wlan_bss_role role);

/** Get interface IP Address in \ref wlan_ip_config
 *
 * This function will get the IP address of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] addr \ref wlan_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_addr(struct wlan_ip_config *addr, void *intrfc_handle);

#ifdef CONFIG_IPV6
/** Get interface IPv6 Addresses & their states in \ref wlan_ip_config
 *
 * This function will get the IPv6 addresses & address states of a given
 * interface. Use net_get_sta_handle() to get interface handle.
 *
 * \param[out] addr \ref wlan_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_ipv6_addr(struct wlan_ip_config *addr, void *intrfc_handle);

/** Get list of preferred IPv6 Addresses of a given interface
 * in \ref wlan_ip_config
 *
 * This function will get the list of IPv6 addresses whose address state
 * is Preferred.
 * Use net_get_sta_handle() to get interface handle.
 *
 * \param[out] addr \ref wlan_ip_config
 * \param[in] intrfc_handle interface handle
 *
 * \return Number of IPv6 addresses whose address state is Preferred
 */
int net_get_if_ipv6_pref_addr(struct wlan_ip_config *addr, void *intrfc_handle);

/** Get the description of IPv6 address state
 *
 * This function will get the IPv6 address state description like -
 * Invalid, Preferred, Deprecated
 *
 * \param[in] addr_state Address state
 *
 * \return IPv6 address state description
 */
char *ipv6_addr_state_to_desc(unsigned char addr_state);

/** Get the description of IPv6 address
 *
 * This function will get the IPv6 address type description like -
 * Linklocal, Global, Sitelocal, Uniquelocal
 *
 * \param[in] ipv6_conf Pointer to IPv6 configuration of type \ref ipv6_config
 *
 * \return IPv6 address description
 */
char *ipv6_addr_addr_to_desc(struct ipv6_config *ipv6_conf);

/** Get the description of IPv6 address type
 *
 * This function will get the IPv6 address type description like -
 * Linklocal, Global, Sitelocal, Uniquelocal
 *
 * \param[in] ipv6_conf Pointer to IPv6 configuration of type \ref ipv6_config
 *
 * \return IPv6 address type description
 */
char *ipv6_addr_type_to_desc(struct ipv6_config *ipv6_conf);
#endif /* CONFIG_IPV6 */

/** Get interface Name string containing name and number
 *
 * This function will get the string containing name and number for given interface.
 * Use net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] if_name interface name pointer
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_name(char *if_name, void *intrfc_handle);

/** Get interface IP Address
 *
 * This function will get the IP Address of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[out] ip ip address pointer
 * \param[in] intrfc_handle interface handle
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_ip_addr(uint32_t *ip, void *intrfc_handle);

/** Get interface IP Subnet-Mask
 *
 * This function will get the Subnet-Mask of a given interface. Use
 * net_get_sta_handle(), net_get_uap_handle() to get
 * interface handle.
 *
 * \param[in] mask Subnet Mask pointer
 * \param[in] intrfc_handle interface
 *
 * \return WM_SUCCESS on success or error code.
 */
int net_get_if_ip_mask(uint32_t *nm, void *intrfc_handle);

/** Initialize the network stack
 *
 *  This function initializes the network stack. This function is
 *  called by wlan_start().
 *
 *  Applications may optionally call this function directly:
 *  if they wish to use the networking stack (loopback interface)
 *  without the wlan functionality.
 *  if they wish to initialize the networking stack even before wlan
 *  comes up.
 *
 * \note This function may safely be called multiple times.
 */
void net_ipv4stack_init(void);

#ifdef CONFIG_IPV6
/** Initialize the IPv6 network stack
 *
 * \param[in] netif network interface on which ipv6 stack is initialized.
 *
 */
void net_ipv6stack_init(struct netif *netif);
#endif

/** Display network statistics
 */
void net_stat(void);


#endif /* _WM_NET_H_ */
