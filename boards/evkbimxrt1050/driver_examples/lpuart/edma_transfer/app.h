/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_LPUART                    LPUART1
#define DEMO_LPUART_CLK_FREQ           BOARD_DebugConsoleSrcFreq()
#define LPUART_TX_DMA_CHANNEL          0U
#define LPUART_RX_DMA_CHANNEL          1U
#define LPUART_TX_DMA_REQUEST          kDmaRequestMuxLPUART1Tx
#define LPUART_RX_DMA_REQUEST          kDmaRequestMuxLPUART1Rx
#define EXAMPLE_LPUART_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_LPUART_DMA_BASEADDR    DMA0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
