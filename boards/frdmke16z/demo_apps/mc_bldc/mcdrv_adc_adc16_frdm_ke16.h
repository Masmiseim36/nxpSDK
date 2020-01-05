/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ADC_ADC16_FRDM_KE16Z_H_
#define _MCDRV_ADC_ADC16_FRDM_KE16Z_H_

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
 *  Board specific file for the tower kv11
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init_frdm_ke16(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init);

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function reads ADC result register containing actual BEMF voltage
 *
 * Result register value is shifted three times to the right and stored
 * to BEMF voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_BemfVoltageGet_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function reads ADC result register containing actual DC-bus voltage sample
 *
 * Result register value is shifted three times to the right and stored
 * to DC-bus voltage pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_VoltDcBusGet_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 * Result register value is shifted three times to the right and stored
 * to DC-bus current pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrDcBusGet_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function initializes phase current channel offset measurement
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibInit_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalib_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function passes measured offset values to main structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalibSet_frdm_ke16(mcdrv_adc16_t *this);

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignDCBusChannel(mcdrv_adc16_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ADC_ADC16_FRDM_KE16Z_H_ */

