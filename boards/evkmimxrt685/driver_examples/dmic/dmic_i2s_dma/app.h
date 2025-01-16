/*
 * Copyright 2018 NXP
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
#define DEMO_I2S_CLOCK_DIVIDER                                                                                 \
    (24576000U / 48000U / 16U / 2) /* I2S source clock 24.576MHZ, sample rate 48KHZ, bits width 16, 2 channel, \
                                  so bitclock should be 48KHZ * 16 = 768KHZ, divider should be 24.576MHZ / 768KHZ */
#define DEMO_DMIC_NUMS 2U

#define DEMO_DMA               (DMA0)
#define DEMO_DMIC_RX_CHANNEL   DMAREQ_DMIC0
#define DEMO_DMIC_RX_CHANNEL_1 17
#define DEMO_I2S_TX_CHANNEL    (7)
#define DEMO_I2S_TX_MODE       kI2S_MasterSlaveNormalSlave

#define DEMO_DMIC_CHANNEL          kDMIC_Channel0
#define DEMO_DMIC_CHANNEL_1        kDMIC_Channel1
#define DEMO_DMIC_CHANNEL_1_ENABLE DMIC_CHANEN_EN_CH1(1)
#define DEMO_DMIC_CHANNEL_ENABLE   DMIC_CHANEN_EN_CH0(1)
#define DEMO_AUDIO_BIT_WIDTH       (16)
#define DEMO_AUDIO_SAMPLE_RATE     (48000)
#define DEMO_AUDIO_PROTOCOL        kCODEC_BusI2S
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
