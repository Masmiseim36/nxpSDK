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
#define EXAMPLE_LPUART              LPUART19
#define EXAMPLE_LPUART_CLK_FREQ     CLOCK_GetLPFlexCommClkFreq(19u)
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define LPUART_TX_DMA_REQUEST       kDmaRequestMuxLpFlexcomm19Tx
#define LPUART_RX_DMA_REQUEST       kDmaRequestMuxLpFlexcomm19Rx
#define EXAMPLE_LPUART_DMA_BASEADDR DMA3
#define EXAMPLE_LPUART_IRQHandler   LP_FLEXCOMM19_IRQHandler
#define EXAMPLE_LPUART_IRQn         LP_FLEXCOMM19_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
