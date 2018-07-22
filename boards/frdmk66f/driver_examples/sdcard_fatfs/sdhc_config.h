/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#ifndef _SDHC_CONFIG_H_
#define _SDHC_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include "fsl_sdhc.h"
#include "board.h"

/*!
 * @addtogroup SDHC Configure
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Card detection way is GPIO not other ways. */
#define SDHC_CARD_DETECT_USING_GPIO

/* Endian mode. */
#define SDHC_ENDIAN_MODE kSDHC_EndianModeLittle

/* DMA mode */
#define SDHC_DMA_MODE kSDHC_DmaModeAdma2

/* Read/write watermark level. The bigger value indicates DMA has higher read/write performance. */
#define SDHC_READ_WATERMARK_LEVEL (0x80U)
#define SDHC_WRITE_WATERMARK_LEVEL (0x80U)

/* ADMA table length united as word.
One ADMA1 table item occupy one word which can transfer maximum 0xFFFFU bytes one time.
One ADMA2 table item occupy two words which can transfer maximum 0xFFFFU bytes one time.
The more data to be transferred in one time, the bigger value of ADMA_TABLE_WORDS need to be set.
*/
#define SDHC_ADMA_TABLE_WORDS (8U)

/*! The maximum timeout time for the transfer complete event */
#define EVENT_TIMEOUT_TRANSFER_COMPLETE (1000U)

/*! @brief Event type  */
typedef enum _event
{
    kEVENT_TransferComplete = 0U, /*!< Transfer complete event */
} event_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDHC Configure Function
 * @{
 */

/*!
 * @brief Initialize timer to implement timeout detection for EVENT_Wait function.
 */
void EVENT_InitTimer(void);

/* Callback function for SDHC */

/*!
 * @brief Create event.
 * @param eventType The event type
 * @retval true Create event successfully.
 * @retval false Create event failed.
 */
bool EVENT_Create(event_t eventType);

/*!
 * @brief Wait event.
 *
 * @param eventType The event type
 * @param timeoutMilliseconds Timeout time in milliseconds.
 * @retval true Wait event successfully.
 * @retval false Wait event failed.
 */
bool EVENT_Wait(event_t eventType, uint32_t timeoutMilliseconds);

/*!
 * @brief Notify event.
 * @param eventType The event type
 * @retval true Notify event successfully.
 * @retval false Notify event failed.
 */
bool EVENT_Notify(event_t eventType);

/*!
 * @brief Delete event.
 * @param eventType The event type
 */
void EVENT_Delete(event_t eventType);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif /* _SDHC_CONFIG_H_ */
