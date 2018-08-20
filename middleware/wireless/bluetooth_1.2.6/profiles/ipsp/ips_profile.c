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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "l2ca_cb_interface.h"
#include "l2ca_types.h"
#include "ipsp_interface.h"

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bleResult_t Ipsp_Connect(ipspConfig_t *pConfig, deviceId_t  peerDeviceId)
{
    return L2ca_ConnectLePsm(gL2ca_Le_Psm_Ipsp_c,
                            peerDeviceId,
                            pConfig->initialCredits);
}

bleResult_t Ipsp_SendData
(
     deviceId_t  peerDeviceId,
     uint16_t    channelId,
     uint8_t*    pPacket,
     uint16_t    packetLength
)
{
    return L2ca_SendLeCbData(peerDeviceId,
                      channelId,
                      pPacket,
                      packetLength);
}

void Ipsp_ControlCallback
(
    ipspConfig_t *pConfig,
    l2capControlMessageType_t  messageType, 
    void* pMessage
)
{
    switch (messageType)
    {
        case gL2ca_LePsmConnectRequest_c:
        {
            l2caLeCbConnectionRequest_t *pConnReq = (l2caLeCbConnectionRequest_t *)pMessage;

            if ((pConfig->role & gIpsp_Router_c) &&
                pConfig->peerDeviceId == pConnReq->deviceId)
            {
                return;
            }
            else
            {
                /* Send Connection Response */
                Ipsp_Connect(pConfig, pConnReq->deviceId);
            }
            break;
        }
        case gL2ca_LePsmConnectionComplete_c:
        {
            l2caLeCbConnectionComplete_t *pConnComplete = (l2caLeCbConnectionComplete_t *)pMessage;

            if (pConnComplete->result == gSuccessful_c)
            {
                pConfig->peerDeviceId = pConnComplete->deviceId;
            }
            break;
        }
        case gL2ca_LePsmDisconnectNotification_c:
        {
            l2caLeCbDisconnection_t *pDisconnect = (l2caLeCbDisconnection_t *)pMessage;

            if (pDisconnect->deviceId == pConfig->peerDeviceId)
            {
                pConfig->peerDeviceId = gInvalidDeviceId_c;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
