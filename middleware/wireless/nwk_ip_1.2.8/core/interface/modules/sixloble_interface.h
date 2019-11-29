/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SIXLOBLE_INTERFACE_H
#define _SIXLOBLE_INTERFACE_H

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "ble_abs_types.h"
#include "sixlowpan_ib.h"
#include "network_utils.h"
#include "event_manager.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*! Structure which defines the ADPD Data Request parameters */
typedef struct adpdBleDataReq_tag
{
    instanceId_t    slwpInstanceId;         /*!< 6LoWPAN instance id */
    nwkBuffer_t     pNsduNwkBuff;           /*!< Pointer to the IPv6 datagram in network buffers */
    ipAddr_t        ipDstAddr;              /*!< Pointer to the Destination (Next Hop) IP Address
                                                 If Not NULL then the IP stack provides the next hop IP address.
                                                 If NULL then 6LoWPAN computes the next hop address from the IP header.*/
    uint8_t         nsduHandle;             /*!< Identifier for the data request */
} adpdBleDataReq_t;

/*! Structure which defines the ADPD Data Indication parameters */
typedef struct adpdBleDataInd_tag
{
    instanceId_t   ifInstanceId;           /*!< upper layer instance id */
    uint8_t *      pNsdu;                  /*!< Pointer to the IPv6 datagram */
    uint16_t       nsduLength;             /*!< Length of the IPv6 datagram. Maximum value is 1280 */
    llAddr_t       srcAddrInfo;
    llAddr_t       dstAddrInfo;
} adpdBleDataInd_t;

typedef struct adpdBleCb_tag
{
    void (* adpdDataInd)            (adpdBleDataInd_t * pParam);
} adpdBleCb_t;

typedef enum sixLoBleDevType_tag
{
    gSloDevBRouter_c = 0x10,
    gSloDevHost_c    = 0x11,
}sixLoBleDevType_t;

typedef struct sloBleStruct_tag
{
    instanceId_t        ifInstanceId;
    bleAbsRequests_t *  pBleAbsReq;
    adpdBleCb_t *       pAdpdCb;
    adpHcEnable_t       adpHc;
    sixLoBleDevType_t   deviceType;
} sloBleStruct_t;


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
\fn     instanceId_t SixLoBle_Init(uint32_t instanceNb, adpIb_t * pDefaultAdpIb)
\brief  6LoWPAN Instance initialization function.

\param  [in]    instanceNb      Instance number.  
\param  [in]    pDefaultAdpIb   Pointer to the ADP IB structure. 

\retval         instanceId_t    Instance ID. Pointer to the 6LoWPAN instance structure.
***************************************************************************************************/
instanceId_t SixLoBle_Init(adpHcEnable_t headerComp);

/*!*************************************************************************************************
\fn     void SWLP_RegisterIfInstance(instanceId_t slwpInstanceId, instanceId_t ifInstanceId)
\brief  Interface function for the 6LoWPAN module. It registers the uper layer interface instance.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    ifInstanceId        Interface instance ID
***************************************************************************************************/
void SixLoBle_RegisterIfInstance(instanceId_t slwpInstanceId, instanceId_t ifInstanceId);

void SixLoBle_RegisterBleInstance(instanceId_t sloInstanceId, bleAbsRequests_t *  pBleAbsReq);
/*!*************************************************************************************************
\fn     void SWLP_RegisterAdpdCallbacks(instanceId_t slwpInstanceId, adpdCb_t * pAdpdCb)
\brief  Interface function for the 6LoWPAN module. It registers the callbacks for the 6LoWPAN.

\param  [in]    slwpInstanceId      6LoWPAN instance ID
\param  [in]    pAdpdCb             Pointer to callback structure
***************************************************************************************************/
void SixLoBle_RegisterAdpdCallbacks(instanceId_t slwpInstanceId, adpdBleCb_t * pAdpdCb);

/*!*************************************************************************************************
\fn     void SixLoBle_DataRequest(adpdDataReq_t * pAdpdDataReq)
\brief  Interface function for the 6LoWPAN module. It sends a 6LoWPAN Data Packet.

\param  [in]    pAdpdDataReq    Pointer to the ADPD Data Request structure.            
***************************************************************************************************/
void SixLoBle_DataRequest(adpdBleDataReq_t * pAdpdDataReq);

/*!*************************************************************************************************
\fn     void SixLoBle_DataIndCB(bleDataInd_t* pBleDataInd)
\brief  Interface function for the 6LoWPAN module. Callback used by MAC to signal a MCPS Data Indication.

\param  [in]    pBleDataInd    Pointer to the BLE Data Indication structure
***************************************************************************************************/
void SixLoBle_DataIndCB(bleDataInd_t* pBleDataInd);

/*!*************************************************************************************************
\fn     void SixLoBle_GetBleAddrFromIID(    uint8_t * pIID,     bleAbsDeviceAddress_t  *pBleAddress)
\brief  Interface function for the 6LoWPAN module. It returns the Link-Layer address from the IID.

\param  [in]    pIID            Pointer to the IID
\param  [out]   aOutBleAddress  Pointer to the variable which will hold the Link-Layer address
***************************************************************************************************/
void SixLoBle_GetBleAddrFromIID
(
    uint8_t * pIID, 
    bleAddr_t  aOutBleAddress
);

/*!*************************************************************************************************
\fn     void SixLoBle_GetIIDFromBleAddr(    uint8_t * pIID,     bleAbsDeviceAddress_t  *pBleAddress)
\brief  Interface function for the 6LoWPAN module. It returns the Link-Layer address from the IID.

\param  [in]    aBleAddress         Pointer to the variable which will hold the Link-Layer address
\param  [out]   pIID            Pointer to the IID
***************************************************************************************************/
void SixLoBle_GetIIDFromBleAddr
(
    bleAddr_t  aBleAddress,
    uint8_t * pIID
);

#ifdef __cplusplus
}
#endif


#endif  /*_SIXLOBLE_INTERFACE_H */
