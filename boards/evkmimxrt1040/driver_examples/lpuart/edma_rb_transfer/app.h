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
#define EXAMPLE_LPUART                 LPUART1
#define EXAMPLE_LPUART_CLK_FREQ        BOARD_DebugConsoleSrcFreq()
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define LPUART_TX_DMA_REQUEST          kDmaRequestMuxLPUART1Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMuxLPUART1Rx
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
#define EXAMPLE_LPUART_IRQHandler      LPUART1_IRQHandler
#define EXAMPLE_LPUART_IRQn            LPUART1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
