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
#define EXAMPLE_CAN                   CAN3
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN3_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN3_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN3_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN3_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN3_IRQHandler

#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

#define USE_CANFD (1)

/* Frame length of data in bytes (DLC) should Less than or equal to MB Payload size in bytes (BYTES_IN_MB). */

/* Select OSC24Mhz as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (1U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetRootClockFreq(kCLOCK_Root_Can3) / 100000U) * 100000U)
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
