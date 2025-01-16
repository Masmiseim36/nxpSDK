/*
 * Copyright 2022 NXP
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
#define BOARD_INPUT_PIN_FLEXIO     FLEXIO2
#define BOARD_INPUT_PIN_FLEXIO_PIN 0U

#define BOARD_RGPIO_OUPUT_PORT     RGPIO4
#define BOARD_RGPIO_OUPUT_PORT_PIN 1U

#define FLEXIO_PIN_UserCallback FLEXIO2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
