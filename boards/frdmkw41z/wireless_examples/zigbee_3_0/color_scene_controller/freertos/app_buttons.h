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
\brief      Button Press detection (Interface)
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    APP_E_BUTTONS_BUTTON_1 = 0,
    APP_E_BUTTONS_BUTTON_SW1,
    APP_E_BUTTONS_BUTTON_SW2,
    APP_E_BUTTONS_BUTTON_SW3,
    APP_E_BUTTONS_BUTTON_SW4,
} APP_teButtons;

#define APP_BUTTONS_BUTTON_1        (8)
#define APP_BUTTONS_BUTTON_SW1      (11)
#define APP_BUTTONS_BUTTON_SW2      (12)
#define APP_BUTTONS_BUTTON_SW3      (17)
#define APP_BUTTONS_BUTTON_SW4      (1)
#define APP_BUTTONS_NFC_FD          (0)
#define APP_BUTTONS_DIO_MASK        ((1 << APP_BUTTONS_BUTTON_1) | (1 << APP_BUTTONS_BUTTON_SW4) | (1 << APP_BUTTONS_BUTTON_SW3) | (1 << APP_BUTTONS_BUTTON_SW2) | (1 << APP_BUTTONS_BUTTON_SW1))
        
typedef enum {
    E_INTERRUPT_UNKNOWN,
    E_INTERRUPT_BUTTON,
    E_INTERRUPT_WAKE_TIMER_EXPIRY
} teInterruptType;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC bool_t APP_bButtonInitialise(void);
extern PUBLIC void vManageWakeUponSysControlISR(teInterruptType eInterruptType);

PUBLIC void APP_cbTimerButtonScan(void *pvParam);
PUBLIC void APP_cbTimerButtonDelay(void *pvParam);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_BUTTONS_H*/
