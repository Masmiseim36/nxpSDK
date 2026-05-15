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
#define BOARD_INPUT_PIN_FLEXIO     FLEXIO1
#define BOARD_INPUT_PIN_FLEXIO_PIN 12U

#define BOARD_RGPIO_OUPUT_PORT     RGPIO2
#define BOARD_RGPIO_OUPUT_PORT_PIN 13U

#define FLEXIO_PIN_UserCallback FLEXIO1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
