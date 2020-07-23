/*
* Copyright 2019 NXP
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

/* SPI Master Data Available Pin */
#define gSpiMasterDap_Port_d  gpioPort_B_c
#define gSpiMasterDap_Pin_d   1

/* SPI Slave Data Available Pin */
#define gSpiSlaveDap_Port_d   gpioPort_B_c
#define gSpiSlaveDap_Pin_d    1

#endif /* __FSL_GPIO_PINS_H__ */
