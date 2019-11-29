/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP4_H_
#define _IP4_H_

/*!
\file       ip4.h
\brief      This is a header file for the IPv4 layer.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"
#include "ip_if_management.h"

#ifndef IP_IP4_STATS_ENABLE
    #define IP_IP4_STATS_ENABLE 0           /*!< IPv4 statistics enable */
#endif

#ifndef IP_IP4_ENABLE_FRAG
    #define IP_IP4_ENABLE_FRAG 0            /*!< IPv4 fragmentation enable */
#endif

#ifndef IP_IP4_ENABLE_REASSEMBLY
    #define IP_IP4_ENABLE_REASSEMBLY    (0 && IP_ENABLE_REASSEMBLY)   /*!< IPv4 reassembly enable */
#endif

#if IP_IP4_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/

#define IP4_HEADER_VERSION          0x40       /*!< IPv4 header version */
#define IP4_HEADER_VERSION_MASK     0xf0       /*!< IPv4 header version mask */

#define IP4_HDR_FRAG_OFFSET_MASK    0x1FFF     /*!< IPv4 header fragmentation offset mask */
#define IP4_HDR_FRAG_MF_FLAG_MASK   0x2000     /*!< IPv4 header fragmentation MF flag mask */

/*! Macro used to create an IPv4 address */
#define IPV4_ADDR(a, b, c, d)       (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

#define IP4_STATIC_ROUTE_USER_MAX_VALUE     9    /*!< IPv6 static route user maximum value */
#define IP4_STATIC_ROUTE_DEFAULT_METRIC     4    /*!< IPv6 static route default metric */

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! IPv4 packet header */
typedef struct ip4Header_tag
{
   uint8_t    versLen;      /*!< hi-nybble=Version, lo-nybble=header len/4 */
   uint8_t    tos;          /*!< Type of service (see TOS_... #define's) */
   uint8_t    length[2];    /*!< Length of packet (header+data) in bytes */

   uint8_t    id[2];        /*!< Packet identification */
   uint8_t    fragment[2];  /*!< Fragment offset & flags */

   uint8_t    ttl;          /*!< Time to live, in seconds or hops */
   uint8_t    protocol;     /*!< Protocol */
   uint8_t    checksum[2];  /*!< IP_checksum */

   uint8_t    srcAddr[4];   /*!< Sender of packet */
   uint8_t    dstAddr[4];   /*!< Destination of packet */

} ip4Header_t;

/*! IPv4 Unicast Routing Table Entry structure */
typedef struct ip4RoutingTblEntry_tag
{
    /*! Destination IPv4 address or prefix of network */
    uint32_t dstPrefix;
    /*! Next hop router to destination */
    uint32_t nextHopRouter;
    /*! Next hop interface to destination */
    void*  nextHopIf;
    /*! Length of IPv4 prefix */
    uint8_t prefixLen;
    /*! Route metric */
    uint8_t routeMetric;
    /* TRUE if this entry is used */
    bool_t isEntryUsed;
} ip4RoutingTblEntry_t;

/*! The base metric value for each type of route */
typedef enum
{
    gIp4RouteBaseStaticMetric_c = 00U    /*!< Static metric */
} ip4RoutingMetric_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void IP4_Init(void)
\brief  Initialize function for IPv4.
***************************************************************************************************/
void IP4_Init(void);

/*!*************************************************************************************************
\fn     void IP4_Service(uint8_t *pInData)
\brief  Parses a received IP4 packet.

\param  [in]  pInData  The received packet
***************************************************************************************************/
void IP4_Service(uint8_t *pInData);

/*!*************************************************************************************************
\fn     uint32_t IP4_Send(ipPktInfo_t *pIpPktInfo)
\brief  Sends an IP4 packet generated on the local host.

\param  [in]  pIpPktInfo  The packet to send

\return       uint32_t    Error
***************************************************************************************************/
uint32_t IP4_Send(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     uint32_t IP4_GetNetMask(uint32_t ipAddr)
\brief  Gets the net mask of the provided IPv4 address by identifying the IP class of the address.

\param  [in]  ipAddr    The IP address

\return       uint32_t  IP address net mask
***************************************************************************************************/
uint32_t IP4_GetNetMask(uint32_t ipAddr);

/*!*************************************************************************************************
\fn     bool_t IP4_AddrIsDirectedBcast(uint32_t ipAddr, ip4IfAddrData_t *ipAddrData)
\brief  Verifies if an IPv4 address is a directed broadcast for a specific address and subnet mask
        combination.

\param  [in]  ipAddr      The IP address
\param  [in]  ipAddrData  Structure containing IP address data

\return       bool_t      TRUE if address is directed broadcast, FALSE otherwise
***************************************************************************************************/
bool_t IP4_AddrIsDirectedBcast(uint32_t ipAddr, ip4IfAddrData_t *ipAddrData);

/*!*************************************************************************************************
\public
\fn     bool_t IPv4_IsFragment
\brief  This function checks if the IPv4 Packet is a fragment

\param  [in]  pIpPktInfo  Pointer to IPv4 packet

\return       bool_t      TRUE if the packet is a fragment
***************************************************************************************************/
bool_t IPv4_IsFragment(ipPktInfo_t *pIpPktInfo);

#if IP_IP4_ROUTING_ENABLE

/*!*************************************************************************************************
\fn     void IP4_RemoveRoute(ipAddr_t *remAddr, ipAddr_t *nextHopAddr)
\brief  Removes a route in the IP4 routing table.

\param  [in]  remAddr      The address to remove from the routing table
\param  [in]  nextHopAddr  The next hop address of the route to remove
***************************************************************************************************/
void IP4_RemoveRoute (uint32_t remAddr, uint32_t nextHopAddr);

/*!*************************************************************************************************
\fn     void IP4_SetStaticRoute(uint32_t destAddr, uint8_t prefixLen, uint8_t routeMetric,
                                uint32_t nextHopAddr, ipIfUniqueId_t ifId)
\brief  Sets a static route in the IP6 routing table.

\param  [in]  destAddr      The address to set in the routing table
\param  [in]  prefixLen     Prefix length of the address to set in the routing table
\param  [in]  routeMetric   Metric value of the route between 0 and IP4_STATIC_ROUTE_USER_MAX_VALUE
                            - this value is added to static route metric base value
\param  [in]  nextHopAddr   The address of the next hop router to destination
\param  [in]  ifId          Interface unique ID
***************************************************************************************************/
void IP4_SetStaticRoute(uint32_t destAddr, uint8_t prefixLen, uint8_t routeMetric,
                        uint32_t nextHopAddr, ipIfUniqueId_t ifId);

/*!*************************************************************************************************
\fn     ip4RoutingTblEntry_t *IP4_SearchDestination(uint32_t destAddr)
\brief  Searches the IP4 Routing Table for the destination address.

\param  [in]  destAddr                 The destination address to search for

\return       ip4RoutingTblEntry_t *   Pointer to found route table entry
***************************************************************************************************/
ip4RoutingTblEntry_t *IP4_SearchDestination (uint32_t destAddr);

/*!*************************************************************************************************
\fn     uint32_t    IP4_HandleUcastForwarding(ipPktInfo_t *pIpPktInfo, uint8_t direction,
                                              ip4RoutingTblEntry_t *dstRoute);
\brief  Handles all common task for IP forwarding.

\param  [in]  pIpPktInfo  The packet to send
\param  [in]  direction   TX or RX
\param  [in]  dstRoute    Destination route

\return       uint32_t*   Pointer to next hop address or NULL
***************************************************************************************************/
uint32_t *IP4_HandleUcastForwarding(ipPktInfo_t *pIpPktInfo, uint8_t direction,
                                    ip4RoutingTblEntry_t *dstRoute);

/*!*************************************************************************************************
\fn     uint32_t *IP4_UnicastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction)
\brief  Forwards an IP4 packet to the destination interface.

\param  [in]  pIpPktInfo  The packet to send
\param  [in]  direction   Forwarding direction: TX or RX

\return       uint32_t *  Pointer to next hop address or NULL
***************************************************************************************************/
uint32_t *IP4_UnicastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction);
#endif

#if IP_IP4_STATS_ENABLE
/*!*************************************************************************************************
\fn      ip4Stats_t *IP4_GetIPStats(void)
\brief   Returns structure of IPv4 statistics.

\return  ip4Stats_t *  Pointer to IPv4 statistics structure
***************************************************************************************************/
ip4Stats_t *IP4_GetIPStats(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* IP_IP4_ENABLE */

/*================================================================================================*/
#endif  /*_IP4_H_ */
