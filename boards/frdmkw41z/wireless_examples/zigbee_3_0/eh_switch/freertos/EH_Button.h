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

#ifndef EH_BUTTON_H_
#define EH_BUTTON_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

 /* DIO mapping for buttons on DK4 and EH switch */

/** Maximum time between two short press in  msec */
#define SHORT_PRESS_TIME  (1000 *10)    /* 10 sec */

/* Kindly Maintain the order as the button numbers are assigned directly */

typedef enum{
    COMM_BUTTON_PRESSED,
    ON_PRESSED,
    OFF_PRESSED,
    UP_PRESSED,
    DOWN_PRESSED,
    UP_AND_ON_TOGETHER_PRESSED,
    UP_AND_OFF_TOGETHER_PRESSED,
    DOWN_AND_OFF_TOGETHER_PRESSED,
    DOWN_AND_ON_TOGETHER_PRESSED,
    COMM_AND_ON_TOGETHER_PRESSED,
    COMM_AND_OFF_TOGETHER_PRESSED,
    COMM_AND_UP_TOGETHER_PRESSED,
    COMM_AND_DOWN_TOGETHER_PRESSED,
    COMM_BUTTON_RELEASED,
    UP_RELEASED,
    DOWN_RELEASED,
    NUMBER_OF_TRANSITION_CODE
}te_TransitionCode;

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported variables                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/
void vHandleButtonPress(te_TransitionCode  eCode );
bool bProcessGPDEvent(void);
bool bButtonInitialize(void);
void vHandleDR1216ButtonPress(void);
void vHandleEHButtonPress(void);
#endif /* EH_BUTTON_H_ */
