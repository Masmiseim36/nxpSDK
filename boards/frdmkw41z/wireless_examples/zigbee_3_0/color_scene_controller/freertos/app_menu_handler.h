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

#ifndef APP__H_
#define aPP__NODEH_

#include "app_led_control.h"
//#ifdef DR1159
#include "app_captouch_buttons.h"
//#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#ifdef DR1199
/*
 * Alturnative key codes for use with the GUI
 */
typedef enum
{
    BUTTON_1,
    BUTTON_SW5,
    BUTTON_SW4,
    BUTTON_SW3,
    BUTTON_SW2,
    KEY_1 = 0x10,
    KEY_2,
    KEY_3,
    KEY_4,

    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,

    KEY_9,
    KEY_10,
    KEY_11,
    KEY_12,

    KEY_13,
    KEY_14,
    KEY_15,
    KEY_16,

} teUserKeyCodes;
#endif


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vHandleFactoryNewKeyPress(teUserKeyCodes eKeyCode);
PUBLIC void APP_vHandleKeyPress(teUserKeyCodes eKeyCode);
PUBLIC void APP_vHandleKeyRelease(teUserKeyCodes eKeyCode);


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC teShiftLevel eShiftLevel;


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP___H_*/
