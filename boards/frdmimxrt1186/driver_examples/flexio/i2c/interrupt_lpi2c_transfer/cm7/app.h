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
#define BOARD_LPI2C_SLAVE_BASE LPI2C3
#define BOARD_LPI2C_SLAVE_IRQn LPI2C3_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO1

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY  CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c0304)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio1)

#define FLEXIO_I2C_SDA_PIN 13U
#define FLEXIO_I2C_SCL_PIN 12U

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
