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
\brief      Base Device application - generic event definitions
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
    APP_E_EVENT_PGM_BUTTON_UP,
    APP_E_EVENT_PGM_BUTTON_DOWN,
    APP_E_EVENT_LEAVE_AND_RESET,
    APP_E_EVENT_MAX
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
