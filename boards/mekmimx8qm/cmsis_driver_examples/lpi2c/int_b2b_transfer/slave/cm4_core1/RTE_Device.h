/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPI2C2_InitPins();
extern void LPI2C2_DeinitPins();

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
#define RTE_I2C2        1
#define RTE_I2C2_DMA_EN 0

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

#endif /* _RTE_DEVICE_H */
