/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/*I2C Driver name mapping*/
#define RTE_I2C4        1
#define RTE_I2C4_DMA_EN 0

/*USART Driver name mapping. */
#define RTE_USART0        1
#define RTE_USART0_DMA_EN 0
#define RTE_USART1        0
#define RTE_USART1_DMA_EN 0
#define RTE_USART2        0
#define RTE_USART2_DMA_EN 0
#define RTE_USART3        0
#define RTE_USART3_DMA_EN 0
#define RTE_USART4        0
#define RTE_USART4_DMA_EN 0
#define RTE_USART5        0
#define RTE_USART5_DMA_EN 0
#define RTE_USART6        0
#define RTE_USART6_DMA_EN 0
#define RTE_USART7        0
#define RTE_USART7_DMA_EN 0

/*SPI Driver name mapping*/
#define RTE_SPI0        0
#define RTE_SPI0_DMA_EN 0
#define RTE_SPI1        0
#define RTE_SPI1_DMA_EN 0
#define RTE_SPI2        0
#define RTE_SPI2_DMA_EN 0
#define RTE_SPI3        0
#define RTE_SPI3_DMA_EN 0
#define RTE_SPI4        0
#define RTE_SPI4_DMA_EN 0
#define RTE_SPI5        0
#define RTE_SPI5_DMA_EN 0
#define RTE_SPI6        0
#define RTE_SPI6_DMA_EN 0
#define RTE_SPI7        1
#define RTE_SPI7_DMA_EN 0

/*I2C configuration*/
#define RTE_I2C4_Master_DMA_BASE DMA0
#define RTE_I2C4_Master_DMA_CH   21

/* USART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART0_RX_BUFFER_ENABLE 1
#define USART1_RX_BUFFER_ENABLE 0
#define USART2_RX_BUFFER_ENABLE 0
#define USART3_RX_BUFFER_ENABLE 0
#define USART4_RX_BUFFER_ENABLE 0
#define USART5_RX_BUFFER_ENABLE 0
#define USART6_RX_BUFFER_ENABLE 0
#define USART7_RX_BUFFER_ENABLE 0

#define RTE_USART0_DMA_TX_CH       5
#define RTE_USART0_DMA_TX_DMA_BASE DMA0
#define RTE_USART0_DMA_RX_CH       4
#define RTE_USART0_DMA_RX_DMA_BASE DMA0

#define RTE_USART1_DMA_TX_CH       7
#define RTE_USART1_DMA_TX_DMA_BASE DMA0
#define RTE_USART1_DMA_RX_CH       6
#define RTE_USART1_DMA_RX_DMA_BASE DMA0

#define RTE_USART2_DMA_TX_CH       8
#define RTE_USART2_DMA_TX_DMA_BASE DMA0
#define RTE_USART2_DMA_RX_CH       9
#define RTE_USART2_DMA_RX_DMA_BASE DMA0

#define RTE_USART3_DMA_TX_CH       10
#define RTE_USART3_DMA_TX_DMA_BASE DMA0
#define RTE_USART3_DMA_RX_CH       11
#define RTE_USART3_DMA_RX_DMA_BASE DMA0

#define RTE_USART4_DMA_TX_CH       13
#define RTE_USART4_DMA_TX_DMA_BASE DMA0
#define RTE_USART4_DMA_RX_CH       12
#define RTE_USART4_DMA_RX_DMA_BASE DMA0

#define RTE_USART5_DMA_TX_CH       15
#define RTE_USART5_DMA_TX_DMA_BASE DMA0
#define RTE_USART5_DMA_RX_CH       14
#define RTE_USART5_DMA_RX_DMA_BASE DMA0

#define RTE_USART6_DMA_TX_CH       17
#define RTE_USART6_DMA_TX_DMA_BASE DMA0
#define RTE_USART6_DMA_RX_CH       16
#define RTE_USART6_DMA_RX_DMA_BASE DMA0

#define RTE_USART7_DMA_TX_CH       19
#define RTE_USART7_DMA_TX_DMA_BASE DMA0
#define RTE_USART7_DMA_RX_CH       18
#define RTE_USART7_DMA_RX_DMA_BASE DMA0

/* SPI configuration. */
#define RTE_SPI0_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI0_DMA_TX_CH       5
#define RTE_SPI0_DMA_TX_DMA_BASE DMA0
#define RTE_SPI0_DMA_RX_CH       4
#define RTE_SPI0_DMA_RX_DMA_BASE DMA0

#define RTE_SPI1_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI1_DMA_TX_CH       7
#define RTE_SPI1_DMA_TX_DMA_BASE DMA0
#define RTE_SPI1_DMA_RX_CH       6
#define RTE_SPI1_DMA_RX_DMA_BASE DMA0

#define RTE_SPI2_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI2_DMA_TX_CH       9
#define RTE_SPI2_DMA_TX_DMA_BASE DMA0
#define RTE_SPI2_DMA_RX_CH       8
#define RTE_SPI2_DMA_RX_DMA_BASE DMA0

#define RTE_SPI3_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI3_DMA_TX_CH       11
#define RTE_SPI3_DMA_TX_DMA_BASE DMA0
#define RTE_SPI3_DMA_RX_CH       10
#define RTE_SPI3_DMA_RX_DMA_BASE DMA0

#define RTE_SPI4_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI4_DMA_TX_CH       13
#define RTE_SPI4_DMA_TX_DMA_BASE DMA0
#define RTE_SPI4_DMA_RX_CH       12
#define RTE_SPI4_DMA_RX_DMA_BASE DMA0

#define RTE_SPI5_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI5_DMA_TX_CH       15
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH       14
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0

#define RTE_SPI6_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI6_DMA_TX_CH       17
#define RTE_SPI6_DMA_TX_DMA_BASE DMA0
#define RTE_SPI6_DMA_RX_CH       16
#define RTE_SPI6_DMA_RX_DMA_BASE DMA0

#define RTE_SPI7_SSEL_NUM        kSPI_Ssel1
#define RTE_SPI7_DMA_TX_CH       19
#define RTE_SPI7_DMA_TX_DMA_BASE DMA0
#define RTE_SPI7_DMA_RX_CH       18
#define RTE_SPI7_DMA_RX_DMA_BASE DMA0
#endif /* __RTE_DEVICE_H */
