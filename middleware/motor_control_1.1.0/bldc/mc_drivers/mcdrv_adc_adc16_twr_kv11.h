/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _MCDRV_ADC_ADC16_TWR_KV11_H_
#define _MCDRV_ADC_ADC16_TWR_KV11_H_

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
bool_t MCDRV_Adc16Init_twr_kv11(mcdrv_adc16_t *this, mcdrv_adc16_init_t *init);

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
bool_t MCDRV_VoltDcBusGet_twr_kv11(mcdrv_adc16_t *this);

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
bool_t MCDRV_CurrDcBusGet_twr_kv11(mcdrv_adc16_t *this);

/*!
 * @brief Function reads current samples and filter them
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_CurrOffsetCalib_twr_kv11(mcdrv_adc16_t *this);

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

#endif /* _MCDRV_ADC_ADC16_TWR_KV11_H_ */

