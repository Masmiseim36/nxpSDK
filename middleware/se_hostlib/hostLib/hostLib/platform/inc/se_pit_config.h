/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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
void se_pit_SetTimer(U16 time_ms);
#endif // FSL_FEATURE_SOC_PIT_COUNT
#endif // _SE_PIT_CONFIG_H
