/*
* Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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

/* SPI Master Data Available Pin */
#define gSpiMasterDap_Port_d  gpioPort_B_c
#define gSpiMasterDap_Pin_d   1
/* SPI Slave Data Available Pin */
#define gSpiSlaveDap_Port_d   gpioPort_B_c
#define gSpiSlaveDap_Pin_d    1
/* I2C Master Data Available Pin */
#define gI2cMasterDap_Port_d  gpioPort_B_c
#define gI2cMasterDap_Pin_d   1
/* I2C Slave Data Available Pin */
#define gI2cSlaveDap_Port_d   gpioPort_B_c
#define gI2cSlaveDap_Pin_d    1

/* switch pins to be initialized by the low power component if the keyboard isn't enabled*/
/* 1<<n for sitchPins[n] */   
#define gLowPower_switchPinsToInitBitmap_d  ( 1<<1 | 1<<0 )
   
extern gpioInputPinConfig_t switchPins[];
extern gpioOutputPinConfig_t ledPins[];

extern gpioInputPinConfig_t   gCoexistence_RfDeny;
extern gpioOutputPinConfig_t  gCoexistence_RfActive;
extern gpioOutputPinConfig_t  gCoexistence_RfStatus;
#endif /* __FSL_GPIO_PINS_H__ */
