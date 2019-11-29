/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
#if ((BR_ROUTER_MODE) || (BR_HOST_MODE))
    #include "nd_events.h"
    #include "app_border_router.h"
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
extern taskMsgQueue_t *gpBrTaskMsgQueue;

/*==================================================================================================
 Private macros
 ==================================================================================================*/
#ifndef gThrDefaultInstanceId_c
    #define gThrDefaultInstanceId_c 0
#endif
/*==================================================================================================
 Private type definitions
 ==================================================================================================*/
typedef void (*evmGenericFunc_t)(void *p);

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
    {gThrEvSet_NwkCreate_c, (pfFunction_t)APP_NwkCreateCb, &mpAppThreadMsgQueue, gEvmAnyInstanceId_c, TRUE},
#endif /* THREAD_ED_CONFIG */
    {gThrEvSet_NwkScan_c, (pfFunction_t)APP_NwkScanHandler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE},
    {gThrEvSet_NwkJoin_c, (pfFunction_t)Stack_to_APP_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE},
    {gThrEvSet_NwkGeneral_c, (pfFunction_t)Stack_to_APP_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE},
    {gThrEvSet_NwkGeneral_c, (pfFunction_t)MESHCOP_StackEventHandler, &mpThrTaskMsgQueue, gThrDefaultInstanceId_c, TRUE},
    {gThrEvSet_NwkCommissioning_c, (pfFunction_t)APP_Commissioning_Handler, &mpAppThreadMsgQueue, gThrDefaultInstanceId_c, TRUE},
    {gSlwpEvSet_Security_c, (pfFunction_t)THR_SecurityEventHandler, &mpThrTaskMsgQueue, gEvmAnyInstanceId_c, FALSE}
#if ((BR_ROUTER_MODE) || (BR_HOST_MODE) )
    , {gNdRaReceived_c, (pfFunction_t)BR_RaReceived, &gpBrTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    , {gNdRaRouteInfoReceived_c, (pfFunction_t)BR_RaRouteInfoReceived, &gpBrTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #if ND_OPT_DNS_RCV_ENABLED
    /* Register for DNS info obtained via ND on both ROUTER and HOST. */
    , {gNdRaDnsInfoReceived_c, (pfFunction_t)BR_RaDnsInfoReceived, &gpBrTaskMsgQueue, gEvmAnyInstanceId_c, TRUE}
    #endif
#endif

};

const uint16_t aStaticEventsTableSize = sizeof(aStaticEventsTable) / sizeof(eventManagerEntry_t);

/*==================================================================================================
 Private functions
 ==================================================================================================*/

/*==================================================================================================
 Public functions
 ==================================================================================================*/

