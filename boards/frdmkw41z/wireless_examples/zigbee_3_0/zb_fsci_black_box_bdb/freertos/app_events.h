/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_GENERIC_EVENTS_H_
#define APP_GENERIC_EVENTS_H_

/*!
\file       app_common.h
\brief      Thhe header file for generic application events
*/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "zll_commission.h"
#include "zll_utility.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define TEN_HZ_TICK_TIME        ZTIMER_TIME_MSEC(100)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    APP_E_EVENT_NONE                 = 0,
    APP_E_EVENT_START_ROUTER,
    APP_E_EVENT_TOUCH_LINK,
    APP_E_EVENT_EP_INFO_MSG,
    APP_E_EVENT_EP_LIST_MSG,
    APP_E_EVENT_GROUP_LIST_MSG,
    APP_E_EVENT_TRANSPORT_HA_KEY,
    APP_E_EVENT_SEND_PERMIT_JOIN,
    APP_E_EVENT_ENCRYPT_SEND_KEY,
    APP_E_EVENT_MAX
} APP_teEventType;


typedef struct
{
    uint8    u8Level;
} APP_tsEventLevel;

typedef struct
{
    uint16    u16SourceShortAddress;
    uint16    u16QueryTimeout;
} APP_tsEventHAQueryRsp;


typedef struct
{
    uint16    u16NwkAddr;
    uint8     u8Endpoint;
    uint16    u16ProfileId;
    uint16    u16DeviceId;
    uint8     u8Version;
    uint64    u64Address;
    uint8     u8MacCap;
} APP_tsEventTouchLink;

//#define APP_tsEventTouchLink tsCLD_ZllEndpointlistRecord

typedef struct {
    uint16                                                u16SrcAddr;
    tsCLD_ZllUtility_EndpointInformationCommandPayload    sPayload;
} APP_tsEventEpInfoMsg;

typedef struct {
    uint8     u8SrcEp;
    uint16    u16SrcAddr;
    tsCLD_ZllUtility_GetEndpointListRspCommandPayload sPayload;
} APP_tsEventEpListMsg;

typedef struct {
    uint8     u8SrcEp;
    uint16    u16SrcAddr;
    tsCLD_ZllUtility_GetGroupIdRspCommandPayload sPayload;
} APP_tsEventGroupListMsg;

typedef struct {
uint64 u64Address;
AESSW_Block_u uKey;
} APP_tsEventEncryptSendMsg;

typedef struct
{
    APP_teEventType eType;
    union
    {
        APP_tsEventTouchLink         sTouchLink;
        APP_tsEventEpInfoMsg         sEpInfoMsg;
        APP_tsEventEpListMsg         sEpListMsg;
        APP_tsEventGroupListMsg      sGroupListMsg;
        APP_tsEventEncryptSendMsg    sEncSendMsg;
        uint64 u64TransportKeyAddress;
    }uEvent;
} APP_tsEvent;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool bAddToEndpointTable(APP_tsEventTouchLink *psEndpointData);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_GENERIC_EVENTS_H_*/
