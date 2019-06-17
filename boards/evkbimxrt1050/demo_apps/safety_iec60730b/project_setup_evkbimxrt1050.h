/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
* 
*
*******************************************************************************/

#ifndef _PROJECT_SETUP_IMXRT_H_
#define _PROJECT_SETUP_IMXRT_H_

#include "MIMXRT1052.h"
#include "safety_config.h"

#include "fsl_lpuart.h"

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
void WatchdogEnable(RTWDOG_Type *WDOGx, unsigned long timeout);
void WatchdogDisable(RTWDOG_Type *WDOGx);
void GPT1_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler);
void GPT2_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler);
void QTMR1_Init(uint32_t compare);
void SystickInitialisation(uint32_t compare);
void SerialInit(LPUART_Type *Uart_X, uint32_t baudRate, uint32_t clockFreq);
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir);
void MPUSetup(void);
void DCPInit(unsigned long *pBuffer);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_IMXRT_H_ */

