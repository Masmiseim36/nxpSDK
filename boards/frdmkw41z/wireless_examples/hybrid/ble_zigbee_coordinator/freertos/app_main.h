/*
* The Clear BSD License
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


#ifndef APP_MAIN_H
#define APP_MAIN_H

/*!
\file       app_main.h
\brief      app main header file.
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "ZQueue.h"
#include "bdb_api.h"

/* Bluetooth Low Energy */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gap_interface.h"
#include "ble_init.h"
#include "ble_config.h"
#include "l2ca_cb_interface.h"

#include "wireless_uart_interface.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define mAppUartBufferSize_c gAttMaxWriteDataSize_d(gAttMaxMtu_c)
#define mAppUartFlushIntervalInMs_c   	(100) 	     /* Flush Timeout in Ms */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/* Host to Application Messages Types */
typedef enum {
    gAppGapGenericMsg_c = 0,
    gAppGapConnectionMsg_c,
    gAppGapAdvertisementMsg_c,
    gAppGapScanMsg_c,
    gAppGattServerMsg_c,
    gAppGattClientProcedureMsg_c,
    gAppGattClientNotificationMsg_c,
    gAppGattClientIndicationMsg_c,
    gAppL2caLeDataMsg_c,
    gAppL2caLeControlMsg_c,
}appHostMsgType_tag;

typedef uint8_t appHostMsgType_t;

/* Host to Application Connection Message */
typedef struct connectionMsg_tag{
    deviceId_t              deviceId;
    gapConnectionEvent_t    connEvent;
}connectionMsg_t;

/* Host to Application GATT Server Message */
typedef struct gattServerMsg_tag{
    deviceId_t          deviceId;
    gattServerEvent_t   serverEvent;
}gattServerMsg_t;

/* Host to Application GATT Client Procedure Message */
typedef struct gattClientProcMsg_tag{
    deviceId_t              deviceId;
    gattProcedureType_t     procedureType;
    gattProcedureResult_t   procedureResult;
    bleResult_t             error;
}gattClientProcMsg_t;

/* Host to Application GATT Client Notification/Indication Message */
typedef struct gattClientNotifIndMsg_tag{
    uint8_t*    aValue;
    uint16_t    characteristicValueHandle;
    uint16_t    valueLength;
    deviceId_t  deviceId;
}gattClientNotifIndMsg_t;

/* L2ca to Application Data Message */
typedef struct l2caLeCbDataMsg_tag{
    deviceId_t  deviceId;
    uint16_t    lePsm;
    uint16_t    packetLength;
    uint8_t     aPacket[0];
}l2caLeCbDataMsg_t;

/* L2ca to Application Control Message */
typedef struct l2caLeCbControlMsg_tag{
    l2capControlMessageType_t   messageType;
    uint16_t                    padding;
    uint8_t                     aMessage[0];
}l2caLeCbControlMsg_t;

typedef struct appMsgFromHost_tag{
    appHostMsgType_t    msgType;
    union {
        gapGenericEvent_t       genericMsg;
        gapAdvertisingEvent_t   advMsg;
        connectionMsg_t         connMsg;
        gapScanningEvent_t      scanMsg;
        gattServerMsg_t         gattServerMsg;
        gattClientProcMsg_t     gattClientProcMsg;
        gattClientNotifIndMsg_t gattClientNotifIndMsg;
        l2caLeCbDataMsg_t       l2caLeCbDataMsg;
        l2caLeCbControlMsg_t    l2caLeCbControlMsg;
    } msgData;
}appMsgFromHost_t;

typedef enum appEvent_tag{
    mAppEvt_PeerConnected_c,
    mAppEvt_PairingComplete_c,
    mAppEvt_ServiceDiscoveryComplete_c,
    mAppEvt_ServiceDiscoveryFailed_c,
    mAppEvt_GattProcComplete_c,
    mAppEvt_GattProcError_c
}appEvent_t;

typedef enum appState_tag
{
    mAppIdle_c, 
    mAppExchangeMtu_c,	
    mAppServiceDisc_c,
    mAppRunning_c
} appState_t;

typedef struct appPeerInfo_tag
{
    deviceId_t  deviceId;
    bool_t      isBonded;
    wucConfig_t clientInfo;
    appState_t appState;
} appPeerInfo_t;

typedef struct advState_tag
{
    bool_t advOn;
} advState_t;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vInitResources(void);
PUBLIC void APP_vSetUpHardware(void);
PUBLIC void APP_vMainLoop(void);
PUBLIC void APP_cbTimerId(void *pvParam);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC uint8 u8TimerButtonScan;
extern PUBLIC uint8 u8TimerZCL;
extern PUBLIC uint8 u8TimerId;
#if (defined APP_NCI_ICODE) || (defined APP_NCI_AES)
extern PUBLIC uint8 u8TimerNci;
#endif

extern PUBLIC tszQueue APP_msgBdbEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */






