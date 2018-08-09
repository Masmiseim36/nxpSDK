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
