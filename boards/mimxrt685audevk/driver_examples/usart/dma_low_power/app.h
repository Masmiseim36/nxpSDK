/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_USART          USART0
#define EXAMPLE_USART_CLK_FREQ CLK_RTC_32K_CLK
#define USART_RX_DMA_CHANNEL   0
#define USART_TX_DMA_CHANNEL   1

#define EXAMPLE_UART_DMA_BASEADDR DMA0
#define EXAMPLE_UART_DMA_IRQn     DMA0_IRQn

#define EXAMPLE_UART_BAUDRATE (9600U)

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define EXAMPLE_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define EXAMPLE_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define EXAMPLE_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                            \
    (((const uint32_t[]){EXAMPLE_DEEPSLEEP_RUNCFG0,                                                               \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         EXAMPLE_DEEPSLEEP_RAM_APD, EXAMPLE_DEEPSLEEP_RAM_PPD}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void EXAMPLE_EnterDeepSleep(void);
void EXAMPLE_EnableDmaWakeup(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
