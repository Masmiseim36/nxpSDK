/*
 * Copyright 2019 NXP
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
#define BOARD_FLEXIO_BASE  FLEXIO1
#define FLEXIO_UART_TX_PIN 21U
#define FLEXIO_UART_RX_PIN 22U

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define FLEXIO_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define FLEXIO_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER (7U)
#define FLEXIO_CLOCK_FREQUENCY \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (FLEXIO_CLOCK_PRE_DIVIDER + 1U) / (FLEXIO_CLOCK_DIVIDER + 1U))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
