/*
 * Copyright 2022 NXP
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
#define EXAMPLE_CAN                   CAN3
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN3_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN3_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN3_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN3_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN3_IRQHandler

/* Considering that the first valid MB must be used as Reserved TX MB for ERR005829. */
#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

#define USE_CANFD (1)

/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Can3)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
