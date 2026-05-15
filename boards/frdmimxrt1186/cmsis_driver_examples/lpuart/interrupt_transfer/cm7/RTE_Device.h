/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitDEBUG_UARTPins();
extern void BOARD_InitDEBUG_UARTPins_deinit();

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART1        1
#define RTE_USART1_DMA_EN 0

/* UART configuration. */
#define RTE_USART1_PIN_INIT   BOARD_InitDEBUG_UARTPins
#define RTE_USART1_PIN_DEINIT BOARD_InitDEBUG_UARTPins_deinit

#endif /* _RTE_DEVICE_H */
