/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_USART                USART0
#define DEMO_USART_CLK_SRC        kCLOCK_Flexcomm0Clk
#define DEMO_USART_CLK_FREQ       CLOCK_GetFlexcommClkFreq(0)
#define USART_RX_DMA_CHANNEL      0
#define USART_TX_DMA_CHANNEL      1
#define EXAMPLE_UART_DMA_BASEADDR DMA0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
