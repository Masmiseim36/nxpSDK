/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/

/*******************************************************************************
 * Variables - only template (to be framework builtable)
 ******************************************************************************/
mcdrv_adc_temp_t g_sM1AdcSensor;
mcdrv_pwm_temp_t g_sM1Pwm3ph;
clock_setup_t g_sClockSetup;
mcdrv_qd_enc_temp_t g_sM1Enc;

#ifdef DUAL_MOTOR
mcdrv_adc_temp_t g_sM2AdcSensor;
mcdrv_pwm_temp_t g_sM2Pwm3ph;
mcdrv_qd_enc_temp_t g_sM2Enc;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
