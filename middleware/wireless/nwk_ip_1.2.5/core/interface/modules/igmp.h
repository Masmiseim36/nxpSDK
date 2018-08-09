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

#ifndef _IGMP_H
#define _IGMP_H

/*!
\file       igmp.h
\brief      This is a header file for the IGMP module. It contains the declarations of
            the interface functions.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Description of the IGMP_V3_SOURCE_FILTERING_ENABLED configuration option: set to 1 to enable
    the source filtering functionality */
#ifndef IGMP_V3_SOURCE_FILTERING_ENABLED
    #define IGMP_V3_SOURCE_FILTERING_ENABLED 0
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! IGMP Multicast Group Filter Mode structure */
typedef enum
{
    gIgmpFilterModeInclude_c,    /*!< Filter Include Mode */
    gIgmpFilterModeExclude_c     /*!< Filter Exclude Mode */
} igmpFilterMode_e;

/*! Typedef for IGMP Filter Mode */
typedef uint32_t igmpFilterMode_t;

/*! IGMP Multicast Group Entry structure */
typedef struct igmpMcastGroupEntry_tag
{
    void *pSocket;                 /*!< Pointer to socket structure */
    uint32_t multicastAddr;        /*!< Multicast address */
    void *ifHandle;                /*!< Pointer to interface handler */
    igmpFilterMode_t filterMode;   /*!< Filter mode */
    nwkBuffer_t * sourcesList;     /*!< Pointer to source list buffer */
} igmpMcastGroupEntry_t;

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
\fn     void IGMP_Init(void)
\brief  Interface function for the IGMP module. It performs the initialization of the module.
***************************************************************************************************/
void IGMP_Init(void);

/*!*************************************************************************************************
\fn     bool_t IGMP_IPMulticastListen(void *pSocket, void *pIfHandle, uint32_t multicastAddr,
                                      igmpFilterMode_t filterMode, nwkBuffer_t *pSourceList)
\brief  Interface function for the IGMP module. It adds a Multicast Group record in the system.

\param  [in]  pSocket        Pointer to application process identifier.
\param  [in]  pIfHandle      Double pointer to interface handle.
\param  [in]  multicastAddr  The multicast address.
\param  [in]  filterMode     Filter mode for the multicast packets.
\param  [in]  pSourceList    Pointer to the list of source addresses.

\return       bool_t         TRUE if the record was added/modified, FALSE otherwise
***************************************************************************************************/
bool_t IGMP_IPMulticastListen(void *pSocket, void *pIfHandle, uint32_t multicastAddr,
                              igmpFilterMode_t filterMode, nwkBuffer_t *pSourceList);

/*!*************************************************************************************************
\fn     igmpMcastGroupEntry_t *IGMP_IsMyMulticastGroupAddr(void *pIfHandle, uint32_t multicastAddress)
\brief  Check to see if the multicast address is part of the multicast group the device is registered to.

\param  [in]  pIfHandle                Double pointer to interface handle
\param  [in]  multicastAddress         Multicast IPv4 address

\return       igmpMcastGroupEntry_t *  Pointer to group entry if found, NULL otherwise
***************************************************************************************************/
igmpMcastGroupEntry_t *IGMP_IsMyMulticastGroupAddr(void *pIfHandle, uint32_t multicastAddress);

/*!*************************************************************************************************
\fn     bool_t IGMP_AllowSocketReception(void *pSocket, void *pIfHandle, ipAddr_t *pMulticastAddress,
                                         ipAddr_t *pSourceAddress)
\brief  Filters the IP address at socket level.

\param  [in]  pSocket            Pointer to socket
\param  [in]  pIfHandle          Double pointer to interface handle
\param  [in]  pMulticastAddress  Pointer to multicast address
\param  [in]  pSourceAddress     Pointer to an source address

\return       bool_t             TRUE if the address is allowed. FALSE otherwise
***************************************************************************************************/
bool_t IGMP_AllowSocketReception(void *pSocket, void *pIfHandle, ipAddr_t *pMulticastAddress,
                                 ipAddr_t *pSourceAddress);

#if IGMP_V3_SOURCE_FILTERING_ENABLED
/*!*************************************************************************************************
\fn     nwkBuffer_t * IGMP_SourceListCreate(ipAddr_t *pSourceIp)
\brief  Creates a list to be passed to IGMP_IPMulticastListen().

\param  [in]  pSourceIp      Pointer to source IP address

\return       nwkBuffer_t *  Pointer to the created source list
***************************************************************************************************/
nwkBuffer_t *IGMP_SourceListCreate(ipAddr_t *pSourceIp);

/*!*************************************************************************************************
\fn     bool_t IGMP_SourceListAdd(nwkBuffer_t **ppSourceList, ipAddr_t *pSourceIp)
\brief  Adds an IP address to a source list.

\param  [in]  ppSourceList  Double pointer to the source list
\param  [in]  pSourceIp     Pointer to the IP address to be added

\return       bool_t        TRUE if the address was successfully added, FALSE otherwise
***************************************************************************************************/
bool_t IGMP_SourceListAdd(nwkBuffer_t **ppSourceList, ipAddr_t *pSourceIp);

#endif /* IGMP_V3_SOURCE_FILTERING_ENABLED */

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _ICMP_H */
