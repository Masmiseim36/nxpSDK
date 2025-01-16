/*
 * Copyright 2022 NXP
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
#define DEMO_LPUART                 LPUART0
#define DEMO_LPUART_CLK_FREQ        CLOCK_GetLPFlexCommClkFreq(0u)
#define LPUART_TX_DMA_CHANNEL       0U
#define LPUART_RX_DMA_CHANNEL       1U
#define DEMO_LPUART_TX_EDMA_CHANNEL kDmaRequestMuxLpFlexcomm0Tx
#define DEMO_LPUART_RX_EDMA_CHANNEL kDmaRequestMuxLpFlexcomm0Rx
#define EXAMPLE_LPUART_DMA_BASEADDR DMA0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
