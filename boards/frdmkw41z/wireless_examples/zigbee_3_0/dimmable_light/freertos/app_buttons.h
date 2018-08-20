/*
* The Clear BSD License
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
