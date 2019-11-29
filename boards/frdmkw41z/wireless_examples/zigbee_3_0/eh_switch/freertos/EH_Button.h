/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
