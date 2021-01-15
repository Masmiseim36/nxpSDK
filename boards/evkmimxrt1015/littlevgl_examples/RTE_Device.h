/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPSPI1_InitPins();
extern void LPI2C1_InitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI1          1
#define RTE_SPI1_PIN_INIT LPSPI1_InitPins
#define RTE_SPI1_DMA_EN   0

/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C1          1
#define RTE_I2C1_PIN_INIT LPI2C1_InitPins
#define RTE_I2C1_DMA_EN   0

/* DSPI configuration. */
#define RTE_SPI1_PCS_TO_SCK_DELAY       0
#define RTE_SPI1_SCK_TO_PSC_DELAY       0
#define RTE_SPI1_BETWEEN_TRANSFER_DELAY 50 /* For 10MHz baudrate. delay(ns) = 10^9/baudrate/2. */
#define RTE_SPI1_MASTER_PCS_PIN_SEL     kLPSPI_MasterPcs0

#endif /* _RTE_DEVICE_H */
