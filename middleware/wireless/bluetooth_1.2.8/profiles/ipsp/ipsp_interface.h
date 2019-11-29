/*! *********************************************************************************
* \addtogroup IPSP
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the interface file for the Internet Protocol Support Profile
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _IPSP_INTERFACE_H_
#define _IPSP_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! IPSP - Role */
typedef enum
{
    gIpsp_NoRole_c           = 0x00,
    gIpsp_Router_c           = BIT0,
    gIpsp_Node_c             = BIT1,
    gIpsp_RouterAndNode_c    = BIT0 | BIT1,
} ipspRole_tag;

typedef uint8_t ipspRole_t;

/*! IPSP - Configuration */
typedef struct ipspConfig_tag
{
    uint16_t        serviceHandle;
    ipspRole_t      role;
    uint16_t        initialCredits;
    deviceId_t      peerDeviceId;
} ipspConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*!**********************************************************************************
* \brief        Connects IPSP router to the peer device
*
* \param[in]    pConfig         Pointer to configuration information.
* \param[in]    peerDeviceId    Peer device Id.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ipsp_Connect(ipspConfig_t *pConfig, deviceId_t  peerDeviceId);

/*!**********************************************************************************
* \brief        Sends IPSP data to the peer device
*
* \param[in]    pConfig         Pointer to configuration information.
* \param[in]    peerDeviceId    Peer device Id.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ipsp_SendData
(
     deviceId_t  peerDeviceId,
     uint16_t    channelId,
     uint8_t*    pPacket,
     uint16_t    packetLength
);

void Ipsp_ControlCallback
(
    ipspConfig_t *pConfig,
    l2capControlMessageType_t  messageType,
    void* pMessage
);

#ifdef __cplusplus
}
#endif 

#endif /* _Ipsp_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
