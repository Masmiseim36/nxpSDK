/*
 * Copyright 2023 NXP
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
#define DEMO_PDM_CLK_FREQ             24576000U
#define DEMO_PDM_FIFO_WATERMARK       (4U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeMedium
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    (0U)
#define DEMO_AUDIO_SAMPLE_RATE        (48000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
