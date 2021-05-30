/*
 * Copyright 2021 NXP.
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

/* bool */
//#ifndef FALSE
//#define FALSE    ((uint16_t)0)
//#endif
//
//#ifndef TRUE
//#define TRUE     ((uint16_t)1)
//#endif


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
void LPTMRInit(uint8_t call_type);
void SystickInit(uint32_t reload_value);
void ClockInit(void);
void PortInit(uint32_t gpio, uint32_t pcr, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux);
void AdcInit(void);
void Tsi0InitSelfCap(void);
void Tsi0InitMutualCap(void);
void Tsi0ClockEnable(void);
void SerialInit();

 #ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_H_ */
