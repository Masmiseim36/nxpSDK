/*
 * Copyright 2018-2020 NXP
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
#define RTE_I2C3_DMA_EN 0

/* LPI2C configuration. */
#define RTE_I2C3_PIN_INIT   BOARD_InitI2CPins
#define RTE_I2C3_PIN_DEINIT BOARD_InitI2CPins_deinit

#endif /* _RTE_DEVICE_H */
