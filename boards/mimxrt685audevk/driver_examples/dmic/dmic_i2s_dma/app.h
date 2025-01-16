/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DMAREQ_DMIC0                    16U
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY CLOCK_GetMclkClkFreq()
#define DEMO_I2S_TX                     (I2S3)
#define DEMO_I2S_CLOCK_DIVIDER          16
#define DEMO_DMA                        (DMA0)
#define DEMO_DMIC_RX_CHANNEL            DMAREQ_DMIC0
#define DEMO_I2S_TX_CHANNEL             (7)
#define DEMO_DMIC_CHANNEL               kDMIC_Channel0
#define DEMO_DMIC_CHANNEL_ENABLE        DMIC_CHANEN_EN_CH0(1)
#define DEMO_AUDIO_BIT_WIDTH            (16)
#define DEMO_AUDIO_SAMPLE_RATE          (48000)
#define DEMO_AUDIO_PROTOCOL             kCODEC_BusI2S
#define DEMO_I2S_TX_MODE                kI2S_MasterSlaveNormalMaster
#define DEMO_CODEC_I2C_BASEADDR         I2C2
#define DEMO_CODEC_I2C_INSTANCE         2U
#define DEMO_CODEC_VOLUME               100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
