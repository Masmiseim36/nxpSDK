/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_GENERIC_EVENTS_H_
#define APP_GENERIC_EVENTS_H_

/*!
\file       app_events.h
\brief      Application Events
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    APP_E_EVENT_NONE = 0,
    APP_E_EVENT_BUTTON_UP,
    APP_E_EVENT_BUTTON_DOWN,
    APP_E_EVENT_PERIODIC_REPORT,
    APP_E_EVENT_SEND_REPORT,
    APP_E_EVENT_WAKE_TIMER,
    APP_E_EVENT_KEEPALIVE_START,
    APP_E_EVENT_KEEPALIVE_STOP,
    APP_E_EVENT_FIND_BIND_START,
    APP_E_EVENT_FIND_BIND_STOP,
    APP_E_EVENT_MAX,
} APP_teEventType;

typedef struct
{
    uint8 u8Button;
    uint32 u32DIOState;
} APP_tsEventButton;

typedef struct
{
    APP_teEventType eType;
    union
    {
        APP_tsEventButton                   sButton;
    }uEvent;
} APP_tsEvent;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_GENERIC_EVENTS_H_*/
