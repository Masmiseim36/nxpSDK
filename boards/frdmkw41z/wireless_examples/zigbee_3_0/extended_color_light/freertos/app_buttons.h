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
\brief      ZLO Demo: DK button handler -Interface
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#include "app_cfg.h"

#if (defined BUTTON_MAP_OM15081)
    typedef enum {
        APP_E_BUTTONS_BUTTON_1 = 0
    } APP_teButtons;

    #define APP_BUTTONS_NUM             (1UL)
    #define APP_BUTTONS_DIO_MASK        (1 << APP_BOARD_SW0_PIN)
	#define APP_BUTTON_INVERT_MASK      (1 << APP_BOARD_SW0_PIN)
	#define DEMO_GINT0_POL_MASK			APP_BUTTON_INVERT_MASK
	#define DEMO_GINT0_ENA_MASK			APP_BUTTONS_DIO_MASK
#else
    typedef enum {

        APP_E_BUTTONS_BUTTON_1 = 0,
        APP_E_BUTTONS_BUTTON_SW2,
        APP_E_BUTTONS_BUTTON_SW3,
        APP_E_BUTTONS_BUTTON_SW4,
        APP_E_BUTTONS_BUTTON_SW5,
    } APP_teButtons;

    #define APP_BUTTONS_NUM             (1UL)
  
    #define APP_BUTTONS_BUTTON_1        (1 << APP_SW1_PIN)
    #define APP_BUTTONS_DIO_MASK        ( (APP_BUTTONS_BUTTON_1)  )
#endif





/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC bool_t APP_bButtonInitialise(void);
PUBLIC uint32 APP_u32GetSwitchIOState(void);
PUBLIC void APP_cbTimerButtonScan(void *pvParam);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_BUTTONS_H*/
