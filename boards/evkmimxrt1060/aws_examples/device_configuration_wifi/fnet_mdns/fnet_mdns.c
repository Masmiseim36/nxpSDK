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
*
***************************************************************************
*
*  mDNS (Bonjour) Server/Responder implementation (RFC6762).
*
***************************************************************************/

#include "fnet_mdns.h"
#include "fnet_mdns_config.h"

#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#if defined(FNET_PORT_LWIP)
#include "lwip/sockets.h"
#include "lwip/def.h"
#elif defined(FNET_PORT_WIFI_QCA)
#include "atheros_stack_offload.h"
#include "a_osapi.h"
#endif


#if FNET_CFG_MDNS

#define fnet_strlen strlen
#define fnet_memset memset
#define fnet_memset_zero(ptr, num) memset(ptr, 0, num)
#define fnet_memcpy memcpy
#define fnet_memcmp memcmp
#define fnet_strcmp strcmp
#define fnet_strcpy strcpy
#define fnet_tolower tolower
#define fnet_snprintf snprintf
#define fnet_print PRINTF

#define fnet_service_mutex_lock()
#define fnet_service_mutex_unlock()

#define FNET_ASSERT assert


/* Size limits. */
#define FNET_DNS_MAME_SIZE      (255U)     /* RFC1035:To simplify implementations, the total length of a domain name (i.e.,
                                           * label octets and label length octets) is restricted to 255 octets or less.*/

/**************************************************************************/ /*!
 * @brief Bitmask of supported address families.
 * @showinitializer
 ******************************************************************************/
#define AF_SUPPORTED ((fnet_address_family_t)((fnet_address_family_t)(AF_INET6*(fnet_address_family_t)FNET_CFG_IP6) | (fnet_address_family_t)(AF_INET*(fnet_address_family_t)FNET_CFG_IP4)))



#if FNET_CFG_DEBUG_MDNS && FNET_CFG_DEBUG
    #define FNET_DEBUG_MDNS(...)   PRINTF(__VA_ARGS__); PRINTF("\r\n")
#else
    #define FNET_DEBUG_MDNS(...)   do{}while(0)
#endif

///* mDNS requires multicast support.*/
//#if FNET_CFG_MULTICAST == 0
//    #error FNET_CFG_MULTICAST must be enabled for MDNS
//#endif

/************************************************************************
*     Definitions
*************************************************************************/
/* MDNS settings */
#define FNET_MDNS_DOMAIN_NAME              "local"                 /* mDNS top level domain (TLD) name: local */
#define FNET_MDNS_PORT                     FNET_CFG_MDNS_PORT      /* mDNS port */

/*  Service Type Enumeration. A DNS query for PTR records with the name "_services._dns-sd._udp.<Domain> */
#define FNET_MDNS_SERVICE_TYPE_ENUMERATION_NAME     "_services._dns-sd._udp"

#define FNET_MDNS_WAIT                     250                     /* Wait 250ms to check domain name. */
#define FNET_MDNS_PROBE_WAIT               (5*1000)                /* Wait 5 seconds before trying again. */
#define FNET_MDNS_PROBE_DEFER_WAIT         (1*1000)                /* Defers to the winning host by waiting one second. */
#define FNET_MDNS_PROBE_INIT_WAIT          (200)                   /* RFC6762: When ready to send its Multicast DNS probe packet(s) the host should
                                                                    first wait for a short random delay time, uniformly distributed in
                                                                    the range 0-250 ms. This random delay is to guard against the case
                                                                    where several devices are powered on simultaneously, or several
                                                                    devices are connected to an Ethernet hub, which is then powered on,
                                                                    or some other external event happens that might cause a group of
                                                                    hosts to all send synchronized probes.*/

/* Delay for shared response. Random amount of time selected with uniform random distribution in the range 20-120 ms. */
#define FNET_MDNS_SHARED_RESPONSE_DELAY_MIN (20)                    /* Minimum delay for shared response 20 ms.*/
#define FNET_MDNS_SHARED_RESPONSE_DELAY_MAX (120)                   /* Maximum delay for shared response 120 ms.*/

#define FNET_MDNS_ANNOUNCE_COUNT           (2)                     /* The Multicast DNS responder MUST send at least two unsolicited responses. */
#define FNET_MDNS_ANNOUNCE_INTERVAL        (1000)                  /* one second apart. */

#define FNET_MDNS_HEADER_CLASS_IN          0x01                    /* Internet */
#define FNET_MDNS_HEADER_CACHE_FLUSH       0x8000                  /* Cache flush */

#define FNET_MDNS_DOMAIN_NAME_COMPRESSION  0xC0                    /* Domain Name Compression flag */

/* MDNS Header Flags.*/
#define FNET_MDNS_HEADER_FLAGS_QR           0x8000
#define FNET_MDNS_HEADER_FLAGS_OPCODE       0x7800
#define FNET_MDNS_HEADER_FLAGS_TC           0x0200                  /* Trancation Flag */
#define FNET_MDNS_HEADER_FLAGS_C            0x0400

#define FNET_MDNS_PACKET_SIZE               512                     /* mDNS maximum packet size*/

/* [RFC6762] All Multicast DNS responses (including responses sent via unicast)
* SHOULD be sent with IP TTL set to 255.  This is recommended to
* provide backwards-compatibility with older Multicast DNS queriers
* (implementing a draft version of this document, posted in February
* 2004) that check the IP TTL on reception to determine whether the
* packet originated on the local link.  These older queriers discard
* all packets with TTLs other than 255.*/
#define FNET_MDNS_IP_TTL              (255u)

#define FNET_MDNS_NAME_MAX              64  /**< Maximum length for MDNS name. */
#define FNET_MDNS_HOST_NAME_LEN_MAX     (FNET_MDNS_NAME_MAX+sizeof(" 4294967295"))               /**< Maximum length of the host name. */
#define FNET_MDNS_RR_NAME_LEN_MAX       (FNET_MDNS_HOST_NAME_LEN_MAX+sizeof("._saccessdata-f2d._tcp."FNET_MDNS_DOMAIN_NAME)) /**< Maximum length of the RR name. */
/* Maximum RR record size of probe RR - SRV RR*/
#define FNET_MDNS_RR_PROBE_LEN_MAX    (FNET_MDNS_RR_NAME_LEN_MAX + sizeof(fnet_mdns_rr_header_t) + sizeof(fnet_mdns_rr_data_srv_t) + FNET_MDNS_RR_NAME_LEN_MAX ) /**< Maximum length of the SRV RR. */

/* Error strings.*/
#define FNET_MDNS_ERR_PARAMS             "MDNS: Wrong input parameters."
#define FNET_MDNS_ERR_SOCKET_CREATION    "MDNS: Socket creation error."
#define FNET_MDNS_ERR_SOCKET_BIND        "MDNS: Socket Error during bind."
#define FNET_MDNS_ERR_SERVICE            "MDNS: Service registration is failed."
#define FNET_MDNS_ERR_IS_INITIALIZED     "MDNS: Is already initialized."
#define FNET_MDNS_ERR_JOIN_MULTICAST     "MDNS: Joining to multicast group is failed."

/*******************************************************************************
* Types definitions
*******************************************************************************/
/* RR record types */
typedef enum
{
    FNET_MDNS_RR_PTR   = 0x000C,    /* PTR record */
    FNET_MDNS_RR_NSEC  = 0x002F,    /* NSEC record */
    FNET_MDNS_RR_TXT   = 0x0010,    /* TXT record */
    FNET_MDNS_RR_ANY   = 0x00FF,    /* ANY record */
    FNET_MDNS_RR_SRV   = 0x0021,    /* SRV record */
    FNET_MDNS_RR_OPT   = 0x0029,    /* OPT record */
    FNET_MDNS_RR_A     = 0x0001,    /* IPv4 record */
    FNET_MDNS_RR_AAAA  = 0x001C,    /* IPv6 record */
} fnet_mdns_rr_type_t;

/* Query RR record types (bitmap). */
typedef enum
{
    FNET_MDNS_QUERY_NONE  = 0,          /* No record */
    FNET_MDNS_QUERY_PTR   = 1 << 0,     /* PTR record */
    FNET_MDNS_QUERY_TXT   = 1 << 1,     /* TXT record */
    FNET_MDNS_QUERY_SRV   = 1 << 2,     /* SRV record */
    FNET_MDNS_QUERY_A     = 1 << 3,     /* IPv4 record */
    FNET_MDNS_QUERY_AAAA  = 1 << 4,     /* IPv6 record */
    FNET_MDNS_QUERY_PTR_ENUM  = 1 << 5, /* Service Type Enumeration PTR record */
    FNET_MDNS_QUERY_ANY   = (FNET_MDNS_QUERY_PTR | FNET_MDNS_QUERY_TXT | FNET_MDNS_QUERY_SRV | FNET_MDNS_QUERY_A | FNET_MDNS_QUERY_AAAA)     /* ANY record  */
} fnet_mdns_query_type_t;

/* mDNS-server states. */
typedef enum
{
    FNET_MDNS_STATE_DISABLED = 0,      /**< @brief mDNS server is not initialized or released. */
    FNET_MDNS_STATE_PROBING_WAIT,      /**< @brief mDNS server is waiting five seconds after any failed probe attempt before trying again. */
    /*RFC6762: Whenever a Multicast DNS responder starts up, wakes up from sleep,
    receives an indication of a network interface "Link Change" event, or
    has any other reason to believe that its network connectivity may
    have changed in some relevant way, it MUST perform the two startup
    steps below: Probing (Section 8.1) and Announcing (Section 8.3).*/
    FNET_MDNS_STATE_PROBING,           /**< @brief mDNS server is sending probe queries. */
    FNET_MDNS_STATE_ANNOUNCING,        /**< @brief mDNS server is sending aannouncements. */
    FNET_MDNS_STATE_WAITING_REQUEST    /**< @brief mDNS server is waiting for a request from a mDNS client. */
} fnet_mdns_state_t;

typedef struct fnet_mdns_service_if_s
{
    const char                  *service_type;                      /* Service Type. Null-terminated string. Example _http._tcp*/
    fnet_uint16_t               service_port;                       /* Service Port number (in network byte order). */
    const fnet_mdns_txt_key_t   *(*service_get_txt)(void);          /* Call-back function, which returns pointer to the service TXT record (null-terminated).
                                                                    * If the service does not provide any TXT record, this parameter must be set to NULL. */
    fnet_uint16_t               offset_service_name;                /* Pointer to service name. Offset from the start of the DNS message. Used for Domain Name Compression. */
    fnet_uint16_t               offset_service_type;                /* Pointer to service type. Offset from the start of the DNS message. Used for Domain Name Compression. */
    fnet_mdns_query_type_t      response_type;                      /* Response type used in mDNS response.*/
} fnet_mdns_service_if_t;

/* MDNS interface structure */
typedef struct fnet_mdns_if
{
    fnet_mdns_state_t           state;                                          /* Current state */
    fnet_bool_t                 is_enabled;
    fnet_service_desc_t         service_descriptor;                             /* Service descriptor. */
    fnet_netif_desc_t           netif;                                          /* Network interface descriptor. */
    fnet_uint32_t               rr_ttl;                                         /* Resource record TTL */
    fnet_address_family_t       addr_family;                                    /* Address family (IPv6 or IPv4 or both) the server will listen and send */
    fnet_size_t                 name_length;                                    /* Length of the service name without index postfix */
    char                        host_name[FNET_MDNS_HOST_NAME_LEN_MAX + 1];     /* Parsed "name" containing only legal symbols, optionally appended with host_name_count */
    fnet_uint32_t               host_name_count;                                /* Count of try device name */
    char                        service_name[FNET_MDNS_HOST_NAME_LEN_MAX + 1];  /* Service name, optionally appended with service_name_count */
    fnet_uint32_t               service_name_count;                             /* Count of try service name */
    fnet_uint32_t               probe_count;                                    /* Number of sent probe queries without name conflict.*/
    fnet_uint32_t               announce_count;                                 /* Count of sent annoncemnts */
    fnet_socket_t               socket_listen;                                  /* Listening socket.*/
    fnet_uint32_t               probe_wait_timestamp;                           /* Last Conflict timestamp. */
    fnet_uint32_t               probe_wait_interval;                            /* Lenghth of time before next probe attempt. */
    fnet_time_t                 send_timestamp;                                 /* Last Send timestamp.*/
    fnet_uint8_t                buffer[FNET_MDNS_PACKET_SIZE];                  /* TX/RX Data buffer. */
    fnet_uint16_t               offset_host_name;                               /* Pointer to host name. Offset from the start of the DNS message. Used for Domain Name Compression. */
    fnet_bool_t                 is_truncated;                                   /* RFC6762:In query messages, if the TC bit is set, it means that additional Known-Answer records may be following shortly.  A responder SHOULD
                                                                                * record this fact, and wait for those additional Known-Answer records, before deciding whether to respond.*/
    fnet_bool_t                 is_legacy_unicast;                              /* If the source UDP port in a received Multicast DNS query is not port 5353*/
    fnet_bool_t                 is_shared;                                      /* Response contains only shared records.
                                                                                * RFC 6762: In any case where there may be multiple responses, such as queries
                                                                                * where the answer is a member of a shared resource record set, each
                                                                                * responder SHOULD delay its response by a random amount of time
                                                                                * selected with uniform random distribution in the range 20-120 ms.  */
    fnet_time_t                 is_shared_timestamp;                            /* Timestamp of shared response query.*/
    fnet_time_t                 is_shared_response_delay;                       /* Random time to wait between HK_IP_MDNS_SHARED_RESPONSE_DELAY_MIN and HK_IP_MDNS_SHARED_RESPONSE_DELAY_MAX */
    struct fnet_sockaddr        remote_address;                                 /* Remote address.*/
    fnet_address_family_t       response_address_family;                        /* Address family used in mDNS response.*/
    fnet_mdns_query_type_t      response_type;                                  /* Response type used in mDNS response.*/
    fnet_mdns_service_if_t      service_if_list[FNET_CFG_MDNS_SERVICE_MAX];     /* Service Discovery List */
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    fnet_mdns_service_if_t      service_if_enum;                                /* Used for service type enumeration */
#endif
} fnet_mdns_if_t;

/* MDNS packet header - count of all records */
FNET_COMP_PACKED_BEGIN
typedef struct fnet_mdns_header_s
{
    fnet_uint16_t    ip  FNET_COMP_PACKED;
    fnet_uint16_t    flags   FNET_COMP_PACKED;          /* flags */
    fnet_uint16_t    qdcount FNET_COMP_PACKED;          /* count of questions */
    fnet_uint16_t    ancount FNET_COMP_PACKED;          /* count of answers */
    fnet_uint16_t    nscount FNET_COMP_PACKED;          /* Authority Record Count. Number of RR in the Authority section. (“NS” - “name server”). */
    fnet_uint16_t    arcount FNET_COMP_PACKED;          /* count of additional */
} fnet_mdns_header_t;
FNET_COMP_PACKED_END

/* MDNS record header */
FNET_COMP_PACKED_BEGIN
typedef struct fnet_mdns_rr_header_s
{
    fnet_uint16_t    type    FNET_COMP_PACKED;          /* RR type */
    fnet_uint16_t    rr_class    FNET_COMP_PACKED;      /* RR class */
    fnet_uint32_t    ttl FNET_COMP_PACKED;              /* time to live (in seconds) */
    fnet_uint16_t    data_length FNET_COMP_PACKED;      /* length of data */
} fnet_mdns_rr_header_t;
FNET_COMP_PACKED_END

/* MDNS query Question Entry header */
FNET_COMP_PACKED_BEGIN
typedef struct fnet_mdns_qe_header_s
{
    fnet_uint16_t    type    FNET_COMP_PACKED;           /* Question  type */
    fnet_uint16_t    rr_class    FNET_COMP_PACKED;       /* Question  class */
} fnet_mdns_qe_header_t;
FNET_COMP_PACKED_END

/* MDNS data RR SRV */
FNET_COMP_PACKED_BEGIN
typedef struct fnet_mdns_rr_data_srv_s
{
    fnet_uint16_t priority   FNET_COMP_PACKED;
    fnet_uint16_t weight FNET_COMP_PACKED;
    fnet_uint16_t port   FNET_COMP_PACKED;
} fnet_mdns_rr_data_srv_t;
FNET_COMP_PACKED_END;

/************************************************************************
*     Function Prototypes
*************************************************************************/
//static void _fnet_mdns_poll( void *fnet_mdns_if_p );
static fnet_uint8_t *_fnet_mdns_add_domain_name(fnet_uint8_t *buf, fnet_uint32_t buf_size, const char *domain_name);
static void _fnet_mdns_send_response(fnet_mdns_if_t *mdns_if, fnet_uint32_t ttl);
static fnet_uint8_t *_fnet_mdns_add_rr_header(fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_rr_type_t type, fnet_bool_t flush, fnet_uint32_t ttl, fnet_uint16_t data_length);
static void _fnet_mdns_send(fnet_mdns_if_t *mdns_if, fnet_address_family_t address_family, fnet_uint8_t *buffer, fnet_uint32_t send_size);
static void fnet_mdns_update_name(fnet_mdns_if_t *mdns_if, const fnet_char_t *name);
static void _fnet_mdns_update_host_name_counter(fnet_mdns_if_t *mdns_if);
static void _fnet_mdns_update_service_name_counter(fnet_mdns_if_t *mdns_if);
static void _fnet_mdns_change_state(fnet_mdns_if_t *mdns_if, fnet_mdns_state_t state);
static void _fnet_mdns_recv(fnet_mdns_if_t *mdns_if);
static void _fnet_mdns_send_probe(fnet_mdns_if_t *mdns_if);
static void _fnet_mdns_process_simultaneous_probe(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *ns_ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size);
fnet_uint8_t *_fnet_mdns_add_rr_txt(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_rr_ptr(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t compression);
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    static fnet_uint8_t *_fnet_mdns_add_rr_ptr_enum(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t compression);
#endif
static fnet_uint8_t *_fnet_mdns_add_rr_srv(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_rr_a(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_rr_aaaa(fnet_mdns_if_t *mdns_if,  fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_qe_any(fnet_uint8_t *buf, fnet_uint32_t buf_size);
static fnet_uint8_t *_fnet_mdns_add_host_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_service_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_service_type_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_bool_t compression);
static fnet_uint8_t *_fnet_mdns_add_domain_name_compression(fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint16_t name_offset);
static const fnet_uint8_t *_fnet_mdns_get_rr_name(char *rr_name, fnet_uint32_t rr_name_size, const fnet_uint8_t *rr, const fnet_uint8_t *packet);
static fnet_bool_t _fnet_mdns_cmp_rr_name(const char *rr_name, const char *name_1, const char *name_2);
static fnet_bool_t _fnet_mdns_is_rr_win(fnet_uint8_t *our_rr, const fnet_uint8_t *ns_ptr, fnet_uint16_t ns_count, fnet_uint8_t *packet);
static const fnet_uint8_t *_fnet_mdns_get_ns(fnet_uint8_t *packet, fnet_uint32_t packet_size);
static const fnet_uint8_t *_fnet_mdns_get_an(const fnet_uint8_t *packet, fnet_uint32_t packet_size);
static fnet_int32_t _fnet_mdns_cmp_rr(fnet_uint8_t *our_rr, const fnet_uint8_t **rr, const fnet_uint8_t *packet);
static fnet_mdns_query_type_t _fnet_mdns_get_query_type(fnet_uint16_t type);
static void _fnet_mdns_process_duplicate_answer(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *an_ptr, const fnet_uint8_t *packet, fnet_uint32_t packet_size);
static fnet_bool_t _fnet_mdns_is_our_host_name(fnet_mdns_if_t *mdns_if, char *host_name);
static fnet_bool_t _fnet_mdns_is_our_service_name(fnet_mdns_if_t *mdns_if, char *service_name);
static fnet_mdns_service_if_t *_fnet_mdns_get_service_by_name(fnet_mdns_if_t *mdns_if, char *service_name);
static fnet_mdns_service_if_t *_fnet_mdns_get_service_by_type(fnet_mdns_if_t *mdns_if, char *service_name);
static void _fnet_mdns_send_announcement(fnet_mdns_if_t *mdns_if, fnet_uint32_t ttl);
static fnet_bool_t _fnet_mdns_cmp_name(const char **rr_name_p, const char *name);
static const fnet_uint8_t *_fnet_mdns_process_query(fnet_mdns_if_t *mdns_if, fnet_address_family_t address_family, const fnet_uint8_t *ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size);
static const fnet_uint8_t *_fnet_mdns_process_response(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size);
static fnet_int32_t _fnet_mdns_cmp_rr(fnet_uint8_t *our_rr, const fnet_uint8_t **rr, const fnet_uint8_t *packet);
/******************************************************************************
 * @brief    Adds TXT record key.
 * @param buf      Buffer where the TXT record key to be added.
 * @param buf_size Buffer size.
 * @param txt_key  TXT record key name (Null-terminated).
 * @param txt_key_value  TXT record key value (Null-terminated).
 * @return This function returns:
 *          - @ref Pointer to the next byte after the added TXT record key.
 *          - @ref FNET_NULL if adding key is failed cause of small buffer size or any other error.
 ******************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_txt_key(fnet_uint8_t *buf, fnet_uint32_t buf_size, const fnet_char_t *txt_key, const fnet_char_t *txt_key_value);

/* RFC 6762:  The destination UDP port in all Multicast DNS responses MUST be 5353,
   and the destination address MUST be the mDNS IPv4 link-local
   multicast address 224.0.0.251 or its IPv6 equivalent FF02::FB, except
   when generating a reply to a query that explicitly requested a
   unicast response*/
#define FNET_MDNS_IP4_MULTICAST_ADDR   FNET_IP4_ADDR_INIT(224u, 0u, 0u, 251u)
//static const fnet_ip6_addr_t fnet_mdns_ip6_multicast_addr = FNET_IP6_ADDR_INIT(0xFF, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFB);

/* The MDNS interface list*/
static  fnet_mdns_if_t fnet_mdns_if_list[FNET_CFG_MDNS];

/************************************************************************
* DESCRIPTION: Initializes mDNS server/responder.
************************************************************************/
fnet_mdns_desc_t fnet_mdns_init( struct fnet_mdns_params *params )
{
    struct fnet_mdns_if     *mdns_if = FNET_NULL;
    fnet_index_t            i;
    struct fnet_sockaddr    local_addr;
    fnet_uint32_t           option = FNET_MDNS_IP_TTL;
    fnet_size_t             name_length;
//    fnet_scope_id_t         scope_id;

    fnet_service_mutex_lock();

    /* Check input paramters. */
    if((params == 0) || (params->netif_desc == 0) || (params->name == 0)
       || ((name_length = fnet_strlen(params->name)) == 0u) || (name_length >= FNET_DNS_MAME_SIZE))
    {
        FNET_DEBUG_MDNS(FNET_MDNS_ERR_PARAMS);
        goto ERROR_1;
    }

//    scope_id = fnet_netif_get_scope_id(params->netif_desc);
//    if(scope_id == 0)
//    {
//        FNET_DEBUG_MDNS(FNET_MDNS_ERR_PARAMS);
//        goto ERROR_1;
//    }

    /* Try to find free mDNS server. */
    for(i = 0u; i < FNET_CFG_MDNS; i++)
    {
        if(fnet_mdns_if_list[i].is_enabled == FNET_FALSE)
        {
            mdns_if = &fnet_mdns_if_list[i];
        }
        else
        {
            if(fnet_mdns_if_list[i].netif == params->netif_desc)
            {
                /* It is not possible to run several mDNS servers on the same networking interface.*/
                mdns_if = 0;
                break;
            }
        }
    }

    /* No free mDNS server descriptor. */
    if(mdns_if == FNET_NULL)
    {
        /* No free mDNS descriptor. */
        FNET_DEBUG_MDNS(FNET_MDNS_ERR_IS_INITIALIZED);
        goto ERROR_1;
    }

    /* Reset interface structure. */
    fnet_memset_zero(mdns_if, sizeof(struct fnet_mdns_if));

    /* Set parameters.*/
    mdns_if->netif = params->netif_desc;
    if(params->rr_ttl == 0u)
    {
        mdns_if->rr_ttl = FNET_CFG_MDNS_RR_TTL;
    }
    else
    {
        mdns_if->rr_ttl = params->rr_ttl;
    }

#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    mdns_if->service_if_enum.service_type = FNET_MDNS_SERVICE_TYPE_ENUMERATION_NAME;                                /* Used for service type enumeration */
#endif

    /* Generate host_name.*/
    fnet_mdns_update_name(mdns_if, params->name);

    /* Init local socket address.*/
    fnet_memset_zero(&local_addr, sizeof(local_addr));
    if(params->addr_family == AF_UNSPEC)
    {
        local_addr.sa_family = AF_SUPPORTED;
    }
    else
    {
        local_addr.sa_family = params->addr_family;
    }
    local_addr.sa_port = FNET_CFG_MDNS_PORT;
//    local_addr.sa_scope_id = scope_id;

    mdns_if->addr_family = local_addr.sa_family;

    /* Create listen socket */
    mdns_if->socket_listen = fnet_socket(local_addr.sa_family, SOCK_DGRAM, 0u);
    if(mdns_if->socket_listen < 0)
    {
        FNET_DEBUG_MDNS(FNET_MDNS_ERR_SOCKET_CREATION);
        goto ERROR_1;
    }

    /* Bind socket. */
    if(fnet_socket_bind(mdns_if->socket_listen, &local_addr, sizeof(local_addr)) == FNET_ERR)
    {
        FNET_DEBUG_MDNS(FNET_MDNS_ERR_SOCKET_BIND);
        goto ERROR_2;
    }

    /* Join Multicast Group.*/
#if FNET_CFG_IP4
    if((local_addr.sa_family & AF_INET) != 0u)
    {
        struct fnet_ip_mreq mreq; /* Multicast group information.*/

#if defined(FNET_PORT_LWIP)
        mreq.imr_multiaddr.s_addr = FNET_MDNS_IP4_MULTICAST_ADDR;
//        mreq.imr_interface = scope_id;
        mreq.imr_interface.s_addr = fnet_netif_get_ip4_addr(mdns_if->netif);
#elif defined(FNET_PORT_WIFI_QCA)
        mreq.imr_multiaddr = FNET_HTONL(FNET_MDNS_IP4_MULTICAST_ADDR);
        mreq.imr_interface = fnet_netif_get_ip4_addr(mdns_if->netif);
#endif

        /* Join multicast group. */
        if(fnet_socket_setopt(mdns_if->socket_listen, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == FNET_ERR)
        {
            FNET_DEBUG_MDNS(FNET_MDNS_ERR_JOIN_MULTICAST);
            goto ERROR_2;
        }
        /* Set IPv4 TTL. */
        fnet_socket_setopt(mdns_if->socket_listen, IPPROTO_IP, IP_MULTICAST_TTL, &option, sizeof(option));
    }
#endif
#if FNET_CFG_IP6
    if((local_addr.sa_family & AF_INET6) != 0u)
    {
        struct fnet_ipv6_mreq mreq6; /* Multicast group information.*/

        FNET_IP6_ADDR_COPY(&fnet_mdns_ip6_multicast_addr, &mreq6.ipv6imr_multiaddr.s6_addr);
        mreq6.ipv6imr_interface = scope_id;

        /* Join multicast group. */
        if(fnet_socket_setopt(mdns_if->socket_listen, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof(mreq6)) == FNET_ERR)
        {
            FNET_DEBUG_MDNS(FNET_MDNS_ERR_JOIN_MULTICAST);
            goto ERROR_2;
        }

        /* Set IPv6 Hop Limit. */
        fnet_socket_setopt(mdns_if->socket_listen, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &option, sizeof(option));
    }
#endif

//    /* Register service. */
//    mdns_if->service_descriptor = fnet_service_register(_fnet_mdns_poll, (void *) mdns_if);
//    if(mdns_if->service_descriptor == 0)
//    {
//        FNET_DEBUG_MDNS(FNET_MDNS_ERR_SERVICE);
//        goto ERROR_2;
//    }

    /* Start probing.*/
    mdns_if->probe_wait_interval = FNET_MDNS_PROBE_INIT_WAIT;
    _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_PROBING_WAIT);

    mdns_if->is_enabled = FNET_TRUE;

    fnet_service_mutex_unlock();
    return (fnet_mdns_desc_t)mdns_if;

ERROR_2:
    fnet_socket_close(mdns_if->socket_listen);
ERROR_1:
    fnet_service_mutex_unlock();
    return 0;
}

/************************************************************************
* DESCRIPTION: Registers application-specific service in the mDNS server.
************************************************************************/
fnet_mdns_service_desc_t fnet_mdns_service_register(fnet_mdns_desc_t mdns_desc, const fnet_mdns_service_t *service)
{
    fnet_mdns_service_desc_t   result = 0;
    fnet_mdns_if_t             *mdns_if = (fnet_mdns_if_t *)mdns_desc;
    fnet_index_t               i;

    if(mdns_if)
    {
        fnet_service_mutex_lock();
        if(service && service->service_type)
        {
            for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
            {
                /* Find empty service entry.*/
                if(mdns_if->service_if_list[i].service_type == NULL)
                {
                    mdns_if->service_if_list[i].service_type = service->service_type;
                    mdns_if->service_if_list[i].service_port = service->service_port;
                    mdns_if->service_if_list[i].service_get_txt = service->service_get_txt;

                    result = (fnet_mdns_service_desc_t)(&mdns_if->service_if_list[i]);
                    break;
                }
            }
        }

        /* Probing new service */
        if(result && ((mdns_if->state == FNET_MDNS_STATE_WAITING_REQUEST) || (mdns_if->state == FNET_MDNS_STATE_ANNOUNCING)) )
        {
            /* Start probing.*/
            mdns_if->probe_wait_interval = FNET_MDNS_PROBE_INIT_WAIT;
            _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_PROBING_WAIT);
        }
        fnet_service_mutex_unlock();
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Unregisters application service from the mDNS server.
************************************************************************/
void fnet_mdns_service_unregister(fnet_mdns_service_desc_t service_desc)
{
    fnet_mdns_service_if_t *service_if = (fnet_mdns_service_if_t *)service_desc;

    /* Free service entry.*/
    if(service_if)
    {
        fnet_service_mutex_lock();
        service_if->service_type = NULL;
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Gets mDNS service descriptor by type.
************************************************************************/
fnet_mdns_service_desc_t fnet_mdns_service_get_by_type(fnet_mdns_desc_t mdns_desc, const char  *service_type)
{
    fnet_mdns_service_desc_t   result = FNET_NULL;
    fnet_mdns_if_t             *mdns_if = (fnet_mdns_if_t *)mdns_desc;
    fnet_index_t               i;

    if(mdns_if && service_type)
    {
        fnet_service_mutex_lock();
        for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
        {
            if(mdns_if->service_if_list[i].service_type)
            {
                if (!fnet_strcmp(mdns_if->service_if_list[i].service_type, service_type))
                {
                    result = (fnet_mdns_service_desc_t)&mdns_if->service_if_list[i];
                    break; /* Found.*/
                }
            }
        }
        fnet_service_mutex_unlock();
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Sends unsolicited mDNS announcement.
************************************************************************/
void fnet_mdns_announce(fnet_mdns_desc_t mdns_desc)
{
    fnet_mdns_if_t   *mdns_if = (fnet_mdns_if_t *)mdns_desc;

    if(mdns_if)
    {
        fnet_service_mutex_lock();
        /* RFC6762: At any time, if the rdata of any of a host's Multicast DNS records
        changes, the host MUST repeat the Announcing step described above to
        update neighboring caches.  For example, if any of a host's IP
        addresses change, it MUST re-announce those address records.  The
        host does not need to repeat the Probing step because it has already
        established unique ownership of that name.*/
        if((mdns_if->state == FNET_MDNS_STATE_WAITING_REQUEST)
           || (mdns_if->state == FNET_MDNS_STATE_ANNOUNCING))
        {
            _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_ANNOUNCING);
        }
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Releases the mDNS server/responder.
************************************************************************/
void fnet_mdns_release(fnet_mdns_desc_t mdns_desc)
{
    struct fnet_mdns_if *mdns_if = (struct fnet_mdns_if *)mdns_desc;

    fnet_service_mutex_lock();
    if(mdns_if && (mdns_if->is_enabled == FNET_TRUE))
    {
        /* Send "goodbye" */
        _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_DISABLED );

        fnet_socket_close(mdns_if->socket_listen);

//        fnet_service_unregister(mdns_if->service_descriptor); /* Delete service.*/

        mdns_if->is_enabled = FNET_FALSE;
    }
    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: This function returns FNET_TRUE if the mDNS server
*              is enabled/initialised.
************************************************************************/
fnet_bool_t fnet_mdns_is_enabled(fnet_mdns_desc_t desc)
{
    struct fnet_mdns_if     *mdns_if = (struct fnet_mdns_if *) desc;
    fnet_bool_t             result;

    if(mdns_if)
    {
        result = mdns_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Looks for a mDNS server assigned to the specified network interface.
************************************************************************/
fnet_mdns_desc_t fnet_mdns_get_by_netif(fnet_netif_desc_t netif)
{
    fnet_mdns_if_t          *mdns_if;
    fnet_index_t            i;
    fnet_mdns_desc_t        mdns_desc = 0;

    if(netif)
    {
        fnet_service_mutex_lock();
        for(i = 0u; i < FNET_CFG_MDNS; i++)
        {
            mdns_if = &fnet_mdns_if_list[i];

            if((mdns_if->is_enabled == FNET_TRUE) && (mdns_if->netif == netif))
            {
                mdns_desc = (fnet_mdns_desc_t)mdns_if;
                break;
            }
        }
        fnet_service_mutex_unlock();
    }

    return mdns_desc;
}

/************************************************************************
* DESCRIPTION: Generate host and service name.
************************************************************************/
static void fnet_mdns_update_name(fnet_mdns_if_t *mdns_if, const fnet_char_t *name)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(name != FNET_NULL);

    fnet_char_t *c;
    fnet_size_t length = fnet_strlen(name);

    if(length <= FNET_MDNS_NAME_MAX)
    {
        fnet_service_mutex_lock();
        /* Reset device name counter */
        mdns_if->host_name_count = 0;

        /* Save name length.*/
        mdns_if->name_length = fnet_strlen(name);

        /* Copy name. */
        fnet_strcpy(mdns_if->host_name, name);
        fnet_strcpy(mdns_if->service_name, name);

        /* Allow only legal characters in address record names.*/
        for(c = mdns_if->host_name; *c != '\0'; c++)
        {
            /*7-bit ASCII embedded in an 8 bit byte whose high order bit is always 0.*/
            if(*c <= 127)
            {
                /*RFC6762: For names that are restricted to US-ASCII [RFC0020] letters, digits and and hyphens*/
                if( !(((*c <= 'Z') && (*c >= 'A')) ||
                      ((*c <= 'z') && (*c >= 'a')) ||
                      ((*c <= '9') && (*c >= '0')) ||
                      (*c == '-') ||
                      (*c == '_') ) )
                {
                    /* Replace illegal label symbols by '-'*/
                    *c = '-';
                }
            }
            /* else UTF-8 */
        }
        fnet_service_mutex_unlock();
    }

    FNET_DEBUG_MDNS("MDNS: Host-name set to (%s).", mdns_if->host_name);
}
/************************************************************************
* DESCRIPTION: Update counter of host.
************************************************************************/
static void _fnet_mdns_update_host_name_counter(fnet_mdns_if_t *mdns_if)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    /* Update name counter. */
    mdns_if->host_name_count++;

    /* Change name to device (count) */
    if(mdns_if->host_name_count > 0)
    {
        fnet_snprintf(&mdns_if->host_name[mdns_if->name_length], (sizeof(mdns_if->host_name) - mdns_if->name_length), "-%ld", mdns_if->host_name_count);
    }
    else /* 0 */
    {
        mdns_if->host_name[mdns_if->name_length] = '\0';
    }

    FNET_DEBUG_MDNS("MDNS: Host-name set to (%s).", mdns_if->host_name);
}

/************************************************************************
* DESCRIPTION: Update counter of service name.
************************************************************************/
static void _fnet_mdns_update_service_name_counter(fnet_mdns_if_t *mdns_if)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    /* Update name counter. */
    mdns_if->service_name_count++;

    /* Change name to device (count) */
    if(mdns_if->service_name_count > 0)
    {
        fnet_snprintf(&mdns_if->service_name[mdns_if->name_length], (sizeof(mdns_if->service_name) - mdns_if->name_length), " %ld", mdns_if->service_name_count);
    }
    else /* 0 */
    {
        mdns_if->service_name[mdns_if->name_length] = '\0';
    }

    FNET_DEBUG_MDNS("MDNS: Service-name set to (%s).", mdns_if->service_name);
}

/************************************************************************
* DESCRIPTION: mDNS server poll (state machine).
************************************************************************/
void fnet_mdns_poll( void *fnet_mdns_if_p )
{
    struct fnet_mdns_if     *mdns_if = (struct fnet_mdns_if *)fnet_mdns_if_p;

    if(mdns_if)
    {
        _fnet_mdns_recv(mdns_if);  /* Receive mDNS packets. */

        switch(mdns_if->state)
        {
            case FNET_MDNS_STATE_PROBING_WAIT:
                if( (fnet_timer_get_ms() - mdns_if->probe_wait_timestamp) > mdns_if->probe_wait_interval )
                {
                    _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_PROBING); /* Start probing.*/
                }
                break;
            case FNET_MDNS_STATE_PROBING:
                /* RFC:   250 ms after the first query, the host should send a second; then,
                   250 ms after that, a third. */
                if( (fnet_timer_get_ms() - mdns_if->send_timestamp) > FNET_MDNS_WAIT )
                {
                    /* Any response from this name => try three times */
                    if(mdns_if->probe_count < 3)
                    {
                        _fnet_mdns_send_probe(mdns_if);   /* Send next probe.*/
                    }
                    else
                    {
                        /* RFC6762: If, by 250 ms after the third probe, no
                        conflicting Multicast DNS responses have been received, the host may
                        move to the next step, announcing. */
                        _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_ANNOUNCING);
                    }
                }
                break;
            case FNET_MDNS_STATE_ANNOUNCING:
                if( (fnet_timer_get_ms() - mdns_if->send_timestamp) > FNET_MDNS_ANNOUNCE_INTERVAL )
                {
                    /* RFC6762: The Multicast DNS responder MUST send at least two unsolicited
                    responses, one second apart.*/
                    if(mdns_if->announce_count < FNET_MDNS_ANNOUNCE_COUNT)
                    {
                        mdns_if->announce_count++;
                        _fnet_mdns_send_announcement(mdns_if, mdns_if->rr_ttl);
                    }
                    else
                    {
                        _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_WAITING_REQUEST);
                    }
                }
                break;
            case FNET_MDNS_STATE_WAITING_REQUEST:

                if(mdns_if->is_truncated == FNET_FALSE)
                {
                    /* RFC6762: In any case where there may be multiple responses, such as queries
                    * where the answer is a member of a shared resource record set, each
                    * responder SHOULD delay its response by a random amount of time
                    * selected with uniform random distribution in the range 20-120 ms.
                    * The reason for requiring that the delay be at least 20 ms is to
                    * accommodate the situation where two or more query packets are sent
                    * back-to-back, because in that case we want a responder with answers
                    * to more than one of those queries to have the opportunity to
                    * aggregate all of its answers into a single response message.*/
                    if( (mdns_if->is_shared == FNET_TRUE) &&
                        ((fnet_timer_get_ms() - mdns_if->is_shared_timestamp) > mdns_if->is_shared_response_delay) )
                    {
                        mdns_if->is_shared = FNET_FALSE; /* REST FLAG.*/
                    }

                    if(mdns_if->is_shared == FNET_FALSE)
                    {
                        /* Send response */
                        _fnet_mdns_send_response(mdns_if, mdns_if->rr_ttl);
                    }
                }
                break;
            default:
                break;
        }
    }
}

/************************************************************************
* DESCRIPTION: Change state of the mDNS server.
************************************************************************/
static void _fnet_mdns_change_state(fnet_mdns_if_t *mdns_if, fnet_mdns_state_t state )
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    mdns_if->state = state;
    switch (state)
    {
        case FNET_MDNS_STATE_DISABLED:
            /* RFC6762: In the case where a host knows that certain resource record data is
            * about to become invalid (for example, when the host is undergoing a
            * clean shutdown), the host SHOULD send an unsolicited Multicast DNS */
            /* "goodbye" announcement with RR TTL zero */
            _fnet_mdns_send_announcement(mdns_if, 0);
            break;
        case FNET_MDNS_STATE_PROBING_WAIT:
            /* Save timestamp.*/
            mdns_if->probe_wait_timestamp = fnet_timer_get_ms();
            break;
        case FNET_MDNS_STATE_PROBING:
            mdns_if->probe_count = 0;   /* Reset probe counter.*/
            _fnet_mdns_send_probe(mdns_if);   /* First probing.*/
            break;
        case FNET_MDNS_STATE_ANNOUNCING:
            /* RFC6762: The second startup step is that the Multicast DNS responder MUST send
            * an unsolicited Multicast DNS response containing, in the Answer
            * Section, all of its newly registered resource records (both shared
            * records, and unique records that have completed the probing step). */
            mdns_if->announce_count = 1;
            _fnet_mdns_send_announcement(mdns_if, mdns_if->rr_ttl);
            break;
        case FNET_MDNS_STATE_WAITING_REQUEST:
            break;
        default:
            break;
    }
}

/************************************************************************
* DESCRIPTION: Compare R names.
************************************************************************/
static fnet_bool_t _fnet_mdns_cmp_rr_name(const char *rr_name, const char *name_1, const char *name_2)
{
    FNET_ASSERT(rr_name != FNET_NULL);
    FNET_ASSERT(name_1 != FNET_NULL);
    FNET_ASSERT(name_2 != FNET_NULL);

    fnet_bool_t   result;

    result = _fnet_mdns_cmp_name(&rr_name, name_1);
    if( result == FNET_TRUE)
    {
        result = _fnet_mdns_cmp_name(&rr_name, name_2);
        if( result == FNET_TRUE)
        {
            result = _fnet_mdns_cmp_name(&rr_name, FNET_MDNS_DOMAIN_NAME);
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Compare R name with sub-name.
************************************************************************/
static fnet_bool_t _fnet_mdns_cmp_name(const char **rr_name_p, const char *name)
{
    FNET_ASSERT(rr_name_p != FNET_NULL);
    FNET_ASSERT(name != FNET_NULL);

    fnet_index_t        i = 0;
    const fnet_char_t   *rr_name = *rr_name_p;
    fnet_uint32_t       name_length = fnet_strlen(name);
    fnet_uint32_t       rr_name_length = fnet_strlen(rr_name);

    if(name_length)
    {
        if(name_length > rr_name_length)
        {
            return FNET_FALSE;
        }

        rr_name++; /* Skip length byte */

        for(i = 0; i < name_length; i++)
        {
            if(name[i] == '.')
            {
                continue;
            }

            if(fnet_tolower(rr_name[i]) != fnet_tolower(name[i]))
            {
                return FNET_FALSE;
            }
        }

        *rr_name_p = &rr_name[i];
    }

    return FNET_TRUE;
}

/************************************************************************
* DESCRIPTION: Put RR name to rr_name. Returns updated next pointer; 0 if error.
If rr_name or rr_name_size are 0, just skip name.
************************************************************************/
static const fnet_uint8_t *_fnet_mdns_get_rr_name(char *rr_name, fnet_uint32_t rr_name_size, const fnet_uint8_t *rr, const fnet_uint8_t *packet)
{
    FNET_ASSERT(rr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    const fnet_uint8_t  *ptr = rr;
    fnet_uint8_t        length = *rr;
    fnet_uint16_t       offset = 0;
    const fnet_uint8_t  *result = ptr;
#define FNET_MDNS_COMPRESSION_LIMIT         4  /* Limits number of compression elements, to avoid potential infinite loop in malformed packets*/
    fnet_index_t        compression_counter = 0;

    while(length != 0)
    {
        /* Check if compression is used */
        while((*ptr & FNET_MDNS_DOMAIN_NAME_COMPRESSION) == FNET_MDNS_DOMAIN_NAME_COMPRESSION)
        {
            if(offset == 0) /* Is it first compression?*/
            {
                result += 2; /* 2 bytes (compression+length) */
            }

            offset = (fnet_uint16_t)((*ptr) & ~FNET_MDNS_DOMAIN_NAME_COMPRESSION) << 8;
            offset = offset + *(ptr + 1);
            if(offset < FNET_MDNS_PACKET_SIZE) /* Check maximum offset */
            {
                ptr = packet + offset;
            }
            else
            {
                /* Wrong offset */
                goto ERROR;
            }

            if((++compression_counter) > FNET_MDNS_COMPRESSION_LIMIT)
            {
                goto ERROR;
            }
        }

        if((*ptr & FNET_MDNS_DOMAIN_NAME_COMPRESSION) != 0) /* Top two bits may be used only by the compression flag.*/
        {
            goto ERROR;
        }

        length = *ptr;

        if( rr_name && rr_name_size ) /* Copy to rr_name buffer.*/
        {
            if((length + 1) <= rr_name_size) /* Check maximum rr_name_size */
            {
                fnet_memcpy(rr_name, ptr, length + 1);

                rr_name += (length + 1);
                rr_name_size -= (length + 1);
            }
            else
            {
                goto ERROR;
            }
        }

        ptr += (length + 1);
        if(ptr > result)
        {
            result = ptr;
        }
    }

    return result;
ERROR:
    return NULL;
}

/************************************************************************
* DESCRIPTION: Get query type.
************************************************************************/
static fnet_mdns_query_type_t _fnet_mdns_get_query_type(fnet_uint16_t type)
{
    fnet_mdns_query_type_t query_rr_type;

    switch(type)
    {
        case FNET_HTONS(FNET_MDNS_RR_PTR):
            query_rr_type = FNET_MDNS_QUERY_PTR;
            break;
        case FNET_HTONS(FNET_MDNS_RR_TXT):
            query_rr_type = FNET_MDNS_QUERY_TXT;
            break;
        case FNET_HTONS(FNET_MDNS_RR_SRV):
            query_rr_type = FNET_MDNS_QUERY_SRV;
            break;
        case FNET_HTONS(FNET_MDNS_RR_A):
            query_rr_type = FNET_MDNS_QUERY_A;
            break;
        case FNET_HTONS(FNET_MDNS_RR_AAAA):
            query_rr_type = FNET_MDNS_QUERY_AAAA;
            break;
        case FNET_HTONS(FNET_MDNS_RR_ANY):
            query_rr_type = FNET_MDNS_QUERY_ANY;
            break;
        default:
            query_rr_type = FNET_MDNS_QUERY_NONE;
            break;
    }

    return query_rr_type;
}

/* Print query name */
/************************************************************************
* DESCRIPTION: Print MDNS query name.
************************************************************************/
#if FNET_CFG_DEBUG_MDNS && FNET_CFG_DEBUG
static void _fnet_mdns_print_qe_name(const fnet_char_t *prefix, const fnet_char_t *qe_name)
{
    fnet_size_t     qe_name_legth = strlen(qe_name);
    fnet_uint8_t    name_length_index = 0;
    fnet_index_t    i;

    fnet_print("%s", prefix); /* Print prefix*/

    for(i = 0; i < qe_name_legth; i++)
    {
        if(i == name_length_index)
        {
            fnet_print("."); /* Instead of name length */
            name_length_index += qe_name[i] + 1; /* Next name length byte */
        }
        else
        {
            fnet_print("%c", qe_name[i]);
        }
    }

    //fnet_prinln("");
    fnet_print("\r\n");
}
#endif

#if defined(__ARMCC_VERSION)
#pragma diag_suppress 1293
#endif

/************************************************************************
* DESCRIPTION: Received MDNS query (request) and process it.
************************************************************************/
static const fnet_uint8_t *_fnet_mdns_process_query(fnet_mdns_if_t *mdns_if, fnet_address_family_t address_family, const fnet_uint8_t *ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(ptr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    fnet_char_t            qe_name[FNET_MDNS_RR_NAME_LEN_MAX];
    fnet_mdns_header_t     *mdns_header = (fnet_mdns_header_t *)packet;
    fnet_mdns_qe_header_t  *qe_header;
    fnet_mdns_service_if_t *service = NULL;

    if( packet_size > (sizeof(fnet_mdns_header_t) + sizeof(fnet_mdns_qe_header_t)) )
    {
        /* Get Requested name */
        ptr = _fnet_mdns_get_rr_name(qe_name, sizeof(qe_name), ptr, packet);

        if(ptr)
        {
            qe_header = (fnet_mdns_qe_header_t *)ptr;

            /* Check query record */
            if( (qe_header->rr_class & (~FNET_HTONS(FNET_MDNS_HEADER_CACHE_FLUSH))) == FNET_HTONS(FNET_MDNS_HEADER_CLASS_IN)) /* Support only IN = Internet protocol. Ignore flush flag.*/
            {
                switch(mdns_if->state)
                {
                    case FNET_MDNS_STATE_ANNOUNCING:
                    case FNET_MDNS_STATE_WAITING_REQUEST:
                    {
                        fnet_mdns_query_type_t query_type = _fnet_mdns_get_query_type(qe_header->type);

                        fnet_bool_t is_our_host_name = _fnet_mdns_is_our_host_name(mdns_if, qe_name);
                        if(!is_our_host_name)
                        {
                            service = _fnet_mdns_get_service_by_name(mdns_if, qe_name);     /* service instance name */
                            if(!service)
                            {
                                service = _fnet_mdns_get_service_by_type(mdns_if, qe_name);   /* service type in a domain */
                            }
                        }

                        /* Compare received hostname with my_device.local, my_device._hap._tcp.local or _hap._tcp.local */
                        if( is_our_host_name || service )
                        {
#if FNET_CFG_DEBUG_MDNS && FNET_CFG_DEBUG
                            _fnet_mdns_print_qe_name("MDNS: RX Query for:", qe_name);
#endif

                            if(_fnet_mdns_get_service_by_type(mdns_if, qe_name)) /* Service Type has only PTR record.*/
                            {
                                query_type &= FNET_MDNS_QUERY_PTR;
                                if(query_type) /* Shared response.*/
                                {
                                    if(mdns_if->is_shared == FNET_FALSE)
                                    {
                                        mdns_if->is_shared = FNET_TRUE;
                                        mdns_if->is_shared_timestamp = fnet_timer_get_ms();
                                        /* Calculated shared response delay */
#if 0 /* Original */
                                        mdns_if->is_shared_response_delay = FNET_MDNS_SHARED_RESPONSE_DELAY_MIN + (fnet_rand() % (FNET_MDNS_SHARED_RESPONSE_DELAY_MAX - FNET_MDNS_SHARED_RESPONSE_DELAY_MIN));
#else /* To workaround BCT warning */
                                        mdns_if->is_shared_response_delay += (FNET_MDNS_SHARED_RESPONSE_DELAY_MAX - FNET_MDNS_SHARED_RESPONSE_DELAY_MIN) / 4;
                                        mdns_if->is_shared_response_delay = mdns_if->is_shared_response_delay % FNET_MDNS_SHARED_RESPONSE_DELAY_MAX;
                                        if(mdns_if->is_shared_response_delay < FNET_MDNS_SHARED_RESPONSE_DELAY_MIN)
                                        {
                                            mdns_if->is_shared_response_delay += FNET_MDNS_SHARED_RESPONSE_DELAY_MIN;
                                        }
#endif
                                    }
                                }
                            }
                            /* Send later */
                            mdns_if->response_type |= query_type;
                            if(service)
                            {
                                service->response_type |= query_type;
                            }
                            mdns_if->response_address_family |= address_family;
                        }
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
                        /*  Service Type Enumeration.
                            A DNS query for PTR records with the name "_services._dns-sd._udp.<Domain> */
                        else if((_fnet_mdns_cmp_rr_name(qe_name, "", FNET_MDNS_SERVICE_TYPE_ENUMERATION_NAME) == FNET_TRUE) &&
                                (query_type == FNET_MDNS_QUERY_PTR) )
                        {
                            fnet_index_t    i;
                            /* Return all PTR records.*/
                            for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
                            {
                                mdns_if->service_if_list[i].response_type |= FNET_MDNS_QUERY_PTR_ENUM;
                            }

                            mdns_if->response_address_family |= address_family;
                        }
#endif
                    }
                    break;
                    case FNET_MDNS_STATE_PROBING:
                    case FNET_MDNS_STATE_PROBING_WAIT:
                        /* Compare received hostname with my_device.local or my_device._hap._tcp.local*/
                        if(_fnet_mdns_is_our_host_name(mdns_if, qe_name)
                           || _fnet_mdns_is_our_service_name(mdns_if, qe_name))
                        {
#if FNET_CFG_DEBUG_MDNS && FNET_CFG_DEBUG
                            _fnet_mdns_print_qe_name("MDNS: RX Probe for:", qe_name);
#endif

                            /* Simultaneous Probe Tiebreaking */
                            /* RFC6762: When a host that is probing for a record sees another host issue a
                            * query for the same record, it consults the Authority Section of that
                            * query.  If it finds any resource record(s) there which answers the
                            * query, then it compares the data of that (those) resource record(s)
                            * with its own tentative data. */
                            if(mdns_header->nscount > 0)
                            {
                                const fnet_uint8_t *authority_ptr;

                                authority_ptr = _fnet_mdns_get_ns(packet, packet_size); /* Authority section */
                                _fnet_mdns_process_simultaneous_probe(mdns_if, authority_ptr, packet, packet_size);
                            }
                        }
                        break;
                    default:
                        /* Ignore.*/
                        break;
                }
            }

            ptr += sizeof(fnet_mdns_qe_header_t);
        }
    }

    return ptr;
}

/************************************************************************
* DESCRIPTION: Avoid duplicate answer.
************************************************************************/
static void _fnet_mdns_process_duplicate_answer(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *an_ptr, const fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(an_ptr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    fnet_uint8_t            our_rr[FNET_MDNS_RR_PROBE_LEN_MAX];
    fnet_mdns_header_t      *mdns_header = (fnet_mdns_header_t *)packet;
    fnet_uint16_t           an_count = fnet_htons(mdns_header->ancount);
    const fnet_uint8_t      *ptr = an_ptr;
    fnet_mdns_rr_header_t   *rr_header;
    fnet_uint32_t           i;
    fnet_mdns_query_type_t  rr_type;
    fnet_bool_t             skip;
    fnet_mdns_service_if_t  *service;
    char                    rr_name[FNET_MDNS_RR_NAME_LEN_MAX];

    /* Check all RRs in known answer records.*/
    for(i = 0; ptr && (i < an_count) && (ptr < (packet + packet_size)); i++ )
    {
        service = NULL;

        rr_header = (fnet_mdns_rr_header_t *)_fnet_mdns_get_rr_name(rr_name, sizeof(rr_name), ptr, packet);
        if(rr_header == NULL)
        {
            goto ERROR;
        }

        rr_type = _fnet_mdns_get_query_type(rr_header->type);

        skip = FNET_FALSE;

        /* Generate our record.*/
        switch(rr_type)
        {
            case FNET_MDNS_QUERY_PTR:
                service = _fnet_mdns_get_service_by_type(mdns_if, rr_name);
                if(service)
                {
                    if(_fnet_mdns_add_rr_ptr(mdns_if, our_rr, sizeof(our_rr), service, mdns_if->rr_ttl, FNET_FALSE) == NULL)
                    {
                        goto ERROR;
                    }
                }
                else
                {
                    skip = FNET_TRUE;
                }
                break;
            case FNET_MDNS_QUERY_TXT:
                service = _fnet_mdns_get_service_by_name(mdns_if, rr_name);
                if(service)
                {
                    if(_fnet_mdns_add_rr_txt(mdns_if, our_rr, sizeof(our_rr), service, mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
                    {
                        goto ERROR;
                    }
                }
                else
                {
                    skip = FNET_TRUE;
                }
                break;
            case FNET_MDNS_QUERY_SRV:
                service = _fnet_mdns_get_service_by_name(mdns_if, rr_name);
                if(service)
                {
                    if(_fnet_mdns_add_rr_srv(mdns_if, our_rr,  sizeof(our_rr), service, mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
                    {
                        goto ERROR;
                    }
                }
                else
                {
                    skip = FNET_TRUE;
                }
                break;
            case FNET_MDNS_QUERY_AAAA:
                if(_fnet_mdns_add_rr_aaaa(mdns_if, our_rr,  sizeof(our_rr), mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
                {
                    goto ERROR;
                }
                break;
            case FNET_MDNS_QUERY_A:
                if(_fnet_mdns_add_rr_a(mdns_if, our_rr,  sizeof(our_rr), mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
                {
                    goto ERROR;
                }
                break;
            default:
                skip = FNET_TRUE;
                break;
        }

        if(skip == FNET_TRUE)
        {
            /* Skip RR record.*/
            ptr += sizeof(fnet_mdns_rr_header_t) + fnet_htons(rr_header->data_length);
        }
        else
        {
            /* Check if the same RR in answer section.*/
            if(_fnet_mdns_cmp_rr(our_rr, &ptr, packet) == 0)
            {
                /* Remove duplicate answer */
                if(service)
                {
                    service->response_type = (fnet_mdns_query_type_t)(service->response_type & (~rr_type));
                }
                else
                {
                    mdns_if->response_type = (fnet_mdns_query_type_t)(mdns_if->response_type & (~rr_type));
                }
                FNET_DEBUG_MDNS("MDNS: RX Duplicate Answer for: %s ; rr_type =0x%X", our_rr, rr_type);
            }
        }
    }
ERROR:
    return;
}

/************************************************************************
* DESCRIPTION: Simultaneous Probe Tiebreaking.
************************************************************************/
static void _fnet_mdns_process_simultaneous_probe(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *ns_ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(ns_ptr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    fnet_uint8_t        our_rr[FNET_MDNS_RR_PROBE_LEN_MAX];
    fnet_mdns_header_t  *mdns_header = (fnet_mdns_header_t *)packet;
    fnet_uint16_t       ns_count = fnet_htons(mdns_header->nscount);
    fnet_bool_t         is_win = FNET_FALSE;

    FNET_DEBUG_MDNS("MDNS: RX Simultaneous probe");

    /* Prepare and Compare Our RR A record */
    if(_fnet_mdns_add_rr_a(mdns_if, our_rr,  sizeof(our_rr), mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
    {
        goto ERROR;
    }

    if(_fnet_mdns_is_rr_win(our_rr, ns_ptr, ns_count, packet) == FNET_TRUE)
    {
        /* Prepare and Compare Our RR AAAA record */
        if(_fnet_mdns_add_rr_aaaa(mdns_if, our_rr,  sizeof(our_rr), mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
        {
            goto ERROR;
        }
        if(_fnet_mdns_is_rr_win(our_rr, ns_ptr, ns_count, packet) == FNET_TRUE)
        {
            fnet_uint32_t    i;

            is_win = FNET_TRUE;

            for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
            {
                if(mdns_if->service_if_list[i].service_type != NULL)
                {
                    /* Prepare and Compare Our RR SRV records */
                    if(_fnet_mdns_add_rr_srv(mdns_if, our_rr,  sizeof(our_rr), &mdns_if->service_if_list[i], mdns_if->rr_ttl, FNET_FALSE, FNET_FALSE) == NULL)
                    {
                        goto ERROR;
                    }
                    if(_fnet_mdns_is_rr_win(our_rr, ns_ptr, ns_count, packet) == FNET_TRUE)
                    {
                        is_win = FNET_TRUE;
                    }
                    else
                    {
                        is_win = FNET_FALSE;
                        break;
                    }
                }
            }
        }
    }

    if(is_win == FNET_FALSE)
    {
        /* RFC6762: If the host finds that its
        own data is lexicographically earlier, then it defers to the winning
        host by waiting one second, and then begins probing for this record again.*/
        mdns_if->probe_wait_interval = FNET_MDNS_PROBE_DEFER_WAIT;
        /* Save conflict timestamp.*/
        _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_PROBING_WAIT); /* Reset probing.*/
    }
ERROR:
    return;
}

/************************************************************************
* DESCRIPTION: Get pointer to Answer Record.
************************************************************************/
static const fnet_uint8_t *_fnet_mdns_get_an(const fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(packet != FNET_NULL);

    fnet_mdns_header_t      *mdns_header = (fnet_mdns_header_t *)packet;
    fnet_uint16_t           qd_count; /* Question Count */
    fnet_uint16_t           an_count; /* Answer Record Count */
    const fnet_uint8_t      *result = NULL;
    fnet_index_t            i;
    const fnet_uint8_t      *ptr;

    if(packet_size > sizeof(fnet_mdns_header_t))
    {
        qd_count = fnet_htons(mdns_header->qdcount);  /* Question Count */
        an_count = fnet_htons(mdns_header->ancount);  /* Answer Record Count */

        /* Skip mDNS header.*/
        ptr = packet + sizeof(fnet_mdns_header_t);

        /* Skip Questions.*/
        for(i = 0; (i < qd_count) && (ptr < (packet + packet_size)); i++)
        {
            /* Get Requested name */
            ptr = _fnet_mdns_get_rr_name(NULL, 0, ptr, packet);

            if(ptr)
            {
                ptr += sizeof(fnet_mdns_qe_header_t);
            }
            else
            {
                goto ERROR;
            }
        }

        if(an_count > 0)
        {
            result = ptr; /*Pointer to Answer Record*/
        }
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION: Get pointer to Name Server (Authority Record).
************************************************************************/
static const fnet_uint8_t *_fnet_mdns_get_ns(fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(packet != FNET_NULL);

    fnet_mdns_header_t      *mdns_header = (fnet_mdns_header_t *)packet;
    fnet_uint16_t           qd_count; /* Question Count */
    fnet_uint16_t           an_count; /* Answer Record Count */
    fnet_uint16_t           ns_count; /* Authority Record Count */
    const fnet_uint8_t      *result = NULL;
    fnet_index_t            i;
    const fnet_uint8_t      *ptr;

    if(packet_size > sizeof(fnet_mdns_header_t))
    {
        qd_count = fnet_htons(mdns_header->qdcount);  /* Question Count */
        an_count = fnet_htons(mdns_header->ancount);  /* Answer Record Count */
        ns_count = fnet_htons(mdns_header->nscount);  /* Authority Record Count */

        /* Skip mDNS header.*/
        ptr = packet + sizeof(fnet_mdns_header_t);

        /* Skip Questions.*/
        for(i = 0; (i < qd_count) && (ptr < (packet + packet_size)); i++)
        {
            /* Get Requested name */
            ptr = _fnet_mdns_get_rr_name(NULL, 0, ptr, packet);

            if(ptr)
            {
                ptr += sizeof(fnet_mdns_qe_header_t);
            }
            else
            {
                goto ERROR;
            }
        }

        /* Skip Answers.*/
        for(i = 0; (i < an_count) && (ptr < (packet + packet_size)); i++)
        {
            fnet_mdns_rr_header_t *rr_header;

            /* Get RR name */
            ptr = _fnet_mdns_get_rr_name(NULL, 0, ptr, packet);

            if(ptr)
            {
                rr_header = (fnet_mdns_rr_header_t *)ptr;
                ptr += sizeof(fnet_mdns_rr_header_t) + fnet_htons(rr_header->data_length);
            }
            else
            {
                goto ERROR;
            }
        }

        if(ns_count > 0)
        {
            result = ptr; /*Pointer to Authority Record*/
        }
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION: Compare RRs.
************************************************************************/
static fnet_int32_t _fnet_mdns_cmp_rr(fnet_uint8_t *our_rr, const fnet_uint8_t **rr, const fnet_uint8_t *packet)
{
    FNET_ASSERT(our_rr != FNET_NULL);
    FNET_ASSERT(rr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    fnet_uint16_t           rr_type;        /* RR type */
    fnet_uint16_t           rr_class;       /* RR class */
    const fnet_uint8_t      *rr_data;
    fnet_uint16_t           rr_data_length;
    fnet_mdns_rr_header_t   *rr_header;
    const fnet_uint8_t      *ptr = *rr;
    fnet_char_t             rr_name[FNET_MDNS_RR_NAME_LEN_MAX] = {0};
    fnet_uint16_t           our_rr_type;        /* RR type */
    fnet_uint16_t           our_rr_class;       /* RR class */
    const fnet_uint8_t      *our_rr_data;
    fnet_uint16_t           our_rr_data_length;
    fnet_mdns_rr_header_t   *our_rr_header;
    fnet_int32_t            result;

    our_rr_header = (fnet_mdns_rr_header_t *)(our_rr + fnet_strlen((char *)our_rr) + 1);
    our_rr_class = fnet_htons(our_rr_header->rr_class);
    our_rr_type = fnet_htons(our_rr_header->type);
    our_rr_data = ((fnet_uint8_t *)our_rr_header) + sizeof(fnet_mdns_rr_header_t);
    our_rr_data_length = fnet_htons(our_rr_header->data_length);

    /* RFC6762: When a host that is probing for a record sees another host issue a
    query for the same record, it consults the Authority Section of that
    query.  If it finds any resource record(s) there which answers the
    query, then it compares the data of that (those) resource record(s)
    with its own tentative data.  We consider first the simple case of a
    host probing for a single record, receiving a simultaneous probe from
    another host also probing for a single record.  The two records are
    compared and the lexicographically later data wins.*/

    /* Get RR name */
    ptr = _fnet_mdns_get_rr_name(rr_name, sizeof(rr_name), ptr, packet);
    if(ptr)
    {
        rr_header = (fnet_mdns_rr_header_t *)ptr;
        ptr += sizeof(fnet_mdns_rr_header_t);

        rr_data_length = fnet_htons(rr_header->data_length);

        /* Compare RR names.*/
        if(fnet_strcmp((char *)our_rr, (char *)rr_name) == 0)
        {
            rr_data = ptr;
            rr_class = fnet_htons(rr_header->rr_class) & (~FNET_HTONS(FNET_MDNS_HEADER_CACHE_FLUSH));
            rr_type = fnet_htons(rr_header->type);

            /* RFC6762: The determination of "lexicographically later" is performed by first
            comparing the record class (excluding the cache-flush bit described
            in Section 10.2), then the record type, then raw comparison of the
            binary content of the rdata without regard for meaning or structure.
            If the record classes differ, then the numerically greater class is
            considered "lexicographically later".*/
            if(rr_class < our_rr_class)
            {
                result = 1; /* Win. */
            }
            else if(rr_class > our_rr_class)
            {
                result = -1; /* Loss. */
            }
            else
            {
                /* RFC6762: Otherwise, if the record types
                differ, then the numerically greater type is considered
                "lexicographically later".*/
                if(rr_type < our_rr_type)
                {
                    result = 1; /* Win. */
                }
                else if(rr_type > our_rr_type)
                {
                    result = -1; /* Loss. */
                }
                /* RFC6762: If the rrtype and rrclass both match,
                then the rdata is compared.*/
                else if(rr_data_length)
                {
                    int             cmp_res;
                    fnet_uint16_t        cmp_length;

                    cmp_length = (rr_data_length > our_rr_data_length) ? our_rr_data_length : rr_data_length; /* Get min length value */

                    cmp_res = fnet_memcmp(rr_data, our_rr_data, cmp_length);

                    if(cmp_res < 0)
                    {
                        result = 1; /* Win. */
                    }
                    else if (cmp_res > 0)
                    {
                        result = -1; /* Loss. */
                    }
                    else /* cmp_res == 0 */
                    {
                        /* RFC:If both lists run out of records at the same time without any difference
                        being found, then this indicates that two devices are advertising
                        identical sets of records, as is sometimes done for fault tolerance,
                        and there is, in fact, no conflict.*/
                        if(rr_data_length < our_rr_data_length)
                        {
                            result = 1; /* Win. */
                        }
                        else if(rr_data_length > our_rr_data_length)
                        {
                            result = -1; /* Loss. */
                        }
                        else
                        {
                            /* Check TTL. Used for Duplicate Suppression */
                            if(fnet_htonl(rr_header->ttl) > (fnet_htonl(our_rr_header->ttl) >> 1))
                            {
                                result = 0; /* Absoulutely same records */
                            }
                            else
                            {
                                result = 1;
                            }
                        }
                    }
                }
                else
                {
                    result = 1; /* Win. */
                }
            }
        }
        else
        {
            result = 1; /* Win. */
        }

        ptr += rr_data_length;
    }
    else
    {
        /* Malformed packet.*/
        result = 1; /* Win. */
    }

    *rr = ptr;

    return result;
}

/************************************************************************
* DESCRIPTION: Is our RR winner.
************************************************************************/
static fnet_bool_t _fnet_mdns_is_rr_win(fnet_uint8_t *our_rr, const fnet_uint8_t *ns_ptr, fnet_uint16_t ns_count, fnet_uint8_t *packet)
{
    FNET_ASSERT(our_rr != FNET_NULL);
    FNET_ASSERT(ns_ptr != FNET_NULL);

    fnet_index_t              i;
    const fnet_uint8_t        *ptr = ns_ptr;
    fnet_bool_t               result;

    /* RFC6762: When a host that is probing for a record sees another host issue a
    query for the same record, it consults the Authority Section of that
    query.  If it finds any resource record(s) there which answers the
    query, then it compares the data of that (those) resource record(s)
    with its own tentative data.  We consider first the simple case of a
    host probing for a single record, receiving a simultaneous probe from
    another host also probing for a single record.  The two records are
    compared and the lexicographically later data wins.*/

    /* Check all RRs. Find lowest class */
    for(i = 0; i < ns_count; i++ )
    {
        if(_fnet_mdns_cmp_rr(our_rr, &ptr, packet) >= 0)
        {
            break; /* Win */
        }
    }

    if(i == ns_count)
    {
        result = FNET_FALSE;
    }
    else
    {
        result = FNET_TRUE;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Process MDNS response
************************************************************************/
static const fnet_uint8_t *_fnet_mdns_process_response(fnet_mdns_if_t *mdns_if, const fnet_uint8_t *ptr, fnet_uint8_t *packet, fnet_uint32_t packet_size)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(ptr != FNET_NULL);
    FNET_ASSERT(packet != FNET_NULL);

    fnet_char_t    rr_name[FNET_MDNS_RR_NAME_LEN_MAX];

    /* Header follows the hostname and the terminating zero */
    fnet_mdns_rr_header_t *rr_header;

    if( packet_size > (sizeof(fnet_mdns_header_t) + sizeof(fnet_mdns_rr_header_t)) ) /* TND make it universal*/
    {
        /* Get RR name */
        ptr = _fnet_mdns_get_rr_name(rr_name, sizeof(rr_name), ptr, packet);

        if(ptr)
        {
            rr_header = (fnet_mdns_rr_header_t *)ptr;
            ptr += sizeof(fnet_mdns_rr_header_t) + fnet_htons(rr_header->data_length);

            /* Compare received name with our names */
            fnet_bool_t is_our_host_name = _fnet_mdns_is_our_host_name(mdns_if, rr_name);
            fnet_bool_t is_our_service_name = _fnet_mdns_is_our_service_name(mdns_if, rr_name);

            if( (is_our_host_name || is_our_service_name)
#if 0 /* Cause of Test warnings.*/
                && ((rr_header->type == FNET_HTONS(FNET_MDNS_RR_SRV)) ||
                    (rr_header->type == FNET_HTONS(FNET_MDNS_RR_A)) ||
                    (rr_header->type == FNET_HTONS(FNET_MDNS_RR_AAAA)))
#endif
              )
            {
#if FNET_CFG_DEBUG_MDNS && FNET_CFG_DEBUG
                _fnet_mdns_print_qe_name("MDNS: RX response for:", rr_name);
#endif

                /* RFC: In the case of a host
                probing using query type "ANY" as recommended above, any answer
                containing a record with that name, of any type, MUST be considered a
                conflicting response and handled accordingly.*/

                /* If in Probing state, change name and try again */
                if((mdns_if->state == FNET_MDNS_STATE_PROBING)
                   || (mdns_if->state == FNET_MDNS_STATE_PROBING_WAIT))
                {
                    /* Regenerate names.*/
                    if(is_our_host_name)
                    {
                        _fnet_mdns_update_host_name_counter(mdns_if);
                    }
                    if(is_our_service_name)
                    {
                        _fnet_mdns_update_service_name_counter(mdns_if);
                    }
                }

                /* RFC: If fifteen conflicts occur within any ten-second period, then the
                   host MUST wait at least five seconds before each successive
                   additional probe attempt.  This is to help ensure that, in the event
                   of software bugs or other unanticipated problems, errant hosts do not
                   flood the network with a continuous stream of multicast traffic.  For
                   very simple devices, a valid way to comply with this requirement is
                   to always wait five seconds after any failed probe attempt before
                   trying again.*/
                /* We use the simple device aproach.*/
                mdns_if->probe_wait_interval = FNET_MDNS_PROBE_WAIT;
                _fnet_mdns_change_state(mdns_if, FNET_MDNS_STATE_PROBING_WAIT); /* Reset probing.*/
            }
        }
    }
    else
    {
        ptr = NULL;
    }

    return ptr;
}

/************************************************************************
* DESCRIPTION: MDNS receive.
************************************************************************/
static void _fnet_mdns_recv(fnet_mdns_if_t *mdns_if)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_ssize_t            received;
    fnet_mdns_header_t      *mdns_header;
    fnet_uint32_t           cnt = 0;
    const fnet_uint8_t      *ptr;
    fnet_index_t            i;
    fnet_size_t             addr_len;

    if(mdns_if->state != FNET_MDNS_STATE_DISABLED)
    {
        /* Receive UDP data */
        addr_len = sizeof(mdns_if->remote_address);
        received = fnet_socket_recvfrom( mdns_if->socket_listen, mdns_if->buffer, sizeof(mdns_if->buffer), 0u, &mdns_if->remote_address, &addr_len );
        if(received > 0)
        {
            /* Received MDNS header */
            if(received > sizeof(fnet_mdns_header_t))
            {
                mdns_header = (fnet_mdns_header_t *)&mdns_if->buffer[0];

                char *hostname = (char *)&mdns_if->buffer[sizeof(fnet_mdns_header_t)];

                /* Query */
                if( ((mdns_header->flags & FNET_HTONS(FNET_MDNS_HEADER_FLAGS_QR)) == 0)       /* Query.*/
                    && ((mdns_header->flags & FNET_HTONS(FNET_MDNS_HEADER_FLAGS_OPCODE)) == 0) ) /* Standard Query */
                {
                    if(mdns_if->remote_address.sa_port != FNET_MDNS_PORT)
                    {
                        mdns_if->is_legacy_unicast = FNET_TRUE;
                    }
                    else
                    {
                        mdns_if->is_legacy_unicast = FNET_FALSE;
                    }

                    if( (mdns_header->flags & FNET_HTONS(FNET_MDNS_HEADER_FLAGS_TC)) == 0)       /* Trancation.*/
                    {
                        mdns_if->is_truncated = FNET_FALSE;
                    }
                    else
                    {
                        mdns_if->is_truncated = FNET_TRUE;
                    }

                    ptr = (fnet_uint8_t *)hostname;
                    for(i = 0; i < fnet_htons(mdns_header->qdcount); i++)
                    {
                        ptr = _fnet_mdns_process_query(mdns_if, mdns_if->remote_address.sa_family, ptr, mdns_if->buffer, received);
                        if(ptr == NULL)
                        {
                            return;
                        }
                    }

                    /* Duplicate Suppression.*/
                    if(mdns_if->response_type != FNET_MDNS_QUERY_NONE)
                    {
                        const fnet_uint8_t           *an_ptr;

                        an_ptr = _fnet_mdns_get_an(mdns_if->buffer, received);

                        /* Eliminate duplicated answers */
                        if(an_ptr)
                        {
                            _fnet_mdns_process_duplicate_answer(mdns_if, an_ptr, mdns_if->buffer, received);
                        }
                    }
                }
                /* Response */
                else if( ( ((mdns_header->flags & FNET_HTONS(FNET_MDNS_HEADER_FLAGS_QR)) != 0) &&      /* Response.*/
                           (mdns_header->qdcount == 0)) &&
                         ( (mdns_header->nscount > 0)
                           || (mdns_header->arcount > 0)
                           || (mdns_header->ancount > 0))
                       )
                {
                    cnt = fnet_htons(mdns_header->nscount) + fnet_htons(mdns_header->arcount) + fnet_htons(mdns_header->ancount) + fnet_htons(mdns_header->qdcount);

                    ptr = (fnet_uint8_t *)hostname;
                    for(i = 0; (i < cnt) && (ptr < (mdns_if->buffer + received)); i++)
                    {
                        ptr = _fnet_mdns_process_response(mdns_if, ptr, mdns_if->buffer, received);
                        if(ptr == NULL)
                        {
                            break;
                        }
                    }
                }
            }

        }
    }
}

/************************************************************************
* DESCRIPTION: Prepare domain name - replace dots by length of string
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_domain_name(fnet_uint8_t *buf, fnet_uint32_t buf_size, const char *domain_name)
{
    FNET_ASSERT(buf != FNET_NULL);
    FNET_ASSERT(domain_name != FNET_NULL);

    fnet_uint32_t   domain_len = fnet_strlen(domain_name) + 1;
    fnet_uint32_t   len = 0;
    fnet_index_t    i;
    fnet_index_t    p = 0;
    fnet_uint8_t    *result = NULL;

    if(domain_len <= buf_size) /* Check buffer size limit */
    {
        for(i = 0; i < domain_len; i++)
        {
            buf[i + 1] = domain_name[i];
            len++;

            if(domain_name[i] == '.')
            {
                buf[p] = len - 1;
                p = i + 1;
                len = 0;
            }
        }

        /* the last part */
        buf[p] = len ? len - 1 : 0;
        result = buf + domain_len;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Send probe query.
   RFC: All probe queries SHOULD be done
   using the desired resource record name and class (usually class 1,
   "Internet"), and query type "ANY" (255), to elicit answers for all
   types of records with that name.  This allows a single question to be
   used in place of several questions, which is more efficient on the
   network.  It also allows a host to verify exclusive ownership of a
   name for all rrtypes, which is desirable in most cases.
************************************************************************/
static void _fnet_mdns_send_probe(fnet_mdns_if_t *mdns_if)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_uint8_t        *ptr;
    fnet_uint32_t       send_size = 0;
    fnet_uint16_t       nscount = 0;
    fnet_uint16_t       qdcount = 0;
    fnet_index_t        i;
    fnet_mdns_header_t  *mdns_header;
    fnet_uint8_t        *buf_end = &mdns_if->buffer[FNET_MDNS_PACKET_SIZE - 1];

    fnet_memset(mdns_if->buffer, 0, sizeof(mdns_if->buffer));

    mdns_header = (fnet_mdns_header_t *)&mdns_if->buffer[0];

    ptr = (fnet_uint8_t *)mdns_header + sizeof(fnet_mdns_header_t);

    /* RFC: All probe queries SHOULD be done
    using the desired resource record name and class (usually class 1,
    "Internet"), and query type "ANY" (255), to elicit answers for all
    types of records with that name. */

    /* Question section: */
    /*RFC6762: When a host is probing for a group of related records with the same
    name (e.g., the SRV and TXT record describing a DNS-SD service), only
    a single question need be placed in the Question Section, since query
    type "ANY" (255) is used, which will elicit answers for all records
    with that name.*/

    /* Prepary ANY records with service name */
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type)
        {
            ptr = _fnet_mdns_add_service_name(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], FNET_TRUE);
            if(ptr == NULL)
            {
                goto ERROR;
            }
            ptr = _fnet_mdns_add_qe_any(ptr, (buf_end - ptr));
            if(ptr == NULL)
            {
                goto ERROR;
            }
            qdcount++;
        }
    }

    /* Prepary ANY record with device/host name */
    ptr = _fnet_mdns_add_host_name(mdns_if, ptr, (buf_end - ptr), FNET_TRUE);
    if(ptr == NULL)
    {
        goto ERROR;
    }
    ptr = _fnet_mdns_add_qe_any(ptr, (buf_end - ptr));
    if(ptr == NULL)
    {
        goto ERROR;
    }
    qdcount++;

    /* Count */
    mdns_header->qdcount = fnet_htons(qdcount);

    /* Authority section: */
    /* RFC6762: A probe query can be distinguished from a normal query by the fact that a probe
    query contains a proposed record in the Authority Section that
    answers the question in the Question Section (for more details, see
    Section 8.2, "Simultaneous Probe Tiebreaking"). */

    /* RFC: For tiebreaking to work correctly in all
    cases, the Authority Section must contain *all* the records and
    proposed rdata being probed for uniqueness.*/

    /*  The cache-flush bit MUST NOT be set in any resource records in the
    Known-Answer list of any query message.*/

    /* Prepare RR SRV records */
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type)
        {
            ptr = _fnet_mdns_add_rr_srv(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], mdns_if->rr_ttl, FNET_FALSE, FNET_TRUE);
            if(ptr == NULL)
            {
                goto ERROR;
            }
            nscount++;
        }
    }

    /* Prepare RR A record */
    ptr = _fnet_mdns_add_rr_a(mdns_if, ptr, (buf_end - ptr), mdns_if->rr_ttl, FNET_FALSE, FNET_TRUE);
    if(ptr == NULL)
    {
        goto ERROR;
    }
    nscount++;

//    /* Prepare RR AAAA record */
//    ptr = _fnet_mdns_add_rr_aaaa(mdns_if, ptr, (buf_end - ptr), mdns_if->rr_ttl, FNET_FALSE, FNET_TRUE);
//    if(ptr == NULL)
//    {
//        goto ERROR;
//    }
//    nscount++;

    mdns_header->nscount = fnet_htons(nscount);

    /* Message size.*/
    send_size = ptr - mdns_if->buffer;

    FNET_DEBUG_MDNS("MDNS: Probing...");

    /* Send mdns packet */
    _fnet_mdns_send(mdns_if, mdns_if->addr_family, mdns_if->buffer, send_size);

ERROR:
    mdns_if->probe_count++;

    /* Reset response parameters.*/
    mdns_if->response_type = FNET_MDNS_QUERY_NONE;
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        mdns_if->service_if_list[i].response_type = FNET_MDNS_QUERY_NONE;
        mdns_if->service_if_list[i].offset_service_name = 0;
        mdns_if->service_if_list[i].offset_service_type = 0;
    }
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    mdns_if->service_if_enum.offset_service_type = 0;
#endif
}

/************************************************************************
* DESCRIPTION: Actual Send.
************************************************************************/
static void _fnet_mdns_send(fnet_mdns_if_t *mdns_if, fnet_address_family_t address_family, fnet_uint8_t *buffer, fnet_uint32_t send_size)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buffer != FNET_NULL);

    fnet_index_t            i;
    struct fnet_sockaddr    *address;
    struct fnet_sockaddr    multicast_addr;

    /* Reset Domain Name Compression pointers.*/
    mdns_if->offset_host_name = 0;
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        mdns_if->service_if_list[i].offset_service_name = 0;
        mdns_if->service_if_list[i].offset_service_type = 0;
    }
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    mdns_if->service_if_enum.offset_service_type = 0;
#endif

    mdns_if->send_timestamp = fnet_timer_get_ms(); /* Save last sending time stamp.*/

#if FNET_CFG_IP4
    if(address_family & AF_INET)
    {
        if(mdns_if->is_legacy_unicast == FNET_TRUE)
        {
            address = &mdns_if->remote_address;
        }
        else
        {
            fnet_memset_zero(&multicast_addr, sizeof(multicast_addr));
            multicast_addr.sa_family = AF_INET;
            multicast_addr.sa_port = FNET_CFG_MDNS_PORT;
//            multicast_addr.sa_scope_id = fnet_netif_get_scope_id(mdns_if->netif);
            ((struct fnet_sockaddr_in *)(&multicast_addr))->sin_addr.s_addr = FNET_MDNS_IP4_MULTICAST_ADDR;

            address = &multicast_addr;
        }

        /* Send to IPv4 address */
        fnet_socket_sendto( mdns_if->socket_listen, buffer, send_size, 0u, address, sizeof(*address) );
    }
#endif
#if FNET_CFG_IP6
    if(address_family & AF_INET6)
    {
        if(mdns_if->is_legacy_unicast == FNET_TRUE)
        {
            address = &mdns_if->remote_address;
        }
        else
        {
            fnet_memset_zero(&multicast_addr, sizeof(multicast_addr));
            multicast_addr.sa_family = AF_INET6;
            multicast_addr.sa_port = FNET_CFG_MDNS_PORT;
            multicast_addr.sa_scope_id = fnet_netif_get_scope_id(mdns_if->netif);
            FNET_IP6_ADDR_COPY(&fnet_mdns_ip6_multicast_addr, &((struct fnet_sockaddr_in6 *)(&multicast_addr))->sin6_addr.s6_addr);

            address = &multicast_addr;
        }

        /* Send to IPv6 address */
        fnet_socket_sendto ( mdns_if->socket_listen, buffer, send_size, 0u, address, sizeof(*address) );
    }
#endif

    /* Reset flag.*/
    mdns_if->is_legacy_unicast = FNET_FALSE;
}

/************************************************************************
* DESCRIPTION: Prepare all mdns records to send.
************************************************************************/
static void _fnet_mdns_send_response(fnet_mdns_if_t *mdns_if, fnet_uint32_t ttl)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_uint8_t        *ptr;
    fnet_uint32_t       send_size = 0;
    fnet_mdns_header_t  *mdns_header = (fnet_mdns_header_t *)&mdns_if->buffer[0];
    fnet_uint16_t       answer_count = 0;
    fnet_uint16_t       additional_count = 0;
    fnet_bool_t         has_srv = FNET_FALSE;
    fnet_bool_t         has_ptr = FNET_FALSE;
    fnet_index_t        i;
    fnet_bool_t         flush;
    fnet_uint8_t        *buf_end = &mdns_if->buffer[FNET_MDNS_PACKET_SIZE - 1];

    fnet_memset(mdns_if->buffer, 0, sizeof(mdns_if->buffer));

    ptr = (fnet_uint8_t *)mdns_header + sizeof(fnet_mdns_header_t);
    send_size += sizeof(fnet_mdns_header_t);

    mdns_header->flags |= FNET_HTONS(FNET_MDNS_HEADER_FLAGS_QR | FNET_MDNS_HEADER_FLAGS_C);

    if(mdns_if->is_legacy_unicast == FNET_TRUE)
    {
        /* RFC6762: The cache-flush  bit MUST NOT be set in legacy unicast responses.*/
        flush = FNET_FALSE;
    }
    else
    {
        flush = FNET_TRUE;
    }

    /* Answer records */
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type)
        {
            /* Prepare TXT record */
            if(mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_TXT)
            {
                if((&mdns_if->service_if_list[i])->service_get_txt) /* Check if service has TXT */
                {
                    ptr = _fnet_mdns_add_rr_txt(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, flush, FNET_TRUE);
                    if(ptr == NULL)
                    {
                        goto ERROR;
                    }
                    answer_count++;
                }
            }

            /* Prepare RR PTR record */
            if(mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_PTR)
            {
                ptr = _fnet_mdns_add_rr_ptr(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, FNET_TRUE);
                if(ptr == NULL)
                {
                    goto ERROR;
                }
                answer_count++;
                has_ptr = FNET_TRUE;
            }

#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
            /* Prepare Service Type Enumeration RR PTR record */
            if(mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_PTR_ENUM)
            {
                ptr = _fnet_mdns_add_rr_ptr_enum(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, FNET_TRUE);
                if(ptr == NULL)
                {
                    goto ERROR;
                }
                answer_count++;
            }
#endif

            /* Prepare RR SRV record */
            if(mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_SRV)
            {
                ptr = _fnet_mdns_add_rr_srv(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, flush, FNET_TRUE);
                if(ptr == NULL)
                {
                    goto ERROR;
                }
                answer_count++;
                has_srv = FNET_TRUE;
            }
        }
    }

    /* Prepare RR A record */
    if(mdns_if->response_type & FNET_MDNS_QUERY_A)
    {
        ptr = _fnet_mdns_add_rr_a(mdns_if, ptr, (buf_end - ptr), ttl, flush, FNET_TRUE);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        answer_count++;
    }

//    /* Prepare RR AAAA record */
//    if(mdns_if->response_type & FNET_MDNS_QUERY_AAAA)
//    {
//        ptr = _fnet_mdns_add_rr_aaaa(mdns_if, ptr, (buf_end - ptr), ttl, flush, FNET_TRUE);
//        if(ptr == NULL)
//        {
//            goto ERROR;
//        }
//        answer_count++;
//    }

    /* Additional records */

    /* RFC6763: When including a DNS-SD Service Instance Enumeration or Selective
    Instance Enumeration (subtype) PTR record in a response packet, the
    server/responder SHOULD include the following additional records:
    o  The SRV record(s) named in the PTR rdata.
    o  The TXT record(s) named in the PTR rdata.
    o  All address records (type "A" and "AAAA") named in the SRV rdata.*/
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type)
        {
            if(mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_PTR)
            {
                if((mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_SRV) == 0)
                {
                    ptr = _fnet_mdns_add_rr_srv(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, flush, FNET_TRUE);
                    if(ptr == NULL)
                    {
                        goto ERROR;
                    }
                    additional_count++;
                }

                if((mdns_if->service_if_list[i].response_type & FNET_MDNS_QUERY_TXT) == 0)
                {
                    if((&mdns_if->service_if_list[i])->service_get_txt) /* Check if service has TXT */
                    {
                        ptr = _fnet_mdns_add_rr_txt(mdns_if, ptr, (buf_end - ptr), &mdns_if->service_if_list[i], ttl, flush, FNET_TRUE);
                        if(ptr == NULL)
                        {
                            goto ERROR;
                        }
                        additional_count++;
                    }
                }
            }
        }
    }

    /* RFC6763: When including an SRV record in a response packet, the
    server/responder SHOULD include the following additional records:
    o  All address records (type "A" and "AAAA") named in the SRV rdata.*/
    if((has_ptr == FNET_TRUE) || (has_srv == FNET_TRUE))
    {
        if((mdns_if->response_type & FNET_MDNS_QUERY_A) == 0)
        {
            /* Prepare RR A record */
            ptr = _fnet_mdns_add_rr_a(mdns_if, ptr, (buf_end - ptr), ttl, flush, FNET_TRUE);
            if(ptr == NULL)
            {
                goto ERROR;
            }
            additional_count++;
        }
        if((mdns_if->response_type & FNET_MDNS_QUERY_AAAA) == 0)
        {
//            /* Prepare RR AAAA record */
//            ptr = _fnet_mdns_add_rr_aaaa(mdns_if, ptr, (buf_end - ptr), ttl, flush, FNET_TRUE);
//            if(ptr == NULL)
//            {
//                goto ERROR;
//            }
//            additional_count++;
        }
    }

    /* RFC6762:  Multicast DNS responses MUST NOT contain any questions in the
    * Question Section.  */
    if(answer_count) /* Check if any answer for sending*/
    {
        /* Answers */
        mdns_header->ancount = FNET_HTONS(answer_count);
        /* Additional */
        mdns_header->arcount = FNET_HTONS(additional_count);

        send_size = ptr - (fnet_uint8_t *)mdns_header;

        /* Send all mdns records */
        _fnet_mdns_send(mdns_if, mdns_if->response_address_family, mdns_if->buffer, send_size);
    }

ERROR:
    /* Reset response parameters.*/
    mdns_if->response_type = FNET_MDNS_QUERY_NONE;
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        mdns_if->service_if_list[i].response_type = FNET_MDNS_QUERY_NONE;
        mdns_if->service_if_list[i].offset_service_name = 0;
        mdns_if->service_if_list[i].offset_service_type = 0;
    }
#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
    mdns_if->service_if_enum.offset_service_type = 0;
#endif
    mdns_if->response_address_family = AF_UNSPEC;
    mdns_if->is_shared = FNET_FALSE;
    mdns_if->is_legacy_unicast = FNET_FALSE;
}

/************************************************************************
* DESCRIPTION: Send announcement.
************************************************************************/
static void _fnet_mdns_send_announcement(fnet_mdns_if_t *mdns_if, fnet_uint32_t ttl)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_index_t i;

    /* Set response_type to ANY */
    mdns_if->response_type = FNET_MDNS_QUERY_ANY;
    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        mdns_if->service_if_list[i].response_type = FNET_MDNS_QUERY_ANY;
    }
    mdns_if->response_address_family = mdns_if->addr_family;

    _fnet_mdns_send_response(mdns_if, ttl);
}

/************************************************************************
* DESCRIPTION: Add record header.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_rr_header(fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_rr_type_t type, fnet_bool_t flush, fnet_uint32_t ttl, fnet_uint16_t data_length)
{
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t            *ptr = buf;
    fnet_mdns_rr_header_t   *mdns_rr_header = (fnet_mdns_rr_header_t *)buf;
    fnet_uint16_t           rr_class = FNET_MDNS_HEADER_CLASS_IN; /* RFC: generally only DNS class 1 ("Internet") is used */
    fnet_uint8_t            *result = NULL;

    if(sizeof(fnet_mdns_rr_header_t) <= buf_size)
    {
        if(flush != FNET_FALSE)
        {
            rr_class |= FNET_MDNS_HEADER_CACHE_FLUSH;
        }

        mdns_rr_header->type        = fnet_htons( (fnet_uint16_t) type );
        mdns_rr_header->rr_class    = fnet_htons( (fnet_uint16_t) rr_class );
        mdns_rr_header->ttl         = fnet_htonl( (fnet_uint32_t) ttl );
        mdns_rr_header->data_length = fnet_htons( (fnet_uint16_t) data_length );

        result = ptr + sizeof(fnet_mdns_rr_header_t);
    }
    return result;
}

/************************************************************************
* DESCRIPTION: Add TXT record to buffer.
************************************************************************/
fnet_uint8_t *_fnet_mdns_add_rr_txt(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t        *rr_header_ptr;
    fnet_uint8_t        *txt_ptr;
    fnet_uint32_t       rr_txt_length = 0;
    fnet_uint8_t        *ptr = buf;
    fnet_uint8_t        *result = FNET_NULL;
    const fnet_mdns_txt_key_t   *txt_key_table;

    /* RR Name */
    /* Add service name */
    ptr = _fnet_mdns_add_service_name(mdns_if, ptr, (buf + buf_size) - ptr, service_if, compression);
    if(ptr == NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > sizeof(fnet_mdns_rr_header_t))
    {
        rr_header_ptr = ptr;

        if(service_if->service_get_txt)
        {
            txt_key_table = service_if->service_get_txt();
            /* Add TXT keys*/
            if(txt_key_table)
            {
                txt_ptr = ptr + sizeof(fnet_mdns_rr_header_t);

                while(txt_key_table->key_name)
                {
                    if(txt_key_table->key_value)
                    {
                        txt_ptr = _fnet_mdns_add_txt_key(txt_ptr, ((buf + buf_size) - txt_ptr), txt_key_table->key_name, txt_key_table->key_value);
                        if(txt_ptr == FNET_NULL)
                        {
                            goto ERROR;
                        }
                    }
                    txt_key_table++;
                }

                rr_txt_length = txt_ptr - (ptr + sizeof(fnet_mdns_rr_header_t));
            }
        }

        /* RR TXT header */
        ptr = _fnet_mdns_add_rr_header(rr_header_ptr, sizeof(fnet_mdns_rr_header_t), FNET_MDNS_RR_TXT, flush, ttl, rr_txt_length);

        ptr += rr_txt_length;

        result = ptr;
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION: Add PTR record to buffer.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_rr_ptr(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t     *rr_header_ptr;
    fnet_uint8_t     *ptr = buf;
    fnet_uint8_t     *result = NULL;

    /* Add service type */
    ptr = _fnet_mdns_add_service_type_name(mdns_if, ptr, (buf + buf_size) - ptr, service_if, compression);
    if(ptr == NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > sizeof(fnet_mdns_rr_header_t))
    {
        /* Add RR header later, when calculate data size */
        rr_header_ptr = ptr;
        ptr = ptr + sizeof(fnet_mdns_rr_header_t);

        /* Add PTR data */
        ptr = _fnet_mdns_add_service_name(mdns_if, ptr, (buf + buf_size) - ptr, service_if, compression);
        if(ptr == NULL)
        {
            goto ERROR;
        }

        /* Add RR PTR header */
        _fnet_mdns_add_rr_header(rr_header_ptr,  sizeof(fnet_mdns_rr_header_t), FNET_MDNS_RR_PTR, FNET_FALSE, ttl, ptr - rr_header_ptr - sizeof(fnet_mdns_rr_header_t));

        result = ptr;
    }
ERROR:
    return result;
}

#if FNET_CFG_MDNS_SERVICE_TYPE_ENUMERATION
static fnet_uint8_t *_fnet_mdns_add_rr_ptr_enum(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t     *rr_header_ptr;
    fnet_uint8_t     *ptr = buf;
    fnet_uint8_t     *result = FNET_NULL;

    /* Add service type */
    ptr = _fnet_mdns_add_service_type_name(mdns_if, ptr, (buf + buf_size) - ptr, &mdns_if->service_if_enum, compression);
    if(ptr == FNET_NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > sizeof(fnet_mdns_rr_header_t))
    {
        /* Add RR header later, when calculate data size */
        rr_header_ptr = ptr;
        ptr = ptr + sizeof(fnet_mdns_rr_header_t);

        /* Add PTR data */
        ptr = _fnet_mdns_add_service_type_name(mdns_if, ptr, (buf + buf_size) - ptr, service_if, compression);
        if(ptr == NULL)
        {
            goto ERROR;
        }

        /* Add RR PTR header */
        _fnet_mdns_add_rr_header(rr_header_ptr,  sizeof(fnet_mdns_rr_header_t), FNET_MDNS_RR_PTR, FNET_FALSE, ttl, ptr - rr_header_ptr - sizeof(fnet_mdns_rr_header_t));

        result = ptr;
    }
ERROR:
    return result;
}
#endif

/************************************************************************
* DESCRIPTION:  Add SRV record to buffer.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_rr_srv(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_mdns_rr_data_srv_t    data;
    fnet_uint8_t                     *rr_header_ptr;
    fnet_uint8_t                     *ptr = buf;
    fnet_uint8_t                     *result = NULL;

    /* Add service name */
    ptr = _fnet_mdns_add_service_name(mdns_if, ptr, (buf + buf_size) - ptr, service_if, compression);
    if(ptr == NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > (sizeof(fnet_mdns_rr_header_t) + sizeof(fnet_mdns_rr_data_srv_t)))
    {
        /* Add RR header later, when calculate data size */
        rr_header_ptr = ptr;
        ptr = ptr + sizeof(fnet_mdns_rr_header_t);

        /* Add SRV data */
        fnet_memset(&data, 0, sizeof(data));
        data.port = service_if->service_port;
        fnet_memcpy(ptr, (fnet_uint8_t *)&data, sizeof(fnet_mdns_rr_data_srv_t));
        ptr += sizeof(data);

        ptr = _fnet_mdns_add_host_name(mdns_if, ptr, (buf + buf_size) - ptr, compression);
        if(ptr == NULL)
        {
            goto ERROR;
        }

        /* Add RR SRV header */
        _fnet_mdns_add_rr_header(rr_header_ptr, sizeof(fnet_mdns_rr_header_t), FNET_MDNS_RR_SRV, flush, ttl, ptr - rr_header_ptr - sizeof(fnet_mdns_rr_header_t));

        result = ptr;
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Add A record to buffer.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_rr_a(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_ip4_addr_t     my_addr;
    fnet_uint8_t        *ptr = buf;
    fnet_uint8_t        *result = NULL;

    my_addr = fnet_netif_get_ip4_addr(mdns_if->netif);

    ptr = _fnet_mdns_add_host_name(mdns_if, ptr, (buf + buf_size) - ptr, compression);
    if(ptr == NULL)
    {
        goto ERROR;
    }

    /* Prepare RR A record */
    ptr = _fnet_mdns_add_rr_header(ptr, (buf + buf_size) - ptr, FNET_MDNS_RR_A, flush, ttl, sizeof(my_addr));
    if(ptr == NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > sizeof(my_addr))
    {
        fnet_memcpy(ptr, &my_addr, sizeof(my_addr));
        ptr += sizeof(my_addr);

        result = ptr;
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Add AAAA record to buffer.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_rr_aaaa(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint32_t ttl, fnet_bool_t flush, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t                *ptr = buf;
    fnet_uint8_t                *result = FNET_NULL;
    fnet_netif_ip6_addr_info_t  addr_info = {.state = FNET_NETIF_IP6_ADDR_STATE_NOT_USED};

    fnet_netif_get_ip6_addr (mdns_if->netif, 0u, &addr_info); /* Just get 1st address.*/

    ptr = _fnet_mdns_add_host_name(mdns_if, ptr, (buf + buf_size) - ptr, compression);
    if(ptr == NULL)
    {
        goto ERROR;
    }

    /* Prepare RR AAAA record */
    ptr = _fnet_mdns_add_rr_header(ptr, (buf + buf_size) - ptr, FNET_MDNS_RR_AAAA, flush, ttl, sizeof(addr_info.address));
    if(ptr == NULL)
    {
        goto ERROR;
    }

    if(((buf + buf_size) - ptr) > sizeof(addr_info.address))
    {
        fnet_memcpy(ptr, &addr_info.address, sizeof(addr_info.address));
        ptr += sizeof(addr_info.address);

        result = ptr;
    }

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Add ANY Question Entry to buffer.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_qe_any(fnet_uint8_t *buf, fnet_uint32_t buf_size)
{
    FNET_ASSERT(buf != FNET_NULL);

    fnet_mdns_qe_header_t   *qe_header;
    fnet_uint8_t            *ptr = buf;
    fnet_uint8_t            *result = NULL;

    if(buf_size > sizeof(fnet_mdns_qe_header_t))
    {
        /* Prepare  Question type and class*/
        qe_header = (fnet_mdns_qe_header_t *)ptr;
        qe_header->rr_class = FNET_HTONS( (fnet_uint16_t) FNET_MDNS_HEADER_CLASS_IN );
        qe_header->type = FNET_HTONS( (fnet_uint16_t) FNET_MDNS_RR_ANY );
        ptr += sizeof(fnet_mdns_qe_header_t);
        result = ptr;
    }

    return result;
}

/************************************************************************
* DESCRIPTION:  Add domain name compression.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_domain_name_compression(fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_uint16_t name_offset)
{
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t     *ptr = buf;
    fnet_uint8_t     *result = NULL;

    if(buf_size > 2)
    {
        *ptr = FNET_MDNS_DOMAIN_NAME_COMPRESSION | ((name_offset & 0x3F00) >> 8);
        ptr++;
        *ptr = name_offset & 0xFF;
        ptr++;

        result = ptr;
    }

    return result;
}

/************************************************************************
* DESCRIPTION:  Add host name string.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_host_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t     *ptr = buf;
    fnet_uint8_t     *result = NULL;

    if(mdns_if->offset_host_name && compression)
    {
        /* Use Domain Name Compression */
        ptr = _fnet_mdns_add_domain_name_compression(ptr, (buf + buf_size) - ptr, mdns_if->offset_host_name);
        if(ptr == NULL)
        {
            goto ERROR;
        }
    }
    else
    {
        if(compression)
        {
            /* Offset, used by Domain Name Compression.*/
            fnet_uint16_t offset = ptr - mdns_if->buffer;

            if(offset <= 0x3FFF) /* Check offset maximum value.*/
            {
                mdns_if->offset_host_name = offset;
            }
        }

        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, mdns_if->host_name);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, FNET_MDNS_DOMAIN_NAME);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        if(((buf + buf_size) - ptr) < 2)
        {
            goto ERROR;
        }
        *ptr++ = 0;
    }

    result = ptr;

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Add service name string.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_service_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint16_t    offset;
    fnet_uint8_t     *ptr = buf;
    fnet_uint8_t     *result = NULL;

    if(service_if->offset_service_name  && compression)
    {
        /* Use Domain Name Compression */
        ptr = _fnet_mdns_add_domain_name_compression(ptr, (buf + buf_size) - ptr, service_if->offset_service_name);
        if(ptr == NULL)
        {
            goto ERROR;
        }
    }
    else
    {
        if(compression)
        {
            /* Offset, used by Domain Name Compression.*/
            offset = ptr - mdns_if->buffer;
            if(offset <= 0x3FFF) /* Check offset maximum value.*/
            {
                service_if->offset_service_name = offset;
            }
        }

        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, mdns_if->service_name);
        if(ptr == NULL)
        {
            goto ERROR;
        }

        if(compression && (service_if->offset_service_type == 0))
        {
            /* Offset, used by Domain Name Compression.*/
            offset = ptr - mdns_if->buffer;
            if(offset <= 0x3FFF) /* Check offset maximum value.*/
            {
                service_if->offset_service_type = offset;
            }
        }

        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, service_if->service_type);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, FNET_MDNS_DOMAIN_NAME);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        if(((buf + buf_size) - ptr) < 2)
        {
            goto ERROR;
        }
        *ptr++ = 0;
    }

    result = ptr;

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Add service type string.
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_service_type_name(fnet_mdns_if_t *mdns_if, fnet_uint8_t *buf, fnet_uint32_t buf_size, fnet_mdns_service_if_t *service_if, fnet_bool_t compression)
{
    FNET_ASSERT(mdns_if != FNET_NULL);
    FNET_ASSERT(buf != FNET_NULL);

    fnet_uint8_t     *ptr = buf;
    fnet_uint16_t    offset;
    fnet_uint8_t     *result = NULL;

    if(service_if->offset_service_type && compression)
    {
        /* Use Domain Name Compression */
        ptr = _fnet_mdns_add_domain_name_compression(ptr, (buf + buf_size) - ptr, service_if->offset_service_type);
        if(ptr == NULL)
        {
            goto ERROR;
        }
    }
    else
    {
        if(compression)
        {
            /* Offset, used by Domain Name Compression.*/
            offset = ptr - mdns_if->buffer;
            if(offset <= 0x3FFF) /* Check offset maximum value.*/
            {
                service_if->offset_service_type = offset;
            }
        }

        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, service_if->service_type);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        ptr = _fnet_mdns_add_domain_name(ptr, (buf + buf_size) - ptr, FNET_MDNS_DOMAIN_NAME);
        if(ptr == NULL)
        {
            goto ERROR;
        }
        if(((buf + buf_size) - ptr) < 2)
        {
            goto ERROR;
        }
        *ptr++ = 0;
    }

    result = ptr;

ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION:  Determines if host_name is our fully qualified domain
*               name FQDN (hostname.domain).
************************************************************************/
static fnet_bool_t _fnet_mdns_is_our_host_name(fnet_mdns_if_t *mdns_if, char *host_name)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    return _fnet_mdns_cmp_rr_name(host_name, "", mdns_if->host_name);
}

/************************************************************************
* DESCRIPTION:  Determines if service_name is our service name.
************************************************************************/
static fnet_bool_t _fnet_mdns_is_our_service_name(fnet_mdns_if_t *mdns_if, char *service_name)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    return (_fnet_mdns_get_service_by_name(mdns_if, service_name) ? FNET_TRUE : FNET_FALSE);
}

/************************************************************************
* DESCRIPTION:  Gets service interfcase by name (servicename.type.domain).
************************************************************************/
static fnet_mdns_service_if_t *_fnet_mdns_get_service_by_name(fnet_mdns_if_t *mdns_if, char *service_name)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_index_t            i;
    fnet_mdns_service_if_t  *result = NULL;

    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type != NULL)
        {
            if(_fnet_mdns_cmp_rr_name(service_name, mdns_if->service_name, mdns_if->service_if_list[i].service_type) == FNET_TRUE)
            {
                result = &mdns_if->service_if_list[i];
                break; /* Found.*/
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Gets service interfcase by type (type.domain).
************************************************************************/
static fnet_mdns_service_if_t *_fnet_mdns_get_service_by_type(fnet_mdns_if_t *mdns_if, char *service_type)
{
    FNET_ASSERT(mdns_if != FNET_NULL);

    fnet_index_t            i;
    fnet_mdns_service_if_t *result = NULL;

    for(i = 0; i < FNET_CFG_MDNS_SERVICE_MAX; i++)
    {
        if(mdns_if->service_if_list[i].service_type != NULL)
        {
            if(_fnet_mdns_cmp_rr_name(service_type, "", mdns_if->service_if_list[i].service_type) == FNET_TRUE)
            {
                result = &mdns_if->service_if_list[i];
                break; /* Found.*/
            }
        }
    }

    return result;
}

/************************************************************************
* DESCRIPTION:  Adds TXT record key
************************************************************************/
static fnet_uint8_t *_fnet_mdns_add_txt_key(fnet_uint8_t *buf, fnet_uint32_t buf_size, const fnet_char_t *txt_key, const fnet_char_t *txt_key_value)
{
    fnet_uint8_t *result = FNET_NULL;

    if(buf && buf_size && txt_key && txt_key_value)
    {
        fnet_size_t  length = fnet_strlen(txt_key) + fnet_strlen(txt_key_value) + 1/*=*/;

        if(buf_size > (length + 1/*length byte*/))
        {
            buf[0] = length; /*length byte*/
            /* TXT key*/
            fnet_snprintf((char *)(&buf[1]), (buf_size - 1), "%s=%s", txt_key, txt_key_value);

            result = &buf[1 + length];
        }
    }

    return result;
}

#endif /* FNET_CFG_MDNS*/
