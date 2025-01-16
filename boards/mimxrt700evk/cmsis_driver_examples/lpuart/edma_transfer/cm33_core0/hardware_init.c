/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
edma_config_t edmaConfig            = {0};
edma_channel_config_t channelConfig = {
    .enableMasterIDReplication = true,
    .securityLevel             = kEDMA_ChannelSecurityLevelSecure,
    .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged,
};
/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* attach FC0 clock to LP_FLEXCOMM (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_DEBUG_UART_FCCLK_DIV, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    EDMA_GetDefaultConfig(&edmaConfig);
    edmaConfig.enableMasterIdReplication           = true;
    edmaConfig.channelConfig[RTE_USART0_DMA_TX_CH] = &channelConfig;
    edmaConfig.channelConfig[RTE_USART0_DMA_RX_CH] = &channelConfig;
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART0_DMA_TX_CH, RTE_USART0_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART0_DMA_RX_CH, RTE_USART0_DMA_RX_PERI_SEL);
#endif
    EDMA_EnableRequest(EXAMPLE_USART_DMA_BASEADDR, DEMO_USART_TX_EDMA_CHANNEL);
    EDMA_EnableRequest(EXAMPLE_USART_DMA_BASEADDR, DEMO_USART_RX_EDMA_CHANNEL);
}

uint32_t LPUART0_GetFreq()
{
    return DEMO_USART_CLK_FREQ;
}
/*${function:end}*/
