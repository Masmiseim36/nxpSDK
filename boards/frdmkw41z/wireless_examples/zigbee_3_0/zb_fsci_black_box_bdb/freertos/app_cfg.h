/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

/*!
\file       app_cfg.h
\brief      This file is a for application configuration of demo applications
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_BOARD_TEST_GPIO_PORT        0

#define APP_BOARD_TEST_LED0_PIN				0
#define APP_BOARD_TEST_LED1_PIN				1
#define APP_BOARD_TEST_LED2_PIN				2
#define APP_BOARD_TEST_LED3_PIN				3

#define APP_SW1_PORT				0
#define APP_SW2_PORT				1
#define APP_SW1_PIN					4
#define APP_SW2_PIN					5

#define LED1_DIO_PIN APP_BOARD_TEST_LED0_PIN
#define LED2_DIO_PIN APP_BOARD_TEST_LED3_PIN
#define LED_DIO_PINS ( 1 << LED1_DIO_PIN | 1 << LED2_DIO_PIN )


/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/


#endif /* _APP_CFG_H_ */
