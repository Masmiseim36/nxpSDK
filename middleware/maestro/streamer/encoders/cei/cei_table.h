/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CEI_TABLE_H__
#define __CEI_TABLE_H__

/**
 * @file    cei_table.h
 * @brief   Static tables for encoder functions, sizes
 */

#include <stdint.h>
#include "cei.h"
#include "cei_enctypes.h"

#ifdef STREAMER_ENABLE_CEI_OPUS
#include "opusenc_cei.h"
#endif

/**
 * @brief Common encoder interface function pointer table
 *
 */
static const CeiEncoderFunctions ceiEncTable[CEIENC_LAST] = {
#ifdef STREAMER_ENABLE_CEI_OPUS
    // CEIENC_OPUS
    {.fnGetMemorySize = &OPUSEncoderGetMemorySize,
     .fnInit          = &OPUSEncoderInit,
     .fnGetConfig     = &OPUSEncoderGetConfig,
     .fnSetConfig     = &OPUSEncoderSetConfig,
     .fnEncode        = &OPUSEncoderEncode}
#else
    {0}
#endif
};

/**
 * @brief Common encoder interface config size table
 *
 */
static const size_t ceiEncConfigSizeTable[CEIENC_LAST] = {
#ifdef STREAMER_ENABLE_CEI_OPUS
    sizeof(CeiOpusConfig) // CEIENC_OPUS
#else
    0
#endif
};

#endif
