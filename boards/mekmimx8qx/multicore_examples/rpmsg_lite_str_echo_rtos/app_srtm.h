/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_SRTM_H_
#define _APP_SRTM_H_

#include "rpmsg_lite.h"
#include "srtm_auto_service.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IRQ handler priority definition, bigger number stands for lower priority */

/* Task priority definition, bigger number stands for higher priority */
#define APP_SRTM_MONITOR_TASK_PRIO (4U)
#define APP_SRTM_DISPATCHER_TASK_PRIO (3U)
#define APP_LPI2C_IRQ_PRIO (5U)

/* Define the timeout ms to polling the CA7 link up status */
#define APP_LINKUP_TIMER_PERIOD_MS (10U)

#define APP_AUTO_EVENT_REGISTER_POLLING_INTERVAL_MS (200U)

#define RPMSG_LITE_SRTM_SHMEM_BASE (0x90000000U)
#define RPMSG_LITE_SRTM_LINK_ID (RL_PLATFORM_IMX8QX_CM4_SRTM_LINK_ID)
#define APP_SRTM_AUDIO_CHANNEL_NAME "rpmsg-audio-channel"
#define APP_SRTM_AUTO_CHANNEL_NAME "rpmsg-vehicle-channel"
#define APP_SRTM_I2C_CHANNEL_NAME "rpmsg-i2c-channel"

#define PEER_CORE_ID (1U)

#define EXAMPLE_LPI2C (CM4__LPI2C)                                  /*Should change to MIPI_CSI_I2C0*/
#define EXAMPLE_LPI2C_BAUDRATE (400000)                             /*in i2c example it is 100000*/
#define I2C_SOURCE_CLOCK_FREQ_M4 CLOCK_GetIpFreq(kCLOCK_M4_0_Lpi2c) /*M4_LPI2C*/

#define EXAMPLE_IOEXP_LPI2C_BAUDRATE (100000) /*in i2c example it is 100000*/
#define EXAMPLE_IOEXP_LPI2C_MASTER ADMA__LPI2C1
#define I2C_SOURCE_CLOCK_FREQ_LPI2C1 CLOCK_GetIpFreq(kCLOCK_DMA_Lpi2c1) /*ADMA_LPI2C1*/

#define I2C_SWITCH_NONE 1 /*MAX SWITCH in this board is 1*/

#define EXAMPLE_I2C_SWITCH_ADDR 0x71

#define APP_UBOOT_MU LSIO__MU8_B /*MU used for SRTM MU Endpoint*/
#define APP_UBOOT_MU_IRQ LSIO_MU8_INT_B_IRQn
#define APP_KERNEL_MU LSIO__MU5_B /*MU used for SRTM RPMSG Endpoint*/

typedef bool (*app_auto_register_handler_t)(uint32_t clientId,
                                            bool registered,
                                            void *param,
                                            uint8_t *reqData,
                                            uint32_t reqLen,
                                            uint8_t *respData,
                                            uint32_t respLen);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* Initialize SRTM contexts */
void APP_SRTM_Init(void);

/* Create RPMsg channel and start SRTM communication */
void APP_SRTM_StartCommunication(void);

/* Set auto register handler for application */
void APP_SRTM_SetAutoRegisterHandler(app_auto_register_handler_t handler, void *param);

/* Send command to Android. */
bool APP_SRTM_SendAutoCommand(uint32_t clientId,
                              srtm_auto_cmd_t cmd,
                              uint8_t *cmdParam,
                              uint32_t paramLen,
                              uint8_t *result,
                              uint32_t resultLen,
                              uint32_t timeout);

/* SRTM suspend: we need to save SRTM context */
void APP_SRTM_Suspend(void);

/* SRTM resume: we need to restore SRTM context.
   Bool param means if this is a real resume or just recovery from failed power mode switch.*/
void APP_SRTM_Resume(bool resume);

typedef void (*app_rpmsg_monitor_t)(struct rpmsg_lite_instance *rpmsgHandle, bool ready, void *param);

/* Set RPMsg channel init/deinit monitor */
void APP_SRTM_SetRpmsgMonitor(app_rpmsg_monitor_t monitor, void *param);

uint8_t APP_Read_I2C_Register(uint8_t busID, uint16_t slaveAddr, uint8_t regIndex);
uint8_t APP_Write_I2C_Register(uint8_t busID, uint16_t slaveAddr, uint8_t regIndex, uint8_t value);
void APP_SRTM_HandlePeerReboot(void);
#if defined(__cplusplus)
}
#endif

#endif /* _APP_SRTM_H_ */
