/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI1_InitPins();
extern void SPI1_DeinitPins();

/*Driver name mapping.*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI1            1
#define RTE_SPI1_PIN_INIT   SPI1_InitPins
#define RTE_SPI1_PIN_DEINIT SPI1_DeinitPins
#define RTE_SPI1_DMA_EN     0

/* SPI configuration. */

#endif /* _RTE_DEVICE_H */
