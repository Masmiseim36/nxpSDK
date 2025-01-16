/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE      FLEXIO0
#define FLEXIO_I2C_SDA_PIN     10
#define FLEXIO_I2C_SCL_PIN     11
#define EXAMPLE_I2C_SLAVE_BASE I2C5
/* Select flexio clock source */
#define FLEXIO_CLOCK_SRC (kFRO_DIV2_to_FLEXIO)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER   (8U)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()

#define BOARD_I2C_SLAVE_BASE      I2C11
#define I2C_SLAVE_CLOCK_FREQUENCY CLOCK_GetFlexcommClkFreq(11)
#define BOARD_I2C_SLAVE_IRQn      FLEXCOMM11_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
