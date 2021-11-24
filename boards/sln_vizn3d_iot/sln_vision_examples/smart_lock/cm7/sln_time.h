/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _FSL_TIME_H_
#define _FSL_TIME_H_

#include "fsl_common.h"

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*! @brief Time_Init.
 *
 * Init the PIT timer 1.
 */
void Time_Init(unsigned int unit_us);

/*! @brief TIME_Now.
 *
 * Get the original decremental PIT timer 1.
 */
unsigned int Time_Now();

/*! @brief Time_Current.
 *
 * Get the incremental PIT timer 1 value.
 */
unsigned int Time_Current();

/*! @brief Time_Unit.
 *
 * Return the unit in us of the PIT timer 1.
 */
unsigned int Time_Unit();

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /*_FSL_TIME_H_*/
