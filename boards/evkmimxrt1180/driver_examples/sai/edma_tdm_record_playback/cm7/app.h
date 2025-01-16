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
#include "fsl_common.h"
#include "fsl_cs42448.h"
/*${macro:start}*/
#define DEMO_PDM                      PDM
#define DEMO_SAI                      SAI1
#define DEMO_SAI_CLK_FREQ             24576000
#define DEMO_SAI_CLOCK_SOURCE         (kSAI_BclkSourceMclkDiv)
#define DEMO_PDM_CLK_FREQ             24576000
#define DEMO_PDM_FIFO_WATERMARK       (4)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (6144000U) /* 6.144MHZ */
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain7

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate48KHz)
/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)

#define DEMO_CS42448_I2C_INSTANCE 3
#define DEMO_CODEC_POWER_GPIO     RGPIO1
#define DEMO_CODEC_POWER_GPIO_PIN 20
#define DEMO_CODEC_RESET_GPIO     RGPIO1
#define DEMO_CODEC_RESET_GPIO_PIN 19

#define DEMO_EDMA               DMA3
#define DEMO_PDM_EDMA_CHANNEL_0 0
#define DEMO_PDM_EDMA_CHANNEL_1 1
#define DEMO_SAI_EDMA_CHANNEL   2
#define DEMO_PDM_REQUEST_SOURCE kDmaRequestMuxPdm
#define DEMO_SAI_REQUEST_SOURCE kDma3RequestMuxSai1Tx
#define EXAMPLE_DMA             DMA3
#define EXAMPLE_TX_CHANNEL      (0U)
#define EXAMPLE_RX_CHANNEL      (1U)
#define EXAMPLE_SAI_TX_SOURCE   kDma3RequestMuxSai1Tx
#define EXAMPLE_SAI_RX_SOURCE   kDma3RequestMuxSai1Rx
#define BOARD_MasterClockConfig()
#define BOARD_MASTER_CLOCK_CONFIG()

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
void BOARD_InitDebugConsole(void);
void BORAD_CodecReset(bool state);
/*${prototype:end}*/

#endif /* _APP_H_ */
