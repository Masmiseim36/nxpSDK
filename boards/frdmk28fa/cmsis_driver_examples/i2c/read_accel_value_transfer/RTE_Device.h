/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/*Driver name mapping*/
#define RTE_I2C3 1
#define RTE_I2C3_DMA_EN 0

/*I2C configuration*/
#define RTE_I2C0_Master_DMA_BASE DMA0
#define RTE_I2C0_Master_DMA_CH 0
#define RTE_I2C0_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_Master_PERI_SEL kDmaRequestMux0I2C0

#define RTE_I2C1_Master_DMA_BASE DMA0
#define RTE_I2C1_Master_DMA_CH 1
#define RTE_I2C1_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C1_Master_PERI_SEL kDmaRequestMux0I2C1

#define RTE_I2C2_Master_DMA_BASE DMA0
#define RTE_I2C2_Master_DMA_CH 2
#define RTE_I2C2_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C2_Master_PERI_SEL kDmaRequestMux0I2C2

#define RTE_I2C3_Master_DMA_BASE DMA0
#define RTE_I2C3_Master_DMA_CH 3
#define RTE_I2C3_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C3_Master_PERI_SEL kDmaRequestMux0I2C3

#endif /* __RTE_DEVICE_H */
