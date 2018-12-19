/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file wait_aml.h
 *
 * This driver creates abstraction for WAIT functions for S32 SDK and MCUXpresso
 * SDK.
 */

#ifndef SOURCE_WAIT_AML_H_
#define SOURCE_WAIT_AML_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "../common_aml.h"

#if (SDK_VERSION == SDK_KSDK)
#include "fsl_clock.h"
#elif (SDK_VERSION == SDK_S32)
#include "clock_manager.h"
#include "device_registers.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup macro_group
 * @{
 */
#define WAIT_AML_SYSTEM_CLOCK_FREQ   (WAIT_AML_GetSystemClockFreq())

#define WAIT_AML_GET_CYCLES_FOR_MS(ms, freq) (((freq) / 1000U) * (ms))            /*!< Gets needed cycles for specified delay in milliseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_US(us, freq) (((freq) / 1000U) * (us) / 1000U)    /*!< Gets needed cycles for specified delay in microseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_NS(ns, freq) (((freq) / 1000000U) * (ns) / 1000U) /*!< Gets needed cycles for specified delay in nanoseconds, calculation is based on core clock frequency. */
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup function_group
 * @{
 */ 

/*!
 * @brief Returns system clock frequency.
*/
uint32_t WAIT_AML_GetSystemClockFreq(void);

/*!
 * @brief Waits for specified amount of cycles which is given by 32bit 
 *        value range. Assumption for this function is that target 
 *        architecture is using 32bit general purpose registers.
 * 
 * @param cycles - Number of cycles to wait.
 */
void WAIT_AML_WaitCycles(uint32_t cycles);

/*!
 * @brief Waits for specified amount of seconds.
 *
 * @param delay - Number of seconds to wait.
 */
void WAIT_AML_WaitSec(uint16_t delay);

/*!
 * @brief Waits for specified amount of milliseconds.
 *
 * @param delay - Number of milliseconds to wait.
 */
void WAIT_AML_WaitMs(uint16_t delay);

/*!
 * @brief Waits for specified amount of microseconds.
 *
 * @param delay - Number of microseconds to wait.
 */
void WAIT_AML_WaitUs(uint16_t delay);
/*! @} */

#endif /* SOURCE_WAIT_AML_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
