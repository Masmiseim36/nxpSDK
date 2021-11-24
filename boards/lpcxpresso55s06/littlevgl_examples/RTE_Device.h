/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

extern void SPI8_InitPins();
extern void SPI8_DeinitPins();
extern void SPI8_InitPins();
extern void SPI8_DeinitPins();
extern void I2C3_InitPins();
extern void I2C3_DeinitPins();

/*Driver name mapping*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI8            1
#define RTE_SPI8_PIN_INIT   SPI8_InitPins
#define RTE_SPI8_PIN_DEINIT SPI8_DeinitPins
#define RTE_SPI8_DMA_EN     1

/* SPI configuration. */
#define RTE_SPI8_SSEL_NUM kSPI_Ssel1

/* SPI DMA configuration */
#if RTE_SPI8_DMA_EN
#define RTE_SPI8_PIN_INIT        SPI8_InitPins
#define RTE_SPI8_PIN_DEINIT      SPI8_DeinitPins
#define RTE_SPI8_DMA_TX_CH       3
#define RTE_SPI8_DMA_TX_DMA_BASE DMA0
#define RTE_SPI8_DMA_RX_CH       2
#define RTE_SPI8_DMA_RX_DMA_BASE DMA0
#endif

#define RTE_I2C3            1
#define RTE_I2C3_PIN_INIT   I2C3_InitPins
#define RTE_I2C3_PIN_DEINIT I2C3_DeinitPins
#define RTE_I2C3_DMA_EN     0

#endif /* __RTE_DEVICE_H */
