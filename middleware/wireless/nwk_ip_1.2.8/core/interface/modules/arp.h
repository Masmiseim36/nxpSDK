/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ARP_H_
#define _ARP_H_

/*!
\file       arp.h
\brief      This is a public header file for the ARP module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "ip_if_management.h"

#if IP_IP4_ENABLE
/*==================================================================================================
Public macros
==================================================================================================*/
/*  MACROS THAT CAN BE SET AT COMPILE TIME (BY THE USER) */
#ifndef ARP_CACHE_SIZE
#define ARP_CACHE_SIZE                (5U)    /*!< ARP cache size */
#endif

#ifndef ARP_MAX_LIFETIME
#define ARP_MAX_LIFETIME          (1200U)    /*!< ARP maximum lifetime */
#endif

#ifndef ARP_PENDING_LIFETIME
#define ARP_PENDING_LIFETIME         (5U)    /*!< ARP pending lifetime */
#endif

#ifndef ARP_UNREACHABLE_LIFETIME
#define ARP_UNREACHABLE_LIFETIME    (20U)    /*!< ARP unreachable lifetime */
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/
 /*! Enumeration for ARP codes */
typedef enum
{
    gArpCodeRequest_c =0X0001U,         /*!< ARP request code */
    gArpCodeReply_c =0x0002U            /*!< ARP reply code */
}arpCode_t;

/*! Enumeration for ARP states */
typedef enum
{
    gArpStateEmpty_c    = 0U,           /*!< ARP empty state */
    gArpStateResolved_c,                /*!< ARP resolved state */
    gArpStatePending_c,                 /*!< ARP pending state */
    gArpStateUnreachable                /*!< ARP unreachable state */
} arpState_t;

/*! ARP packet structure */
typedef struct arpPacket_tag
{
    uint8_t linkLayerType[2];      /*!< Link layer type */
    uint8_t protoType[2];          /*!< Protocol type */
    uint8_t linkLayerLength;       /*!< Link layer length */
    uint8_t protoLength;           /*!< Protocol length */
    uint8_t opCode[2];             /*!< Operation code */
    uint8_t senderMACAddr[6];      /*!< Sender MAC address */
    uint8_t senderIPaddr[4];       /*!< Sender IP address */
    uint8_t targetMACaddr[6];      /*!< Targeted MAC address */
    uint8_t targetIPaddr[4];       /*!< Targeted IP address */
} arpPacket_t;

/*! ARP cache entry structure */
typedef struct arpCacheEntry_tag
{
    uint32_t  ipAddress;            /*!< IP address */
    ifHandle_t IfHandle;            /*!< Interface handle */
    uint32_t lifetime;              /*!< Entry lifetime */
    ipPktInfo_t *pWaitingPacket;    /*!< Pointer to waiting packet */
    llAddr_t llAdress;              /*!< Link layer address */
    uint8_t state;                  /*!< Entry state */
    bool_t usedEntry;               /*!< Used entry flag */
} arpCacheEntry_t;

/*! Function for interfacing ARP module with LL Driver Send */
typedef void (* arpSend)(uint8_t*, uint32_t, llAddr_t*);

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
\fn     uint32_t ARP_Open(ifHandle_t ifHandle)
\brief  Interface function for the ARP module. It starts the ARP on an interface.

\param  [in]  pIfHandle  Pointer to interface handle

\return       uint32_t   Status
***************************************************************************************************/
uint32_t ARP_Open(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     bool_t ARP_Close(ifHandle_t ifHandle)
\brief  Interface function for the ARP module. It stops the ARP on an interface.

\param [in]  pIfHandle   Pointer to interface handle

\return      bool_t      Status
***************************************************************************************************/
bool_t ARP_Close(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     llAddr_t *void ARP_Resolve(ipPktInfo_t *pIpPktInfo)
\brief  This function finds the matching link layer address for the given IP address or sends an
        ARP packet request if it is not found.

\param  [in]  pIpPktInfo  Pointer to the IP packet

\return       llAddr_t *  Pointer to the matching link layer address for the given IP address
***************************************************************************************************/
llAddr_t *ARP_Resolve(ipPktInfo_t *pIpPktInfo);

/*!*************************************************************************************************
\fn     void ARP_Service(uint8_t* pInData)
\brief  This function parses a received ARP packet.

\param  [in]  pInData  Pointer to the received packet
***************************************************************************************************/
void ARP_Service(uint8_t* pInData);

/*!*************************************************************************************************
\fn     void ARP_ManageCache(uint32_t timerInterval)
\brief  This function manages the ARP cache entries.

\param  [in]  timerInterval  The time interval that passed since the last call of the function
***************************************************************************************************/
void ARP_ManageCache(uint32_t timerInterval);

#ifdef __cplusplus
}
#endif

#endif
/*================================================================================================*/
#endif  /*_ARP_H_ */
