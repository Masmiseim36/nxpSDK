/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef LPM_H
#define LPM_H

#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define configOSTIMER_CLOCK_HZ (CLOCK_GetOSTimerClkFreq())
#define portMAX_32_BIT_NUMBER  (0xffffffffUL)
#if defined(PMC0)
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER_CPU0  /*!< Tickless timer base address. */
#else
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER_CPU1  /*!< Tickless timer base address. */
#endif
#define TICKLESS_OSTIMER_IRQn     OS_EVENT_IRQn /*!< Tickless timer IRQ number. */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Configure the LPTimer(OSTIMER) before entering the low power mode.
 * @return Return the OSTIMER start value.
 */
TickType_t LPM_EnterTicklessIdle(TickType_t xExpectedIdleTime, uint64_t *pCounter);
/*!
 * @brief Configure the LPTimer(OSTIMER) after exist the low power mode.
 * @param base the OSTIMER start value before low power mode.
 */
void LPM_ExitTicklessIdle(TickType_t xExpectedIdleTime, uint64_t base);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* LPM_H */