/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_CAN CAN3

/* Considering that the first valid MB must be used as Reserved TX MB for ERR005829,
 * if RX FIFO enables (RFEN bit in MCE set as 1) and RFFN in CTRL2 is set default as zero,
 * the first valid TX MB Number shall be 8;
 * if RX FIFO enables (RFEN bit in MCE set as 1) and RFFN in CTRL2 is set by other values (0x1~0xF),
 * the user should consider to detail the first valid MB number;
 * if RX FIFO disables (RFEN bit in MCE set as 0) , the first valid MB number would be zero.
 */
#define TX_MESSAGE_BUFFER_NUM (9)

/* Select 60M clock divided by USB1 PLL (480 MHz) as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (2U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)

#define EXAMPLE_CAN_DMA           DMA0
#define EXAMPLE_CAN_DMAMUX        DMAMUX
#define EXAMPLE_CAN_DMA_REQUEST   kDmaRequestMuxCAN3
#define EXAMPLE_CAN_DMA_CHANNEL   0U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
