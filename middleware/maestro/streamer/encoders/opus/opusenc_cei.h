/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
