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

/*!
\file       event_manager_globals.c
\brief      This is the source file that contains parameters for the EVENT MANAGER module that can
            be configured by the application
*/
/*==================================================================================================
 Include Files
 ==================================================================================================*/
#include "event_manager.h"
#include "event_manager_cfg.h"
#include "sixlowpan_interface.h"
#include "thread_network.h"
#if ((ENET_ROUTER) || (ENET_HOST))
    #include "nd_events.h"
    #include "app_ethernet.h"
#endif
#if ((USBENET_ROUTER) || (USBENET_HOST))
    #include "nd_events.h"
    #include "app_ethernet_usb.h"
#endif
#if (THR_SERIAL_TUN_ROUTER)
    #include "nd_events.h"
    #include "app_serial_tun.h"
#endif

extern void APP_Commissioning_Handler(void *param);
#if !THREAD_ED_CONFIG
    extern void APP_NwkCreateCb(void *pParam);
#endif /* THREAD_ED_CONFIG */
extern void APP_NwkScanHandler(void *param);
extern void Stack_to_APP_Handler(void *param);
extern void THR_SecurityEventHandler(void *pEvent);
extern void MESHCOP_StackEventHandler(void *pEvent);

extern taskMsgQueue_t *mpAppThreadMsgQueue;
extern taskMsgQueue_t *mpThrTaskMsgQueue;

/*==================================================================================================
 Private macros
 ==================================================================================================*/
#ifndef gThrDefaultInstanceId_c
    #define gThrDefaultInstanceId_c 0
#endif
/*==================================================================================================
 Private type definitions
 ==================================================================================================*/

/*==================================================================================================
 Private prototypes
 ==================================================================================================*/

/*==================================================================================================
 Private global variables declarations
 ==================================================================================================*/

/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/
/* Register event sets */
const eventManagerEntry_t aStaticEventsTable[] =
{
#if !THREAD_ED_CONFIG
    {gThrEvSet_NwkCreate_c, APP_NwkCreateCb, &mpThrTaskMsgQueue, gEvmAnyInstanceId_c, TRUE },
#endif /* THREAD_ED_CONFIG */
    {gThrEvSet_NwkScan_c, APP_NwkScanHandler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE },
    {gThrEvSet_NwkJoin_c, Stack_to_APP_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE },
    {gThrEvSet_NwkGeneral_c, Stack_to_APP_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE },
    {gThrEvSet_NwkGeneral_c, MESHCOP_StackEventHandler, &mpThrTaskMsgQueue, gThrDefaultInstanceId_c, TRUE },
    {gThrEvSet_NwkCommissioning_c, APP_Commissioning_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE },
    {gSlwpEvSet_Security_c, THR_SecurityEventHandler, &mpThrTaskMsgQueue, gEvmAnyInstanceId_c, FALSE }
#if ((ENET_ROUTER) || (ENET_HOST) )
    , {gNdRaReceived_c, NET_RaReceived, &gpEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    , {gNdRaRouteInfoReceived_c, NET_RaRouteInfoReceived, &gpEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #if ND_OPT_DNS_RCV_ENABLED
    /* Register for DNS info obtained via ND on both ROUTER and HOST. */
    , {gNdRaDnsInfoReceived_c, NET_RaDnsInfoReceived, &gpEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #endif
#elif ((USBENET_ROUTER) || (USBENET_HOST))
    , {gNdRaReceived_c, NET_RaReceived, &gpUSBEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    , {gNdRaRouteInfoReceived_c, NET_RaRouteInfoReceived, &gpUSBEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #if ND_OPT_DNS_RCV_ENABLED
    /* Register for DNS info obtained via ND on both ROUTER and HOST. */
    , {gNdRaDnsInfoReceived_c, NET_RaDnsInfoReceived, &gpUSBEthTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #endif
#elif (THR_SERIAL_TUN_ENABLE_ND_HOST)
    , {gNdRaReceived_c, Serialtun_RaReceived, &gpSerialTunTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    , {gNdRaRouteInfoReceived_c, Serialtun_RaRouteInfoReceived, &gpSerialTunTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
#endif

};

const uint16_t aStaticEventsTableSize = sizeof(aStaticEventsTable) / sizeof(eventManagerEntry_t);

/*==================================================================================================
 Private functions
 ==================================================================================================*/

/*==================================================================================================
 Public functions
 ==================================================================================================*/

