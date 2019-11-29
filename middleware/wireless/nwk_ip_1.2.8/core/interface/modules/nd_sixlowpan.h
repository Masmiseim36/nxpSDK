/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ND_SIXLOWPAN_H
#define _ND_SIXLOWPAN_H
/*!=================================================================================================
\file       nd_sixlowpan.h
\brief      This is a header file for the 6LoWPAN Neighbor Discovery for IP version 6 (IPv6) module.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"
/* Network Includes */
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/



/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum ndSlpDevType_tag
{
    gNdSlpDevBRouter_c = 0x10,
    gNdSlpDevRouter_c = 0x11,
    gNdSlpDevHost_c = 0x12
} ndSlpDevType_t;


/*==================================================================================================
Public global variables declarations
==================================================================================================*/



/*==================================================================================================
Public function prototypes
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#if IP_IP6_ENABLE

#if ND_ENABLED

#if ND_MSG_DAC_RCV_ENABLED

/*!*************************************************************************************************
\fn     void ND_MsgDarSend(ifHandle_t ifHandle, uint32_t regLifetime, uint8_t *pEui64, ipAddr_t *pIpAddr)
\brief  Interface function for the Neighbor Discovery module. It sends a duplicate address request message.

\param  [in]    ifHandle      Pointer to media interface configuration structure
\param  [in]    regLifetime     Registration lifetime in seconds
\param  [in]    pEui64          Pointer to EUI64 address
\param  [in]    pIpAddr         Pointer to link layer address
***************************************************************************************************/
void ND_MsgDarSend(ifHandle_t ifHandle, uint32_t regLifetime, uint8_t *pEui64, ipAddr_t *pIpAddr);

/*!*************************************************************************************************
\fn     void ND_MsgDacRcv(ipPktInfo_t *pRxIpPktInfo)
\brief  Interface function for the Neighbor Discovery module. It drops a received duplicate address
        confirmation message.

\param  [in]    pRxIpPktInfo        Pointer to received IP packet
***************************************************************************************************/
void ND_MsgDacRcv(ipPktInfo_t *pRxIpPktInfo);
#endif

#if ND_MSG_DAR_RCV_ENABLED

/*!*************************************************************************************************
\fn     void ND_MsgDarRcv(ipPktInfo_t *pRxIpPktInfo)
\brief  Interface function for the Neighbor Discovery module. It drops a received duplicate address
        request message.

\param  [in]    pRxIpPktInfo        Pointer to received IP packet
***************************************************************************************************/
void ND_MsgDarRcv(ipPktInfo_t *pRxIpPktInfo);

#endif

/*!*************************************************************************************************
\fn     uint32_t ND_OptContextGetSize(ifHandle_t ifHandle)
\brief  Interface function for the Neighbor Discovery module. It gets the context size.

\param  [in]    ifHandle         Pointer to media interface configuration structure

\return         uint32_t         context size
***************************************************************************************************/
uint32_t ND_OptContextGetSize(ifHandle_t ifHandle);

/*!*************************************************************************************************
\fn     void ND_OptContextRcv(ifHandle_t ifHandle, ndOptionContext_t *pRcvContext)
\brief  This function processes a received context option.

\param [in]   ifHandle         Pointer to media interface configuration structure
\param [in]   pRcvContext       Pointer to the context option
***************************************************************************************************/
void ND_OptContextRcv(ifHandle_t ifHandle, ndOptionContext_t *pRcvContext);

/*!*************************************************************************************************
\fn     void ND_OptContextAdd(ifHandle_t ifHandle, uint8_t **ppOptionPos)

\brief  This function add a context option to an outgoing packet. The information from the context
        entry is taken and put in the form of an context option.

\param [in]   ifHandle         Pointer to the interface handler
\param [in]   ppOptionPos       Pointer to the location to where to create the prefix info option.
***************************************************************************************************/
void ND_OptContextAdd(ifHandle_t ifHandle, uint8_t **ppOptionPos);

/*!*************************************************************************************************
\fn     void ND_OptArRcv(ifHandle_t ifHandle, ndOptionAr_t *pNdOptAr, ndOptionLLA_t *pNdOptSlla,
            ipAddr_t *pSrcIpAddr, ipPktInfo_t *pRxIpPktInfo, uint32_t linkLayerAddrSize, uint8_t naFlags);

\brief  This function processes a received Address Registration Option.

\param [in]   ifHandle              Pointer to the interface handle
\param [in]   pNdOptAr              Pointer to the Address Registration option
\param [in]   pNdOptSlla            Pointer to the SLAA option
\param [in]   pSrcIpAddr            Pointer to the source IP address
\param [in]   pRxIpPktInfo          Pointer to the IP packet
\param [in]   linkLayerAddrSize     The size of the link layer address
\param [in]   naFlags               Neighbor Advertisement flags
***************************************************************************************************/
void ND_OptArRcv(ifHandle_t ifHandle, ndOptionAr_t *pNdOptAr, ndOptionLLA_t *pNdOptSlla, ipAddr_t *pSrcIpAddr,
                 ipPktInfo_t *pRxIpPktInfo, uint32_t linkLayerAddrSize, uint8_t naFlags);

/*!*************************************************************************************************
\fn      uint32_t ND_OptArGetSize(void)
\brief   This function returns the size of ARO in bytes.

\return  uint32_t  Size of ARO in bytes
***************************************************************************************************/
uint32_t ND_OptArGetSize(void);

/*!*************************************************************************************************
\fn     void ND_OptArAdd(uint32_t status, uint32_t registrationLifetime, uint8_t *pEui64,
            ifHandle_t ifHandle, uint8_t **ppOptPos)

\brief  This function adds an Address Registration Option to an ND message.

\param  [in]    status                  ARO status
\param  [in]    registrationLifetime    The lifetime of the address registration
\param  [in]    pEui64                  Pointer to EUI64 address
\param  [in]    ifHandle               Pointer to the interface handles
\param  [in]    ppOptPos                Double pointer to the position where to store the option
***************************************************************************************************/
void ND_OptArAdd(uint32_t status, uint32_t registrationLifetime, uint8_t *pEui64, ifHandle_t ifHandle,
                 uint8_t **ppOptPos);

/*!*************************************************************************************************
\fn     bool_t ND_OptAbroRcv(ndCfg_t *pNdCfg, ndOptionAbro_t *pAbroOption)

\brief  This function checks the received version number from the Border Router against the last
        one received updates it if is is newer.

\param  [in]    pNdCfg              Pointer to the ND configuration
\param  [in]    pAbroOption         Pointer to the ABRO option received in the RA

\retval         TRUE                The version number is newer.
\retval         FALSE               The version number is older.
***************************************************************************************************/
bool_t ND_OptAbroRcv(ndCfg_t *pNdCfg, ndOptionAbro_t *pAbroOption);

/*!*************************************************************************************************
\fn     void ND_RegistrationLifetimeSet(ifHandle_t ifHandle, uint32_t regLifetime)
\brief  Interface function for the Neighbor Discovery module. It sets the Host/Router registration lifetime.

\param  [in]    ifHandle       Pointer to media interface configuration structure
\param  [in]    regLifetime     Registration lifetime in seconds
***************************************************************************************************/
void ND_RegistrationLifetimeSet(ifHandle_t ifHandle, uint32_t regLifetime);

#endif /* IP_IP6_ENABLE */

#endif /* ND_ENABLED */

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _ND_SIXLOWPAN_H */
