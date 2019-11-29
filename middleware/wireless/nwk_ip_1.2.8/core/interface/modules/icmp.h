/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ICMP_H
#define _ICMP_H

/*!
\file       icmp.h
\brief      This is a header file for the ICMP (ICMPv6 and ICMPv4) module. It contains the declarations
            of the interface functions.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "icmp6.h"
#include "icmp4.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Description of the ICMP_STATISTICS_ENABLED configuration option: set to 1 to enable statistics */
#ifndef ICMP_STATISTICS_ENABLED
#define ICMP_STATISTICS_ENABLED 0
#endif

/*! Description of the ICMP_UNREGISTER_MSG_TYPE_HANDLER_ENABLED configuration option: set to 1 to
enable unregister message type handler functionality */
#ifndef ICMP_UNREGISTER_MSG_TYPE_HANDLER_ENABLED
#define ICMP_UNREGISTER_MSG_TYPE_HANDLER_ENABLED 0
#endif

/*! Description of the ICMP_USE_ASYNC_SEND configuration option: If this option is enabled, the module
sends packets asynchronously using OS messages. This will save stack size but at the cost of code size
and slower operation. */
#ifndef ICMP_USE_ASYNC_SEND
#define ICMP_USE_ASYNC_SEND 0
#endif

#define ICMP_DEFAULT_HOP_LIMIT     128        /*!< Default hop limit value */
#define ICMP_CODE_DEFAULT          0U         /*!< Default code */
#define ICMP_CODE_ANY              0xFFU      /*!< Any code */
#define ICMP_HEADER_SIZE           4U         /*!< Header size */
#define ICMP_CODE_OFFSET           1U         /*!< Code offset */
#define ICMP_CHECKSUM_OFFSET       2U         /*!< Checksum offset */
#define ICMP_ERROR_PARAM_OFFSET    4U         /*!< Error parameter offset */
#define ICMP_ERROR_PAYLOAD_OFFSET  8U         /*!< Error payload offset */
#define ICMP_ECHO_ID_OFFSET        4U         /*!< Echo ID offset */
#define ICMP_ECHO_SEQ_NO_OFFSET    6U         /*!< Echo sequence number offset */

#define ICMP_CHECKSUM_SIZE         2U         /*!< Checksum size */
#define ICMP_IDENTIFIER_SIZE       2U         /*!< Identifier size */
#define ICMP_SEQUENCE_NUMBER_SIZE  2U         /*!< Sequence number size */

#define ICMP_HEADER_EXTENDED_SIZE  8U         /*!< Use this macro when allocating the ICMP header
                                                   and payload for messages defined in RFC 4443 */
#define ICMP_PARAM_SIZE            4U         /*!< Parameter size */

#define ICMP_RESERVED              255U       /*!< Reserved */

#define ICMP_DIRECTION_UP          1U         /*!< Message received from IP layer */
#define ICMP_DIRECTION_DOWN        2U         /*!< Message to be sent to IP layer */

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! ICMP receive callback */
typedef void (*icmpMessageTypeHandlerCb_t)(ipPktInfo_t *pIpPktInfo);

/*! ICMP statistics structure  */
typedef struct icmpStats_tag
{
    uint32_t count;
    uint8_t  type;
}icmpStats_t;

/*! Receiving callback(s) registering structure */
typedef struct icmpProtMsgTypeHandler_tag
{
    icmpMessageTypeHandlerCb_t icmpMessageTypeHandlerCb;    /*!< Pointer to callback */
    uint8_t type;                                           /*!< Any value accepted */
    uint8_t code;                                           /*!< Use ICMP_CODE_ANY to accept all codes */
}icmpProtMsgTypeHandler_t;

/*! Message types handler structure */
typedef struct icmpMsgTypeHandler_tag
{
    void* pIfHandle;                                      /*!< Pointer to interface handler */
    icmpProtMsgTypeHandler_t* pIcmpProtMsgTypeHandler;    /*!< Pointer to receiving callback structure */
    uint8_t protocol;                                     /*!< Protocol */
    uint8_t* pIcmpProtMsgTypeHandlerSize;                 /*!< Pointer to size of ICMP protocol message type handler */
    uint8_t padding[2];                                   /*!< Padding bytes */
}icmpMsgTypeHandler_t;

/*! Enumeration for ICMP statistics types */
typedef enum
{
    gIcmpStatsTypeBadChecksum_c = 250U,                  /*!< Bad checksum */
    gIcmpStatsTypeTotalSuccess_c = 251U,                 /*!< Total success */
    gIcmpStatsTypeTotalFailed_c = 252U,                  /*!< Total failed */
#if IP_IP6_ENABLE
    gIcmpStatsTypeDestinationUnreachable6_c = gIcmp6TypeDestinationUnreachable_c,   /*!< IPv6 unreachable destination */
    gIcmpStatsTypePacketTooBig6_c = gIcmp6TypePacketTooBig_c,                       /*!< IPv6 packet too big */
    gIcmpStatsTypeTimeExceeded6_c = gIcmp6TypeTimeExceeded_c,                       /*!< IPv6 time exceeded */
    gIcmpStatsTypeParameterProblem6_c = gIcmp6TypeParameterProblem_c,               /*!< IPv6 parameter problem */
    gIcmpStatsTypeEchoRequest6_c = gIcmp6TypeEchoRequest_c,                         /*!< IPv6 echo request */
    gIcmpStatsTypeEchoReply6_c = gIcmp6TypeEchoReply_c,                             /*!< IPv6 echo reply */
#endif /* IP_IP6_ENABLE */

#if IP_IP4_ENABLE
    gIcmpStatsTypeDestinationUnreachable4_c = gIcmp4TypeDestinationUnreachable_c,  /*!< IPv4 unreachable destination */
    gIcmpStatsTypeTimeExceeded4_c = gIcmp4TypeTimeExceeded_c,                      /*!< IPv4 time exceeded */
    gIcmpStatsTypeParameterProblem4_c = gIcmp4TypeParameterProblem_c,              /*!< IPv4 parameter problem */
    gIcmpStatsTypeEchoRequest4_c = gIcmp4TypeEchoRequest_c,                        /*!< IPv4 echo request */
    gIcmpStatsTypeEchoReply4_c = gIcmp4TypeEchoReply_c,                            /*!< IPv4 echo reply */
    gIcmpStatsTypeRedirect4_c = gIcmp4TypeRedirect_c,                              /*!< IPv4 redirect */
    gIcmpStatsTypeTimestampReply4_c = gIcmp4TypeTimestampReply_c,                  /*!< IPv4 timestamp reply */
#endif /* IP_IP4_ENABLE */
}icmpStatsType_t;

/*! Enumeration for ICMP statistics direction */
typedef enum
{
    gIcmpStatsDirReceive_c = 0U,  /*!< Receive */
    gIcmpStatsDirTransmit_c,      /*!< Transmit */
}icmpStatsDirection_t;

/*! Enumeration for ICMP statistics protocol */
typedef enum
{
    gIcmpStatsProtIcmp6_c = 58U,  /*!< ICMPv6 */
    gIcmpStatsProtIcmp4_c = 1U,   /*!< ICMPv4 */
}icmpStatsProtocol_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern const icmpMsgTypeHandler_t gaIcmpMsgTypeHandlerTable[];
extern const uint8_t gIcmpMsgTypeHandlerSize;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     void ICMP_Init(taskMsgQueue_t *pTaskMsgQueue)
\brief  This function initializes the ICMP module (e.g. resets the statistics).

\param  [in]  pTaskMsgQueue  Pointer to the task message queue.
***************************************************************************************************/
void ICMP_Init(taskMsgQueue_t *pTaskMsgQueue);


#if ICMP_UNREGISTER_MSG_TYPE_HANDLER_ENABLED

/*!*************************************************************************************************
\fn     bool_t ICMP_UnregisterMsgTypeHandler(uint8_t protocol, void *pIfHandle,
                                             icmpProtMsgTypeHandler_t *pIcmpProtMsgTypeHandler)
\brief  This function unregisters all callback handlers corresponding to the "protocol" parameter and to
        "pIcmpProtMsgTypeHandler" parameter

\param  [in]  protocol                 Protocol number as assigned by IANA (e.g. IPPROTO_ICMPV6,
                                       IPPROTO_ICMP, IPPROTO_UDP, IPPROTO_TCP)
\param  [in]  pIfHandle                Pointer to the media interface handler (if known) or NULL.
                                       When set to NULL this callback is unregistered on all interfaces
\param  [in]  pIcmpProtMsgTypeHandler  Pointer to a table containing one or more ICMP message type,
                                       ICMP message code and icmpMessageTypeHandlerCb_t entries

\return       bool_t                    Success or failure of the operation
***************************************************************************************************/
bool_t ICMP_UnregisterMsgTypeHandler(uint8_t protocol, void *pIfHandle,
                                     icmpProtMsgTypeHandler_t *pIcmpProtMsgTypeHandler);

#endif /* ICMP_UNREGISTER_MSG_TYPE_HANDLER_ENABLED */

/*!*************************************************************************************************
\fn     void ICMP_Receive(ipPktInfo_t *pIpPktInfo)
\brief  This function is called by IP layer whenever an ICMP packet is received

\param  [in]  pIpPktInfo  Pointer to received IP packet and additional IP and link layer information.
                          pNextProt represents the start address of the ICMP header.
                          If the pIpPktInfo pointer or its pIpDstAddr, pIpSrcAddr, pIpPktOptions
                          and pNwkBuff fields are not valid then the message is not processed.
                          This parameter is freed before returning from this function.
***************************************************************************************************/
void ICMP_Receive(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     void ICMP_SendError(ipPktInfo_t **ppErrorIpPktInfo, uint8_t type, uint8_t code, uint32_t param)
\brief  This function is called for sending an ICMP error packet to IP transmission module.

\param  [in]  ppErrorIpPktInfo  Double pointer to the received incorrect IP packet and additional IP and link
                                layer information. If the incorrect IP packet must not generate an error
                                then the error is not be sent.
                                pErrorIpPktInfo->pNwkBuff->pData must point to the IP header
                                This parameter will be freed in this function.
\param  [in]  type              ICMP error message type
\param  [in]  code              ICMP error message code
\param  [in]  param             ICMP error parameter (the first 4 bytes after the ICMP header)
***************************************************************************************************/
void ICMP_SendError(ipPktInfo_t **ppErrorIpPktInfo, uint8_t type, uint8_t code, uint32_t param);

/*!*************************************************************************************************
\fn     void ICMP_Send(ipPktInfo_t *pIpPktInfo, uint8_t type, uint8_t code)
\brief  This function is called for sending a generic ICMP packet to IP transmission module.

\param  [in]  pIpPktInfo  Pointer to the IP packet and additional IP and link layer information
                          that will be sent to IP layer
                          If the pIpPktInfo pointer or its pIpDstAddr, pIpPktOptions and pNwkBuff
                          fields are not valid then the message is not sent and the pIpPktInfo is freed.
                          The pointer to pIpSrcAddrIP source address may be missing from pIpPktInfo.
                          In this case the source address will be determined based on the
                          destination address.
\param  [in]  type        ICMP message type
\param  [in]  code        ICMP message code
***************************************************************************************************/
void ICMP_Send(ipPktInfo_t* pIpPktInfo, uint8_t type, uint8_t code);

/*!*************************************************************************************************
\fn     uint32_t ICMP_GetStatistics(icmpStatsProtocol_t icmpStatsProtocol,
                              icmpStatsDirection_t icmpStatsDirection, icmpStatsType_t icmpStatsType)
\brief  This function is used for the reading of the ICMP statistics.

\param  [in]  icmpStatsProtocol  ICMP statistics protocol (ICMPv6 or ICMPv4)
\param  [in]  icmpStatsDirection ICMP statistics direction (reception or transmission)
\param  [in]  icmpStatsType      ICMP statistics

\retval       uint32_t           Number of statistics
***************************************************************************************************/
uint32_t ICMP_GetStatistics(icmpStatsProtocol_t icmpStatsProtocol, icmpStatsDirection_t icmpStatsDirection,
                            icmpStatsType_t icmpStatsType);

/*!*************************************************************************************************
\fn     void ICMP_ResetStatistics(void)
\brief  This function is used for reseting the ICMP statistics
***************************************************************************************************/
void ICMP_ResetStatistics(void);

/*!*************************************************************************************************
\fn     uint16_t ICMP_ComputeTxChecksum(uint32_t protocol, ipAddr_t *pSrcIpAddr,
                                               ipAddr_t* pDstIpAddr, nwkBuffer_t* pIcmpMsg)
\brief  This function computes the ICMP (v4 or v6) checksum for a packet to be transmitted.

\param  [in]   protocol     Used protocol (ICMPv4 or ICMPv6)
\param  [in]   pSrcIpAddr   Pointer to source IP address (either IPv6 or IPv4)
\param  [in]   pDstIpAddr   Pointer to destination IP address (either IPv6 or IPv4)
\param  [in]   pIcmpMsg     Pointer to the ICMP message (either ICMPv6 or ICMPv4)

\return        uint16_t     Computed checksum
***************************************************************************************************/
uint16_t ICMP_ComputeTxChecksum(uint32_t protocol, ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr,
                                            nwkBuffer_t *pIcmpMsg);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _ICMP_H */
