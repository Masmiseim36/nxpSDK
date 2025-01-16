/*
 * Copyright 2021 NXP
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
#define BOARD_FLEXIO_BASE      FLEXIO3

/* Select USB1 PLL (480 MHz) as slave lpi2c clock source */
#define LPI2C_CLOCK_SELECT (0U)
/* Clock divider for slave lpi2c clock source */
#define LPI2C_CLOCK_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_DIVIDER + 1U))

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define FLEXIO_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define FLEXIO_CLOCK_PRE_DIVIDER (5U)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER (7U)
#define FLEXIO_CLOCK_FREQUENCY \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (FLEXIO_CLOCK_PRE_DIVIDER + 1U) / (FLEXIO_CLOCK_DIVIDER + 1U))

#define FLEXIO_I2C_SDA_PIN 0U
#define FLEXIO_I2C_SCL_PIN 1U

/* I2C Baudrate 150K */
#define I2C_BAUDRATE (150000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
