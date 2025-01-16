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
#define DEMO_LPUART                  LPUART1
#define DEMO_LPUART_CLK_FREQ         BOARD_DebugConsoleSrcFreq()
#define LPUART_TX_DMA_CHANNEL        0U
#define LPUART_RX_DMA_CHANNEL        1U
#define DEMO_LPUART_TX_EDMA_CHANNEL  kDma3RequestMuxLPUART1Tx
#define DEMO_LPUART_RX_EDMA_CHANNEL  kDma3RequestMuxLPUART1Rx
#define EXAMPLE_LPUART_DMA_BASEADDR  DMA3
#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication                 = true;                 \
        config.channelConfig[LPUART_TX_DMA_CHANNEL] = &channelConfig;       \
        config.channelConfig[LPUART_RX_DMA_CHANNEL] = &channelConfig;       \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
