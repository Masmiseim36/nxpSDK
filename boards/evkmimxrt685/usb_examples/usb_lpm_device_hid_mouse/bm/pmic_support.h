/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PMIC_SUPPORT_H_
#define _PMIC_SUPPORT_H_

#include "fsl_pca9420.h"
#include "fsl_power.h"

/*******************************************************************************
 * DEFINITION
 ******************************************************************************/
extern pca9420_handle_t pca9420Handle;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/
void BOARD_SetPmicVoltageForFreq(power_part_temp_range temp_range, uint32_t main_clk_freq, uint32_t dsp_main_clk_freq);
void BOARD_InitPmic(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _PMIC_SUPPORT_H_ */
