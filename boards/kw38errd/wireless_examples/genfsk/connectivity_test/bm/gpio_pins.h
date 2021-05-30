/*
* Copyright 2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef __FSL_GPIO_PINS_H__
#define __FSL_GPIO_PINS_H__

#include "GPIO_Adapter.h"

/*! @file */
/*!*/
/*! This file contains gpio pin definitions used by gpio peripheral driver.*/
/*! The enums in _gpio_pins map to the real gpio pin numbers defined in*/
/*! gpioPinLookupTable. And this might be different in different board.*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief gpio pin names.*/
/*!*/ 
/*! This should be defined according to board setting.*/
   
extern gpioInputPinConfig_t switchPins[];
extern gpioOutputPinConfig_t ledPins[];

#if defined(gBoard_ExtPaSupport_d) && (gBoard_ExtPaSupport_d > 0)
/* Declare External PA GPIO pins - shall be in the order given by structure BOARD_ExtPaCtrlPins_t in board_extPa.h
     two other pads are directly configured by the XCVR drivers */
extern gpioOutputPinConfig_t ExtPaCtrlPins[];
#endif

#endif /* __FSL_GPIO_PINS_H__ */
