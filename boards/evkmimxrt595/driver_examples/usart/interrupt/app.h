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
#define DEMO_USART            USART0
#define DEMO_USART_CLK_SRC    kCLOCK_Flexcomm0Clk
#define DEMO_USART_CLK_FREQ   CLOCK_GetFlexcommClkFreq(0)
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn       FLEXCOMM0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
