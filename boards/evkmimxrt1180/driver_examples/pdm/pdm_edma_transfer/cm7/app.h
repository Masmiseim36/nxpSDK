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
#define DEMO_PDM                  PDM
#define DEMO_PDM_ERROR_IRQn       PDM_ERROR_IRQn
#define DEMO_PDM_ERROR_IRQHandler PDM_ERROR_IRQHandler

#define DEMO_PDM_CLK_FREQ            24576000
#define DEMO_PDM_FIFO_WATERMARK      (4)
#define DEMO_PDM_QUALITY_MODE        kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT (0U)

#define DEMO_EDMA             DMA4
#define DEMO_EDMA_CHANNEL     0
#define DEMO_PDM_EDMA_CHANNEL kDma4RequestMuxPdmRx

#define DEMO_AUDIO_SAMPLE_RATE 16000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
