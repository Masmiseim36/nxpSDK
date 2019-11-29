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
\brief      Light Bulb application generic event definitions
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#include "zll_commission.h"
#include "zll_utility.h"

#define TEN_HZ_TICK_TIME        ZPS_TSV_TIME_MSEC(100)

typedef enum
{
    APP_E_EVENT_NONE = 0,
    APP_E_EVENT_BUTTON_UP,
    APP_E_EVENT_BUTTON_DOWN,
    APP_E_EVENT_LEAVE_AND_RESET,
    APP_EVENT_POR_FIND_BIND,
    APP_EVENT_POR_CLEAR_BINDINGS,
    APP_EVENT_POR_RESET_GP_TABLES,

    APP_E_EVENT_MAX
} APP_teEventType;

typedef struct
{
    uint8 u8Button;
} APP_tsEventButton;

typedef struct
{
    uint8 u8Level;
} APP_tsEventLevel;

typedef struct
{
    uint16 u16SourceShortAddress;
    uint16 u16QueryTimeout;
} APP_tsEventHAQueryRsp;



typedef struct
{
    APP_teEventType eType;
    union
    {
        APP_tsEventButton           sButton;
    }uEvent;
} APP_tsEvent;

typedef struct
{
    APP_teEventType eType;
    union
    {
        APP_tsEventButton                   sButton;
    }uEvent;
} APP_tsLightEvent;




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
