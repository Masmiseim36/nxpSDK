/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_BUTTONS_H
#define APP_BUTTONS_H

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    APP_E_BUTTONS_BUTTON_1,
    APP_E_BUTTONS_BUTTON_SW1,
    APP_E_BUTTONS_BUTTON_SW2,
    APP_E_BUTTONS_BUTTON_SW3,
    APP_E_BUTTONS_BUTTON_SW4,
} APP_teButtons;

#define APP_BUTTONS_BUTTON_1            (8)
#define APP_BUTTONS_NUM                 (1UL)
#define APP_BUTTONS_DIO_MASK            (1 << APP_BUTTONS_BUTTON_1)

typedef enum {
    E_INTERRUPT_UNKNOWN,
    E_INTERRUPT_BUTTON,
    E_INTERRUPT_WAKE_TIMER_EXPIRY
} teInterruptType;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_BUTTONS_H*/
