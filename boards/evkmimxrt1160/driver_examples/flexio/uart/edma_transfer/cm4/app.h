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
#define BOARD_FLEXIO_BASE  FLEXIO2
#define FLEXIO_UART_TX_PIN 10U
#define FLEXIO_UART_RX_PIN 11U

#define FLEXIO_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2))

#define FLEXIO_DMA_REQUEST_BASE             kDmaRequestMuxFlexIO2Request0Request1
#define EXAMPLE_FLEXIO_UART_DMAMUX_BASEADDR DMAMUX1
#define EXAMPLE_FLEXIO_UART_DMA_BASEADDR    DMA1
#define FLEXIO_UART_TX_DMA_CHANNEL          0U
#define FLEXIO_UART_RX_DMA_CHANNEL          1U
#define FLEXIO_TX_SHIFTER_INDEX             0U
#define FLEXIO_RX_SHIFTER_INDEX             2U
#define EXAMPLE_TX_DMA_SOURCE               kDmaRequestMuxFlexIO2Request0Request1
#define EXAMPLE_RX_DMA_SOURCE               kDmaRequestMuxFlexIO2Request2Request3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
