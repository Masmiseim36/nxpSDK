/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_edma_soc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPUART              LPUART0
#define EXAMPLE_LPUART_CLK_FREQ     CLOCK_GetLPFlexCommClkFreq(0u)
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define LPUART_TX_DMA_REQUEST       kDmaRequestMuxLpFlexcomm0Tx
#define LPUART_RX_DMA_REQUEST       kDmaRequestMuxLpFlexcomm0Rx
#define EXAMPLE_LPUART_DMA_BASEADDR DMA0
#define EXAMPLE_LPUART_IRQHandler   LP_FLEXCOMM0_IRQHandler
#define EXAMPLE_LPUART_IRQn         LP_FLEXCOMM0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
