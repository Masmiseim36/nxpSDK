/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_KV11_H_
#define _PROJECT_SETUP_KV11_H_

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
void ClockInit(void);
void SystickInit(uint32_t reloadValue);
void LPTMRInit(void);
void AdcInit(void);
void PortInit(uint32_t gpio, uint32_t pcr, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux);
void SerialInit(void);

 #ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_KV11_H_ */
