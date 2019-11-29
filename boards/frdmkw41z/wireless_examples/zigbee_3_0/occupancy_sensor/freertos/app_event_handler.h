/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
