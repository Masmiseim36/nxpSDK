/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SDMMC_OSA_H_
#define _FSL_SDMMC_OSA_H_

#include "fsl_common.h"
#include "fsl_os_abstraction.h"

/*!
 * @addtogroup SDMMC_OSA
 * @{
 */
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!@brief transfer event */
#define SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS  (1U << 0U)
#define SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL     (1U << 1U)
#define SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS (1U << 2U)
#define SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL    (1U << 3U)

/*!@brief card detect event, start from index 8 */
#define SDMMC_OSA_EVENT_CARD_INSERTED (1U << 8U)
#define SDMMC_OSA_EVENT_CARD_REMOVED  (1U << 9U)
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name sdmmc osa Function
 * @{
 */

/*!
 * @brief Initialize OSA.
 */
void SDMMC_OSAInit(void);

/*!
 * @brief OSA Create event.
 * @param eventHandle event handle.
 * @retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventCreate(void *eventHandle);

/*!
 * @brief Wait event.
 *
 * @param eventHandle The event type
 * @param eventType Timeout time in milliseconds.
 * @param timeoutMilliseconds timeout value in ms.
 * @param event event flags.
 * @retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventWait(void *eventHandle, uint32_t eventType, uint32_t timeoutMilliseconds, uint32_t *event);

/*!
 * @brief set event.
 * @param eventHandle event handle.
 * @param eventType The event type
 * @retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventSet(void *eventHandle, uint32_t eventType);

/*!
 * @brief Get event flag.
 * @param eventHandle event handle.
 * @param mask event mask.
 * @param flag pointer to store event value.
 * @retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventGet(void *eventHandle, uint32_t mask, uint32_t *flag);

/*!
 * @brief clear event flag.
 * @param eventHandle event handle.
 * @param eventType The event type
 * @retval kStatus_Fail or kStatus_Success.
 */
status_t SDMMC_OSAEventClear(void *eventHandle, uint32_t eventType);

/*!
 * @brief Delete event.
 * @param eventHandle The event handle.
 */
status_t SDMMC_OSAEventDestroy(void *eventHandle);

/*!
 * @brief sdmmc delay.
 * @param milliseconds time to delay
 */
void SDMMC_OSADelay(uint32_t milliseconds);

/* @} */

#if defined(__cplusplus)
}
#endif
/* @} */
#endif /* _FSL_SDMMC_OSA_H_*/
