/*
 * Copyright 2023 NXP
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
#define BOARD_FLEXIO_BASE FLEXIO

#define I2C_BAUDRATE 400000

#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()

#define FLEXIO_I2C_SDA_PIN 0U
#define FLEXIO_I2C_SCL_PIN 1U

#define BOARD_LPI2C_SLAVE_BASE LPI2C8
#define BOARD_LPI2C_SLAVE_IRQn LP_FLEXCOMM8_IRQn

#define LPI2C_CLOCK_FREQUENCY CLOCK_GetLPFlexCommClkFreq(8U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
