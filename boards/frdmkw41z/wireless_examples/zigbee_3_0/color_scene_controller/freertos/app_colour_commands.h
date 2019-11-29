/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_COLOUR_COMMANDS_H_
#define APP_COLOUR_COMMANDS_H_

#include "ColourControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SAT_CHANGE_STEPS_PER_SEC        (20)
#define COL_TEMP_CHANGE_STEPS_PER_SEC   (100)
#define COL_TEMP_PHY_MIN                (0U)       /* Seting these two paramters to zero means the colour temperature    */
#define COL_TEMP_PHY_MAX                (0U)       /* is limited by the bulb capabilities, not what the remote sends out */
#define STOP_COLOUR_LOOP                FALSE

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
#ifdef CLD_COLOUR_CONTROL
PUBLIC void vAppMoveSaturation(teCLD_ColourControl_MoveMode eMode, uint8 u8StepsPerSec);
PUBLIC void vAppStepSaturation(teCLD_ColourControl_StepMode eMode, uint8 u8StepSize);
PUBLIC void vAppMoveToHue(uint8 u8Hue, teCLD_ColourControl_Direction eDirection, uint16 u16TransitionTime);
PUBLIC void vAppEnhancedMoveToHue(uint16 u16Hue, teCLD_ColourControl_Direction eDirection);
PUBLIC void vAppMoveToHueAndSaturation(uint8 u8Hue, uint8 u8Saturation, uint16 u16Time);
PUBLIC void vAppEnhancedMoveToHueAndSaturation(uint16 u16Hue, uint8 u8Saturation, uint16 u16Time);
PUBLIC void vAppGotoColour(void /*uint8 u8Colour*/);
PUBLIC void vAppMoveColour(int16 i16RateX, int16 i16RateY);
PUBLIC void vAppStepColour(int16 i16StepX, int16 i16StepY, uint16 u16TransitionTime);
PUBLIC void vAppMoveHue(teCLD_ColourControl_MoveMode eMode, uint8 u8StepsPerSec);
PUBLIC void vAppEnhancedMoveHue(teCLD_ColourControl_MoveMode eMode, uint16 u16StepsPerSec);
PUBLIC void vAppStepHue(teCLD_ColourControl_StepMode eMode, uint8 u8StepSize, uint8 u8TransitionTime);
PUBLIC void vAppEnhancedStepHue(teCLD_ColourControl_StepMode eMode, uint16 u16StepSize, uint16 u16TransitionTime);
PUBLIC void vAppMoveToSaturaiton(uint8 u8Saturation, uint16 u16TransitionTime);
PUBLIC void vAppStepColourTemperature(teCLD_ColourControl_StepMode eMode, uint16 u16StepSize);
PUBLIC void vAppStepMoveStop(void);
PUBLIC void vAppMoveToColourTemperature(uint16 u16ColourTemperatureMired, uint16 u16TransitionTime);
PUBLIC void vAppMoveColourTemperature(teCLD_ColourControl_MoveMode eMode, uint16 u16StepsPerSec, uint16 u16ColourTemperatureMin, uint16 u16ColourTemperatureMax);
PUBLIC void vAppColorLoopSet(uint8 u8UpdateFlags, teCLD_ColourControl_LoopAction eAction,teCLD_ColourControl_LoopDirection eDirection, uint16 u16Time, uint16 u16StartHue);
PUBLIC void vAppStopMoveStep();
PUBLIC void vAppGotoHueAndSat( uint8 u8Direction);
PUBLIC void vAPPColourLoopStop(void);
#endif





/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COLOUR_COMMANDS_H__*/
