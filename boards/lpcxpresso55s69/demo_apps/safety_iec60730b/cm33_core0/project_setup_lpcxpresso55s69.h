/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_H_
#define _PROJECT_SETUP_H_

#include "safety_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BACKUP  0
#define RESTORE 1


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
 void CTIMER_initialisation(void);
 void second_timer_inicialization(void); /* Second timer for CLOKC TEST */
 void SystickInitialisation(uint32_t reload_value);
 void ClockSetup(void);
 void PortSetup(uint8_t *pByte, uint32_t *pDir, uint32_t *pIocon, uint32_t pinDir, uint32_t pinNum, uint32_t pull, uint32_t clock_enable_shift);
 void Adc0Setup(void);
 void Tsi0SetupSelfCap(void);
 void Tsi0SetupMutualCap(void);
 void InitSerial(USART_Type *Uart_X, uint32_t brate, uint64_t bclk);

 #ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_H_ */
