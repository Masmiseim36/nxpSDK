/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_PIR_EVENTS_H_
#define APP_PIR_EVENTS_H_

/*!
\file       ZLO Demo: This is the superclass which determines which PIR
\brief      we are using and calls that function.
*/
     
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_APP_TIMER_NONE,
    E_APP_TIMER_OCCUPIED_TO_UNOCCUPIED,
    E_APP_TIMER_UNOCCUPIED_TO_OCCUPIED
}teRunningTimer;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vHandleFallingEdgeEvent(void);
PUBLIC void vHandleRisingEdgeEvent(void);
PUBLIC void vHandleWakeTimeoutEvent(void);
PUBLIC void vHandleNewJoinEvent(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_PIR_EVENTS_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
