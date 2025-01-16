/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPDIF          SPDIF
#define EXAMPLE_DMA            DMA4
#define SPDIF_IRQ              SPDIF_IRQn
#define SPDIF_ErrorHandle      SPDIF_IRQHandler
#define SPDIF_RX_LEFT_CHANNEL  (0)
#define SPDIF_RX_RIGHT_CHANNEL (1)
#define SPDIF_TX_LEFT_CHANNEL  (2)
#define SPDIF_TX_RIGHT_CHANNEL (3)
#define SPDIF_RX_SOURCE        (kDma4RequestMuxSpdifRx)
#define SPDIF_TX_SOURCE        (kDma4RequestMuxSpdifTx)
#define DEMO_SPDIF_CLOCK_FREQ  CLOCK_GetPllFreq(kCLOCK_Pll_AudioPll)
#define DEMO_SPDIF_SAMPLE_RATE 48000

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
        config.channelConfig[1]          = &channelConfig;                       \
        config.channelConfig[2]          = &channelConfig;                       \
        config.channelConfig[3]          = &channelConfig;                       \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
