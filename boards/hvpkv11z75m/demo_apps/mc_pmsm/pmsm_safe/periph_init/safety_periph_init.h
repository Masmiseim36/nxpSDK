/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_PERIPH_INIT_H_
#define _SAFETY_PERIPH_INIT_H_

#include "hardware_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   Initialization of safety-related peripherals (clocks, pin-muxes,
 *          watchdog,...).
 *
 * @param   void
 *
 * @return  none
 */
void FSPER_fsInit(void);


/*!
 * @brief  Watchdog periphery configuration (e.g. clock source, timeout and
 *         pre-scaler) and enable functions.
 *
 * @param  void
 *
 * @return none
 */
void FSPER_fsWatchdogEnable(void);

/*!
 * @brief  Watchdog periphery disable functions.
 *
 * @note   The interrupts are globally disabled during function execution.
 *
 * @param  void
 *
 * @return none
 */
void FSPER_fsWatchdogDisable(void);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SAFETY_PERIPH_INIT_H_ */
