/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPI2C8_InitPins();
extern void LPI2C8_DeinitPins();

extern void LPSPI14_InitPins();
extern void LPSPI14_DeinitPins();

extern void LPUART0_InitPins();
extern void LPUART0_DeinitPins();


/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C8        1
#define RTE_I2C8_DMA_EN 0

/* LPI2C configuration. */
#define RTE_I2C8_PIN_INIT        LPI2C8_InitPins
#define RTE_I2C8_PIN_DEINIT      LPI2C8_DeinitPins
#define RTE_I2C8_DMA_TX_CH       0
#define RTE_I2C8_DMA_TX_PERI_SEL (uint16_t) kDmaRequestMuxLpFlexcomm8Tx
#define RTE_I2C8_DMA_TX_DMA_BASE DMA0
#define RTE_I2C8_DMA_RX_CH       1
#define RTE_I2C8_DMA_RX_PERI_SEL (uint16_t) kDmaRequestMuxLpFlexcomm8Rx
#define RTE_I2C8_DMA_RX_DMA_BASE DMA0

/*Driver name mapping.*/
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI14        1
#define RTE_SPI14_DMA_EN 0

/* SPI configuration. */
#define RTE_SPI14_PIN_INIT               LPSPI14_InitPins
#define RTE_SPI14_PIN_DEINIT             LPSPI14_DeinitPins
#define RTE_SPI14_DMA_TX_CH              0
#define RTE_SPI14_DMA_TX_PERI_SEL        kDmaRequestMuxLpspi14Tx
#define RTE_SPI14_DMA_TX_DMA_BASE        DMA0
#define RTE_SPI14_DMA_RX_CH              1
#define RTE_SPI14_DMA_RX_PERI_SEL        kDmaRequestMuxLpspi14Rx
#define RTE_SPI14_DMA_RX_DMA_BASE        DMA0

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART0        1
#define RTE_USART0_DMA_EN 0

/* UART configuration. */
#define RTE_USART0_PIN_INIT        LPUART0_InitPins
#define RTE_USART0_PIN_DEINIT      LPUART0_DeinitPins
#define RTE_USART0_DMA_TX_CH       0
#define RTE_USART0_DMA_TX_PERI_SEL (uint16_t) kDmaRequestMuxLpFlexcomm0Tx
#define RTE_USART0_DMA_TX_DMA_BASE DMA0
#define RTE_USART0_DMA_RX_CH       1
#define RTE_USART0_DMA_RX_PERI_SEL (uint16_t) kDmaRequestMuxLpFlexcomm0Rx
#define RTE_USART0_DMA_RX_DMA_BASE DMA0

#endif /* _RTE_DEVICE_H */
