/**************************************************************************
*
* Copyright 2016-2018 by Andrey Butok. FNET Community.
* Copyright 2018 NXP. Not a Contribution
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef _FNET_MDNS_PORT_H_
#define _FNET_MDNS_PORT_H_

#include "fnet_types.h"
#include "fnet_user_config.h"
#include "fnet_comp.h"


#if defined(FNET_PORT_LWIP)
#include "lwip/sockets.h"
#elif defined(FNET_PORT_WIFI_QCA)
#include "atheros_stack_offload.h"
#else
    #error "This port requires the definition of one of these macros: FNET_PORT_LWIP or FNET_PORT_WIFI_QCA"
#endif


#if defined(FNET_PORT_WIFI_QCA)
#define fnet_in_addr ip_addr
#define fnet_ip_mreq _ip_mreq

#define AF_UNSPEC (0)
#define AF_INET ATH_AF_INET
#define AF_INET6 ATH_AF_INET6

#define SOCK_DGRAM SOCK_DGRAM_TYPE
#define IPPROTO_IP ATH_IPPROTO_IP

#define FNET_HTONS A_CPU2BE16
#define FNET_HTONL A_CPU2BE32
#define FNET_NTOHL A_LE2CPU32

fnet_uint16_t fnet_htons(fnet_uint16_t short_var);
fnet_uint32_t fnet_htonl(fnet_uint32_t long_var);
#endif


#if defined(FNET_PORT_LWIP)
#define fnet_in_addr in_addr
#define fnet_ip_mreq ip_mreq

#define FNET_HTONS PP_HTONS
#define FNET_NTOHL PP_NTOHL
#define fnet_htons lwip_htons
#define fnet_htonl lwip_htonl
#endif


/**************************************************************************/ /*!
 * @def FNET_IP4_ADDR_INIT
 * @param a First octet of an IP address.
 * @param b Second octet of an IP address.
 * @param c Third  octet of an IP address.
 * @param d Fourth  octet of an IP address.
 * @hideinitializer
 * @brief Converts the standard dotted-decimal notation @c a.b.c.d
 *        to an integer value, suitable for use as an Internet address (in network byte order).
 ******************************************************************************/
#define FNET_IP4_ADDR_INIT(a, b, c, d)   ((fnet_ip4_addr_t)(FNET_NTOHL((((fnet_uint32_t)(a)&0xFFUL)<< 24U) + (((fnet_uint32_t)(b)&0xFFUL)<< 16U) + (((fnet_uint32_t)(c)&0xFFUL)<< 8U ) + ((fnet_uint32_t)(d)&0xFFUL))))


#define FNET_IP6_ADDR_INIT(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16)      \
    {{ (a1), (a2), (a3), (a4), (a5), (a6), (a7), (a8), (a9), (a10), (a11), (a12), (a13), (a14), (a15), (a16) }}

/* Copying address. */
#define FNET_IP6_ADDR_COPY(from_addr, to_addr)  \
    (fnet_memcpy( to_addr->s6_addr , from_addr->s6_addr , sizeof(fnet_ip6_addr_t)))


typedef int fnet_protocol_t;
typedef int fnet_socket_options_t;
typedef int fnet_socket_type_t;

/**************************************************************************/ /*!
 * @brief Socket descriptor.
 ******************************************************************************/
typedef int fnet_socket_t;

/**************************************************************************/ /*!
 * @brief Address family type.
 * @see AF_UNSPEC, AF_INET, AF_INET6, AF_SUPPORTED
 ******************************************************************************/
typedef fnet_uint16_t fnet_address_family_t;

/**************************************************************************/ /*!
 * @brief Unsigned integer type representing time uinits.
 * It can be ticks, seconds or milliseconds.
 ******************************************************************************/
typedef fnet_uint32_t fnet_time_t;

/**************************************************************************/ /*!
 * @brief 32-bit IPv4 address type.
 ******************************************************************************/
typedef fnet_uint32_t fnet_ip4_addr_t;

/**************************************************************************/ /*!
 * @brief Network interface descriptor.
 ******************************************************************************/
typedef void *fnet_netif_desc_t;

/**************************************************************************/ /*!
 * @brief Descriptor of a registered service.
 ******************************************************************************/
typedef void *fnet_service_desc_t;

/**************************************************************************/ /*!
 * @brief 128-bit IPv6 address type.
 ******************************************************************************/
FNET_COMP_PACKED_BEGIN
/* 128-bit IP6 address */
typedef union
{
    fnet_uint8_t   addr[16];
    fnet_uint16_t  addr16[8];
    fnet_uint32_t  addr32[4];
} fnet_ip6_addr_t;
FNET_COMP_PACKED_END


/**************************************************************************/ /*!
 * @brief Possible IPv6 address states.
 * @see fnet_netif_get_ip6_addr(), fnet_netif_ip6_addr_info
 ******************************************************************************/
typedef enum
{
    FNET_NETIF_IP6_ADDR_STATE_NOT_USED = 0,     /**< @brief Not used.*/
    FNET_NETIF_IP6_ADDR_STATE_TENTATIVE = 1,    /**< @brief Tentative address - (RFC4862) an address whose uniqueness on a link is being
                                                 * verified, prior to its assignment to an interface. A tentative
                                                 * address is not considered assigned to an interface in the usual
                                                 * sense. An interface discards received packets addressed to a
                                                 * tentative address, but accepts Neighbor Discovery packets related
                                                 * to Duplicate Address Detection for the tentative address.
                                                 */
    FNET_NETIF_IP6_ADDR_STATE_PREFERRED = 2 	/**< @brief Preferred address - (RFC4862) an address assigned to an interface whose use by
                                                 * upper-layer protocols is unrestricted. Preferred addresses may be
                                                 * used as the source (or destination) address of packets sent from
                                                 * (or to) the interface.
                                                 */
} fnet_netif_ip6_addr_state_t;

/**************************************************************************/ /*!
 * @brief Possible IPv6 address types.
 * @see fnet_netif_get_ip6_addr(), fnet_netif_ip6_addr_info
 ******************************************************************************/
typedef enum
{
    FNET_NETIF_IP_ADDR_TYPE_MANUAL = 0,            /**< @brief The address is set manually.*/
    FNET_NETIF_IP_ADDR_TYPE_AUTOCONFIGURABLE = 1,  /**< @brief The address is set using "Auto-IP" link-local autoconfiguration. */
    FNET_NETIF_IP_ADDR_TYPE_DHCP = 2               /**< @brief The address is set using DHCP. */
} fnet_netif_ip_addr_type_t;

/**************************************************************************/ /*!
 * @brief Interface IPv6 address information structure.
 * @see fnet_netif_get_ip6_addr()
 ******************************************************************************/
typedef struct fnet_netif_ip6_addr_info
{
    fnet_ip6_addr_t             address;            /**< @brief IPv6 address.*/
    fnet_netif_ip6_addr_state_t state;              /**< @brief Address current state.*/
    fnet_netif_ip_addr_type_t   type;               /**< @brief How the address was acquired.*/
} fnet_netif_ip6_addr_info_t;



/**************************************************************************/ /*!
 * @brief IPv4 Socket address structure.
 *
 * @see fnet_sockaddr
 *
 * To make manipulation of the @ref fnet_sockaddr structure easier, the TCP/IPv4 stack
 * also defines the equivalent structure @ref fnet_sockaddr_in
 * ("in" means "Internet").
 ******************************************************************************/
struct fnet_sockaddr_in
{
    fnet_address_family_t   sin_family;     /**< @brief Specifies the address family. @n
                                            * It must ne set to @ref AF_INET.*/
    fnet_uint16_t           sin_port;       /**< @brief 16-bit port number used to
                                            * demultiplex the transport-level messages
                                            * (in network byte order).*/
//    fnet_scope_id_t         sin_scope_id;   /**< @brief Scope zone index, defining network interface.*/
    struct fnet_in_addr     sin_addr;       /**< @brief 32-bit internet address.*/
};


/**************************************************************************/ /*!
 * @brief Size of sa_data[] field of @ref fnet_sockaddr structure. @n
 * It used to cover fnet_sockaddr_in and fnet_sockaddr_in6.
 * @showinitializer
 ******************************************************************************/
#if FNET_CFG_IP6
    #define FNET_SA_DATA_SIZE       (sizeof(struct fnet_in6_addr))
#else /* IPv4 */
    #define FNET_SA_DATA_SIZE       (sizeof(struct fnet_in_addr))
#endif


/**************************************************************************/ /*!
 * @brief Socket address structure.
 *
 * @see fnet_sockaddr_in, fnet_sockaddr_in6
 *
 * The original goal of the @ref fnet_sockaddr structure is to support multiple
 * protocols. For the TCP/IP stack @c sa_data, it contains a destination address
 * and port number for a socket.
 ******************************************************************************/
struct fnet_sockaddr
{
    fnet_address_family_t   sa_family;      /**< @brief Address family. Specifies the
                                            * address family, to which the address belongs. @n
                                            * It is defined by @ref fnet_address_family_t.*/
    fnet_uint16_t           sa_port;        /**< @brief 16-bit port number used to
                                            * demultiplex the transport-level messages
                                            * (in network byte order).*/
//    fnet_scope_id_t         sa_scope_id;    /**< @brief Scope zone index, defining network interface.*/
    fnet_uint8_t            sa_data[FNET_SA_DATA_SIZE];/**< @brief Address value. For the TCP/IP stack,
                                            * it contains the destination address and port
                                            * number for a socket.*/
};



fnet_socket_t fnet_socket( fnet_address_family_t family, fnet_socket_type_t type, fnet_uint32_t protocol );
fnet_return_t fnet_socket_bind( fnet_socket_t s, const struct fnet_sockaddr *name, fnet_size_t namelen );
fnet_ssize_t fnet_socket_recvfrom( fnet_socket_t s, void *buf, fnet_size_t len, fnet_flag_t flags, struct fnet_sockaddr *from, fnet_size_t *fromlen );
fnet_ssize_t fnet_socket_sendto( fnet_socket_t s, const void *buf, fnet_size_t len, fnet_flag_t flags, const struct fnet_sockaddr *to, fnet_size_t tolen );
fnet_return_t fnet_socket_close( fnet_socket_t s );
fnet_return_t fnet_socket_setopt( fnet_socket_t s, fnet_protocol_t level, fnet_socket_options_t optname, const void *optval, fnet_size_t optvallen );
fnet_time_t fnet_timer_get_ms( void );

fnet_ip4_addr_t fnet_netif_get_ip4_addr( fnet_netif_desc_t netif_desc );
fnet_bool_t fnet_netif_get_ip6_addr ( fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_addr_info_t *addr_info );


#endif /* _FNET_MDNS_PORT_H_ */
