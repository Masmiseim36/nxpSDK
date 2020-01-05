/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* UART Select, LPUART0 - LPUART2. */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_USART0 0
#define RTE_USART1 0
#define RTE_USART2 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN 64
#define USART0_RX_BUFFER_ENABLE 0
#define USART1_RX_BUFFER_ENABLE 0
#define USART2_RX_BUFFER_ENABLE 0

/* I2C Select, LPI2C0. */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_I2C0 0

/* SPI Select, LPSPI0. */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_SPI0 0

/* SPI configuration. */
#define RTE_SPI0_PCS_TO_SCK_DELAY 1000
#define RTE_SPI0_SCK_TO_PSC_DELAY 1000
#define RTE_SPI0_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI0_MASTER_PCS_PIN_SEL (kLPSPI_MasterPcs3)
#define RTE_SPI0_SLAVE_PCS_PIN_SEL (kLPSPI_SlavePcs3)

#endif /* __RTE_DEVICE_H */
