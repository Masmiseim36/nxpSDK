/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _ICMP_4_H
#define _ICMP_4_H
/*!=================================================================================================
\file       icmp4.h
\brief      This is a header file for the ICMPv4 (RFC 792) module. It contains the declarations of
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
/* Enumeration for ICMPv4 message types */
typedef enum
{
    gIcmp4TypeEchoReply_c              = 0U,   /*!< Echo Reply */
    gIcmp4TypeDestinationUnreachable_c = 3U,   /*!< Destination Unreachable */
    gIcmp4TypeRedirect_c               = 5U,   /*!< Redirect */
    gIcmp4TypeEchoRequest_c            = 8U,   /*!< Echo Request */
    gIcmp4TypeTimeExceeded_c           = 11U,  /*!< Time Exceeded */
    gIcmp4TypeParameterProblem_c       = 12U,  /*!< Parameter Problem */
    gIcmp4TypeTimestampReply_c         = 14U,  /*!< Timestamp Reply */
}icmp4Type_t;

/* Destination Unreachable codes */
#define gImcp4CodeNetUnreachable_c       0U   /*!< Net unreachable */
#define gImcp4CodeHostUnreachable_c      1U   /*!< Host unreachable */
#define gImcp4CodeProtocolUnreachable_c  2U   /*!< Protocol unreachable */
#define gImcp4CodePortUnreachable_c      3U   /*!< Port unreachable */
#define gImcp4CodeFragNeeded_c           4U   /*!< Fragmentation needed and DF set */
#define gImcp4CodeSourceRouteFailed_c    5U   /*!< Source route failed */

/* Time Exceeded codes */
#define gImcp4CodeTtlExceeded_c          0U   /*!< Time to live exceeded in transit */
#define gImcp4CodeFragReassembly_c       1U   /*!< Fragment reassembly time exceeded */

/* Parameter Problem codes */
#define gImcp4CodeError_c                0U   /*!< Pointer indicates the error */

/* Redirect codes */
#define gImcp4CodeNetwork_c              0U   /*!< Redirect datagrams for the Network */
#define gImcp4CodeHost_c                 1U   /*!< Redirect datagrams for the Host */
#define gImcp4CodeToSNetwork_c           2U   /*!< Redirect datagrams for the ToS and Network */
#define gImcp4CodeToSHost_c              3U   /*!< Redirect datagrams for the ToS and Host */

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
\fn     uint16_t ICMP_ComputeRxChecksum4(ipPktInfo_t *pIpPktInfo)
\brief  This function computes the ICMPv4 checksum for received packets. The ICMPv4 message is
        received in a linear buffer as a part of the IP packet.
        The steps for checksum computation are described below:
        1: an ICMPv4 message of nwkBuffer_t type is constructed
        2: the checksum is computed based on the new ICMPv4 message
        3: the new ICMPv4 message is freed

\param  [in]   pIpPktInfo  Pointer to received IPv4 packet and additional IPv4 and link layer information.
                           pNextProt represents the start address of the ICMPv4 header.

\return        uint16_t    Computed checksum
***************************************************************************************************/
uint16_t ICMP_ComputeRxChecksum4(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     bool_t ICMP_ReceivedError4(uint32_t type)
\brief  This function verifies if an error message is received

\param  [in]  type     ICMPv4 message type

\return       bool_t   TRUE, returned when an error message is received,FALSE otherwise
***************************************************************************************************/
bool_t ICMP_ReceivedError4(uint32_t type);

/*!*************************************************************************************************
\fn     bool_t ICMP_CheckIfErrorCanBeSent4(uint32_t errorType, uint32_t errorCode,
                                           ipPktInfo_t *pErrorIpPktInfo)
\brief  This function verifies if an error must be sent in reply to receiving an incorrect IPv4
        packet.

\param  [in]  errorType        ICMPv4 error message type to be transmitted
\param  [in]  errorCode        ICMPv4 error message code to be transmitted
\param  [in]  pErrorIpPktInfo  pointer to received incorrect IPv4 packet and additional IPv4 and
                               link layer information.

\retval       TRUE   Returned when the error must be sent
\retval       FALSE  Otherwise
***************************************************************************************************/
bool_t ICMP_CheckIfErrorCanBeSent4(uint32_t errorType, uint32_t errorCode, ipPktInfo_t *pErrorIpPktInfo);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _ICMP_4_H */
