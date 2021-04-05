/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_IMXRT_H_
#define _PROJECT_SETUP_IMXRT_H_

#include "MIMX8MM6_cm4.h"
#include "safety_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Project setup functions
 * @{
 */
/*******************************************************************************
 * API
 ******************************************************************************/
void WatchdogEnable(void);
void WatchdogDisable(void);
void GPT1_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler);
void QTMR1_Init(uint32_t compare);
void SystickInitialisation(uint32_t compare);
void SerialInit(UART_Type *Uart_X, uint32_t baudRate, uint32_t clockFreq);
void ClockInit(void);
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_IMXRT_H_ */
