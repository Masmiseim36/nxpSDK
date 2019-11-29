/*
 * Copyright 2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_SRTM_H_
#define _APP_SRTM_H_

#include "rpmsg_lite.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_SRTM_CODEC_WM8524_USED (0U)
#define APP_SRTM_CODEC_AK4497_USED (1U)

#if APP_SRTM_CODEC_AK4497_USED

#define APP_SRTM_I2C I2C3
#define APP_SRTM_I2C_IRQn I2C3_IRQn
#define APP_SRTM_I2C_IRQ_PRIO (5U)
#define APP_SRTM_I2C_DELAY (100U)
#define APP_SRTM_I2C_BAUDRATE (100000U)
#define APP_CODEC_I2C_INSTANCE (3U)
#define APP_SRTM_I2C_CLOCK_FREQ                                                            \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootI2c3)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootI2c3)) / 5

#define APP_AUDIO_I2C_SCL_GPIO GPIO5
#define APP_AUDIO_I2C_SCL_PIN (18U)
#define APP_AUDIO_I2C_SDA_GPIO GPIO5
#define APP_AUDIO_I2C_SDA_PIN (19U)
#endif

#if APP_SRTM_CODEC_WM8524_USED
#define APP_SRTM_SAI (I2S3)
#define APP_SRTM_SAI_IRQn I2S3_IRQn
#define APP_SAI_CLK_FREQ                                                                  \
    CLOCK_GetPllFreq(kCLOCK_AudioPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootSai3))
#else
#define APP_SRTM_SAI (I2S1)
#define APP_SRTM_SAI_IRQn I2S1_IRQn
#define APP_SAI_CLK_FREQ                                                                  \
    CLOCK_GetPllFreq(kCLOCK_AudioPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai1)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootSai1))
#endif
#define APP_SRTM_DMA SDMAARM3
#define APP_SRTM_DMA_IRQn SDMA3_IRQn
/* IRQ handler priority definition, bigger number stands for lower priority */
#define APP_SAI_TX_DMA_IRQ_PRIO (5U)
#define APP_SAI_RX_DMA_IRQ_PRIO (5U)
#define APP_SAI_IRQ_PRIO (5U)
/* Task priority definition, bigger number stands for higher priority */
#define APP_SRTM_MONITOR_TASK_PRIO (4U)
#define APP_SRTM_DISPATCHER_TASK_PRIO (3U)
/* SAI SDMA channel */
#define APP_SAI_TX_DMA_CHANNEL (1U)
#define APP_SAI_RX_DMA_CHANNEL (0U)

#if APP_SRTM_CODEC_WM8524_USED
#define APP_SAI_RX_DMA_SOURCE (4U)
#define APP_SAI_TX_DMA_SOURCE (5U)
#else
#define APP_SAI_RX_DMA_SOURCE (0U)
#define APP_SAI_TX_DMA_SOURCE (1U)
#endif

#define APP_SAI_TX_DMA_CHANNEL_PRIORITY (2U)
#define APP_SAI_RX_DMA_CHANNEL_PRIORITY (2U)

#if APP_SRTM_CODEC_WM8524_USED
/* WM8524 Pin Set*/
#define APP_CODEC_BUS_PIN (NULL)
#define APP_CODEC_BUS_PIN_NUM (0)
#define APP_CODEC_MUTE_PIN (GPIO5)
#define APP_CODEC_MUTE_PIN_NUM (21)
#endif
/* Define the timeout ms to polling the CA7 link up status */
#define APP_LINKUP_TIMER_PERIOD_MS (10U)

#define RPMSG_LITE_SRTM_SHMEM_BASE (0xB8000000U)
#define RPMSG_LITE_SRTM_LINK_ID (0U)

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
#if APP_SRTM_CODEC_AK4497_USED
/* Release I2C buses used by SRTM services */
void APP_SRTM_I2C_ReleaseBus(void);
#endif
/* Check the SRTM services busy or idle.*/
bool APP_SRTM_ServiceIdle(void);
#if defined(__cplusplus)
}
#endif

#endif /* _APP_SRTM_H_ */
