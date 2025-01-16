/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_LPI2C_SLAVE_BASE LPI2C2
#define BOARD_LPI2C_SLAVE_IRQn LPI2C2_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO2

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY  CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c0102)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2)

#define FLEXIO_I2C_SDA_PIN 3U
#define FLEXIO_I2C_SCL_PIN 2U

/* I2C Baudrate 100K */
#define I2C_BAUDRATE (100000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
