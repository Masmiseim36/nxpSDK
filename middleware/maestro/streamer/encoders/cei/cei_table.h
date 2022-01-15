#ifndef __CEI_TABLE_H__
#define __CEI_TABLE_H__

#include <stdint.h>
#include "cei.h"
#include "cei_enctypes.h"

#include "opusenc_cei.h"

static const CeiEncoderFunctions ceiEncTable[CEIENC_LAST] = {
    #ifdef STREAMER_ENABLE_CEI_OPUS
    // CEIENC_OPUS
    {
        .fnGetMemorySize = &OPUSEncoderGetMemorySize,
        .fnInit = &OPUSEncoderInit,
        .fnGetConfig = &OPUSEncoderGetConfig,
        .fnSetConfig = &OPUSEncoderSetConfig,
        .fnEncode = &OPUSEncoderEncode
    }
    #else
    { 0 }
    #endif
};

static const size_t ceiEncConfigSizeTable[CEIENC_LAST] = {
    #ifdef STREAMER_ENABLE_CEI_OPUS
    sizeof(CeiOpusConfig) // CEIENC_OPUS
    #else
	0
    #endif
};

#endif