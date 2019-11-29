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
* SPDX-License-Identifier: BSD-3-Clause
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
