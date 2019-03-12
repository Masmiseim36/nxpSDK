/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_debug_console.h"
#include "arm_math.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE (384U)
#define SAMPLE_RATE (kSAI_SampleRate16KHz)
#define BUFFER_SIZE (512)
#define BUFFER_NUM (4)
#if defined BOARD_HAS_SDCARD && (BOARD_HAS_SDCARD != 0)
#define DEMO_SDCARD (1U)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PlaybackSine(I2S_Type *base, uint32_t SineWaveFreqHz, uint32_t time_s);
void RecordSDCard(I2S_Type *base, uint32_t time_s);
void RecordPlayback(I2S_Type *base, uint32_t time_s);
/*******************************************************************************
 * Variables
 ******************************************************************************/
