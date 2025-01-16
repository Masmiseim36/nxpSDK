/*
 * Copyright 2019-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             24576000
#define DEMO_PDM_FIFO_WATERMARK       (4)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain2

#define DEMO_SAI              SAI1
#define DEMO_SAI_CLK_FREQ     24576000
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)

#define DEMO_PDM_EDMA           DMA4
#define DEMO_PDM_EDMA_CHANNEL_0 0
#define DEMO_PDM_REQUEST_SOURCE kDma4RequestMuxPdmRx

#define DEMO_SAI_EDMA           DMA3
#define DEMO_SAI_EDMA_CHANNEL   0
#define DEMO_SAI_REQUEST_SOURCE kDma3RequestMuxSai1Tx

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits

#define BOARD_MasterClockConfig()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
