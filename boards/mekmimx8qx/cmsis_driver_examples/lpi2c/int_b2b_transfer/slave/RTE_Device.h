/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPI2C2_InitPins();
extern void LPI2C2_DeinitPins();

/* LPI2C instance mapping */
#define LPI2C0 CM4__LPI2C
#define LPI2C1 ADMA__LPI2C0
#define LPI2C2 ADMA__LPI2C1
#define LPI2C3 ADMA__LPI2C2
#define LPI2C4 ADMA__LPI2C3

/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C2        1
#define RTE_I2C2_DMA_EN 0

/* LPI2C configuration. */
/*Note: LPI2C0 not support DMA */

#define RTE_I2C2_PIN_INIT        LPI2C2_InitPins
#define RTE_I2C2_PIN_DEINIT      LPI2C2_DeinitPins
#define RTE_I2C2_DMA_TX_CH       3
#define RTE_I2C2_DMA_TX_PERI_SEL 3
#define RTE_I2C2_DMA_TX_DMA_BASE ADMA__EDMA3
#define RTE_I2C2_DMA_RX_CH       2
#define RTE_I2C2_DMA_RX_PERI_SEL 2
#define RTE_I2C2_DMA_RX_DMA_BASE ADMA__EDMA3

#endif /* _RTE_DEVICE_H */
