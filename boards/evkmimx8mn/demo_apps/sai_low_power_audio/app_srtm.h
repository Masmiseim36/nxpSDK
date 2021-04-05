/*
 * Copyright 2018-2020, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_SRTM_H_
#define _APP_SRTM_H_

#include "rpmsg_lite.h"
#include "rsc_table.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum
{
    APP_SRTM_StateRun = 0x0U,
    APP_SRTM_StateLinkedUp,
} app_srtm_state_t;

#define APP_SRTM_SAI      (I2S3)
#define APP_SRTM_SAI_IRQn I2S3_IRQn
/* The MCLK of the SAI is 24576000Hz by default which can be changed when playback the music. */
#define APP_SAI_CLK_FREQ (24576000U)
/* The frequency of the audio pll 1/2 are the fixed value. */
#define APP_AUDIO_PLL1_FREQ (393216000U)
#define APP_AUDIO_PLL2_FREQ (361267200U)
#define APP_SRTM_DMA        SDMAARM3
#define APP_SRTM_DMA_IRQn   SDMA3_IRQn
/* IRQ handler priority definition, bigger number stands for lower priority */
#define APP_SAI_TX_DMA_IRQ_PRIO (5U)
#define APP_SAI_RX_DMA_IRQ_PRIO (5U)
#define APP_SAI_IRQ_PRIO        (5U)
/* Task priority definition, bigger number stands for higher priority */
#define APP_SRTM_MONITOR_TASK_PRIO    (4U)
#define APP_SRTM_DISPATCHER_TASK_PRIO (3U)
/* SAI SDMA channel */
#define APP_SAI_TX_DMA_CHANNEL          (1U)
#define APP_SAI_RX_DMA_CHANNEL          (2U)
#define APP_SAI_RX_DMA_SOURCE           (4U)
#define APP_SAI_TX_DMA_SOURCE           (5U)
#define APP_SAI_TX_DMA_CHANNEL_PRIORITY (2U)
#define APP_SAI_RX_DMA_CHANNEL_PRIORITY (2U)
/* WM8524 Pin Set*/
#define APP_CODEC_BUS_PIN      (NULL)
#define APP_CODEC_BUS_PIN_NUM  (0)
#define APP_CODEC_MUTE_PIN     (GPIO5)
#define APP_CODEC_MUTE_PIN_NUM (21)
/* Define the timeout ms to polling the CA7 link up status */
#define APP_LINKUP_TIMER_PERIOD_MS (10U)

#define RPMSG_LITE_SRTM_SHMEM_BASE (VDEV0_VRING_BASE)
#define RPMSG_LITE_SRTM_LINK_ID    (0U)

#define APP_SRTM_AUDIO_CHANNEL_NAME "rpmsg-audio-channel"

typedef void (*app_rpmsg_monitor_t)(struct rpmsg_lite_instance *rpmsgHandle, bool ready, void *param);
typedef void (*app_irq_handler_t)(IRQn_Type irq, void *param);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* Initialize SRTM contexts */
void APP_SRTM_Init(void);

/* Deinit SRTM service in suspend */
void APP_SRTM_Suspend(void);

/* Restore SRTM service in resume */
void APP_SRTM_Resume(void);

/* Set RPMsg channel init/deinit monitor */
void APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor_t monitor, void *param);

/* Set SNVS IRQ handler for application */
void APP_SRTM_SetIRQHandler(app_irq_handler_t handler, void *param);

/* Check the SRTM services busy or idle.*/
bool APP_SRTM_ServiceIdle(void);
#if defined(__cplusplus)
}
#endif

#endif /* _APP_SRTM_H_ */
