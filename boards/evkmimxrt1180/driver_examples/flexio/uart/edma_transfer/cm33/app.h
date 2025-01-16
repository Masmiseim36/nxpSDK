/*
 * Copyright 2018 NXP
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
#define BOARD_FLEXIO_BASE  FLEXIO2
#define FLEXIO_UART_TX_PIN 2U
#define FLEXIO_UART_RX_PIN 3U

#define FLEXIO_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2))

#define EXAMPLE_FLEXIO_UART_DMA_BASEADDR DMA4
#define FLEXIO_UART_TX_DMA_CHANNEL       0U
#define FLEXIO_UART_RX_DMA_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX          0U
#define FLEXIO_RX_SHIFTER_INDEX          2U
#define EXAMPLE_TX_DMA_SOURCE            kDma4RequestMuxFlexIO2Request0
#define EXAMPLE_RX_DMA_SOURCE            kDma4RequestMuxFlexIO2Request2

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication                 = true;                 \
        config.channelConfig[FLEXIO_UART_TX_DMA_CHANNEL] = &channelConfig;       \
        config.channelConfig[FLEXIO_UART_RX_DMA_CHANNEL] = &channelConfig;       \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
