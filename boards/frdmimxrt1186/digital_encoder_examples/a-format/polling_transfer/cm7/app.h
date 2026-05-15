/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE      FLEXIO1

#define FLEXIO_A_FORMAT_RX_PIN    0U
#define FLEXIO_A_FORMAT_DR_PIN    1U
#define FLEXIO_A_FORMAT_TX_PIN    3U
#define A_FORMAT_TX_SHIFTER_INDEX 0U
#define A_FORMAT_RX_SHIFTER_INDEX 1U
#define A_FORMAT_DR_TIMER_INDEX   0U
#define A_FORMAT_TX_TIMER_INDEX   1U
#define A_FORMAT_RX_TIMER_INDEX   2U
#define DEMO_FLEXIO_CLOCK         (kCLOCK_Root_Flexio1)
#define FLEXIO_CLOCK_FREQUENCY    (CLOCK_GetRootClockFreq(DEMO_FLEXIO_CLOCK))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
