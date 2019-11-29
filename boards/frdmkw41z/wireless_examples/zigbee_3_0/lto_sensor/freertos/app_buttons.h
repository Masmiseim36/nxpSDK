/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_BUTTONS_H
#define APP_BUTTONS_H

/*!
\file       app_buttons.h
\brief      This is a header file for the Application Buttons module.
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    APP_E_BUTTONS_BUTTON_1 = 0,
    APP_E_BUTTONS_BUTTON_SW2,
    APP_E_BUTTONS_BUTTON_SW3,
    APP_E_BUTTONS_BUTTON_SW4,
    APP_E_BUTTONS_BUTTON_SW5,
} APP_teButtons;

typedef enum {
    E_INTERRUPT_UNKNOWN,
    E_INTERRUPT_BUTTON,
    E_INTERRUPT_WAKE_TIMER_EXPIRY
} teInterruptType;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t APP_bButtonInitialise(void);
PUBLIC void APP_cbTimerButtonScan(void *pvParam);
PUBLIC void APP_cbTimerButtonDelay(void *pvParam);
PUBLIC uint32 APP_u32GetSwitchIOState(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_BUTTONS_H*/
