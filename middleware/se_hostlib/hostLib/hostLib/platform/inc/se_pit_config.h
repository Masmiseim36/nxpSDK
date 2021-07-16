/* Copyright 2018-2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SE_PIT_CONFIG_H
#define _SE_PIT_CONFIG_H

#include "sm_types.h"

#if FSL_FEATURE_SOC_PIT_COUNT > 0
/*
 * Sets the periodic timer for reseting the IC
 *
 *   input params:
 *       time_ms: Time in ms
 *
*/
void se_pit_SetTimer(uint32_t time_ms);
#endif // FSL_FEATURE_SOC_PIT_COUNT
#endif // _SE_PIT_CONFIG_H
