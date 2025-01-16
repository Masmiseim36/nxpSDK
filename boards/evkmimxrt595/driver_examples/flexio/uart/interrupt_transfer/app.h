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
#define BOARD_FLEXIO_BASE  FLEXIO0
#define FLEXIO_UART_TX_PIN 10U
#define FLEXIO_UART_RX_PIN 11U

/* Select flexio clock source */
#define FLEXIO_CLOCK_SRC (kFRG_to_FLEXIO)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER   (33U)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
