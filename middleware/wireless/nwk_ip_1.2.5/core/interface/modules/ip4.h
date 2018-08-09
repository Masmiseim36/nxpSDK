/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    #define IP_IP4_ENABLE_REASSEMBLY 0      /*!< IPv4 reassembly enable */
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
\fn     void IP4_Service(void *pInData)
\brief  Parses a received IP4 packet.

\param  [in]  pInData  The received packet
***************************************************************************************************/
void IP4_Service(void *pInData);

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
