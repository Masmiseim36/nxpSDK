/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void I2C0_InitPins();
extern void I2C0_DeinitPins();
extern void I2C1_InitPins();
extern void I2C1_DeinitPins();
extern void DSPI1_InitPins();
extern void DSPI1_DeinitPins();
extern void UART1_InitPins();
extern void UART1_DeinitPins();
extern void UART2_InitPins();
extern void UART2_DeinitPins();
extern void DSPI1_InitPins();
extern void DSPI1_DeinitPins();
extern void UART1_InitPins();
extern void UART1_DeinitPins();
extern void UART2_InitPins();
extern void UART2_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C0            1
#define RTE_I2C0_PIN_INIT   I2C0_InitPins
#define RTE_I2C0_PIN_DEINIT I2C0_DeinitPins
#define RTE_I2C0_DMA_EN     0
#define RTE_I2C1            1
#define RTE_I2C1_PIN_INIT   I2C1_InitPins
#define RTE_I2C1_PIN_DEINIT I2C1_DeinitPins
#define RTE_I2C1_DMA_EN     0

/* User needs to provide the implementation of DSPIX_GetFreq/DSPIX_InitPins/DSPIX_DeinitPins for the enabled DSPI
 * instance. */
#define RTE_SPI1            1
#define RTE_SPI1_PIN_INIT   DSPI1_InitPins
#define RTE_SPI1_PIN_DEINIT DSPI1_DeinitPins
#define RTE_SPI1_DMA_EN     0

/* Use UART0 - UART2. */
/* User needs to provide the implementation of UARTX_GetFreq/UARTX_InitPins/UARTX_DeinitPins for the enabled UART
 * instance. */
#define RTE_USART1            1
#define RTE_USART1_PIN_INIT   UART1_InitPins
#define RTE_USART1_PIN_DEINIT UART1_DeinitPins
#define RTE_USART1_DMA_EN     0
#define RTE_USART2            1
#define RTE_USART2_PIN_INIT   UART2_InitPins
#define RTE_USART2_PIN_DEINIT UART2_DeinitPins
#define RTE_USART2_DMA_EN     0
/* Use LPUART0. */

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART1_RX_BUFFER_ENABLE 1
#define USART2_RX_BUFFER_ENABLE 1

/* DSPI configuration. */

#define RTE_SPI1_PCS_TO_SCK_DELAY       1000
#define RTE_SPI1_SCK_TO_PSC_DELAY       1000
#define RTE_SPI1_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI1_MASTER_PCS_PIN_SEL     kDSPI_MasterPcs0
#define RTE_SPI1_PIN_INIT               DSPI1_InitPins
#define RTE_SPI1_PIN_DEINIT             DSPI1_DeinitPins
#define RTE_SPI1_DMA_TX_CH              0
#define RTE_SPI1_DMA_TX_PERI_SEL        (uint8_t) kDmaRequestMux0SPI1
#define RTE_SPI1_DMA_TX_DMAMUX_BASE     DMAMUX0
#define RTE_SPI1_DMA_TX_DMA_BASE        DMA0
#define RTE_SPI1_DMA_RX_CH              1
#define RTE_SPI1_DMA_RX_PERI_SEL        (uint8_t) kDmaRequestMux0SPI1
#define RTE_SPI1_DMA_RX_DMAMUX_BASE     DMAMUX0
#define RTE_SPI1_DMA_RX_DMA_BASE        DMA0
#define RTE_SPI1_DMA_LINK_DMA_BASE      DMA0
#define RTE_SPI1_DMA_LINK_CH            2

/*I2C configuration*/
#define RTE_I2C0_Master_DMA_BASE    DMA0
#define RTE_I2C0_Master_DMA_CH      0
#define RTE_I2C0_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_Master_PERI_SEL    kDmaRequestMux0I2C0

#define RTE_I2C1_Master_DMA_BASE    DMA0
#define RTE_I2C1_Master_DMA_CH      1
#define RTE_I2C1_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C1_Master_PERI_SEL    kDmaRequestMux0I2C1

/* UART configuration. */

#define RTE_USART1_PIN_INIT           UART1_InitPins
#define RTE_USART1_PIN_DEINIT         UART1_DeinitPins
#define RTE_USART1_DMA_TX_CH          0
#define RTE_USART1_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMux0UART1Tx
#define RTE_USART1_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_USART1_DMA_TX_DMA_BASE    DMA0
#define RTE_USART1_DMA_RX_CH          1
#define RTE_USART1_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMux0UART1Rx
#define RTE_USART1_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_USART1_DMA_RX_DMA_BASE    DMA0

#define RTE_USART2_PIN_INIT           UART2_InitPins
#define RTE_USART2_PIN_DEINIT         UART2_DeinitPins
#define RTE_USART2_DMA_TX_CH          0
#define RTE_USART2_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMux0UART2Tx
#define RTE_USART2_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_USART2_DMA_TX_DMA_BASE    DMA0
#define RTE_USART2_DMA_RX_CH          1
#define RTE_USART2_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMux0UART2Rx
#define RTE_USART2_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_USART2_DMA_RX_DMA_BASE    DMA0

#endif /* _RTE_DEVICE_H */
