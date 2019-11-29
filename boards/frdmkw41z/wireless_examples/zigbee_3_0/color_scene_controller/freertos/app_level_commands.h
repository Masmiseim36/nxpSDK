/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_LEVEL_COMMANDS_H_
#define APP_LEVEL_COMMANDS_H_

#include "LevelControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define LEVEL_CHANGE_STEPS_PER_SEC_SLOW (20)
#define LEVEL_CHANGE_STEPS_PER_SEC_MED  (60)
#define LEVEL_CHANGE_STEPS_PER_SEC_FAST (100)



/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vAppLevelStepMove(teCLD_LevelControl_MoveMode eMode, uint8 u8StepSize, uint16 u16TransitionTime, bool_t bWithOnOff);
PUBLIC void vAppLevelMove(teCLD_LevelControl_MoveMode eMode, uint8 u8Rate, bool_t bWithOnOff);
PUBLIC void vAppLevelStop(bool_t bWithOnOff);



/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_LEVEL_COMMANDS_H_*/
