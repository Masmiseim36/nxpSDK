/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_SPI1 1
#define RTE_SPI1_DMA_EN 0

#define RTE_I2C1 1
#define RTE_I2C1_DMA_EN 0

/* DSPI configuration. */
#define RTE_SPI1_PCS_TO_SCK_DELAY 0
#define RTE_SPI1_SCK_TO_PSC_DELAY 0
#define RTE_SPI1_BETWEEN_TRANSFER_DELAY 0
#define RTE_SPI1_MASTER_PCS_PIN_SEL kLPSPI_MasterPcs0

#endif /* __RTE_DEVICE_H */
