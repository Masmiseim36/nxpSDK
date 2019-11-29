/*! *********************************************************************************
 * \defgroup IPIF  Network IP Interface
 * @{
 ***********************************************************************************/

/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP_IF_MANAGEMENT_H_
#define _IP_IF_MANAGEMENT_H_

/*!
\file       ip_if_management.h
\brief      This is a header file for the IP interface management module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip.h"
#include "ip6.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#define IP_IF_MAC_ADDR_NB       2U  /*! Maximum supported number of link layer addresses */

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! IPv6 address types */
typedef enum
{
    ip6AddrTypeManual_c = 0xFF,               /*!< Manual Address */
    ip6AddrTypeAutoconfigurable_c = 0x00,     /*!< Autoconfigurable address - default MAC address */
    ip6AddrTypeAutoconfigurableMac2_c = 0x01  /*!< Autoconfigurable address - second MAC address */
} ip6AddrType_t;

/*! Information about an IPv4 interface */
typedef struct ip4IfStruct_tag
{
    uint32_t       *(*ip4Forward)(ipPktInfo_t *, uint8_t);  /*! IPv4 forwarding callback */
} ip4IfStruct_t;

struct ipIfStruct_tag;

/*! Information about an IPv6 interface */
typedef struct ip6IfStruct_tag
{
    /*! Interface IPv6 unicast forwarding enable/disable */
    bool_t     bIfUcastFwEnabled;
    /*! Interface IPv6 multicast forwarding enable/disable */
    bool_t     bIfMcastFwEnabled;
    /*! The scope ID of the interface, useful in link-local communication */
    uint32_t   scope_id;
    /*! ND configuration */
    void**     ppNdCfg;
    /*! Detects if pIpDestAddr is On-link */
    bool_t     (*ip6IsAddrOnLink)(ipAddr_t *pIpDestAddr, struct ipIfStruct_tag *instanceId);
    /*! Selects the unicast address needed to reach pIpDestAddr */
    bool_t     (*ip6ResolveUnicastAddr)(ipPktInfo_t *pIpDestAddr);
    /*! Callback that allows management layer inspection on received packets */
    void       (*ip6UpperMgtLayerCb)(ipPktInfo_t *pIpDestAddr);
    /*! IPv6 multicast forwarding callback */
    uint32_t   (*ip6McastForward)(ipPktInfo_t *, uint8_t, ipAddr_t *);
    /*! IPv6 unicast forwarding callback */
    ipAddr_t  *(*ip6UnicastForward)(ipPktInfo_t *, uint8_t);
} ip6IfStruct_t;

/*! Information about a media interface */
typedef struct mediaIfStruct_tag
{
    /*! Open interface function pointer */
    uint32_t (* ifOpen) (struct ipIfStruct_tag*);
    /*! Close interface function pointer */
    uint32_t (* ifClose)(struct ipIfStruct_tag*);
    /*! Send IPv4 packet */
    uint32_t (* ifSend4) (ipPktInfo_t*);
    /*! Send IPv4 ARP packet */
    uint32_t (* ifSendArp) (ipPktInfo_t*, llAddr_t*);
    /*! Send IPv6 packet */
    uint32_t (* ifSend6) (ipPktInfo_t*);
    /*! Get the interface identifier */
    uint32_t (* ifGetIID)(struct ipIfStruct_tag*,llAddr_t*, ipAddr_t*);
    /*! Join a group on the physical interface */
    uint32_t (* ifJoin) (struct ipIfStruct_tag*, ipAddr_t*,uint16_t);
    /*! Leave a group on the physical interface */
    uint32_t (* ifLeave)(struct ipIfStruct_tag*, ipAddr_t*,uint16_t);
} mediaIfStruct_t;

/*! Information about a network interface (IPv4 or IPv6) */
typedef struct ipIfStruct_tag
{
    /*! Handle for media link layer module */
    void                *ifDriverHandle;
     /*! Pointer to media interface functions */
    mediaIfStruct_t     *ifFunctions;
    /*! Interface maximum transmission unit */
    uint16_t             ifMtu;
    /*! If ipVersion4 == 1->IPv4 is enabled on this interface */
    uint8_t              ipVersion4;
    /*! If ipVersion6 == 1->IPv6 is enabled on this interface */
    uint8_t              ipVersion6;
#if IP_IP6_ENABLE
    /*! Structure encompassing specific IPv6 interface information */
    ip6IfStruct_t        ip6If;
#endif

#if IP_IP4_ENABLE
    /*! Structure encompassing specific IPv4 interface information */
    ip4IfStruct_t        ip4If;
#endif

#if IP_IF_STATS_ENABLE
    /*! Statistics on the network interface */
    ipIfStats_t         stats;
#endif

   /*! Media link layer address */
   llAddr_t             ifDevAddrTbl[IP_IF_MAC_ADDR_NB];
   /*! Interface unique ID */
   ipIfUniqueId_t       ifUniqueId;
   /*! Interface metric */
   uint8_t              ifMetric;
} ipIfStruct_t;

/*! Typedef for interface handler */
typedef ipIfStruct_t *ifHandle_t;

/*! Information about the addressing of an IPv4 interface */
typedef struct ip4IfAddrData_tag
{
    /*! Interface ID of the interface this IP4 address is binded to */
    ipIfUniqueId_t ipIfId;
    /*! IPv4 address in host byte order */
    uint32_t      ip4Addr;
    /*! IPv4 address subnet mask in host byte order */
    uint32_t      ip4SubnetMask;
    /*! IPv4 default gateway for the interface in host byte order */
    uint32_t      ip4DefaultGw;
} ip4IfAddrData_t;

/*! Information about the addressing of an IPv6 interface */
typedef struct ip6IfAddrData_tag
{
    /*! IPv6 address */
    ipAddr_t      ip6Addr;
    /*! Interface ID of the interface this IP6 address is binded to */
    ipIfUniqueId_t ipIfId;
    /*! Time of entry creation (in seconds) */
    uint32_t      creationTime;
    /*! Address lifetime expire timestamp (in seconds). 0xFFFFFFFF= Infinite Lifetime */
    uint32_t      lifetime;
    /*! Address type (4 bits) and current state (4 bits) .*/
    uint8_t        ip6AddrTypeAndState;
    /*! Counter used by DAD. Equals to the number of NS transmits till DAD is finished */
    uint8_t        dadTransmitCounter;
    /*! The number of leading bits in the Prefix that are valid. -Not used, maybe used for routing */
    uint8_t        prefixLength;
    /*! Index in the interface MAC address table of the MAC address this IP6 address is assigned to */
    uint8_t        macAddrIndex;
} ip6IfAddrData_t;

/*! Typedef needed by the Thread Stack to select the source address when the destination
    address is a ML16 or ML64 */
typedef void (*ip6IfSelThreadMLSrcAddr6_t)(ipAddr_t *pDestAddr,ipAddr_t **pBestSourceAddr);

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
\fn     void IP_IF_Init(void)
\brief  Init the global interface table.
***************************************************************************************************/
void IP_IF_Init(void);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_Add(ipIfUniqueId_t ifId, uint8_t *driverHandle, mediaIfStruct_t *pIfStruct,
                           uint16_t ipVersEnabled)
\brief  Adds a new interface to the global interface table.

\param  [in]  ifId           Interface unique ID
\param  [in]  driverHandle   Pointer to the packet driver handle (can be NULL)
\param  [in]  pIfStruct      Call table for the interface
\param  [in]  ipVersEnabled  The IP version that wants to be enabled on this interface
                             (gIpProtv4_c, gIpProtv6_c or gIpProtv4_c | gIpProtv6_c)

\return       uint32_t       Result of the operation
***************************************************************************************************/
uint32_t IP_IF_Add(ipIfUniqueId_t ifId, uint8_t *driverHandle, mediaIfStruct_t *pIfStruct,
                   uint16_t ipVersEnabled);

/*!*************************************************************************************************
\fn     ifHandle_t IP_IF_GetIfHandle(ipIfUniqueId_t ifId)
\brief  Returns  pointer interface handle structure identified by unique ID.

\param  [in]  ifId           Interface unique ID

\return       ifHandle_t     interface handle
***************************************************************************************************/
ipIfStruct_t *IP_IF_GetIfHandle(ipIfUniqueId_t ifId);

/*!*************************************************************************************************
\fn     int32_t IP_IF_GetIfIndex(ipIfUniqueId_t ipIfId)
\brief  Returns the index (from zero) in the interface table of the provided interface.

\param  [in]  ipIfId   IP interface identifier

\return       int32_t  Interface index or -1 in case of error
***************************************************************************************************/
int32_t IP_IF_GetIfIndex(ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     bool_t IP_IF_IsMyAddr(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Checks if an unicast address is attached/bound to the interface.

\param  [in]  ipIfId   IP interface identifier
\param  [in]  pIpAddr  Pointer to IP address

\return       bool_t   TRUE if the address is attached/bound, FALSE otherwise
***************************************************************************************************/
bool_t IP_IF_IsMyAddr(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     void IP_IF_Join(ipIfUniqueId_t ipIfId, ipAddr_t *groupIp)
\brief  Adds a multicast group into the physical interface.

\param  [in]  ipIfId   IP interface identifier
\param  [in]  groupIp  Pointer to the IP multicast address to join
***************************************************************************************************/
void IP_IF_Join(ipIfUniqueId_t ipIfId, ipAddr_t *groupIp);

/*!*************************************************************************************************
\fn     void IP_IF_Leave(ipIfUniqueId_t ipIfId, ipAddr_t *groupIp)
\brief  Removes a multicast group from the physical interface.

\param  [in]  ipIfId   IP interface identifier
\param  [in]  groupIp  Pointer to the IP multicast group address to leave
***************************************************************************************************/
void IP_IF_Leave(ipIfUniqueId_t ipIfId, ipAddr_t *groupIp);

/*!*************************************************************************************************
\fn     ipIfUniqueId_t IP_IF_GetIfIdByIndex(uint32_t ifIndex)
\brief  Returns the interface unique ID according to its index (from zero).

\param  [in]  ifIndex           The interface Index

\return       ipIfUniqueId_t    Interface unique ID
***************************************************************************************************/
ipIfUniqueId_t IP_IF_GetIfIdByIndex(uint32_t ifIndex);

/*!*************************************************************************************************
\fn     ifHandle_t IP_IF_GetIfByIndex(uint32_t ifIndex)
\brief  Returns  double pointer to ifNumber interface according to its index (from zero).

\param  [in]  ifIndex        The interface Index

\return       ifHandle_t     It returns NULL if there is no interface with the ifNumber index
***************************************************************************************************/
ifHandle_t IP_IF_GetIfByIndex(uint32_t ifIndex);

/*!*************************************************************************************************
\fn     ifHandle_t IP_IF_GetIfByAddr(ipAddr_t *pIpAddr)
\brief  This function returns pointer to interface which has the provided address.

\param  [in]  pIpAddr         Pointer to the IP address

\return       ifHandle_t *    It returns NULL if there is no interface with the address
***************************************************************************************************/
ifHandle_t IP_IF_GetIfByAddr(ipAddr_t *pIpAddr);

#if IP_IP4_ENABLE
/*!*************************************************************************************************
\fn     ip4IfAddrData_t IP_IF_GetAddressDataStruct4(ifHandle_t ifHandle, uint32_t ipAddr)
\brief  Gets the address data structure of the provided IPv4 address.

\param  [in]  ifHandle           IP interface structure
\param  [in]  ipAddr              The IP address

\return       ip6IfAddrData_t *   Pointer to address data structure or NULL if not found
***************************************************************************************************/
ip4IfAddrData_t *IP_IF_GetAddressDataStruct4(ifHandle_t ifHandle, uint32_t ipAddr);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_BindAddr4(ipIfUniqueId_t ifId, uint32_t ipAddr, uint32_t subnetMask, uint32_t defaultGw)
\brief  Binds an IP address to a hardware interface.

\param  [in]  ifId        IP interface identifier
\param  [in]  ipAddr      The IP address to set. Must be in class A,B or C
\param  [in]  subnetMask  Subnet mask of the IP address
\param  [in]  defaultGw   Default gateway for this source address

\return       uint32_t    Result of the operation
***************************************************************************************************/
uint32_t IP_IF_BindAddr4(ipIfUniqueId_t ifId, uint32_t ipAddr, uint32_t subnetMask,
                         uint32_t defaultGw);

/*!*************************************************************************************************
\fn     bool_t IP_IF_IsNotBound4(ifHandle_t  ifHandle)
\brief  Checks if an interface is not bound to any IP address.

\param  [in]  ifHandle  Pointer to IP interface structure

\return       bool_t     TRUE or FALSE
***************************************************************************************************/
bool_t IP_IF_IsNotBound4(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_UnbindAddr4(ifHandle_t ifHandle, uint32_t ipAddr)
\brief  Unbinds an IP address from a hardware interface.

\param  [in]  ifHandle   Pointer to IP interface structure
\param  [in]  ipAddr     The IP address we want to unbind

\return       uint32_t   gIpOk_c if OK, gIpInvalidParameterError_c if error
***************************************************************************************************/
uint32_t IP_IF_UnbindAddr4(ifHandle_t ifHandle, uint32_t ipAddr);

/*!*************************************************************************************************
\fn     bool_t IP_IF_IsMyBcastAddr4(ifHandle_t ifHandle,ipAddr_t *pIpAddr)
\brief  Checks if a IPv4 broadcast address is attached/bound to the interface.

\param  [in]  ifHandle  Pointer to IP interface structure
\param  [in]  pIpAddr    Pointer to the IP address

\return       bool_t     TRUE if the address is attached/bound, FALSE otherwise
***************************************************************************************************/
bool_t IP_IF_IsMyBcastAddr4(ifHandle_t ifHandle,ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_SelSrcAddr4(ipAddr_t *pDestAddr, ifHandle_t pIfDest)
\brief  Selects the best IPv4 source address to use with an IPv4 destination address. Based on
        RFC 1122

\param  [in]  pDestAddr  Pointer to the IPv4 address
\param  [in]  pIfDest    Double pointer to interface - used for broadcast and multicast

\return       uint32_t   IP address to structure or IP4_ADDR_ANY if not found
***************************************************************************************************/
uint32_t IP_IF_SelSrcAddr4(ipAddr_t *pDestAddr, ipIfStruct_t *pIfDest);

/*!*************************************************************************************************
\fn     ip4IfAddrData_t *IP_IF_GetAddrByIf4(ipIfUniqueId_t ipIfId, uint32_t addrIndex)
\brief  Searches the global IPv4 table for the address corresponding to the pIfHandle interface

\param  [in]  ifHandle         Pointer to IP interface structure

\return       ip4IfAddrData_t  Pointer to IPv4 address data structure of the interface
***************************************************************************************************/
ip4IfAddrData_t *IP_IF_GetAddrByIf4(ipIfUniqueId_t ipIfId, uint32_t addrIndex);

#endif

#if IP_IP6_ENABLE
/*!*************************************************************************************************
\fn     bool_t IP_IF_IsNodeIpAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Checks if an unicast address identifies the current node. A link local address identifies
        the current node only if it is bound to the receiving interface.

\param  [in]  ipIfId   IP interface identifier
\param  [in]  pIpAddr  Pointer to IP address

\return       bool_t   TRUE if destination is a valid address of the node, FALSE otherwise
***************************************************************************************************/
bool_t IP_IF_IsNodeIpAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_AddMulticastGroup6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Adds a new multicast group for an interface.

\param  [in]  ipIfId    IP interface identifier
\param  [in]  pIpAddr   Pointer to the IP multicast address

\return       uint32_t  gIpOk_c or error
***************************************************************************************************/
uint32_t IP_IF_AddMulticastGroup6(ipIfUniqueId_t ipIfId,  ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_LeaveMulticastGroup6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Removes a multicast group for an interface.

\param  [in]  ipIfId      IP interface identifier
\param  [in]  pIpAddr     Pointer to the IP multicast address

\return       uint32_t    gIpOk_c or error
***************************************************************************************************/
uint32_t IP_IF_LeaveMulticastGroup6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     bool_t IP_IF_IsMyMulticastGroupAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Checks if an address belongs to a multicast group registered on the interface.

\param  [in]  ipIfId    IP interface identifier
\param  [in]  pIpAddr   Pointer to the IP multicast address

\return       uint32_t  gIpOk_c or error
***************************************************************************************************/
bool_t IP_IF_IsMyMulticastGroupAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     bool_t IP_IF_HasPrefferedLLAddr6(ipIfUniqueId_t ipIfId)
\brief  Checks if an interface has a Preferred Link Local address.

\param  [in]  ipIfId  IP interface unique ID

\return       bool_t  TRUE if link local address is preferred
***************************************************************************************************/
bool_t IP_IF_HasPrefferedLLAddr6(ipIfUniqueId_t ipIfId);

/*!*************************************************************************************************
\fn     ip6IfAddrData_t** IP_IF_GetAddressDataStruct6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Gets the address data structure of the provided ipv6 address.

\param  [in]  ipIfId              IP interface unique ID
\param  [in]  pIpAddr             Pointer to the IP address

\return       ip6IfAddrData_t **  Double pointer to address data structure or NULL if not found
***************************************************************************************************/
ip6IfAddrData_t **IP_IF_GetAddressDataStruct6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     bool_t IP_IF_IsMySolicitedMcastAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr)
\brief  Checks if a solicited multicast address is attached/bound to the interface.

\param  [in]  ipIfId  IP interface unique ID
\param  [in]  ipAddr  Pointer to the IP address

\return       bool_t  TRUE if the address is attached/bound, FALSE otherwise
***************************************************************************************************/
bool_t IP_IF_IsMySolicitedMcastAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *pIpAddr);


/*!*************************************************************************************************
\fn     bool_t IP_IF_IsSolicitedMcastAddr6(ipAddr_t *pIpAddr)
\brief  Checks if a given ip is a solicited multicast address

\param  [in]  ipAddr  Pointer to the IP address

\return       bool_t  TRUE if the address is in solicited-node-multicast format, FALSE otherwise
***************************************************************************************************/
bool_t IP_IF_IsSolicitedMcastAddr6(ipAddr_t *pIpAddr);


/*!*************************************************************************************************
\fn     ip6IfAddrData_t *IP_IF_GetAddrByIf6(ipIfUniqueId_t ipIfId, uint32_t addrIndex, bool_t countAnycast)
\brief  Searches the global IPv6 table for the address with addrIndex index corresponding to the
        pIfHandle interface, taking into account if it is required to return anycast addresses.

\param  [in]  ipIfId             IP interface identifier
\param  [in]  addrIndex          Index of address assigned to an interface
\param  [in]  countAnycast       Anycast address count flag

\return       ip6IfAddrData_t *  Pointer to address data structure
***************************************************************************************************/
ip6IfAddrData_t *IP_IF_GetAddrByIf6(ipIfUniqueId_t ipIfId, uint32_t addrIndex, bool_t countAnycast);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_GetAddrNbByIfIndex6(uint32_t ifIndex, bool_t countAnycast)
\brief  Searches the global IPv6 table for the address with index corresponding to the
        pIfHandle interface, taking into account if it is required to count anycast addresses.

\param  [in]  ifIndex        IP interface index
\param  [in]  countAnycast   Anycast address count flag

\return       uint32_t       Number of IP addresses
***************************************************************************************************/
uint32_t IP_IF_GetAddrNbByIfIndex6(uint32_t ifIndex, bool_t countAnycast);

/*!*************************************************************************************************
\fn     ip6IfAddrData_t **IP_IF_GetAddrByNr6(uint32_t addrNumber)
\brief  Returns a double pointer to addrNumber IPv6 address according to its index (from zero).

\param  [in]  addrNumber        The address number

\return       ip6IfAddrData_t   It returns NULL if there is no address with the addrNumber index
***************************************************************************************************/
ip6IfAddrData_t **IP_IF_GetAddrByNr6(uint32_t addrNumber);

/*!*************************************************************************************************
\fn     ipIfUniqueId_t IP_IF_GetIfByScopeId6(uint32_t scopeId)
\brief  This function returns pointer the interface according its Scope ID.

\param  [in]  scopeId         Pointer to IP interface structure

\return       ipIfUniqueId_t  Interface Unique Id or gIpIfUndef_c in case of error
***************************************************************************************************/
ipIfUniqueId_t IP_IF_GetIfByScopeId6(uint32_t scopeId);

/*!*************************************************************************************************
\fn     ipAddr_t *IP_IF_SelSrcAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *destAddr)
\brief  Selects the best source address to use with a destination address, based on RFC3484.

\param  [in]  ipIfId      IP interface identifier - Optional
\param  [in]  ipAddr      The IP address

\return      ipAddr_t *   Pointer to IPv6 address or NULL if not found
***************************************************************************************************/
ipAddr_t *IP_IF_SelSrcAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *destAddr);

/*!*************************************************************************************************
\fn     void IP_IF_RegisterThreadMeshLocalAddrSelCb(ip6IfSelThreadMLSrcAddr6_t pfSelThreadMLSrcAddr6)
\brief  This function registers a callback needed by Thread Stack to select
        the source address when the destination address is a ML16 or ML64 .

\param  [in]  pfSelThreadMLSrcAddr6  Pointer to a callback to select the source address
***************************************************************************************************/
void IP_IF_RegisterThreadMeshLocalAddrSelCb(ip6IfSelThreadMLSrcAddr6_t pfSelThreadMLSrcAddr6);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_BindAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *ipAddr, ip6AddrType_t addrType,
                                 uint32_t lifetime, uint8_t prefixLength)
\brief  Binds an IP address to a hardware interface.

\param  [in]      ipIfId        IP interface identifier
\param  [in,out]  ipAddr        Pointer to the IP address or prefix if we want to use autoconfiguration
                                (NULL to use link local prefix)
\param  [in]      addrType      Manual/autoconfigurable address
\param  [in]      lifeTime      Address lifetime(IP6_ADDRESS_LIFETIME_INFINITE)
\param  [in]      prefixLength  Prefix length

\return           uint32_t      Result of the operation
***************************************************************************************************/
uint32_t IP_IF_BindAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *ipAddr, ip6AddrType_t addrType,
                                  uint32_t lifetime, uint8_t prefixLength);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_UnbindAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *ipAddr)
\brief  Unbinds an IP address from a hardware interface.

\param  [in]  ipIfId    IP interface identifier
\param  [in]  ipAddr    Pointer to the IP address or prefix if we want to unbind

\return       uint32_t  Result of the operation
***************************************************************************************************/
uint32_t IP_IF_UnbindAddr6(ipIfUniqueId_t ipIfId, ipAddr_t *ipAddr);

#endif /* IP_IP6_ENABLE */

/*!*************************************************************************************************
\fn     uint32_t IP_IF_GetInterfaceTableSize(void)
\brief  This function returns the size of the interface table.

\return    uint32_t  Interface table size
***************************************************************************************************/
uint32_t IP_IF_GetInterfaceTableSize(void);

/*!*************************************************************************************************
\fn     uint32_t IP_IF_GetMcastAddrTableSize(void)
\brief  This function returns the size of the multicast address table.

\return    uint32_t  Multicast Address table size
***************************************************************************************************/
uint32_t IP_IF_GetMcastAddrTableSize(void);

/*!*************************************************************************************************
\fn     ipIfStruct_t *IP_IF_GetInterfaceTableEntry(uint32_t ifNo)
\brief  This function returns interface table entry corresponding to the interface number ifNo.
\param  [in]  ifNo            Number of interface in table

\return       ipIfStruct_t *  Pointer to the interface entry
***************************************************************************************************/
ipIfStruct_t *IP_IF_GetInterfaceTableEntry(uint32_t ifNo);

/*!*************************************************************************************************
\fn     ip6MulticastAddrData_t *IP_IF_GetMcastAddrTableEntry(uint32_t index)
\brief  This function returns multicast address table entry corresponding to the index.
\param  [in]  ifNo                      Entry index

\return       ip6MulticastAddrData_t *  Pointer to the multicast address table entry
***************************************************************************************************/
ip6MulticastAddrData_t *IP_IF_GetMcastAddrTableEntry(uint32_t index);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*_IP_IF_MANAGEMENT_H_ */
