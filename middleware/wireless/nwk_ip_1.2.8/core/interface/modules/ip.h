/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IP_H_
#define _IP_H_

/*!
\file       ip.h
\brief      This is a header file for the IP layer.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"
#include "TimersManager.h"

/*==================================================================================================
Public macros
==================================================================================================*/


#ifndef IP_IP6_ENABLE
   #define IP_IP6_ENABLE 0
#endif

#ifndef IP_IP4_ENABLE
   #define IP_IP4_ENABLE 0
#endif

#if (IP_IP6_ENABLE == 0) && (IP_IP4_ENABLE == 0)
#error "Cannot disable both IPv6 and IPv4"
#endif

#ifndef IP_IF_STATS_ENABLE
    #define IP_IF_STATS_ENABLE 0
#endif

#define IP_VERSION_4       0x04        /* Version 4 */
#define IP_VERSION_6       0x06        /* Version 6 */

/* IP protocol types */
#if !X86
#define IPPROTO_HOPOPTS    0        /* IPv6 hop-by-hop options            */
#define IPPROTO_ICMP       1        /* Internet Control Message Protocol  */
#define IPPROTO_IGMP       2        /* Internet Group Management Protocol */
#define IPPROTO_IP         4        /* IP-in-IP encapsulation             */
#define IPPROTO_IPIP       4        /* IP-in-IP encapsulation             */
#define IPPROTO_TCP        6        /* Transmission Control Protocol      */
#define IPPROTO_UDP        17       /* User Datagram Protocol             */
#define IPPROTO_IPV6       41       /* IPv6-in-IP encapsulation           */
#define IPPROTO_ROUTING    43       /* IPv6 routing header                */
#define IPPROTO_FRAGMENT   44       /* IPv6 fragmentation header          */
#define IPPROTO_ESP        50       /* Encapsulating Security Payload     */
#define IPPROTO_AH         51       /* Authentication Header              */
#define IPPROTO_ICMPV6     58       /* ICMPv6                             */
#define IPPROTO_NONE       59       /* IPv6 no next header                */
#define IPPROTO_DSTOPTS    60       /* IPv6 destination options           */
#define IPPROTO_OSPF       89       /* Open Shortest Path Protocol        */
#define IPPROTO_COMP       108      /* IP compression                     */
#endif

#define IP6_ADDRESS_LIFETIME_INFINITE (0xFFFFFFFF)

/* IP Traffic Class types */
#define IP_PRIORITY_TCLASS_0     0x00   /*!< IP precedence 0: Best Effort */
#define IP_PRIORITY_TCLASS_1     0x20   /*!< IP precedence 1: Priority */
#define IP_PRIORITY_TCLASS_2     0x40   /*!< IP precedence 2: Immediate */
#define IP_PRIORITY_TCLASS_3     0x60   /*!< IP precedence 3: Flash */
#define IP_PRIORITY_TCLASS_4     0x80   /*!< IP precedence 4: Flash Override */
#define IP_PRIORITY_TCLASS_5     0xA0   /*!< IP precedence 5: Critical  */
#define IP_PRIORITY_TCLASS_6     0xC0   /*!< IP precedence 6: Internetwork Control */
#define IP_PRIORITY_TCLASS_7     0xE0   /*!< IP precedence 7: Network Control */

/* ip statistics macros */
#if IP_IP6_STATS_ENABLE
    #define IP6_STATS_ENABLED(x) x
#else
    #define IP6_STATS_ENABLED(x)
#endif

/* ip statistics macros */
#if IP_IP4_STATS_ENABLE
    #define IP4_STATS_ENABLED(x) x
#else
    #define IP4_STATS_ENABLED(x)
#endif

#if IP_IF_STATS_ENABLE
    #define IPIF_STATS_ENABLED(x) x
#else
    #define IPIF_STATS_ENABLED(x)
#endif

#ifndef IP_ENABLE_REASSEMBLY
#define IP_ENABLE_REASSEMBLY    (1)    /*!< Define for IP reassembly enable */
#endif

#define IP_REASSEM_EXPIRE_TIME 60000U

#define IP_TIMER_PERIOD_MS      1000U                      /*!< Timer interval in milliseconds */
#define IP_TIMER_PERIOD_SEC     1U

/*==================================================================================================
Public type definitions
==================================================================================================*/

typedef enum
{
    gIpOk_c = 0x00000000U,
    gIpGeneralError_c,
    gIpBadAddrError_c,
    gIpUnreachableError_c,
    gIpInvalidParameterError_c,
    gIpNoAddressSpaceError_c,
    gIpReasemFrag_c,
    gIpReasemDatagram_c,
    gIpMemoryAllocError_c,
    gIpDuplicatePktTx_c,
    gIpUnusedError_c = 0xFFFFFFFF,
}ipErrorTypes_t;

typedef enum
{
    gIpProtv4_c = 0x0800,
    gIpProtv6_c = 0x86DD,
    gIpProtArp_c = 0x0806
}ipProtocolType_t;

typedef enum
{
    gIpForwardTx_c,
    gIpForwardRx_c
}ipForwardDirection_t;

typedef enum
{
    gIpEnetMetric_c = 10,
    gIpWifiMetric_c = 20U,
    gIpUsbMetric_c = 30U,
    gIpTunMetric_c = 30U,
    gIp6LowPanMetric_c = 40U,
    gIpLoopbackMetric_c = 50U,
}ipIfMetric_t;

#if IP_IF_STATS_ENABLE
typedef struct ipCommonStats_tag
{
   uint32_t  rxTotal;
   uint32_t  rxMissed;
   uint32_t  rxDiscarded;
   uint32_t  rxErrors;

   uint32_t  txTotal;
   uint32_t  txMissed;
   uint32_t  txDiscarded;
   uint32_t  txErrors;
}ipCommonStats_t;


typedef struct ip6Stats_tag
{
   ipCommonStats_t commonStats;

   uint32_t  rxHdrErrors;       /* Discarded -- error in IP header    */
   uint32_t  rxAddrErrors;      /* Discarded -- illegal destination   */
   uint32_t  rxNoProto;         /* Discarded -- unrecognized protocol */
   uint32_t  rxDelivered;       /* Datagrams delivered to upper layer */
   uint32_t  rxForwarded;       /* Datagrams forwarded                */

   /* These are included in rxDiscarded and rxHdrErrors */
   uint32_t  rxBadVersion;      /* Datagrams with version != 6        */
   uint32_t  rxBadSource;       /* Datagrams with invalid src address */
   uint32_t  rxSmallPkt;        /* Datagrams larger than frame        */
   uint32_t  rxTtlExceeded;     /* Datagrams to route with TTL = 0    */

   uint32_t  rxFragRecvd;       /* Number of received IP fragments    */
   uint32_t  rxFragReasmd;      /* Number of reassembled datagrams    */
   uint32_t  rxFragDiscarded;   /* Number of discarded fragments      */

   uint32_t  txFragSent;        /* Number of sent fragments           */
   uint32_t  txFragFragd;       /* Number of fragmented datagrams     */
   uint32_t  txFragDiscarded;   /* Number of fragmentation failures   */

}ip6Stats_t;


typedef struct ip4Stats_tag
{
   ipCommonStats_t commonStats;

   uint32_t  rxHdrErrors;       /* Discarded -- error in IP header    */
   uint32_t  rxAddrErrors;      /* Discarded -- illegal destination   */
   uint32_t  rxNoProto;         /* Discarded -- unrecognized protocol */
   uint32_t  rxDelivered;       /* Datagrams delivered to upper layer */
   uint32_t  rxForwarded;       /* Datagrams forwarded                */

   /* These are included in rxDiscarded and rxHdrErrors */
   uint32_t  rxBadVersion;      /* Datagrams with version != 4        */
   uint32_t  rxBadChecksum;     /* Datagrams with invalid checksum    */
   uint32_t  rxBadSource;       /* Datagrams with invalid src address */
   uint32_t  rxSmallHdr;        /* Datagrams with header too small    */
   uint32_t  rxSmallPkt;        /* Datagrams larger than frame        */
   uint32_t  rxTtlExceeded;     /* Datagrams to route with TTL = 0    */

   uint32_t  rxFragRecvd;       /* Number of received IP fragments    */
   uint32_t  rxFragReasmd;      /* Number of reassembled datagrams    */
   uint32_t  rxFragDiscarded;   /* Number of discarded fragments      */

   uint32_t  txFragSent;        /* Number of sent fragments           */
   uint32_t  txFragFragd;       /* Number of fragmented datagrams     */
   uint32_t  txFragDiscarded;   /* Number of fragmentation failures   */

}ip4Stats_t;

typedef struct ipIfStats_tag
{
   ipCommonStats_t commonStats;

   uint32_t  rxOctets;           /* total bytes received       */
   uint32_t  rxUnicast;          /* unicast packets received   */
   uint32_t  rxMulticast;        /* multicast packets received */
   uint32_t  rxBroadcast;        /* broadcast packets received */

   uint32_t  txOctets;           /* total bytes sent           */
   uint32_t  txUnicast;          /* unicast packets sent       */
   uint32_t  txMulticast;        /* multicast packets sent     */
   uint32_t  txBroadcast;        /* broadcast packets sent     */

}ipIfStats_t;
#endif

typedef struct ipReasemStruct_tag
{
    /*! datagram ID used for reassembly */
    uint32_t id;
    /*! total length of the fragmented part of the IP datagram */
    uint32_t totalLen;
    /*! unfragmentable part size */
    uint16_t unfragtLen;
    /*! next header value of the first fragment header */
    uint8_t nextHdr;
    /*! dummy byte used for alignment */
    uint8_t dummy;
    /*! handle for timer event used for IP reassembly timeout */
    tmrTimerID_t timerID;
}ipReasemStruct_t;

/*! Function for servicing transport packets */
typedef void (* ipTransportService)(ipPktInfo_t* );

/*! Function for packet forwarding */
typedef void (* ipForwardService)(ipPktInfo_t *);

typedef struct ipTransportServiceStruct_tag
{
   /*! Owner's service function */
   ipTransportService service;
   /*! Next header identifier */
   uint32_t protocol;
} ipTransportServiceStruct_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern taskMsgQueue_t * pIpMsgQueue;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void IP_Task(osaTaskParam_t argument)
\brief  IP layer task.

\param  [in]  argument   Task private data
***************************************************************************************************/
void IP_Task(osaTaskParam_t argument);

/*!*************************************************************************************************
\fn      taskMsgQueue_t *IP_Init(void)
\brief   Initializes the IP layer.

\return  taskMsgQueue_t *     Pointer to IP Task Queue
***************************************************************************************************/
taskMsgQueue_t *IP_Init(void);

/*!*************************************************************************************************
\fn     taskMsgQueue_t *Ip_TaskMsgQueueInit(void)
\brief  Initialize IP task message queue (if enabled can create IP Task).

\return  [out]  taskMsgQueue_t *    Pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t *Ip_TaskMsgQueueInit(void);

/*!*************************************************************************************************
\fn     taskMsgQueue_t *Ip_TaskMsgQueueInit(void)
\brief  Initialize IP task message queue (if enabled can create IP Task).

\return  [out]  taskMsgQueue_t *    Pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t* Tcp_TaskMsgQueueInit(void);

/*!*************************************************************************************************
\fn     uint32_t IP_Send(ipPktInfo_t *pIpPktInfo, uint32_t  protocol)
\brief  Sends an IP packet generated on the local host. Determines based on source and destination.
        address the IP protocol(v4, v6).

\param  [in]  pIpPktInfo  Pointer to the packet to send
\param  [in]  protocol    The upper layer protocol

\return       uint32_t    Error
***************************************************************************************************/
uint32_t IP_Send(ipPktInfo_t *pIpPktInfo, uint32_t  protocol);

/*!*************************************************************************************************
\fn     void IP_Receive(ipPktInfo_t *pIpPktInfo, uint32_t  protocol)
\brief  Receives a data packet from a media interface and, based on the received protocol(IPv4,
        IPv6) sends a message to the IP task appropriate function.

\param  [in]  pIpPktInfo  Pointer to the packet to send
\param  [in]  protocol    The upper layer protocol
***************************************************************************************************/
void IP_Receive(ipPktInfo_t *pIpPktInfo, uint32_t  protocol);

/*!*************************************************************************************************
\fn     ipTransportServiceStruct_t *IP_GetTransportServiceStruct(uint32_t transportProtocol)
\brief  Returns the pointer to the ipTransportServiceStruct_t that matches the provided protocol.

\param  [in]  transportProtocol               Transport protocol next header value

\return       ipTransportServiceStruct_t *    Pointer to transport service descriptor structure
***************************************************************************************************/
ipTransportServiceStruct_t *IP_GetTransportServiceStruct(uint32_t transportProtocol);

/*!*************************************************************************************************
\fn     uint32_t IP_SetTransportServiceStruct(ipTransportServiceStruct_t *pServiceStructData)
\brief  Registers a transport layer protocol callback with the IP layer.

\param  [in]  pServiceStructData  Pointer to service data structure

\return       uint32_t            gIpOk_c if OK, gIpGeneralError_c if error
***************************************************************************************************/
uint32_t IP_SetTransportServiceStruct(ipTransportServiceStruct_t *pServiceStructData);

/*!*************************************************************************************************
\fn     uint32_t IP_GetUpperLayerProt(uint8_t *pIpDatagram,ipAddr_t *pIpDstAddr,
                                      uint32_t *pUpperLayerOffset)
\brief  Processes an IP datagram and returns the upper layer transport protocol present in the
        IP datagram.

\param  [in]   pIpDatagram        Pointer to the start of IP datagram
\param  [in]   pIpDstAddr         Pointer to destination ipAddr to determine if packet is IP6/4
\param  [out]  pUpperLayerOffset  Can be NULL. When not NULL it shall indicate the offset to upper
                                  transport layer data from start of IP datagram

\return        uint32_t           The upper layer protocol present in the IP datagram
***************************************************************************************************/
uint32_t IP_GetUpperLayerProt(uint8_t *pIpDatagram,ipAddr_t *pIpDstAddr,
                              uint32_t *pUpperLayerOffset);

#if IP_ENABLE_REASSEMBLY
/*!*************************************************************************************************
\fn     ipPktInfo_t *IP_AddReasemDatagram(ipPktInfo_t *pInData, uint32_t id,
                                          list_t *pIpReasemblyList)
\brief  Find the ipPktInfo_t type datagram descriptor matching the received IP fragment based on
        ipsrc, ipdst, proto and id or creates a new one.

\param  [in]  pInData            Pointer to the packet to process
\param  [in]  id                 Datagram ID
\param  [in]  pIpReasemblyList   Pointer to the descriptor of the list that keeps the all
                                 the datagrams waiting reassembly

\return       ipPktInfo_t *      Pointer to the IP datagram descriptor
***************************************************************************************************/
ipPktInfo_t *IP_AddReasemDatagram(ipPktInfo_t *pInData, uint32_t id,
                                  list_t *pIpReasemblyList);

/*!*************************************************************************************************
\fn     void IP_AddDatagramFrag(ipPktInfo_t *dgram, ipPktInfo_t **pInData, uint8_t *pFragData,
                                uint32_t fragLen, uint32_t fragOffset)
\brief  Adds a received IP fragment to an ipPktInfo_t datagram.

\param  [in]  dgram       Pointer to ipPktInfo_t type datagram that holds the fragments received so far
\param  [in]  pInData     Double pointer to the IP fragment to add
\param  [in]  pFragData   Pointer to the fragment data
\param  [in]  fragLen     Length of the fragment data
\param  [in]  fragOffset  Offset in the original datagram of the fragment data
***************************************************************************************************/
void IP_AddDatagramFrag(ipPktInfo_t *dgram, ipPktInfo_t **pInData, uint8_t *pFragData,
                        uint32_t fragLen, uint32_t fragOffset);

/*!*************************************************************************************************
\fn     bool_t IP_CanReasemDgram(ipPktInfo_t *dgram)
\brief  Checks if a datagram can be reassembled by looking if all the received fragments combined
        fill the entire original datagram data.

\param  [in]  dgram    Pointer to ipPktInfo_t type datagram that holds the fragments received so far

\return       bool_t   TRUE if datagram can be reassembled, FALSE otherwise
***************************************************************************************************/
bool_t IP_CanReasemDgram(ipPktInfo_t *dgram);

/*!*************************************************************************************************
\fn     void IP_ReasemDgram(ipPktInfo_t **dgram)
\brief  Reassembles an ipPktInfo_t datagram from all the IP fragments.

\param  [in]  dgram  Double pointer to ipPktInfo_t type datagram that holds the fragments received so far
***************************************************************************************************/
void IP_ReasemDgram(ipPktInfo_t **dgram);
#endif

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*_IP_H_ */
