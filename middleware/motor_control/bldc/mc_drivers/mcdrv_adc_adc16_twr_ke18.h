/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ADC_ADC16_TWR_KE18_H_
#define _MCDRV_ADC_ADC16_TWR_KE18_H_

#include "gdflib.h"
#include "gmclib_types.h"
#include "fsl_device_registers.h"
#include "mcdrv_adc_adc16.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initializes ADC driver to measure DC-bus current, DC-bus voltage
 *        and BEMF voltage for BLDC sensorless algorithm
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init_twr_ke18(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init);

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_twr_ke18(mcdrv_adc16_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_ADC16_TWR_KE18_H_ */

