/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ALI_IOT_CONFIG_H_
#define _ALI_IOT_CONFIG_H_

#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ALI_IOT_PRODUCT_KEY    "a1Y2qrYXwAx"
#define ALI_IOT_PRODUCT_SECRET ""
#define ALI_IOT_DEVICE_NAME    "KL27"
#define ALI_IOT_DEVICE_SECRET  "gqeXXh2eTNq34FEy2BL4BRu9qxTavEEB"

#define ALI_IOT_DEVICE_PORT_UART_BASE     UART2
#define ALI_IOT_DEVICE_PORT_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
#ifndef FSL_RTOS_FREE_RTOS
/*!
 * @brief Calls initialization the timer.
 *
 */
void ali_iot_timer_init(uint32_t timerInstance, uint32_t sourceClock);

/*!
 * @brief Calls get timer ticks
 *
 */
uint32_t ali_iot_timer_get_ticks(void);
#endif

#endif /* _ALI_IOT_CONFIG_H_ */
