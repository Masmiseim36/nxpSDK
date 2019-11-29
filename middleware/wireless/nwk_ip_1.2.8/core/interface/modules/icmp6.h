/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _ICMP_6_H
#define _ICMP_6_H


/*!=================================================================================================
\file       icmp6.h
\brief      This is a header file for the ICMPv6 (RFC 4443) module. It contains the declarations of
            the interface functions.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/


/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! Enumeration for ICMPv6 informational messages */
typedef enum
{
    gIcmp6TypeDestinationUnreachable_c = 1U,  /*!< Destination Unreachable */
    gIcmp6TypePacketTooBig_c           = 2U,  /*!< Packet Too Big */
    gIcmp6TypeTimeExceeded_c           = 3U,  /*!< Time Exceeded */
    gIcmp6TypeParameterProblem_c       = 4U,  /*!< Parameter Problem */
    gIcmp6TypeEchoRequest_c            = 128U, /*!< Echo Request */
    gIcmp6TypeEchoReply_c              = 129U, /*!< Echo Reply */
}icmp6Type_t;

#define gIcmp6DefaultErrorCode_c            0U
/* Destination Unreachable codes */
#define gImcp6CodeNoRoute_c                 0U   /*!< No route to destination */
#define gImcp6CodeAdminProhibited_c         1U   /*!< Communication with destination administratively prohibited */
#define gImcp6CodeBeyondScope_c             2U   /*!< Beyond scope of source address */
#define gImcp6CodeAddressUnreachable_c      3U   /*!< Address unreachable */
#define gImcp6CodePortUnreachable_c         4U   /*!< Port unreachable */
#define gImcp6CodeAddressFailed_c           5U   /*!< Source address failed ingress/egress policy */
#define gImcp6CodeRejectRoute_c             6U   /*!< Reject route to destination */

/* Time Exceeded codes */
#define gImcp6CodeHopLimit_c                0U   /*!< Hop limit exceeded in transit */
#define gImcp6CodeFragReassembly_c          1U   /*!< Fragment reassembly time exceeded */

/* Parameter Problem codes */
#define gImcp6CodeHeader_c                  0U   /*!< Erroneous header field encountered */
#define gImcp6CodeNextHeader_c              1U   /*!< Unrecognized Next Header type encountered */
#define gImcp6CodeOption_c                  2U   /*!< Unrecognized IPv6 option encountered */

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
\fn     uint16_t ICMP_ComputeTxChecksum6(ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr,
                                         nwkBuffer_t *pIcmpMsg)
\brief  This function computes the ICMPv6 checksum for transmitted packets.
        The steps for checksum computation are described below:
        1: an IPv6 pseudo-header constructed and attached to the ICMPv6 message
        2: the checksum is computed based on the entire packet
        3: the attached IPv6 pseudo-header is freed

\param  [in]  pSrcIpAddr  Pointer to source IPv6 source address
\param  [in]  pDstIpAddr  Pointer to source IPv6 destination address
\param  [in]  pIcmpMsg    Pointer to the ICMPv6 message. The ICMPv6 header and payload use
                          nwkBuffer_t as data type.

\return       uint16_t    Computed checksum
***************************************************************************************************/
uint16_t ICMP_ComputeTxChecksum6(ipAddr_t *pSrcIpAddr, ipAddr_t *pDstIpAddr, nwkBuffer_t *pIcmpMsg);

/*!*************************************************************************************************
\fn     uint16_t ICMP_ComputeRxChecksum6(ipPktInfo_t *pIpPktInfo)
\brief  This function computes the ICMPv6 checksum for received packets. The ICMPv6 message is
        received in a linear buffer as a part of the IP packet.
        The steps for checksum computation are described below:
        1: an IPv6 pseudo-header constructed and attached to the ICMPv6 message
        2: the checksum is computed based on the entire packet
        3: the attached IPv6 pseudo-header is freed

\param  [in]  pIpPktInfo  Pointer to received IPv6 packet and additional IPv6 and link layer information.
                          pNextProt represents the start address of the ICMPv6 header.

\return       uint16_t    Computed checksum
***************************************************************************************************/
uint16_t ICMP_ComputeRxChecksum6(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     bool_t ICMP_CheckIfErrorCanBeSent6(uint32_t errorType, uint32_t errorCode,
                                           ipPktInfo_t *pErrorIpPktInfo)
\brief  This function verifies if an error must be sent in reply to receiving an incorrect IPv6
        packet.

\param  [in]  errorType        ICMPv6 error message type to be transmitted
\param  [in]  errorCode        ICMPv6 error message code to be transmitted
\param  [in]  pErrorIpPktInfo  Pointer to received incorrect IPv6 packet and additional IPv6 and
                               link layer information.

\return       bool_t           TRUE, returned when the error must be sent, FALSE otherwise
***************************************************************************************************/
bool_t ICMP_CheckIfErrorCanBeSent6(uint32_t errorType, uint32_t errorCode, ipPktInfo_t *pErrorIpPktInfo);

/*!*************************************************************************************************
\fn     bool_t ICMP_ReceivedError6(uint32_t type)
\brief  This function verifies if an error message is received.

\param  [in]  type    ICMPv6 message type

\return       bool_t  TRUE, returned when an error message is received, FALSE otherwise
***************************************************************************************************/
bool_t ICMP_ReceivedError6(uint32_t type);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _ICMP_6_H */
