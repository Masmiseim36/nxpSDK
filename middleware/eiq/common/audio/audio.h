/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdint.h>

#include "fsl_common.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t AUDIO_GetSpectralSample(uint8_t* dstData, int size);
void AUDIO_PreprocessSample(uint8_t* dstData, const int16_t* srcData, int size);
const char* AUDIO_GetSampleName();

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _AUDIO_H_ */
