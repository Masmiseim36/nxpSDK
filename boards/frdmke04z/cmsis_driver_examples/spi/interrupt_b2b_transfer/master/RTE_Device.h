/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void SPI0_InitPins();
extern void SPI0_DeinitPins();

/*Driver name mapping.*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI0            1
#define RTE_SPI0_PIN_INIT   SPI0_InitPins
#define RTE_SPI0_PIN_DEINIT SPI0_DeinitPins
#define RTE_SPI0_DMA_EN     0

/* SPI configuration. */

#endif /* _RTE_DEVICE_H */
