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

#ifndef APP_EVENT_HANDLER_H_
#define APP_EVENT_HANDLER_H_

/*!
\file       app_event_handler.h
\brief      ZLO Demo: Handles all the different type of Application events
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define BUTTON_RELEASED_OFFSET 0x80

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Kindly Maintain the order as the button numbers are assigned directly */

typedef enum{
    COMM_BUTTON_PRESSED,
    SW1_PRESSED,
    SW2_PRESSED,
    SW3_PRESSED,
    SW4_PRESSED,
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    FD_PRESSED,
#endif
    // Make sure each switch value matches its counterpart
    COMM_BUTTON_RELEASED = BUTTON_RELEASED_OFFSET,
    SW1_RELEASED,
    SW2_RELEASED,
    SW3_RELEASED,
    SW4_RELEASED,
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    FD_RELEASED,
#endif
    NUMBER_OF_TRANSITION_CODE
}te_TransitionCode;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vAppHandleAppEvent(APP_tsEvent sButton);
PUBLIC void vEventStopFindAndBind(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_EVENT_HANDLER_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
