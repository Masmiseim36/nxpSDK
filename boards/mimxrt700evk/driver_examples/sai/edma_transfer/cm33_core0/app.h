/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance and clock */
#define DEMO_SAI                SAI0
#define DEMO_SAI_CHANNEL        (0)
#define DEMO_SAI_TX_SYNC_MODE   kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE   kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE   kSAI_Master
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

#define DEMO_DMA              DMA0
#define DEMO_EDMA_CHANNEL     0U
#define DEMO_SAI_EDMA_CHANNEL kDmaRequestMuxSai0Tx

/* Get frequency of sai clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetSaiClkFreq()

/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ 24000000U /* CLOCK_GetLPI2cClkFreq(2) */
#define BOARD_SAI_RXCONFIG(config, mode)

#define DEMO_QUICKACCESS_SECTION_CACHEABLE 1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MASTER_CLOCK_CONFIG(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
