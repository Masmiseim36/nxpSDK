/*! *********************************************************************************
 * \defgroup NUT  Thread Network Utilities Interface
 * @{
 ***********************************************************************************/
/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _NETWORK_UTILS_H
#define _NETWORK_UTILS_H

/*!
\file       network_utils.h
\brief      This is a header file for the Network Utils module.
*/


/*==================================================================================================
Include Files
==================================================================================================*/
/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"


#include "Messaging.h"
#include "fsl_os_abstraction.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#if __ICCARM__
    #define gLittleEndian_c __LITTLE_ENDIAN__
#elif __GNUC__
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        #define gLittleEndian_c 1
    #else
        #define gLittleEndian_c 0
    #endif
#else
    #warning "No Compiler was set"
#endif

/*! Max unsigned 64bit integers value */
#define THR_ALL_FFs64                       0xFFFFFFFFFFFFFFFF

/*! Max unsigned 32bit integers value */
#define THR_ALL_FFs32                       0xFFFFFFFF

/*! Max unsigned 16bit integers value */
#define THR_ALL_FFs16                       0xFFFF

/*! Max unsigned 8bit integers value */
#define THR_ALL_FFs8                        0xFF

/*! Length for IP address string size (used to compute size used in ntop). Value for 16 bytes strings */
#define INET_ADDRSTRLEN         16

/*! Length for IP address string size (used to compute size used in ntop). Value for 46 bytes strings */
#define INET6_ADDRSTRLEN        46

/*! Length for IP address string size (used to compute size used in ntop). Value for IID strings */
#define INET6_IID_LEN            8

/*! Minimum MTU value */
#define IP6_MINIMUM_MTU         1280U

/*! IPv6 Pseudo HDR size */
#define IP6_PSEUDO_HDR_SIZE     (2 * sizeof(ipAddr_t) + 2 * sizeof(uint32_t))

/*! IPv4 Pseudo HDR size */
#define IP4_PSEUDO_HDR_SIZE     (3 * sizeof(uint32_t))

/*! IPv4 any address */
#define IP4_ADDR_ANY               0UL

/*! IPv4 loopback address */
#define IP4_ADDR_LOOPBACK          0x7F000001L

/*! IPv4 all host group address */
#define IP4_ADDR_ALLHOSTS_GROUP    0xE0000001L

/*! IPv4 all routers group address */
#define IP4_ADDR_ALLROUTERS_GROUP  0xE0000002L

/*! IPv4 RIP group address */
#define IP4_ADDR_RIP_GROUP         0xE0000009L

/*! IPv4 NTP group address */
#define IP4_ADDR_NTP_GROUP         0xE0000101L

/*! IPv4 IGMP group address */
#define IP4_ADDR_IGMP_GROUP        0xE0000016L

/*! IPv4 all routers group address */
#define IP4_ADDR_BROADCAST         0xFFFFFFFFL

/*! IPv4 any address mapped to IPv6 */
#define INADDR_ANY_INIT                                    \
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 }

/*! IPv4 broadcast address mapped to IPv6 */
#define INADDR_BCAST_INIT                                    \
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

/*! Macro to classify IPv4 address to any */
#define IP4_ZERONET(a)         (((a) & 0xFF000000L) == 0x00000000L)

/*! Macro to classify IPv4 address to loopback */
#define IP4_LOOPBACK(a)        (((a) & 0xFF000000L) == 0x7F000000L)

/*! Macro to classify IPv4 address to multicast */
#define IP4_MULTICAST(a)       (((a) & 0xF0000000L) == 0xE0000000L)

/*! Macro to classify IPv4 address to local multicast */
#define IP4_LOCAL_MULTICAST(a) (((a) & 0xFFFFFF00L) == 0xE0000000L)

/*! Macro to classify IPv4 address to experimental */
#define IP4_EXPERIMENTAL(a)    (((a) & 0xF0000000L) == 0xF0000000L)

/*! Macro to classify IPv4 address to class A */
#define IP4_CLASS_A(a)          (((a) & 0x80000000L) == 0x00000000L)

/*! IPv4 Class A mask */
#define IP4_CLASS_A_MASK         0xFF000000L

/*! Macro to classify IPv4 address to class B */
#define IP4_CLASS_B(a)          (((a) & 0xC0000000L) == 0x80000000L)

/*! IPv4 Class B mask */
#define IP4_CLASS_B_MASK         0xFFFF0000L

/*! Macro to classify IPv4 address to class C */
#define IP4_CLASS_C(a)          (((a) & 0xE0000000L) == 0xC0000000L)

/*! IPv4 Class C mask */
#define IP4_CLASS_C_MASK         0xFFFFFF00L

/*! IPV6 any address */
#define IN6ADDR_ANY_INIT                                    \
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

/*! IPV6 loopback address */
#define IN6ADDR_LOOPBACK_INIT                               \
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/*! IPV6 node local all nodes address */
#define IN6ADDR_NODELOCAL_ALLNODES_INIT                     \
        { 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/*! IPV6 interface local all nodes address */
#define IN6ADDR_INTFACELOCAL_ALLNODES_INIT                  \
        { 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/*! IPV6 link local all nodes address */
#define IN6ADDR_LINKLOCAL_ALLNODES_INIT                     \
        { 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

/*! IPV6 link local all routers address */
#define IN6ADDR_LINKLOCAL_ALLROUTERS_INIT                   \
        { 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }

/*! IPV6 link local all v2 routers address */
#define IN6ADDR_LINKLOCAL_ALLV2ROUTERS_INIT                 \
        { 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   \
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16 }

/*! IPV6 link local all DHCP routers and relay agents address */
#define IN6ADDR_LINKLOCAL_ALL_DHCP_ROUTERS_AND_RELAY_AGENTS  \
        {0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02}

/*! IPV6 realm local all DHCP lease query servers address */
#define IN6ADDR_REALMLOCAL_ALL_DHCP_LEASEQUERY_SERVERS       \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03}

/*! IPV6 realm local multicast 3ead address */
#define IN6ADDR_REALMLOCAL_MCAST_3EAD \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xAD}

/*! IPV6 realm local multicast 3ead address */
#define IN6ADDR_REALMLOCAL_ALLMPLFORWARDERS \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc}

/*! IPV6 site local all DHCP servers address */
#define IN6ADDR_SITELOCAL_ALLDHCPSERVERS                                  \
        {0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03}

/*! IPV6 realm local all nodes address */
#define IN6ADDR_REALMLOCAL_ALLNODES_INIT \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}

/*! IPV6 realm local all routers address */
#define IN6ADDR_REALMLOCAL_ALLROUTERS_INIT \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}

/*! IPV6 site local all nodes address */
#define IN6ADDR_SITELOCAL_ALLNODES_INIT \
        {0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}

/*! IPV6 site local all routers address */
#define IN6ADDR_SITELOCAL_ALLROUTERS_INIT \
        {0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}

/*! IPV6 link local prefix address */
#define IN6ADDR_LINK_LOCAL_PREFIX_INIT \
        {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

/*! IPV6 all FFs address */
#define IN6ADDR_ALL_FFs                                    \
        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   \
          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

/*! IPV6 link local all CoAP nodes address */
#define IN6ADDR_LINKLOCAL_ALL_COAP_NODES_INIT  \
        {0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd}

/*! IPV6 realm local all CoAP nodes address */
#define IN6ADDR_REALMLOCAL_ALL_COAP_NODES_INIT \
        {0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd}

/*! IPV6 admin local all CoAP nodes address */
#define IN6ADDR_ADMINLOCAL_ALL_COAP_NODES_INIT \
        {0xff, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd}

/*! IPV6 realm local all CoAP nodes address */
#define IN6ADDR_SITELOCAL_ALL_COAP_NODES_INIT       \
        {0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     \
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd}

/*! Macro for IP address copy */
#define IP_AddrCopy(dst, src) \
        (dst)->addr64[0] = (src)->addr64[0]; \
        (dst)->addr64[1] = (src)->addr64[1];

/*! Macro for IP address copy */
#define IP_AddrCopyFromArray(ip, buf, len) \
        for (uint8_t idx = 0; idx < len; idx++) \
            (ip)->addr8[idx] = (buf)[idx];

/*! Macro for IP address copy */
#define IP_AddrCopyToArray(ip, buf, len) \
        for (uint8_t idx = 0; idx < len; idx++) \
            (buf)[idx] = (ip)->addr8[idx];


/*! Macro for IP address conversion to uint32_t */
#define IP4_AddrToUint32(addr) (ntohal(&(addr)->addr8[12]))


/*! Macro for IPV6 address comparison */
#define IP_IsAddrEqual(addr1, addr2) \
        (((addr1)->addr64[0] == (addr2)->addr64[0]) && \
         ((addr1)->addr64[1] == (addr2)->addr64[1]))

/*! Macro for unspecified IPV6 address inquiry */
#define IP6_IsUnspecifiedAddr(addr) \
        (((addr)->addr64[0] == 0U) && \
         ((addr)->addr64[1] == 0U))

/*! Macro for link local IPV6 address inquiry */
#define IP6_IsLinkLocalAddr(addr) \
        (((addr)->addr8[0] == 0xFE) && (((addr)->addr8[1] & 0xC0) == 0x80))

/*! Macro for site local IPV6 address inquiry */
#define IP6_IsSiteLocalAddr(addr) \
        (((addr)->addr8[0] == 0xFE) && (((addr)->addr8[1] & 0xC0) == 0xC0))

/*! Macro for unique local IPV6 address inquiry */
#define IP6_IsUniqueLocalAddr(addr) \
        (((addr)->addr8[0] == 0xFD)||((addr)->addr8[0] == 0xFC) )

/*! Macro for global IPV6 address inquiry */
#define IP6_IsGlobalAddr(addr) \
        ((((addr)->addr8[0] & 0xF0) == 0x20)||(((addr)->addr8[0] & 0xF0) == 0x30))

/*! Macro for multicast IPV6 address inquiry */
#define IP6_IsMulticastAddr(addr) \
        ((addr)->addr8[0] == THR_ALL_FFs8)

/*! Macro for anycast IPV6 address inquiry */
#define IP6_IsAnycastAddr(addr) \
        (((addr)->addr8[11] == THR_ALL_FFs8) && ((addr)->addr8[12] == 0xFE) && ((addr)->addr8[14] == 0xFC))

/*! Macro for loopback IPV6 address inquiry */
#define IP6_IsLoopbackAddr(addr) \
        (((addr)->addr64[0] == 0U) && \
         ((addr)->addr64[1] == 0x0100000000000000U))

/*! Macro for local multicast all nodes IPV6 address inquiry */
#define IP6_IsLocalMulticastAllNodes(addr) \
        (((addr)->addr64[0] == 0x00000000000002FFU) && \
         ((addr)->addr64[1] == 0x0100000000000000U))

/*! Macro for local multicast all routers IPV6 address inquiry */
#define IP6_IsLocalMulticastAllRouters(addr) \
        (((addr)->addr64[0] == 0x00000000000002FFU) && \
         ((addr)->addr64[1] == 0x0200000000000000U))

/*! Macro for mesh multicast all nodes IPV6 address inquiry */
#define IP6_IsMeshMulticastAllNodes(addr) \
        (((addr)->addr64[0] == 0x00000000000003FFU) && \
         ((addr)->addr64[1] == 0x0100000000000000U))

/*! Macro for EUI64 IPV6 address inquiry */
#define IP6_IsAddrEui64(addr) \
        (!(((addr)->addr8[10] == 0x00) && ((addr)->addr8[11] == THR_ALL_FFs8) && \
        ((addr)->addr8[12] == 0xFE) && ((addr)->addr8[13] == 0x00)))

/*! Macro for values to IP address array transformation */
#define IP_ADDR(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16) \
        { (a1), (a2), (a3), (a4), (a5), (a6), (a7), (a8),       \
    (a9), (a10), (a11), (a12), (a13), (a14), (a15), (a16) }

/*! Mask for IPV4 address identification(RFC4291: 2.5.5.2) */
#define IPV4_Mask32_g   (THR_ALL_FFs16)

/*! Macro for IPV4 in IPv6 address inquiry */
#define IP_IsAddrIPv4(addr) \
        ((addr)->addr64[0] == 0U &&         \
         (addr)->addr16[4] == 0U &&         \
         (addr)->addr16[5] == IPV4_Mask32_g)

/*! Macro for IPV4 unspecified address inquiry */
#define IP4_IsUnspecifiedAddr(addr) \
        (((addr)->addr32[3] == 0U))

/*! Macro for IPV6 address inquiry */
#define IP_IsAddrIPv6(addr) (!(IP_IsAddrIPv4(addr)))

/*! Macro for appending network buffer */
#define NWKU_AppendNwkBuffer(dst, src) \
        (dst)->next = (src);

/*! Macro for link layer address validity inquiry */
#define NWKU_IsLlAddrValid(llAddr) (gLlayerAddrNoAddr_c != llAddr.addrSize)

/*! Macro for retrieving the last index of an array */
#define NWKU_GetLastArrayIndex(arraySize) ((arraySize) - 1U)

/*! Macro for host variable to 24 bit network array conversion */
#define htona24(p, x)\
    do\
    {\
        *((uint8_t*)(p)) = ((x) >> 16) & THR_ALL_FFs8;\
        htonas((uint8_t*)(p + 1), ((x) & THR_ALL_FFs16));\
    } while(0);

/*! Macro for 24 bit network array to host variable conversion */
#define ntoha24(p)\
    ((((uint32_t) *(((unsigned char*)(p)))) << 16)    \
   | (((uint32_t) *(((unsigned char*)(p))+1)) << 8) \
   | (((uint32_t) *(((unsigned char*)(p))+2))))

/*! Macro for host variable to 48 bit network array conversion */
#define htona48(p, x)\
    do\
    {\
        htonal((uint8_t*)p, ((x >> 16) & THR_ALL_FFs32));\
        htonas((uint8_t*)(p + 4), (x & THR_ALL_FFs16));\
    } while(0);

/*! Macro for 48 bit network array to host variable conversion */
#define ntoha48(p)\
    ((uint64_t)ntohal(p) << 16) | \
    (ntohas(p + 4) & THR_ALL_FFs16)

#if gLittleEndian_c

/*! Macro for network to host short conversion. Network byte order is OTA order */
#ifndef ntohs
#define ntohs(val)                      NWKU_Revert16((uint16_t)(val))
#endif

/*! Macro for host short to network conversion. Network byte order is OTA order */
#ifndef htons
#define htons(val)                      NWKU_Revert16((uint16_t)(val))
#endif

/*! Macro for network to host 32bit conversion. Network byte order is OTA order */
#ifndef ntohl
#define ntohl(val)                      NWKU_Revert32((uint32_t)(val))
#endif

/*! Macro for host 32bit to network conversion. Network byte order is OTA order */
#ifndef htonl
#define htonl(val)                      NWKU_Revert32((uint32_t)(val))
#endif

/*! Macro for network to host 64bit conversion. Network byte order is OTA order */
#ifndef ntohll
#define ntohll(val)                     NWKU_Revert64((uint64_t)(val))
#endif

/*! Macro for host 64bit to network conversion. Network byte order is OTA order */
#ifndef htonll
#define htonll(val)                     NWKU_Revert64((uint64_t)(val))
#endif

/*! Macro for network array to host short conversion. Network byte order is OTA order */
#ifndef ntohas
#define ntohas(p)                       NWKU_TransformArrayToUint16(p)
#endif

/*! Macro for host short to network array conversion. Network byte order is OTA order */
#ifndef htonas
#define htonas(p, x)                    NWKU_TransformUint16ToArray(p, (uint16_t)(x))
#endif

/*! Macro for network array to host 32bit conversion. Network byte order is OTA order */
#ifndef ntohal
#define ntohal(p)                       NWKU_TransformArrayToUint32(p)
#endif

/*! Macro for host 32bit to network array conversion. Network byte order is OTA order */
#ifndef htonal
#define htonal(p, x)                    NWKU_TransformUint32ToArray(p, (uint32_t)(x))
#endif

/*! Macro for network array to host 64bit conversion. Network byte order is OTA order */
#ifndef ntohall
#define ntohall(p)                      NWKU_TransformArrayToUint64(p)
#endif

/*! Macro for host 64bit to network array conversion. Network byte order is OTA order */
#ifndef htonall
#define htonall(p, x)                   NWKU_TransformUint64ToArray(p, (uint64_t)(x))
#endif

#else /*gBigEndian_c */
/*! Macro for network to host short conversion */
#ifndef ntohs
#define ntohs(val)                      (uint16_t)(val)
#endif

/*! Macro for host short to network conversion */
#ifndef htons
#define htons(val)                      (uint16_t)(val)
#endif

/*! Macro for network to host 32bit conversion */
#ifndef ntohl
#define ntohl(val)                      (uint32_t)(val)
#endif

/*! Macro for host 32bit to network conversion */
#ifndef htonl
#define htonl(val)                      (uint32_t)(val)
#endif

/*! Macro for network to host 64bit conversion*/
#ifndef ntohll
#define ntohll(val)                     (uint64_t)(val)
#endif

/*! Macro for host 64bit to network conversion */
#ifndef htonll
#define htonll(val)                     (uint64_t)(val)
#endif

/*! Macro for network array to host short conversion */
#ifndef ntohas
#define ntohas(p)                       (*((uint16_t*)(p)))
#endif

/*! Macro for host short to network array conversion */
#ifndef htonas
#define htonas(p, x)                    *((uint16_t*)(p)) = x
#endif

/*! Macro for network array to host 32bit conversion */
#ifndef ntohal
#define ntohal(p)                       (*((uint32_t*)(p)))
#endif

/*! Macro for host 32bit to network array conversion */
#ifndef htonal
#define htonal(p, x)                    *((uint32_t*)(p)) = x
#endif

/*! Macro for network array to host 64bit conversion */
#ifndef ntohall
#define ntohall(p)                      (*((uint64_t*)(p)))
#endif

/*! Macro for host 64bit to network array conversion */
#ifndef htonall
#define htonall(p, x)                   *((uint64_t*)(p)) = x
#endif

#endif

/* Supported address families */
#define AF_UNSPEC       0   /*!< Unspecified sockets */
#define AF_INET         2   /*!< Internet IP Protocol */
#define AF_INET6        10  /*!< IP version 6 */

/*! Default index for link layer address */
#define DEFAULT_LLADDR_IDX    0

/*! Macro for obtaining the minimum value variable between two input variables */
#ifndef MIN
#   define MIN(a,b)                     (((a) < (b))?(a):(b))
#endif

/*! Generic Message Event */
#define NWKU_GENERIC_MSG_EVENT 1

#define gNoIPv6FlowInfo_c               THR_ALL_FFs32
/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! Generic structure for holding uint16 values */
typedef union uuint16_tag
{
    uint16_t    u16;                 /*!< 16bit variable */
    uint8_t     u8[2];               /*!< 8bit array */
} uuint16_t;

/*! Generic structure for holding uint32 values */
typedef union uuint32_tag
{
    uint32_t    u32;                 /*!< 32bit variable */
    uint16_t    u16[2];              /*!< 16bit array */
    uint8_t     u8[4];               /*!< 8bit array */
} uuint32_t;

/*! Generic structure for holding uint64 values */
typedef union uuint64_tag
{
    uint64_t    u64;                 /*!< 64bit variable */
    uint32_t    u32[2];              /*!< 32bit array */
    uint16_t    u16[4];              /*!< 16bit array */
    uint8_t     u8[8];               /*!< 8bit array */
} uuint64_t;


/*! Generic structure for holding IP address information */
typedef union ipAddr_tag
{
    uint8_t     addr8[16];           /*!< 8bit array */
    uint16_t    addr16[8];           /*!< 16bit array */
    uint32_t    addr32[4];           /*!< 32bit array */
    uint64_t    addr64[2];           /*!< 64bit array */
} ipAddr_t;

/* Socket structures */
typedef struct sockaddrIn
{
    ipAddr_t      sin_addr;     /*!< Internet address     */
    uint16_t      sin_family;   /*!< Address family       */
    uint16_t      sin_port;     /*!< Port number          */
} sockaddrIn_t; /*!< Structure containing socket address information(IPV4) */

typedef struct sockaddrIn6
{
    ipAddr_t    sin6_addr;      /*!< IPV6 address */
    uint16_t    sin6_family;    /*!< The address family we used when we set up the socket (AF_INET6) */
    uint16_t    sin6_port;      /*!< The port number (the transport address) */
    uint32_t    sin6_flowinfo;  /*!< IPV6 flow information
                                    (LSB= (MAC key id mode) | (MAC security level) ) */
    uint32_t    sin6_scope_id;  /*!< set of interfaces for a scope (RFC2553) or media interface
                                    handle */
} sockaddrIn6_t; /*!< Structure containing socket address information(IPV6) */

typedef struct sockaddrStorage
{
    uint8_t     ss_addr[16];    /*!< Internet address */
    uint16_t    ss_family;      /*!< Address family */
    uint8_t     _data[sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t)];  /*!< Storage large enough and aligned
                                for storing the socket address data structure of any family */
} sockaddrStorage_t; /*!< Structure containing socket address information */

/*! Generic structure for holding buffer information */
typedef struct nwkBuffer_tag
{
    struct nwkBuffer_tag *next;        /*!< Pointer to next buffer */
    uint8_t *pData;                    /*!< Pointer to data */
    uint32_t size;                     /*!< Size of data */
    uint8_t freeBuffer;                /*!< Flag used to notify buffer clearance */
} nwkBuffer_t;

/*! Enumeration for address size */
typedef enum
{
    gLlayerAddrNoAddr_c     = 0x00,     /*!< No address (addressing fields omitted) */
    gLlayerAddrReserved_c   = 0x01,     /*!< Reserved value*/
    gLlayerAddrEui16_c      = 0x02,     /*!< 16-bit short Link Layer address (size 2 bytes) */
    gLlayerAddrEui48_c      = 0x06,     /*!< 48-bit Ethernet MAC Address (size 6 bytes) */
    gLlayerAddrEui64_c      = 0x08,     /*!< 64-bit extended Link Layer address (size 8 bytes) */
} llAddrSize_t;

/*! Unique interface ID enumeration*/
typedef enum
{
    gIpIfSlp0_c = 0x00,              /*!< SLWP0 interface */
    gIpIfSlp1_c,                     /*!< SLWP1 interface */
    gIpIfEth0_c,                     /*!< ETH0 interface */
    gIpIfEth1_c,                     /*!< ETH1 interface */
    gIpIfWifi0_c,                    /*!< WiFi0 interface */
    gIpIfWifi1_c,                    /*!< WiFi1 interface */
    gIpIfUsbRndis_c,                 /*!< RNDIS interface  */
    gIpIfSerialTun_c,                /*!< Serial TUN interface  */
    gIpIfBle0_c,                     /*!< BLE0 interface  */
    gIpIfBle1_c,                     /*!< BLE1 interface  */
    gIpIfLoopback_c,                 /*!< Loopback interface */
    gIpIfUndef_c = THR_ALL_FFs8      /*!< Undefined interface  */
}ipIfUniqueId_t;

/*! Generic structure for link layer address */
typedef struct llAddr_tag
{
    uint8_t         eui[8];          /*!< Destination address: short/extended */
    llAddrSize_t    addrSize;        /*!< Destination address type: short/extended */
} llAddr_t;

/*! Generic structure for IPv6 header */
typedef struct ip6Header_tag
{
    uint8_t versionTraficClass;      /*!< Version Traffic Class */
    uint8_t trafficClassFlowLabel;   /*!< Traffic Class Flow label */
    uint8_t flowLabel[2];            /*!< Flow label */
    uint8_t payloadLength[2];        /*!< Payload length */
    uint8_t nextHeader;              /*!< Next header */
    uint8_t hopLimit;                /*!< Hop limit */
    uint8_t srcAddr[16];             /*!< Source Address */
    uint8_t dstAddr[16];             /*!< Destination Address */
}ip6Header_t;

/*! Generic structure for IP packet options */
typedef struct ipPktOptions_tag
{
    void* ifHandle;                          /*!< Pointer to interface handler */
    nwkBuffer_t *ipExtensionHeaderBuffer;    /*!< Pointer to extended options buffer */
    void *ipReassemblyOptions;               /*!< Pointer to IP reassembly structure */
    llAddr_t srcLlInfo;                      /*!< Source Link Layer information */
    uint8_t ipHdrOffset;                     /*!< Offset from beginning of RX data where IP HDR is found */
    uint8_t hopLimit;                        /*!< Hop limit */
    uint8_t security;                        /*!< Security option */
    uint8_t lqi;                             /*!< Packet LQI */
    uint8_t qos;                             /*!< Packet Quality of Service */
    uint8_t isRelay;                         /*!< Flag to specify if packet is relay */
    uint8_t macSecKeyIdMode;                 /*!< MacSec Key ID Mode */
    uint8_t channel;                         /*!< Packet Channel */
    uint16_t destPanId;                      /*!< Destination PAN ID */
    uint16_t srcPanId;                       /*!< Source PAN ID */
} ipPktOptions_t;

/*! Received packet options structure */
typedef struct recvOptions_tag
{
    ipIfUniqueId_t ipIfId;        /*!< ID of the interface */
    uint8_t hopLimit;             /*!< Hop limit */
    uint8_t security;             /*!< Security option */
    uint8_t lqi;                  /*!< Packet LQI */
    uint8_t isRelay;              /*!< Flag to specify if packet is relay */
    uint8_t channel;              /*!< Packet Channel */
    uint8_t macSecKeyIdMode;      /*!< MacSec Key ID Mode */
    uint16_t macSrcPanId;         /*!< MAC Source PAN ID */
} recvOptions_t;



/* IP packet information structure */
typedef struct ipPktInfo_tag
{
    nwkBuffer_t*    pNwkBuff;      /*!< Pointer to network buffer */
    ipAddr_t*       pIpSrcAddr;    /*!< Pointer to source IP address */
    ipAddr_t*       pIpDstAddr;    /*!< Pointer to destination IP address */
    uint8_t*        pNextProt;     /*!< Pointer to the next protocol in pNwkBuff->pData. Do not free this one! */
    ipAddr_t        ipSrcAddr;     /*!< Source IP address */
    ipAddr_t        ipDstAddr;     /*!< Destination IP address */
    /*! Protocol information */
    union
    {
        uint32_t    nextProtLen;   /*!< Size of the data of next protocol in pNwkBuff->pData */
        uint32_t    protocolType;  /*!< Protocol type */
    }prot;
    uint16_t        srcPort;       /*!< Source port */
    uint16_t        dstPort;       /*!< Destination port */
    ipPktOptions_t  ipPktOptions;  /*!< IP packet options */
} ipPktInfo_t;

/*!
\brief    Callback function for servicing network messages
*/
typedef void (* nwkMsgHandler)(uint8_t *pData);

/*!
\brief    Callback function to be resumed after the response in received or the time is up
*/
typedef void (*appReturnHandler_t)(uint8_t* pMsg);

/*! Generic structure for network message */
typedef struct nwkMsg_tag
{
    nwkMsgHandler pFunc;      /*!< Pointer to packet handler */
    void* pPload;             /*!< Pointer to handler payload */
}nwkMsg_t;

/*! Task Message Queue structure */
typedef struct taskMsgQueue_tag
{
    msgQueue_t msgQueue;        /*!< Pointer to task message queue */
    osaTaskId_t taskId;         /*!< Pointer to task ID */
    osaEventId_t taskEventId;   /*!< Pointer to task event ID */
}taskMsgQueue_t;

/*!
\brief    Callback function for servicing transport packets

\param    [in]    tspConnIndex    Connection index
*/
typedef void (* tspDataIndCb_t)(uint8_t tspConnIndex);

/*! Lookup tables with 8 bits elements */
typedef struct lut8_tag
{
    uint8_t type;    /*!< Type */
    uint8_t idx;     /*!< Index */
}lut8_t;

/*! Network statistics, for debug */
typedef struct nwkuStats_tag
{
    uint8_t ipktUsed;        /*!< IP packets used */
    uint8_t ipktMax;         /*!< Maximum IP packets */
    uint8_t nwkBuffUsed;     /*!< Network buffers used */
    uint8_t nwkBuffMax;      /*!< Maximum network buffers */
} nwkStats_t;

/*! Structure for holding IP prefix information */
typedef struct ipPrefix_tag
{
    uint8_t prefixLen;    /*!< Size of the prefix in bits */
    uint8_t aPrefix[];    /*!< Pointer to the start of the prefix */
} ipPrefix_t;

/*! Network generic status enumeration */
typedef enum nwkStatus_tag
{
    gNwkStatusSuccess_c      = 0,              /*!< Network Status: Success */
    gNwkStatusMemAllocErr_c  = 1,              /*!< Network Status: Memory allocation error */
    gNwkStatusNotAllowed_c   = 2,              /*!< Operation was not allowed */
    gNwkStatusInvalidParam_c = 3,              /*!< Input parameters are invalid */
    gNwkStatusFail_c         = THR_ALL_FFs8,   /*!< Network Status: Fail */
}nwkStatus_t;

/*! Structure used for pbkdf2 generation */
typedef struct pbkdf2Params_tag
{
    uint8_t *pPass;     /*!< Pointer to the password */
    uint32_t passLen;   /*!< Length of the password */
    uint8_t *pSalt;     /*!< Pointer to the salt */
    uint32_t saltLen;   /*!< Length of the salt */
    uint32_t rounds;    /*!< Number of rounds */
}pbkdf2Params_t;

/*! Sequence number arithmetic comparison status */
typedef enum nwkSeqNbStatus_tag
{
    gNwkSeqNbLower_c     = 0,              /*!< Sequence number is lower status */
    gNwkSeqNbEqual_c     = 1,              /*!< Sequence number is equal status */
    gNwkSeqNbHigher_c    = 2,              /*!< Sequence number is higher status*/
}nwkSeqNbStatus_t;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const ipAddr_t inaddr_any;
extern const ipAddr_t inaddr_bcast;
extern const ipAddr_t in6addr_any;
extern const ipAddr_t in6addr_loopback;
extern const ipAddr_t in6addr_nodelocal_allnodes;
extern const ipAddr_t in6addr_linklocal_allnodes;
extern const ipAddr_t in6addr_linklocal_allrouters;
extern const ipAddr_t in6addr_linklocal_allv2routers;
extern const ipAddr_t in6addr_sitelocal_alldhcpservers;
extern const ipAddr_t in6addr_realmlocal_allnodes;
extern const ipAddr_t in6addr_realmlocal_allrouters;
extern const ipAddr_t in6addr_realmlocal_allleasequeryservers;
extern const ipAddr_t in6addr_realmlocal_mcast_3ead;
extern const ipAddr_t in6addr_realmlocal_allmplforwarders;
extern const ipAddr_t in6addr_sitelocal_allnodes;
extern const ipAddr_t in6addr_sitelocal_allrouters;
extern const ipAddr_t in6addr_link_local_prefix;
extern const ipAddr_t in6addr_linklocal_allcoapnodes;
extern const ipAddr_t in6addr_realmlocal_allcoapnodes;
extern const ipAddr_t in6addr_adminlocal_allcoapnodes;
extern const ipAddr_t in6addr_sitelocal_allcoapnodes;
extern const uint32_t in4addr_any;
/* RAM global addresses - updated when the device join the network */
extern ipAddr_t in6addr_linklocal_allthreadnodes;
extern ipAddr_t in6addr_realmlocal_allthreadnodes;
extern ipAddr_t in6addr_realmlocal_threadleaderanycast;

extern const uint8_t gNwkPoolId;

/*! \brief Macro for memory buffer allocation
 *
 *  \param   [in]  a  Size of requested memory buffer */
#define NWKU_MEM_BufferAlloc(a) MEM_BufferAllocWithId(a,gNwkPoolId,(void*)__get_LR())

/*! \brief Macro for memory buffer allocation. The allocated memory buffer will never be freed
 *
 *  \param   [in]  a  Size of requested memory buffer */
#define NWKU_MEM_BufferAllocForever(a) MEM_BufferAllocForever(a, gNwkPoolId)

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     bool_t NWKU_SendMsg(nwkMsgHandler pFunc, void* pPload, taskMsgQueue_t* msgQueue)
\brief  Network Utils module function used to send a message between two tasks.

\param  [in]   pFunc     Pointer to message handler function
\param  [in]   pPload    Pointer to message data
\param  [in]   msgQueue  Pointer to structure holding message queue and task id to send message

\return        TRUE      If the message was sent successfully
\return        FALSE     If not
 ***************************************************************************************************/
bool_t NWKU_SendMsg(nwkMsgHandler pFunc, void* pPload, taskMsgQueue_t* msgQueue);

/*!*************************************************************************************************
\fn    void NWKU_RecvMsg(taskMsgQueue_t* msgQueue)
\brief Network Utils module function used to receive and handle a message in a task.

\param [in]   pMsgQueue  Pointer to structure holding message queue and task id to receive message
 ***************************************************************************************************/
void NWKU_RecvMsg(taskMsgQueue_t* pMsgQueue);

/*!*************************************************************************************************
\fn     bool_t NWKU_MsgHandler(taskMsgQueue_t* pMsgQueue)
\brief  Network Utils module function used to dequeue and handle a task message.

\param  [in]  pMsgQueue  Pointer to structure holding message queue and task id to receive message

\return       TRUE       If there was a message in the queue
\return       FALSE      Otherwise
 ***************************************************************************************************/
bool_t NWKU_MsgHandler(taskMsgQueue_t* pMsgQueue);

/*!*************************************************************************************************
\fn     ipAddr_t *NWKU_CreateIpAddr(void)
\brief  Network Utils module function used to create an ipAddr_t structure.

\return           Pointer to the allocated ipAddr_t structure
\return           NULL if memory cannot be allocated
 ***************************************************************************************************/
ipAddr_t *NWKU_CreateIpAddr(void);

/*!*************************************************************************************************
\fn     void NWKU_ConvertIp4Addr(uint32_t  ip4Addr, ipAddr_t* pOutIpAddr)
\brief  Network Utils module function used to convert an IPv4 address in uint32_t format to an
        ipAddr_t type address.

\param  [in]   ip4Addr       IPv4 address
\param  [out]  pOutIpAddr    Pointer to ipAddr_t to store the converted address
 ***************************************************************************************************/
void NWKU_ConvertIp4Addr(uint32_t  ip4Addr, ipAddr_t* pOutIpAddr);

/*!*************************************************************************************************
\fn    void NWKU_ConvertIp4AddrWellKnown(uint32_t  ip4Addr, ipAddr_t *pOutIpAddr)
\brief Network Utils module function used to convert an IPv4 address in uint32_t format to an
       ipAddr_t type address using NAT64 whell known prefix

\param [in]   ip4Addr       IPv4 address
\param [out]  pOutIpAddr    pointer to ipAddr_t to store the converted address
 ***************************************************************************************************/
void NWKU_ConvertIp4AddrWellKnown(uint32_t  ip4Addr, ipAddr_t *pOutIpAddr);

/*!*************************************************************************************************
\fn    void NWKU_SetSockAddrInfo(sockaddrStorage_t *pSockAddr, uint16_t addrFamily, uint16_t port,
                                    uint32_t flowinfo, ipIfUniqueId_t ifId)
\brief Network Utils module function used to populate sockaddrStorage_t fields.

\param [in]   pSockAddr       Pointer to sockaddrStorage_t structure
\param [in]   addrFamily      IP address family
\param [in]   port            UDP port
\param [in]   flowinfo        Used only for IPv6
\param [in]   ifId            PI interface id
 ***************************************************************************************************/
void NWKU_SetSockAddrInfo(sockaddrStorage_t *pSockAddr, ipAddr_t *pIpAddr, uint16_t addrFamily,
                                    uint16_t port, uint32_t flowinfo, ipIfUniqueId_t ifId);
/*!*************************************************************************************************
\fn    bool_t NWKU_CompareSockAddrStorage(sockaddrStorage_t *pSockAddr1, sockaddrStorage_t *pSockAddr2)
\brief Network Utils module function used to compare two sockaddrStorage_t structures. The two
structures are considered equal if either of them has IPv6 address in6addr_any and all other fields equal.

\param [in]   pSockAddr1       Pointer to first sockaddrStorage_t structure to compare.
\param [in]   pSockAddr2       Pointer to second sockaddrStorage_t structure to compare.
***************************************************************************************************/
bool_t NWKU_CompareSockAddrStorage(sockaddrStorage_t *pSockAddr1, sockaddrStorage_t *pSockAddr2);

/*!*************************************************************************************************
\fn    bool_t NWKU_CompareAddrAndPort(sockaddrStorage_t *pSockAddr1, sockaddrStorage_t *pSockAddr2)
\brief Network Utils module function used to compare two sockaddrStorage_t structures from the
perspective of IP address and port. The two structures are considered equal if either of them has
IPv6 address in6addr_any and all other fields equal.

\param [in]   pSockAddr1       Pointer to first sockaddrStorage_t structure to compare.
\param [in]   pSockAddr2       Pointer to second sockaddrStorage_t structure to compare.
***************************************************************************************************/
bool_t NWKU_CompareAddrAndPort(sockaddrStorage_t *pSockAddr1, sockaddrStorage_t *pSockAddr2);

/*!*************************************************************************************************
\fn     bool_t IP6_IsRealmLocalAddr(ipAddr_t* pIpAddr)
\brief  Network Utils module function used determine if an IPv6 address has realm local scope - valid
        only in the context of a THREAD stack.

\param  [in]  pIpAddr  IPv6 address

\return       TRUE     If address is realm local
\return       FALSE    If not or if not supported
 ***************************************************************************************************/
bool_t IP6_IsRealmLocalAddr(ipAddr_t* pIpAddr);

/*!*************************************************************************************************
\fn     ipPktInfo_t *NWKU_CreateIpPktInfo(void)
\brief  Network Utils module function used to create an ipPktInfo_t structure.

\return            Pointer to the allocated ipPktInfo_t structure
\return            NULL if memory cannot be allocated
 ***************************************************************************************************/
ipPktInfo_t *NWKU_CreateIpPktInfo(void);

/*!*************************************************************************************************
\fn     void NWKU_FreeIpPktInfo(ipPktInfo_t **pIpPktInfo)
\brief  Network Utils module function used to free one ipPktInfo_t structure.

\param  [in]   pIpPktInfo      Double pointer to the ipPktInfo_t structure
 ***************************************************************************************************/
void NWKU_FreeIpPktInfo(ipPktInfo_t **pIpPktInfo);

/*!*************************************************************************************************
\fn     nwkBuffer_t *NWKU_CreateNwkBuffer(uint32_t dataSize)
\brief  Network Utils module function used to create a nwkBuffer_t structure and allocate memory for
        data.

\param  [in]   dataSize     Size of the data available in the buffer

\return                     Pointer to the allocated nwkBuffer_t structure
\return                     NULL if memory cannot be allocated
 ***************************************************************************************************/
nwkBuffer_t *NWKU_CreateNwkBuffer(uint32_t dataSize);

/*!*************************************************************************************************
\fn     void NWKU_FreeAllNwkBuffers(nwkBuffer_t **pNwkBufferStart)
\brief  Network Utils module function used to free all nwkBuffer_t structures(starting with
        pNwkBufferStart) and change the start of the list to NULL.

\param  [in]   pNwkBufferStart    Double pointer to the start of data buffer
 ***************************************************************************************************/
void NWKU_FreeAllNwkBuffers(nwkBuffer_t **pNwkBufferStart);

/*!*************************************************************************************************
\fn     void NWKU_FreeNwkBufferElem(nwkBuffer_t **pNwkBufferStart, nwkBuffer_t *pElem)
\brief  Network Utils module function used to free one nwkBuffer_t element.

\param  [in]   pNwkBufferStart     Double pointer to the start of data buffer
\param  [in]   pElem               Pointer to the element to be freed
 ***************************************************************************************************/
void NWKU_FreeNwkBufferElem(nwkBuffer_t **pNwkBufferStart, nwkBuffer_t *pElem);

/*!*************************************************************************************************
\fn     uint32_t NWKU_NwkBufferTotalSize(nwkBuffer_t *pNwkBufferStart)
\brief  Network Utils module function used to calculate the total size of a nwkBuffer_t list,
        starting with pNwkBufferStart.

\param  [in]   pNwkBufferStart   Pointer to the start of nwkBuffer

\return                          Size of the whole list
 ***************************************************************************************************/
uint32_t NWKU_NwkBufferTotalSize(nwkBuffer_t *pNwkBufferStart);

/*!*************************************************************************************************
\fn     void NWKU_MemCopyFromNwkBuffer(nwkBuffer_t** pNwkBuffer, uint8_t** pSrcPtr, uint8_t* pDstPtr,
                                   uint32_t size)
\brief  Network Utils module function used to copy from a network fragmented buffer into a regular
        linear buffer.

\param  [in,out]  pNwkBuffer  Pointer to the start network buffer - pointer to end network buffer
\param  [in,out]  pSrcPtr     Pointer to the source date in the start network buffer - returns last
                              position in the end network buffer
\param  [in]      pDstPtr     Destination pointer
\param  [in]      size        Size to copy
 ***************************************************************************************************/
void NWKU_MemCopyFromNwkBuffer(nwkBuffer_t** pNwkBuffer,uint8_t** pSrcPtr, uint8_t* pDstPtr,
                               uint32_t size);

/*!*************************************************************************************************
\fn     void NWKU_NwkBufferAddOffset(nwkBuffer_t** pNwkBuffer, uint8_t** pSrcPtr, uint32_t size)
\brief  Network Utils module function used to add data into a buffer using an offset.

\param  [in,out]  pNwkBuffer  Pointer to the start network buffer - pointer to end network buffer
\param  [in,out]  pSrcPtr     Pointer to the source date in the start network buffer - returns last
                              position in the end network buffer
\param  [in]      size        Size to copy
 ***************************************************************************************************/
void NWKU_NwkBufferAddOffset(nwkBuffer_t** pNwkBuffer, uint8_t** pSrcPtr, uint32_t size);

/*!*************************************************************************************************
\fn     uint32_t NWKU_NwkBufferNumber(nwkBuffer_t *pNwkBufferStart)
\brief  Network Utils module function used to return the number of nwkBuffer_t fragments in the list

\param  [in]   pNwkBufferStart   Pointer to the start of data buffer

\return                          Number of nwkBuffer_t fragments in the list
 ***************************************************************************************************/
uint32_t NWKU_NwkBufferNumber(nwkBuffer_t *pNwkBufferStart);

/*!*************************************************************************************************
\fn     uint8_t * NWKU_NwkBufferToRegularBuffer(nwkBuffer_t *pNwkBufferStart, uint8_t *pRegularBuffer)
\brief  Network Utils module function used to transform a network fragmented buffer into a regular
        linear buffer.

\param  [in]   pNwkBufferStart   Pointer to the start of network buffer
\param  [in]   pRegularBuffer    Pointer to the provided Buffer, if null then allocate

\return                          Pointer to an allocated regular buffer that gets created
\return                          NULL if memory cannot be allocated
***************************************************************************************************/
uint8_t * NWKU_NwkBufferToRegularBuffer(nwkBuffer_t *pNwkBufferStart, uint8_t *pRegularBuffer);

/*!*************************************************************************************************
\fn     void NWKU_CreatePseudoHeader4(nwkBuffer_t *pNwkBuff, ipAddr_t *pSrcIp, ipAddr_t *pDstIp,
                                     uint32_t length, uint8_t nextHeader)
\brief  Network Utils module function used to create the pseudoheader for IPv4 protocols

\param  [out] pNwkBuff         Pointer to the nwkBuffer_t element containing the pseudoheader
\param  [in]  pSrcIp           Pointer to the source IP address
\param  [in]  pDstIp           Pointer to the destination IP address
\param  [in]  length           Length of the protocol(header + data)
\param  [in]  nextHeader       Value of the next header
 ***************************************************************************************************/
void NWKU_CreatePseudoHeader4(nwkBuffer_t *pNwkBuff, ipAddr_t *pSrcIp, ipAddr_t *pDstIp,
                              uint32_t length, uint8_t nextHeader);

/*!*************************************************************************************************
\fn     void NWKU_CreatePseudoHeader6(nwkBuffer_t *pNwkBuff, ipAddr_t *pSrcIp, ipAddr_t *pDstIp,
                                     uint32_t length, uint8_t nextHeader)
\brief  Network Utils module function used to create the pseudoheader for IPv6 protocols

\param  [out] pNwkBuff         Pointer to the nwkBuffer_t element containing the pseudoheader
\param  [in]  pSrcIp           Pointer to the source IP address
\param  [in]  pDstIp           Pointer to the destination IP address
\param  [in]  length           Length of the protocol(header + data)
\param  [in]  nextHeader       Value of the next header
 ***************************************************************************************************/
void NWKU_CreatePseudoHeader6(nwkBuffer_t *pNwkBuff, ipAddr_t *pSrcIp, ipAddr_t *pDstIp,
                              uint32_t length, uint8_t nextHeader);

/*!*************************************************************************************************
\fn     uint16_t NWKU_CalculateChecksum(nwkBuffer_t *pStart)
\brief  Network Utils module function used to calculate the checksum for a nwkBuffer_t list starting
        with pStart element

\param  [in]  pStart      Pointer to the start of the list

\return                   Checksum for the whole list
 ***************************************************************************************************/
uint16_t NWKU_CalculateChecksum(nwkBuffer_t *pStart);

/*!*************************************************************************************************
\fn     bool_t NWKU_CmpAddrPrefix6(uint8_t * addr1, uint8_t *addr2, uint32_t prefixLen)
\brief  Compares first "prefixLen" bits of the ipv6 addresses.

\param  [in]  addr1      First prefix to compare
\param  [in]  addr2      Second prefix to compare
\param  [in]  prefixLen  Length in bits to compare

\return       TRUE       If match
\return       FALSE      Otherwise
****************************************************************************************************/
bool_t NWKU_CmpAddrPrefix6(uint8_t * addr1, uint8_t *addr2, uint32_t prefixLen);

/*!*************************************************************************************************
\fn    bool_t NWKU_CmpAddr4(uint32_t destAddr, uint32_t netAddr, uint8_t prefixLen)
\brief Compare two IPv4 addresses using netMask

\param [in]  destAddr   destination address
\param [in]  netAddr    network address
\param [in]  prefixLen  network mask

\return      bool_t     TRUE if match
                        FALSE otherwise
***************************************************************************************************/
bool_t NWKU_CmpAddr4(uint32_t destAddr, uint32_t netAddr, uint8_t prefixLen);

/*!*************************************************************************************************
\fn    bool_t NWKU_MemCmpToVal(uint8_t *pAddr, uint8_t val, uint32_t len)
\brief Compare each octet of a given location to a value.

\param [in]     pAddr      location to be compared
\param [in]     val        reference value
\param [in]     len        length of location to be compared

\return         TRUE       If match
\return         FALSE      Otherwise
***************************************************************************************************/
bool_t NWKU_MemCmpToVal(uint8_t *pAddr, uint8_t val, uint32_t len);

/*!*************************************************************************************************
\fn     bool_t NWKU_BitCmp(uint8_t *pStr1, uint8_t *pStr2, uint8_t startBit, uint8_t stopBit)
\brief  Compare two strings bit by bit

\param  [in]   pStr1            The start address of the first string to be compared
\param  [in]   pStr2            The start address of the second string to be compared
\param  [in]   startBit         The start bit number in the the 2 strings
\param  [in]   stopBit          The stop bit number in the the 2 strings

\return        TRUE             If the strings match
\return        FALSE            If the strings don't match
***************************************************************************************************/
bool_t NWKU_BitCmp(uint8_t *pStr1, uint8_t *pStr2, uint8_t startBit, uint8_t stopBit);

/*!*************************************************************************************************
\fn     bool_t NWKU_IsLLAddrEqual(uint8_t *pFirstLlAddr, uint32_t firstLlAddrSize,
                                  uint8_t *pSecondLlAddr,uint32_t secondLlAddrSize)
\brief  Compare two Link Layer addresses

\param  [in]   pFirstLlAddr      The start address of the first address to be compared
\param  [in]   firstLlAddrSize   The size of the first address to be compared
\param  [in]   pSecondLlAddr     The start address of the second address to be compared
\param  [in]   secondLlAddrSize  The size of the second address to be compared

\return        TRUE              If the Link Layer addresses are the same
\return        FALSE             If the Link Layer addresses are different
***************************************************************************************************/
bool_t NWKU_IsLLAddrEqual(uint8_t *pFirstLlAddr, uint32_t firstLlAddrSize, uint8_t *pSecondLlAddr,
                                    uint32_t secondLlAddrSize);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetCommonPrefixLen6(ipAddr_t *addr1, ipAddr_t*addr2)
\brief  The common prefix length CommonPrefixLen(A, B) of two addresses A and B is the length of
        the longest prefix (looking at the most significant, or leftmost, bits) that the two
        addresses have in common.

\param  [in]  addr1      First prefix to compare
\param  [in]  addr2      Second prefix to compare

\return                  Longest prefix length in bits (0 - 128)
****************************************************************************************************/
uint32_t NWKU_GetCommonPrefixLen6(ipAddr_t *addr1, ipAddr_t*addr2);

/*!*************************************************************************************************
\fn     uint64_t NWKU_TransformArrayToValue(uint8_t* pArray, uint32_t nbOfBytes)
\brief  Converts an array to a numeric value.

\param  [in]   pArray          The start address of the array
\param  [in]   nbOfBytes       The length of the data to be converted

\return                        The value converted from the array
****************************************************************************************************/
uint64_t NWKU_TransformArrayToValue(uint8_t* pArray, uint32_t nbOfBytes);

/*!*************************************************************************************************
\fn     void NWKU_TransformValueToArray(uint64_t value, uint8_t* pArray, uint32_t nbOfBytes)
\brief  Converts a numeric value to array.

\param  [in]    value            The value to be converted
\param  [out]   pArray           The start address of the array
\param  [in]    nbOfBytes        The length of the data to be converted
****************************************************************************************************/
void NWKU_TransformValueToArray(uint64_t value, uint8_t* pArray, uint32_t nbOfBytes);

/*!*************************************************************************************************
\fn     uint16_t NWKU_Revert16(uint16_t value)
\brief  Reverts a 16 bit numeric value.

\param  [in]    value       The value to be converted

\return                     The converted value
 ***************************************************************************************************/
uint16_t NWKU_Revert16(uint16_t value);

/*!*************************************************************************************************
\fn     uint32_t NWKU_Revert32(uint32_t value)
\brief  Reverts a 32 bit numeric value.

\param  [in]    value      The value to be converted

\return                    The converted value
 ***************************************************************************************************/
uint32_t NWKU_Revert32(uint32_t value);

/*!*************************************************************************************************
\fn     uint64_t NWKU_Revert64(uint64_t value)
\brief  Reverts a 64 bit numeric value.

\param  [in]    value      The value to be converted

\return                    The converted value
****************************************************************************************************/
uint64_t NWKU_Revert64(uint64_t value);

/*!*************************************************************************************************
\fn     uint16_t NWKU_TransformArrayToUint16(uint8_t* pArray)
\brief  Converts an big endian array to a 16 bit numeric value.

\param  [in]    pArray      The start address of the array

\return                     The converted value
 ***************************************************************************************************/
uint16_t NWKU_TransformArrayToUint16(uint8_t* pArray);

/*!*************************************************************************************************
\fn     uint32_t NWKU_TransformArrayToUint32(uint8_t* pArray)
\brief  Converts an big endian array to a 32 bit numeric value.

\param  [in]    pArray     The start address of the array

\return                    The converted value
****************************************************************************************************/
uint32_t NWKU_TransformArrayToUint32(uint8_t* pArray);

/*!*************************************************************************************************
\fn     uint64_t NWKU_TransformArrayToUint64(uint8_t* pArray)
\brief  Converts an big endian array to a 64 bit numeric value.

\param  [in]    pArray      The start address of the array

\return                     The converted value
****************************************************************************************************/
uint64_t NWKU_TransformArrayToUint64(uint8_t* pArray);

/*!*************************************************************************************************
\fn     void NWKU_TransformUint16ToArray(uint8_t* pArray, uint16_t value)
\brief  Converts a 16 bit numeric value to array.

\param  [in]    value            The value to be converted
\param  [out]   pArray           The start address of the array
 ***************************************************************************************************/
void NWKU_TransformUint16ToArray(uint8_t* pArray, uint16_t value);

/*!*************************************************************************************************
\fn     void NWKU_TransformUint32ToArray(uint8_t* pArray, uint32_t value)
\brief  Converts a 32 bit numeric value to array.

\param  [in]    value            The value to be converted
\param  [out]   pArray           The start address of the array
****************************************************************************************************/
void NWKU_TransformUint32ToArray(uint8_t* pArray, uint32_t value);

/*!*************************************************************************************************
\fn     void NWKU_TransformUint64ToArray(uint8_t* pArray, uint64_t value)
\brief  Converts a 64 bit numeric value to array.

\param  [in]    value            The value to be converted
\param  [out]   pArray           The start address of the array
****************************************************************************************************/
void NWKU_TransformUint64ToArray(uint8_t* pArray, uint64_t value);

/*!*************************************************************************************************
\fn     bool_t NWKU_GetLut8(lut8_t* pLutTable, uint8_t lutTableSize, uint8_t type,
                           uint8_t* pEntryIndex)
\brief  Searches an entry in the lookup table indicated by pLutTable.

\param  [in]    pLutTable     Pointer to the lookup table
\param  [in]    lutTableSize  Lookup table size
\param  [in]    type          Type to find

\param  [out]   pEntryIndex   Index of the entry in case the entry is found

\return         TRUE          Returned when the entry is found
\return         FALSE         Otherwise
 ***************************************************************************************************/
bool_t NWKU_GetLut8(lut8_t* pLutTable, uint8_t lutTableSize, uint8_t type, uint8_t* pEntryIndex);

/*!*************************************************************************************************
\fn     int32_t NWKU_atoi(char *str)
\brief  Converts a string into an integer.

\param  [in]    pStr       Pointer to string

\return                    Integer converted from string.
***************************************************************************************************/
int32_t NWKU_atoi(char *pStr);

/*!*************************************************************************************************
\fn    int64_t NWKU_atol(char *str)
\brief  Converts a string into an long integer.

\param [in]    pStr       pointer to string

\retval     int64_t       integer converted from string.
***************************************************************************************************/
int64_t NWKU_atol(char *pStr);

/*!*************************************************************************************************
\fn     void NWKU_PrintDec(uint64_t value, uint8_t* pString, uint32_t nbOfDigits, bool_t bLeadingZeros)
\brief  Prints in a string decimal values.

\param  [in]         value           Integer value
\param  [in/out]     pString         Pointer to output location
\param  [in]         nbPrintDigits   Number of digits to be printed
\param  [in]         bLeadingZeros   Indicate if leading zeros are put or omitted
                                     TRUE - print leading zeros
                                     FALSE - do not print leading zeros
***************************************************************************************************/
void NWKU_PrintDec(uint64_t value, uint8_t* pString, uint32_t nbPrintDigits, bool_t bLeadingZeros);

/*!*************************************************************************************************
\fn     int32_t pton(uint8_t af, char *pTxt, ipAddr_t *pIpAddr)
\brief  Converts a string into an ipAddr_t. Presentation to network function.

\param  [in]    af       Address family(AF_INET, AF_INET6)
\param  [in]    pTxt     Pointer to the start of the string to be parsed
\param  [in]    pIpAddr  Pointer to the start of the allocated ipAddr_t structure

\return                  1 on success
\return                  0 string address is not valid
\return                  -1 on error
***************************************************************************************************/
int32_t pton(uint8_t af, char *pTxt, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     char *ntop(uint8_t af, ipAddr_t *pIpAddr, char *pStr, uint32_t strLen)
\brief  Converts an ipAddr_t into a string. Network to presentation function.

\param  [in]    af       Address family(AF_INET, AF_INET6)
\param  [in]    pIpAddr  Pointer to the start of the allocated ipAddr_t structure
\param  [out]   pStr     Pointer to the allocated string where to put the result
\param  [in]    strLen   Size of the input buffer

\return                  Pointer to the resulted buffer
***************************************************************************************************/
char *ntop(uint8_t af, ipAddr_t *pIpAddr, char *pStr, uint32_t strLen);

/*!*************************************************************************************************
\fn    ptoll(uint8_t *pIn, uint32_t len, llAddr_t *pLlAddr)
\brief Converts a string into an llAddr_t. Presentation to ll function.

\param [in]    pIn      Pointer to the input buffer
\param [in]    len      Size of the input buffer
\param [in]    pLlAddr  Pointer to the start of the allocated llAddr_t structure

\return        TRUE     On success
\return        FALSE    On error
***************************************************************************************************/
bool_t ptoll(uint8_t *pIn, uint32_t len, llAddr_t *pLlAddr);

/*!*************************************************************************************************
\fn     uint32_t NWKU_AsciiToHex(uint8_t* pString, uint32_t strLen)
\brief  Converts a string into hex.

\param  [in]    pString     Pointer to string
\param  [in]    strLen      String length

\return                     Value in hex
***************************************************************************************************/
uint32_t NWKU_AsciiToHex(uint8_t* pString, uint32_t strLen);

/*!*************************************************************************************************
\fn     uint32_t NWKU_AsciiToDec(uint8_t* pString, uint32_t strLen)
\brief  Converts a string into hex.

\param  [in]    pString     Pointer to string
\param  [in]    strLen      String length

\return                     Value in decimal
***************************************************************************************************/
uint32_t NWKU_AsciiToDec(uint8_t* pString, uint32_t strLen);

/*!*************************************************************************************************
\fn     uint8_t NWKU_ByteToDec(uint8_t byte)
\brief  Converts a byte from ASCII to decimal.

\param  [in]    byte     Byte value in ASCII

\return                  Value in decimal
***************************************************************************************************/
uint8_t NWKU_ByteToDec(uint8_t byte);

/*!*************************************************************************************************
\fn     uint8_t NWKU_NibToAscii(int8_t nib, bool_t useUpperCase)
\brief  Converts a nib from hex to ASCII.

\param  [in]    nib            Nib value in hex
\param  [in]    useUpperCase   Flag to specify if conversion is to ASCII uppercase

\return                        Value in ASCII
***************************************************************************************************/
uint8_t NWKU_NibToAscii(int8_t nib, bool_t useUpperCase);

/*!*************************************************************************************************
\fn     void NWKU_HexToAscii(uint8_t* pInputBuff,uint32_t inputBuffLen,uint8_t* pOutputBuffer,
                             uint32_t outputBuffLen, bool_t useUpperCase)
\brief  Converts a byte to ASCII.

\param  [in]    pInputBuff      Pointer to input buffer
\param  [in]    inputBuffLen    Length of the input buffer
\param  [in]    pOutputBuffer   Pointer to output buffer
\param  [in]    outputBuffLen   Length of the output buffer
\param  [in]    useUpperCase    Indicate if the output shall be in upper/lower case
***************************************************************************************************/
void NWKU_HexToAscii(uint8_t* pInputBuff,uint32_t inputBuffLen,uint8_t* pOutputBuffer,
                                uint32_t outputBuffLen, bool_t useUpperCase);

/*!*************************************************************************************************
\fn     uint32_t NWKU_TmrRtcGetElapsedTimeInSeconds(uint32_t timestamp)
\brief  Calculates the time passed in seconds from the provided timestamp.

\param  [in]    timestamp     Timestamp in seconds

\return                       Number of seconds that have passed since the provided timestamp
 ***************************************************************************************************/
uint32_t NWKU_TmrRtcGetElapsedTimeInSeconds(uint32_t timestamp);

/*!*************************************************************************************************
\fn     bool_t NWKU_IsNUmber(char *pString)
\brief  Check if a string is a number.

\param  [in]   pString     Pointer to the string

\return        TRUE        If the string represents a number
\return        FALSE       If the string does not represent a number
***************************************************************************************************/
bool_t NWKU_IsNUmber(char *pString);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetRandomNoFromInterval(uint32_t startInterval, uint32_t endInterval)
\brief  This function returns a random number from a given interval.

\param  [in]  startInterval   Start value of the interval
\param  [in]  endInterval     End value of the interval

\return                       Random value
***************************************************************************************************/
uint32_t NWKU_GetRandomNoFromInterval(uint32_t startInterval, uint32_t endInterval);

/*!*************************************************************************************************
\fn     void NWKU_IncrementIp6Addr(ipAddr_t* pIpAddr)
\brief  This function increments a IPv6 type address

\param  [in]  pIpAddr   Pointer to IPv6 address
***************************************************************************************************/
void NWKU_IncrementIp6Addr(ipAddr_t* pIpAddr);

/*!*************************************************************************************************
\fn     uint32_t NWKU_RightRotate(uint32_t val, uint8_t amount)
\brief  This function rotates a 32bit number to the right with an amount of bits.

\param  [in]  val       Number
\param  [in]  amount    Number of bits to rotate

\return                 Result of the rotation
***************************************************************************************************/
uint32_t NWKU_RightRotate(uint32_t val, uint8_t amount);

/*!*************************************************************************************************
\fn     void NWKU_GetIIDFromLLADDR(llAddr_t * pLlAddr, uint16_t panId, uint8_t * pIID)
\brief  The function returns the IID from a Link-Layer address.

\param  [in]    pLlAddr         Pointer to the Link-Layer address
\param  [in]    panId           PAN ID
\param  [out]   pIID            Pointer to the variable which will hold the IID
***************************************************************************************************/
void NWKU_GetIIDFromLLADDR(llAddr_t* llAddr, uint16_t panId, uint8_t * pIID);

/*!*************************************************************************************************
\fn     void NWKU_GetLLAddrFromIID(uint8_t * pIID, llAddr_t * pLlAddr)
\brief  This function returns the Link-Layer address from the IID.

\param  [in]    pIID            Pointer to the IID
\param  [out]   pLlAddr         Pointer to the variable which will hold the Link-Layer address
***************************************************************************************************/
void NWKU_GetLLAddrFromIID(uint8_t * pIID, llAddr_t * pLlAddr);

/*!*************************************************************************************************
\fn     void bool_t NWKU_IsIPAddrBasedOnShort(ipAddr_t* pIpAddr)
\brief  This function returns true if the IPv6 address is formed with short MAC address.

\param  [in]   pIpAddr     Pointer to the IPv6 address

\return        TRUE        If address is based on short MAC address
\return        FALSE       Otherwise.
***************************************************************************************************/
bool_t NWKU_IsIPAddrBasedOnShort(ipAddr_t* pIpAddr);

/*!*************************************************************************************************
\fn     bool_t NWKU_GetBit(uint32_t bitNr, uint8_t* pArray)
\brief  This function returns the value of a bit in an array.

\param  [in]    bitNr           Bit number in the whole array
\param  [in]    pArray          Pointer to the start of the array

\return         TRUE            If the bit is set
\return         FALSE           If the bit is not set
***************************************************************************************************/
bool_t NWKU_GetBit(uint32_t bitNr, uint8_t* pArray);

/*!*************************************************************************************************
\fn     void NWKU_SetBit(uint32_t bitNr, uint8_t* pArray)
\brief  This function sets a bit in an array.

\param  [in]    bitNr           Bit number in the whole array
\param  [in]    pArray          Pointer to the start of the array
***************************************************************************************************/
void NWKU_SetBit(uint32_t bitNr, uint8_t* pArray);

/*!*************************************************************************************************
\fn     void NWKU_ClearBit(uint32_t bitNr, uint8_t* pArray)
\brief  This function clears a bit in an array.

\param  [in]    bitNr           Bit number in the whole array
\param  [in]    pArray          Pointer to the start of the array
***************************************************************************************************/
void NWKU_ClearBit(uint32_t bitNr, uint8_t* pArray);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetFirstBitValueInRange(uint8_t* pArray, uint32_t lowBitNr, uint32_t
                                              highBitNr, bool_t bitValue)
\brief  This function returns the first bit with value=bitValue in a range in the array.

\param  [in]    pArray          Pointer to the start of the array
\param  [in]    lowBitNr        Starting bit number
\param  [in]    highBitNr       Ending bit number
\param  [in]    bitValue        Bit value

\return         uint32_t        Bit number
***************************************************************************************************/
uint32_t NWKU_GetFirstBitValueInRange(uint8_t* pArray, uint32_t lowBitNr, uint32_t highBitNr, bool_t bitValue);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetFirstBitValue(uint8_t* pArray, uint32_t arrayBytes, bool_t bitValue)
\brief  This function returns the index of the first bit with value=bitValue.

\param  [in]    pArray          Pointer to the start of the array
\param  [in]    arrayBytes      Number of bytes in the array
\param  [in]    bitValue        Bit value

\return                         Bit value
***************************************************************************************************/
uint32_t NWKU_GetFirstBitValue(uint8_t* pArray, uint32_t arrayBytes, bool_t bitValue);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetNumOfBits(uint8_t* pArray, uint32_t arrayBytes, bool_t bitValue)
\brief  This function returns number of bits of value bitValue from an array

\param  [in]    pArray          Pointer to the start of the array
\param  [in]    arrayBytes      Number of bytes in the array
\param  [in]    bitValue        Bit value

\return                         Bit value
***************************************************************************************************/
uint32_t NWKU_GetNumOfBits(uint8_t* pArray, uint32_t arrayBytes, bool_t bitValue);

/*!*************************************************************************************************
\fn     uint32_t NWKU_ReverseBits(uint32_t num)
\brief  Reverse bits

\param  [in]  num       The bits to reverse

\return                 The reversed bits
***************************************************************************************************/
uint32_t NWKU_ReverseBits(uint32_t num);

/*!*************************************************************************************************
\fn     uint32_t NWKU_AddTblEntry(uint32_t entry, uint32_t *pTable, uint32_t tableSize)
\brief  This function adds a new entry in a table. The table needs to have uint32_t elements.

\param  [in]    entry       Entry value
\param  [in]    pTable      Pointer to the start of the table
\param  [in]    tableSize   The size of the table

\return                     Entry index or -1(0xFFFFFFFF) in case of error
***************************************************************************************************/
uint32_t NWKU_AddTblEntry(uint32_t entry, uint32_t *pTable, uint32_t tableSize);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetTblEntry(uint32_t entry, uint32_t *pTable, uint32_t tableSize)
\brief  This function search for an element in a table.

\param  [in]    entry       Entry value
\param  [in]    pTable      Pointer to the start of the table
\param  [in]    tableSize   The size of the table

\return                     Entry index or NULL in case of error
***************************************************************************************************/
uint32_t NWKU_GetTblEntry(uint32_t index, uint32_t *pTable, uint32_t tableSize);

/*!*************************************************************************************************
\fn     void NWKU_SwapArrayBytes(uint8_t* pByte, uint8_t numOfBytes)
\brief  This function swaps the bytes in an array and puts the result in the same array.

\param  [in/out]    pByte        Pointer to the start of the array
\param  [in]        numOfBytes   Size of the array
***************************************************************************************************/
void NWKU_SwapArrayBytes(uint8_t* pByte, uint8_t numOfBytes);

/*!*************************************************************************************************
\fn     void NWKU_GenRand(uint8_t *pRand, uint8_t randLen)
\brief  This function generates a random value in the desired array.

\param  [out]   pRand     Pointer to the start of the output array
\param  [in]    randLen   Size of the array
***************************************************************************************************/
void NWKU_GenRand(uint8_t *pRand, uint8_t randLen);

/*!*************************************************************************************************
\fn     uint32_t NWKU_GetTlvLen(uint8_t type, uint8_t *pStart, uint32_t len)
\brief  This function returns the length of the TLV type specified.

\param  [in]   type      Type identifier for the TLV
\param  [in]   pStart    Pointer to the start if the TLVs
\param  [in]   len       Size of the TLVs buffer

\return                  Length of the specified TLV type
***************************************************************************************************/
uint32_t NWKU_GetTlvLen(uint8_t type, uint8_t *pStart, uint32_t len);

/*!*************************************************************************************************
\fn     uint8_t *NWKU_GetTlvValue(uint8_t type, uint8_t *pStart, uint32_t len, uint8_t *pOut)
\brief  This function returns the value of a requested TLV in a list of TLVs. The pointer to the
        value(if found) will be returned and copied in pOut buffer(if pOut is not NULL).

\param  [in]    type      Type identifier for the TLV
\param  [in]    pStart    Pointer to the start if the TLVs
\param  [in]    len       Size of the TLVs buffer
\param  [in]    pOut      Pointer to the output preallocated buffer or NULL

\return                   Pointer to the value of the requested TLV
***************************************************************************************************/
uint8_t *NWKU_GetTlvValue(uint8_t type, uint8_t *pStart, uint32_t len, uint8_t *pOut);

/*!*************************************************************************************************
\fn     uint8_t *NWKU_GetTlv(uint8_t type, uint8_t *pStart, uint32_t len, uint8_t *ppOut,
                             uint32_t outBufLen)
\brief  This function returns the start address of the TLV in the pStart buffer.

\param  [in]    type         Type identifier for the TLV
\param  [in]    pStart       Pointer to the start if the TLVs
\param  [in]    len          Size of the TLVs buffer
\param  [out]   ppOut        If this buffer is provided, the found TLVs will be copied inside and the
                             pointer updated with the copied value
\param  [out]   outBufLen    Length of the output buffer (used for bounds checking)

\return                     Pointer to the TLV
\return                     NULL if the requested TLV was not found
***************************************************************************************************/
uint8_t *NWKU_GetTlv(uint8_t type, uint8_t *pStart, uint32_t len, uint8_t **ppOut,
                     uint32_t outBufLen);

/*!*************************************************************************************************
\fn     bool_t NWKU_Pbkdf2(pbkdf2Params_t *pInput, uint8_t *pOut, uint32_t outLen)
\brief  This function calculates pbkdf2 for an input.

\param  [in]    pInput  Structure containing the input parameters
                NOTE: - pInput->pSalt should include the "salt" plus 4 bytes more at the end
                      - pInput->pSalt should specify the "salt" length without the above 4 bytes
\param  [in]    pOut    Pointer to the output
\param  [in]    outLen  Size of the output buffer

\return         TRUE    If the operation has succeeded
\return         FALSE   If the operation hasn't succeeded
***************************************************************************************************/
bool_t NWKU_Pbkdf2(pbkdf2Params_t *pInput, uint8_t *pOut, uint32_t outLen);

/*!*************************************************************************************************
\fn     uint64_t NWKU_GetTimestampMs(void)
\brief  Get the timestamp in milliseconds.

\return                Timestamp in milliseconds
***************************************************************************************************/
uint64_t NWKU_GetTimestampMs(void);

/*!*************************************************************************************************
\fn     int8_t NWKU_isArrayGreater(const uint8_t *a, const uint8_t *b, uint8_t length)
\brief  Compare two numbers represented as array.

\param  [in]  a        First array
\param  [in]  b        Second array
\param  [in]  length   How many bytes to compare

\return                0 - are equal
\return                1 - a > b
\return                -1 - b < a
***************************************************************************************************/
int8_t NWKU_isArrayGreater(const uint8_t *a, const uint8_t *b, uint8_t length);

/*!*************************************************************************************************
\fn     nwkSeqNbStatus_t NWKU_IsSeqNbHigher(uint8_t oldSeqNb, uint8_t newSeqNb)
\brief  Uses Serial number arithmetic to compare two 8 bit sequence numbers

\param  [in] oldSeqNb      the current sequence number value
\param  [in] newSeqNb      the new sequence number value

\return gNwkSeqNbLower_c  newSeqNb is lower than oldSeqNb
\return gNwkSeqNbEqual_c  newSeqNb is equal to oldSeqNb
\return gNwkSeqNbHigher_c newSeqNb is higher than oldSeqNb
***************************************************************************************************/
nwkSeqNbStatus_t NWKU_IsSeqNbHigher(uint8_t oldSeqNb, uint8_t newSeqNb);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _NETWORK_UTILS_H */
