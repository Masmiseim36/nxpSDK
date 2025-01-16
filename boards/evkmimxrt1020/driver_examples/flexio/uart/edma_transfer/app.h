/*
 * Copyright 2018 NXP
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
#define FLEXIO_UART_TX_PIN 6U
#define FLEXIO_UART_RX_PIN 7U

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define FLEXIO_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define FLEXIO_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER (7U)
#define FLEXIO_CLOCK_FREQUENCY \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (FLEXIO_CLOCK_PRE_DIVIDER + 1U) / (FLEXIO_CLOCK_DIVIDER + 1U))

#define EXAMPLE_FLEXIO_UART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_FLEXIO_UART_DMA_BASEADDR    DMA0
#define FLEXIO_UART_TX_DMA_CHANNEL          0U
#define FLEXIO_UART_RX_DMA_CHANNEL          1U
#define FLEXIO_TX_SHIFTER_INDEX             0U
#define FLEXIO_RX_SHIFTER_INDEX             2U
#define EXAMPLE_TX_DMA_SOURCE               kDmaRequestMuxFlexIO1Request0Request1
#define EXAMPLE_RX_DMA_SOURCE               kDmaRequestMuxFlexIO1Request2Request3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
