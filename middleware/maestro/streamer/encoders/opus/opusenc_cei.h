/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OPUSENC_CEI_H__
#define __OPUSENC_CEI_H__

#include <stdint.h>
#include "cei.h"

int OPUSEncoderGetMemorySize(uint8_t channels);
int OPUSEncoderInit(void *memory, uint32_t sampleRate, uint8_t channels);
int OPUSEncoderGetConfig(void *memory, void *config);
int OPUSEncoderSetConfig(void *memory, void *config);
int OPUSEncoderEncode(void *memory, BufferInfo *source, BufferInfo *dest);

#endif
