/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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
