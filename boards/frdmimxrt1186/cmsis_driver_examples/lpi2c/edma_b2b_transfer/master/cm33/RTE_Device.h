/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitI2CPins();
extern void BOARD_InitI2CPins_deinit();

/* Driver name mapping. */
/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C3        1
#define RTE_I2C3_DMA_EN 1

/* LPI2C configuration. */
#define RTE_I2C3_PIN_INIT        BOARD_InitI2CPins
#define RTE_I2C3_PIN_DEINIT      BOARD_InitI2CPins_deinit
#define RTE_I2C3_DMA_TX_CH       0
#define RTE_I2C3_DMA_TX_PERI_SEL kDma4RequestMuxLPI2C3Tx
#define RTE_I2C3_DMA_TX_DMA_BASE DMA4
#define RTE_I2C3_DMA_RX_CH       1
#define RTE_I2C3_DMA_RX_PERI_SEL kDma4RequestMuxLPI2C3Rx
#define RTE_I2C3_DMA_RX_DMA_BASE DMA4
#endif /* _RTE_DEVICE_H */
