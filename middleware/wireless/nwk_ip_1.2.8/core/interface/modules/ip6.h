/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP6_H_
#define _IP6_H_

/*!
\file       ip6.h
\brief      This is a header file for the IPv6 layer.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"
//#include "ip_if_management.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#ifndef IP_IP6_STATS_ENABLE
#define IP_IP6_STATS_ENABLE 0        /*!< Define for IPv6 statistics enable */
#endif

#ifndef IP_IP6_ROUTING_ENABLE
#define IP_IP6_ROUTING_ENABLE 0     /*!< Define for IPv6 routing enable */
#endif

#ifndef IP_IP6_MCAST_FW_ENABLE
#define IP_IP6_MCAST_FW_ENABLE 0     /*!< Define for IPv6 multicast forwarding enable */
#endif

#ifndef IP_IP6_ENABLE_FRAG
#define IP_IP6_ENABLE_FRAG 0       /*!< Define for IPv6 fragmentation enable */
#endif

#ifndef IP_IP6_LOOPBACK_MULTICAST
#define IP_IP6_LOOPBACK_MULTICAST 0    /*!< Define for IPv6 loopback multicast */
#endif

#ifndef IP_IP6_LOOPBACK
#define IP_IP6_LOOPBACK 0              /*!< Define for IPv6 loopback */
#endif

#ifndef IP_IP6_ENABLE_REASSEMBLY
#define IP_IP6_ENABLE_REASSEMBLY    (1 && IP_ENABLE_REASSEMBLY)   /*!< IPv6 reassembly enable */
#endif

#ifndef IP_IP6_FIREWALL_ENABLE
#define IP_IP6_FIREWALL_ENABLE 0     /*!< Define for IPv6 firewall enable */
#endif

/* TRUE == accept all and FALSE == reject all */
#ifndef IP_IP6_FIREWALL_DEFAULT_POLICY
#define IP_IP6_FIREWALL_DEFAULT_POLICY TRUE    /*!< Define for IPv6 firewall default policy */
#endif

#if IP_IP6_ENABLE
#define IP6_HDR_LEN             (40U)     /*!< IPv6 Header size */
#define IP6_DEFAULT_HOPLIMIT    64        /*!< Default Hop Limit */

#define IP6_HEADER_VERSION      0x60      /*!< IPv6 header version */
#define IP6_HEADER_VERSION_MASK 0xf0      /*!< IPv6 header version mask */

#define IP6_PAD1_OPTION         (0x00)    /*!< IPv6 padding1 option */
#define IP6_PADN_OPTION         (0x01)    /*!< IPv6 paddingN option */
#define IP6_MPL_OPTION          (0x6D)    /*!< IPv6 MPL option */
#define IP6_MLD_OPTION          (0x00)    /*!< IPv6 MPL option */

/*! IPv6 option type unrecognized mask */
#define IP6_OPTION_TYPE_UNRECOGNIZED_MASK           (0xC0)
/*! IPv6 skip unrecognized option type */
#define IP6_SKIP_UNRECOGNIZED_OPTION_TYPE           (0x00)
/*! IPv6 discard unrecognized option type */
#define IP6_DISCARD_UNRECOGNIZED_OPTION_TYPE        (0x40)
/*! IPv6 discard ICMP unrecognized option type */
#define IP6_DISCARD_ICMP_UNRECOGNIZED_OPTION_TYPE   (0x80)
/*! IPv6 discard UICMP unrecognized option type */
#define IP6_DISCARD_UICMP_UNRECOGNIZED_OPTION_TYPE  (0xC0)

#define IP6_FRAGMENT_MF             0x1   /*!< IPv6 fragment MF. If 1, this is not last fragment */
#define IP6_FRAGMENT_OFFSET_SHIFT   3U    /*!< IPv6 fragment offset shift */

#define ADDR_TYPE_MASK      0xF0U  /*!< Address type mask */
#define ADDR_TYPE_OFFSET    4U     /*!< Address type offset */
#define ADDR_STATE_MASK     0x0FU  /*!< Address state mask */
#define ADDR_STATE_OFFSET   0U     /*!< Address state offset */

/*! Macro for address type set */
#define ADDR_TYPE_SET(a, b)  ((a) = (((a) & (~ADDR_TYPE_MASK)) | ((b) << ADDR_TYPE_OFFSET)))
/*! Macro for address type get */
#define ADDR_TYPE_GET(a)     (((a) & ADDR_TYPE_MASK) >> ADDR_TYPE_OFFSET)
/*! Macro for address state set */
#define ADDR_STATE_SET(a, b)  ((a) = (((a) & (~ADDR_STATE_MASK)) | ((b) << ADDR_STATE_OFFSET )))
/*! Macro for address state get */
#define ADDR_STATE_GET(a)     (((a) & ADDR_STATE_MASK) >> ADDR_STATE_OFFSET)

#define IP6_STATIC_ROUTE_USER_MAX_VALUE     9    /*!< IPv6 static route user maximum value */
#define IP6_STATIC_ROUTE_DEFAULT_METRIC     4    /*!< IPv6 static route default metric */

/*! IPv6 static route with MAX(0x00) metric. This route cannot be modified and can only be removed */
#define IP6_STATIC_ROUTE_SUPER_METRIC         0x00

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! IPv6 address states */
typedef enum
{
    ip6AddrStateNotUsed_c = 0x00,    /*!< Not used */
    ip6AddrStateTentative_c,         /*!< Tentative */
    ip6AddrStatePreferred_c          /*!< Preferred */
} ip6AddrState_t;

/*! IPv6 routing types */
typedef enum
{
    gIp6RouteStatic_c = 0x00U,    /*!< Static */
    gIp6RouteRIPng_c = 0x01U      /*!< RIPng */
}ip6RoutingTypes_t;

/*! IPv6 firewall rules */
typedef enum
{
    gIp6FirewallReject_c = 0x00U,    /*!< Firewall reject */
    gIp6FirewallAccept_c = 0x01U     /*!< Firewall accept */
}ip6FirewallRule_t;

/*! The base metric value for each type of route */
typedef enum
{
    gIp6RouteBaseStaticMetric_c = 00U,    /*!< Static metric */
    gIp6RouteBaseRIPngMetric_c = 50U      /*!< RIPng metric */
}ip6RoutingMetric_t;

/*! IPv6 address scope */
typedef enum
{
    ip6AddrScopeReserved_c = 0x0,           /*!< Reserved */
    ip6AddrScopeInterfaceLocal_c = 0x01,    /*!< Interface local */
    ip6AddrScopeLinkLocal_c = 0x02,         /*!< Link local */
    ip6AddrScopeReamLocal_c = 0x03,         /*!< Realm Local */
    ip6AddrScopeAdminLocal_c = 0x04,        /*!< Admin Local */
    ip6AddrScopeSiteLocal_c = 0x05,         /*!< Site local */
    ip6AddrScopeOrgLocal_c = 0x08,          /*!< Organization local */
    ip6AddrScopeUniqueLocal_c = 0x0d,       /*!< Unique local */
    ip6AddrScopeGlobal_c = 0x0e,            /*!< Global */
} ip6AddrScope_t;

/*! IPv6 extended header types */
typedef enum
{
    ipExtHdrHopByHopOpt_c = 0,              /*!< Hop by hop option */
    ip6Header_c = 41,                       /*!< IPv6 header */
    ipExtHdrRoutingHeader_c = 43,           /*!< Routing header */
    ipExtHdrFragmentHeader_c = 44,          /*!< Fragment header */
    ipExtHdrEncapSecHeader_c = 50,          /*!< Encapsulation Security header */
    ipExtHdrAuthenticationHeader_c = 51,    /*!< Authentification header */
    ipExtHdrNoNextHeader_c = 59,            /*!< No next header */
    ipExtHdrDestOptions_c = 60,             /*!< Destination options */
    ipExtHdrMobilityHeader_c = 135,         /*!< Mobility header */
} ip6ExtHdrType_t;

/*! IPv6 multicast address data */
typedef struct ip6MulticastAddrData_tag
{
    ipAddr_t       mCastAddr;    /*!< IPv6 multicast address */
    ipIfUniqueId_t ifUniqueId;   /*!< Pointer to interface this IP6 address is binded to */
}ip6MulticastAddrData_t;

/*  The IPv6 Hop-by-Hop Options header is used to carry MPL information and has the
    following format:

    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Next Header  |  Hdr Ext Len  |                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
    |                                                               |
    .                                                               .
    .                            Options                            .
    .                                                               .
    |                                                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/*! IPv6 Hop-by-Hop Options Header structure */
typedef struct ip6HopByHopOptHdr_tag
{
   /*! 8-bit selector. Identifies the type of header immediately following the Options header */
   uint8_t  nextHeader;
   /*! 8-bit unsigned integer. Length of the Hop-by-Hop Options header in 8-octet units, not
      including the first 8 octets */
   uint8_t  hdrExtLength;
}ip6HopByHopOptHdr_t;

/*! IPv6 Options Header structure */
typedef struct ip6OptionHdr_tag
{
   /*! 8-bit identifier of the type of option */
   uint8_t type;
   /*! 8-bit unsigned integer. Length of the Option Data field of this option, in octets */
   uint8_t dataLength;
}ip6OptionHdr_t;

/*! IPv6 Routing Header structure */
typedef struct ip6RoutingHdr_tag
{
    /*! Identifies the type of header immediately following the Options header */
    uint8_t    nextHeader;
    /*! Length in 8-octet units, not including the first 8 octets */
    uint8_t    hdrExtLength;
    /*! 8-bit identifier of a particular Routing header variant */
    uint8_t    routingType;
    /*! Number of route segments remaining */
    uint8_t    segmentsLeft;
    /*! The complete Routing header is an integer multiple of 8 octets long */
    uint8_t    data[4];
}ip6RoutingHdr_t;

/*! IPv6 Fragmentation Header structure */
typedef struct ip6FragmentHdr_tag
{
   /*! Identifies the initial header type of the Fragmentable Part of the original packet */
   uint8_t    nextHeader;
   /*! Initialized to zero for transmission; ignored on reception */
   uint8_t    reserved;
   /*! @ 13-bit The offset, in 8-octet units, of the data following this header, relative to the
        start of the Fragmentable Part of the original packet.
       @ 2-bit reserved field.  Initialized to zero for transmission; ignored on reception.
       @ 1 = more fragments; 0 = last fragment */
   uint8_t    fragOffset[2];
   /*! Identification */
   uint8_t    id[4];
} ip6FragmentHdr_t;

/*! IPv6 Unicast Routing Table Entry structure */
typedef struct ip6RoutingTblEntry_tag
{
    /*! Destination IPv6 address or prefix of network */
    ipAddr_t dstPrefix;
    /*! Pointer in the neighbor cache of the next hop router to destination */
    ipAddr_t nextHopRouter;
    /*! IP interface ID of the next hop */
    ipIfUniqueId_t nextHopIfId;
    /*! Length of IPv6 prefix */
    uint8_t prefixLen;
    /*! Route metric */
    uint8_t routeMetric;
    /*! Routing protocol type */
    uint8_t routeProtType;
    /*! Flag that signals if a route is valid or is in a deletion phase */
    uint8_t validRoute;
}ip6RoutingTblEntry_t;

/*! IPv6 Multicast Forward Table Entry structure */
typedef struct ip6McastFwTblEntry_tag
{
    /*! Destination IPv6 address or prefix of network */
    ipAddr_t mcastGroup;
    /*! Pointer in the neighbor cache of the next hop router to destination */
    ipIfUniqueId_t forwardIf;
    ipIfUniqueId_t receiveIf;
}ip6McastFwTblEntry_t;

/*! IPv6 Firewall Table Entry structure */
typedef struct ip6FirewallTblEntry_tag
{
    /*! Source IPv6 address or prefix of network */
    uint8_t srcPrefix[16];
    /*! Destination IPv6 address or prefix of network */
    uint8_t dstPrefix[16];
    /*! Source port */
    uint16_t srcPort;
    /*! Destination port */
    uint16_t dstPort;
    /*! Source IPv6 prefix length in bits */
    uint8_t srcPrefixLenBits;
    /*! Destination IPv6 prefix length in bits */
    uint8_t dstPrefixLenBits;
    /*! MAC security level */
    int8_t secLevel;
    /*! Interface unique ID */
    uint8_t ifUniqueId;
    /*! Firewall rule can be accept or drop */
    uint8_t rule;
    /*! Firewall rule is enabled or not */
    bool_t enable;
}ip6FirewallTblEntry_t;

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
\fn     void IP6_Init(void)
\brief  Initialize function for IPv6.
***************************************************************************************************/
void IP6_Init(void);

/*!*************************************************************************************************
\fn     uint32_t IP6_Send(ipPktInfo_t *pIpPktInfo)
\brief  Sends an IP6 packet generated on the local host.

\param  [in]  pIpPktInfo  The packet to send

\return       uint32_t    Error
***************************************************************************************************/
uint32_t IP6_Send(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     void IP6_Service(uint8_t *pInData)
\brief  Parses a received IP6 packet.

\param  [in]  pInData  The packet received
***************************************************************************************************/
void IP6_Service(uint8_t *pInData);

/*!*************************************************************************************************
\fn     void IP6_UnicastFwEnable(bool_t enable,ipIfUniqueId_t ifUniqueId)
\brief  Enables/disables IPv6 unicast global or interface forwarding

\param  [in]  bEnable     TRUE = enable / FALSE = disable
\param  [in]  ifUniqueId  Interface Unique Id, if gIpIfUndef_c global forwarding flag will be modified
***************************************************************************************************/
void IP6_UnicastFwEnable(bool_t bEnable, ipIfUniqueId_t ifUniqueId);

/*!*************************************************************************************************
\fn     void IP6_MulticastFwEnable(bool_t enable,ipIfUniqueId_t ifUniqueId)
\brief  Enables/disables IPv6 multicast global or interface forwarding

\param  [in]  bEnable  TRUE = enable / FALSE = disable
\param  [in]  ifUniqueId  Interface Unique Id, if gIpIfUndef_c global forwarding flag will be modified
***************************************************************************************************/
void IP6_MulticastFwEnable(bool_t bEnable, ipIfUniqueId_t ifUniqueId);

#if IP_IP6_ROUTING_ENABLE
/*!*************************************************************************************************
\fn     ipAddr_t *IP6_UnicastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction)
\brief  Forwards an IP6 packet to the destination interface.

\param  [in]  pIpPktInfo  The packet to send
\param  [in]  direction   Forwarding direction: TX or RX

\return       ipAddr_t *  Pointer to next hop address or NULL
***************************************************************************************************/
ipAddr_t *IP6_UnicastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction);

/*!*************************************************************************************************
\fn     ipAddr_t *IP6_GetDefaultRoute(ipPktInfo_t *pIpPktInfo, uint8_t direction)
\brief  Searches the IP6 Routing Table for the default route.

\param  [in]  pIpPktInfo    The packet to send
\param  [in]  direction     TX or RX

\return       ipAddr_t *    Pointer to next hop address or NULL
***************************************************************************************************/
ipAddr_t *IP6_GetDefaultRoute(ipPktInfo_t *pIpPktInfo, uint8_t direction);

/*!*************************************************************************************************
\fn     ip6RoutingTblEntry_t **IP6_GetRoutingTblPtr(uint32_t *tableSize)
\brief  Get pointer to IP6 Routing Table(Routing table only holds pointer to each routing table
        entry which is allocated for dynamic memory or kept in NVM).

\param  [out]  tableSize                Pointer to IP routing table size

\return        ip6RoutingTblEntry_t **  Double pointer to start of routing table
***************************************************************************************************/
ip6RoutingTblEntry_t **IP6_GetRoutingTblPtr(uint32_t *tableSize);

/*!*************************************************************************************************
\fn     ip6RoutingTblEntry_t *IP6_GetRoute(uint32_t index)
\brief  Returns the pointer to the entry corresponding to the give index in routing table.

\param  [in]  index                Index in routing table

\return        ip6RoutingTblEntry_t*  Pointer to routing entry
***************************************************************************************************/
ip6RoutingTblEntry_t *IP6_GetRoute(uint32_t index);

/*!*************************************************************************************************
\fn     uint32_t IP6_GetRoutingTableSize()
\brief  Returns the size of the routing table.

\param  [in]    none

\return        uint32_t  Size of routing table
***************************************************************************************************/
uint32_t IP6_GetRoutingTableSize(void);

/*!*************************************************************************************************
\fn     void IP6_SetStaticRoute(ipAddr_t *pDestAddr, uint8_t prefixLen, uint8_t routeMetric,
                                ipAddr_t *nextHopAddr, ipIfUniqueId_t ifId)
\brief  Sets a static route in the IP6 routing table.

\param  [in]  pDestAddr     The address to set in the routing table
\param  [in]  prefixLen     Prefix length of the address to set in the routing table
\param  [in]  routeMetric   Metric value of the route between 1 and IP6_STATIC_ROUTE_USER_MAX_VALUE
                            - this value is added to static route metric base value
                            If IP6_STATIC_ROUTE_SUPER_METRIC is used the route is blocked and cannot
                            be updated and can only be removed. This is "super route" that takes
                            precedence over any other equal route(same prefix and next hop)
\param  [in]  pNextHopAddr  The address of the next hop router to destination
\param  [in]  ifId          Interface unique ID
***************************************************************************************************/
void IP6_SetStaticRoute(ipAddr_t *pDestAddr, uint8_t prefixLen, uint8_t routeMetric,
                        ipAddr_t *pNextHopAddr, ipIfUniqueId_t ifId);

/*!*************************************************************************************************
\fn     void IP6_RemoveRoute(ipAddr_t *remAddr, ipAddr_t *nextHopAddr)
\brief  Removes a route in the IP6 routing table.

\param  [in]  remAddr      The address to remove from the routing table
\param  [in]  nextHopAddr  The next hop address of the route to remove or NULL if any
***************************************************************************************************/
void IP6_RemoveRoute(ipAddr_t *remAddr, ipAddr_t *nextHopAddr);

/*!*************************************************************************************************
\fn     void IP6_RemoveAllRoutes(ipIfUniqueId_t ifId)
\brief  Removes all routes in the IP6 routing table for a specified IF handle.

\param  [in]  ifId            Interface unique ID
\param  [in]  pIfHandle       IP interface handle
***************************************************************************************************/
void IP6_RemoveAllRoutes(ipIfUniqueId_t ifId);

/*!*************************************************************************************************
\fn     ipAddr_t *IP6_HandleUcastForwarding(ipPktInfo_t *pIpPktInfo, uint8_t direction,
                                       ip6RoutingTblEntry_t *dstRoute);
\brief  Handles all common task for IP forwarding.

\param  [in]  pIpPktInfo  The packet to send
\param  [in]  direction   TX or RX
\param  [in]  dstRoute    Destination route

\return      ipAddr_t *   Pointer to next hop address or NULL
***************************************************************************************************/
ipAddr_t *IP6_HandleUcastForwarding(ipPktInfo_t *pIpPktInfo, uint8_t direction,
                               ip6RoutingTblEntry_t *dstRoute);

/*!*************************************************************************************************
\fn     void IP6_AddIpHeader(ipPktInfo_t *pIpPktInfo, uint32_t protocol, ip6Header_t *pOutHeader)
\brief  Adds the IPv6 header to a packet.

\param  [in]  pIpPktInfo    The packet to send
\param  [in]  protocol      The upper layer protocol
\param  [out] pOutHeader    Pointer to start of IP6 header data. Populated by this function
***************************************************************************************************/
void IP6_AddIpHeader(ipPktInfo_t *pIpPktInfo, uint32_t protocol, ip6Header_t *pOutHeader);

/*!*************************************************************************************************
\fn     ip6RoutingTblEntry_t *IP6_SearchDestination(ipAddr_t *destAddr)
\brief  Searches the IP6 Routing Table for the destination address.

\param  [in]  destAddr                 The destination address to search for

\return       ip6RoutingTblEntry_t *   Pointer to found route table entry
***************************************************************************************************/
ip6RoutingTblEntry_t *IP6_SearchDestination(ipAddr_t *destAddr);

#endif /* IP_IP6_ROUTING_ENABLE */

#if IP_IP6_MCAST_FW_ENABLE
/*!*************************************************************************************************
\fn     uint32_t IP6_MulticastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction, ipAddr_t *nextHop)
\brief  Forwards an IP6 multicast packet to the destination interface.

\param  [in]   pIpPktInfo  The packet to send
\param  [in]   direction   Forwarding direction: TX or RX
\param  [out]  nextHop     Next hop address

\return        uint32_t    Result of the operation
***************************************************************************************************/
uint32_t IP6_MulticastForward(ipPktInfo_t *pIpPktInfo, uint8_t direction, ipAddr_t *nextHop);

/*!*************************************************************************************************
\fn     void IP6_HandleMcastForwarding(ipPktInfo_t *pIpPktInfo,  uint8_t direction,
        ipIfUniqueId_t forwardIf)
\brief  Handles all common task for IP multicast forwarding.

\param  [in]  pIpPktInfo  The packet to send
\param  [in]  direction   TX or RX
\param  [in]  forwardIf   Destination interface

\return       none
***************************************************************************************************/
void IP6_HandleMcastForwarding(ipPktInfo_t *pIpPktInfo,uint8_t direction,
                                             ipIfUniqueId_t forwardIf);

/*!*************************************************************************************************
\fn     void IP6_SetMulticastRoute(ipAddr_t *pMcastAddr,ipIfUniqueId_t fwIfId,ipIfUniqueId_t rcvIfId,
        bool_t bSaveToNvm)
\brief  Sets a multicast forwarding route in the IP6 multicast forwarding table.

\param  [in]  pDestAddr     The address to set in the multicast forward table
\param  [in]  fwIfId        Forward interface unique ID
\param  [in]  rcvIfId       Receive interface unique ID
\param  [in]  bSaveToNvm    If TRUE,route will be saved to nvm
***************************************************************************************************/
void IP6_SetMulticastRoute(ipAddr_t *pMcastAddr, ipIfUniqueId_t fwIfId,
                                       ipIfUniqueId_t rcvIfId, bool_t bSaveToNvm);

/*!*************************************************************************************************
\fn     void IP6_RemoveMulticastRoute(ipAddr_t *pMcastAddr,ipIfUniqueId_t fwIfId, bool_t bFromNvm)
\brief  Removes a multicast forwarding route from the IP6 multicast forwarding table.

\param  [in]  pDestAddr     The address to remove from the multicast forward table
\param  [in]  fwIfId        Forward Interface unique ID
\param  [in]  bFromNvm      TRUE if entry was saved in NVM
***************************************************************************************************/
void IP6_RemoveMulticastRoute(ipAddr_t *pMcastAddr, ipIfUniqueId_t fwIfId,
                                            bool_t bFromNvm);

/*!*************************************************************************************************
\fn     ip6McastFwTblEntry_t *IP6_GetMcastFwTblEntry(ipAddr_t *pMcastAddr, ipIfUniqueId_t ifId)
\brief  Get pointer to IP6 Multicast Forwarding Table entry

\param  [in]  pMcastAddr    The address to get from the multicast forward table
\param  [in]  fwIfId        Forward interface unique ID

\return       ip6McastFwTblEntry_t *   Pointer to multicast forwarding table entry
***************************************************************************************************/
ip6McastFwTblEntry_t *IP6_GetMcastFwTblEntry(ipAddr_t *pMcastAddr,ipIfUniqueId_t fwIfId);

/*!*************************************************************************************************
\fn     bool_t IP6_IsMcastGroupJoined(ipAddr_t *pMcastAddr, ipIfUniqueId_t ifId)
\brief  Returns TRUE if a multicast forwarding rule is listening for the multicast address on the
        receiving interface

\param  [in]  pMcastAddr     The address to check from the multicast forward table
\param  [in]  rcvIfid        Receiving Interface unique ID

\return       bool_t         TRUE if multicsat group is used, FALSE otherwise
***************************************************************************************************/
bool_t IP6_IsMcastGroupJoined(ipAddr_t *pMcastAddr, ipIfUniqueId_t rcvIfid);

/*!*************************************************************************************************
\fn     void IP6_MulticastRouteRestore()
\brief  Checks for permanent(NVM saved) multicast routes and rejoins groups at MAC layer

\return       None
***************************************************************************************************/
void IP6_MulticastRouteRestore();

#endif /* IP_IP6_MCAST_FW_ENABLE */

#if IP_IP6_FIREWALL_ENABLE
/*!*************************************************************************************************
\fn     uint32_t IP6_FirewallSetRule(ip6FirewallTblEntry_t *ip6FirewallTblEntry)
\brief  Sets a new Firewall rule.

\param  [in]  ip6FirewallTblEntry   Pointer to the firewall table entry

\return       uint32_t              gIpOk_c or gIpMemoryAllocError_c
***************************************************************************************************/
uint32_t IP6_FirewallSetRule(ip6FirewallTblEntry_t *ip6FirewallTblEntry);

/*!*************************************************************************************************
\fn     uint32_t IP6_FirewallGetEntryIndex(ip6FirewallTblEntry_t *ip6FirewallTblEntry,
                                           uint32_t *pIndex)
\brief  Searches for an ip6FirewallTblEntry.

\param  [in]  ip6FirewallTblEntry   Pointer to the firewall table entry
\param  [in]  pIndex                -1/Index of the searched firewall entry

\return       bool_t                TRUE if the entry was found
***************************************************************************************************/
bool_t IP6_FirewallGetEntryIndex(ip6FirewallTblEntry_t *ip6FirewallTblEntry, uint32_t *pIndex);

/*!*************************************************************************************************
\fn     uint32_t IP6_FirewallSetRule(ip6FirewallTblEntry_t *ip6FirewallTblEntry)
\brief  Sets a new Firewall rule.

\param  [in]  ip6FirewallTblEntry   Pointer to the firewall table entry

\return       uint32_t              gIpOk_c or gIpMemoryAllocError_c
***************************************************************************************************/
uint32_t IP6_FirewallRemoveRule(uint32_t entryNb);

/*!*************************************************************************************************
\fn     uint32_t IP6_GetFirewallTableSize(void)
\brief  Get the size of the IPv6 Firewall Table.

\return        uint32_t     Size of the IPv6 Firewall Table.
***************************************************************************************************/
uint32_t IP6_GetFirewallTableSize(void);

/*!*************************************************************************************************
\fn     ip6FirewallTblEntry_t *IP6_GetFirewallRule(uint32_t index)
\brief  Returns the pointer to the entry corresponding to the give index in Firewall table.

\param  [in]  index                Index in Firewall table

\return        ip6FirewallTblEntry_t*  Pointer to Firewall entry
***************************************************************************************************/
ip6FirewallTblEntry_t *IP6_GetFirewallRule(uint32_t index);

#endif /* IP_IP6_FIREWALL_ENABLE */

/*!*************************************************************************************************
\fn     void IP6_FirewallEnabled(bool_t enable)
\brief  Enables/disables IPv6 Firewall.

\param  [in]  bEnable  TRUE = enable / FALSE = disable
***************************************************************************************************/
void IP6_FirewallEnabled(bool_t bEnable);

/*!*************************************************************************************************
\fn     uint32_t IP_GetAddrScope6(ipAddr_t *ipAddr)
\brief  Returns scope of the IPv6 address (Node-local,link-local, site-local or global).

\param  [in]  ipAddr    The IP address

\return       uint32_t  Address scope
***************************************************************************************************/
uint32_t IP_GetAddrScope6(ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     void IP6_GetSolicitedMcastAddr(ipAddr_t *ipAddr, ipAddr_t *solicitedMastAddr)
\brief  Get IPv6 solicited-node multicast address. It has the prefix FF02:0:0:0:0:1:FF00:0000/104
        concatenated with the 24 low-order bits of a corresponding IPv6 unicast or anycast address.

\param  [in]  ipAddr                 The IP address from witch to construct multicast address
\param  [out] solicitedMastAddr      The solicited multicast address
***************************************************************************************************/
void IP6_GetSolicitedMcastAddr(ipAddr_t *ipAddr, ipAddr_t *solicitedMastAddr);

/*!*************************************************************************************************
\fn     void IP6_AddPadding(uint8_t *pHeader, uint8_t headerLength, uint8_t dataLength)
\brief  Function used to add Pad1 or PadN option when an IP extension header is not multiple of 8 bytes.

\param  [in]  pHeader         Pointer to the start of the header
\param  [in]  headerLength    Total header length (multiple of 8 bytes)
\param  [in]  dataLength      Data length including header
****************************************************************************************************/
void IP6_AddPadding(uint8_t *pHeader, uint8_t headerLength, uint8_t dataLength);

/*!*************************************************************************************************
\fn     void IP6_AddIpHeader(ipPktInfo_t *pIpPktInfo, uint32_t protocol, ip6Header_t *pOutHeader)
\brief  Adds the IPv6 header to a packet.

\param  [in]  pIpPktInfo    The packet to send
\param  [in]  protocol      The upper layer protocol
\param  [out] pOutHeader    Pointer to start of IP6 header data. Populated by this function
***************************************************************************************************/
void IP6_AddIpHeader(ipPktInfo_t *pIpPktInfo, uint32_t protocol, ip6Header_t *pOutHeader);

#if IP_IP6_ENABLE_REASSEMBLY
/*!*************************************************************************************************
\fn     void IP6_ReassemExpireEventHandle(ipPktInfo_t *dgram)
\brief  Called by the Timer. Expire an IP datagram and frees all the occupied memory.

\param  [in]  dgram  ipPktInfo_t type datagram that holds the fragments received so far
***************************************************************************************************/
void IP6_ReassemExpireEventHandle(ipPktInfo_t *dgram);
#endif

#if IP_IP6_STATS_ENABLE
/*!*************************************************************************************************
\fn     ip6Stats_t *IP6_GetIPStats(void)
\brief  Returns structure of IPv6 statistics.

\return ip6Stats_t *   Pointer to IPv6 statistics structure
***************************************************************************************************/
ip6Stats_t *IP6_GetIPStats(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* IP_IP6_ENABLE */
/*================================================================================================*/
#endif  /*_IP6_H_ */
