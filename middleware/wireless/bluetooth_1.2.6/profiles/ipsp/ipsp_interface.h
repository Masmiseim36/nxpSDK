/*! *********************************************************************************
* \addtogroup IPSP
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the interface file for the Internet Protocol Support Profile
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
