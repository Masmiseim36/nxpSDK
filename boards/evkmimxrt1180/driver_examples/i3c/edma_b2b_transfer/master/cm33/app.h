/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER                 I3C2
#define EXAMPLE_I2C_BAUDRATE           (400000U)
#define EXAMPLE_I3C_OD_BAUDRATE        (1500000U)
#define EXAMPLE_I3C_PP_BAUDRATE        (4000000U)
#define I3C_MASTER_CLOCK_FREQUENCY     CLOCK_GetRootClockFreq(kCLOCK_Root_I3c2)

#define EXAMPLE_DMA                    DMA4
#define EXAMPLE_I3C_TX_DMA_CHANNEL     (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL     (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX (kDma4RequestMuxI3C2ToBusRequest)
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX (kDma4RequestMuxI3C2FromBusRequest)

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
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
