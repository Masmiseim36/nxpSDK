/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_lpuart_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_USART                 Driver_USART19
#define EXAMPLE_USART_DMA_BASEADDR DMA2
#define EXAMPLE_DMA_CLOCK          kCLOCK_Dma0
#define DEMO_USART_CLK_FREQ       CLOCK_GetLPFlexCommClkFreq(19u)
#define DEMO_USART_TX_EDMA_CHANNEL kDmaRequestMuxLpFlexcomm19Tx
#define DEMO_USART_RX_EDMA_CHANNEL kDmaRequestMuxLpFlexcomm19Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
