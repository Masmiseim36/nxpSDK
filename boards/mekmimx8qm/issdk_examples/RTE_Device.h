/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPI2C2_InitPins();
extern void LPI2C2_DeinitPins();
extern void LPSPI2_InitPins();
extern void LPSPI2_DeinitPins();
extern void LPUART4_InitPins();
extern void LPUART4_DeinitPins();
extern void LPI2C2_InitPins();
extern void LPI2C2_DeinitPins();
extern void LPUART4_InitPins();
extern void LPUART4_DeinitPins();

/* LPI2C instance mapping */
#define LPI2C0 CM4_0__LPI2C
#define LPI2C1 CM4_1__LPI2C
#define LPI2C2 DMA__LPI2C0
#define LPI2C3 DMA__LPI2C1
#define LPI2C4 DMA__LPI2C2
#define LPI2C5 DMA__LPI2C3
#define LPI2C6 DMA__LPI2C4

/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C2            1
#define RTE_I2C2_PIN_INIT   LPI2C2_InitPins
#define RTE_I2C2_PIN_DEINIT LPI2C2_DeinitPins
#define RTE_I2C2_DMA_EN     0

/* SPI instance name mapping */
#define LPSPI0 DMA__LPSPI0
#define LPSPI1 DMA__LPSPI1
#define LPSPI2 DMA__LPSPI2
#define LPSPI3 DMA__LPSPI3

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI2            1
#define RTE_SPI2_PIN_INIT   LPSPI2_InitPins
#define RTE_SPI2_PIN_DEINIT LPSPI2_DeinitPins
#define RTE_SPI2_DMA_EN     0

/* USART instance mapping */
#define LPUART0 CM4_0__LPUART
#define LPUART1 CM4_1__LPUART
#define LPUART2 DMA__LPUART0
#define LPUART3 DMA__LPUART1
#define LPUART4 DMA__LPUART2
#define LPUART5 DMA__LPUART3
#define LPUART6 DMA__LPUART4

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART4            1
#define RTE_USART4_PIN_INIT   LPUART4_InitPins
#define RTE_USART4_PIN_DEINIT LPUART4_DeinitPins
#define RTE_USART4_DMA_EN     0

/* LPI2C configuration. */
/*Note: LPI2C0 and LPI2C1 not support DMA */
#define RTE_I2C2_PIN_INIT        LPI2C2_InitPins
#define RTE_I2C2_PIN_DEINIT      LPI2C2_DeinitPins
#define RTE_I2C2_DMA_TX_CH       1
#define RTE_I2C2_DMA_TX_PERI_SEL 1
#define RTE_I2C2_DMA_TX_DMA_BASE DMA__EDMA1
#define RTE_I2C2_DMA_RX_CH       0
#define RTE_I2C2_DMA_RX_PERI_SEL 0
#define RTE_I2C2_DMA_RX_DMA_BASE DMA__EDMA1

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART4_RX_BUFFER_ENABLE 1

/* Note: LPUART0, LPUART1 not support DMA mode */

#define RTE_USART4_PIN_INIT        LPUART4_InitPins
#define RTE_USART4_PIN_DEINIT      LPUART4_DeinitPins
#define RTE_USART4_DMA_TX_CH       17
#define RTE_USART4_DMA_TX_PERI_SEL 17
#define RTE_USART4_DMA_TX_DMA_BASE DMA__EDMA0
#define RTE_USART4_DMA_RX_CH       16
#define RTE_USART4_DMA_RX_PERI_SEL 16
#define RTE_USART4_DMA_RX_DMA_BASE DMA__EDMA0

#endif /* _RTE_DEVICE_H */
