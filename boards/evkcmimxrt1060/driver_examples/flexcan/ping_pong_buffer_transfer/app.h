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
#define EXAMPLE_CAN                   CAN2
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN2_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN2_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN2_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN2_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN2_IRQHandler

/* Considering that the first valid MB must be used as Reserved TX MB for ERR005829. */
#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

/* Select 60M clock divided by USB1 PLL (480 MHz) as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (2U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8U) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))
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
