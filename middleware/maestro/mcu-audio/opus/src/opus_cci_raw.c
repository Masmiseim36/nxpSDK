////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opusdec OPUS Decoder
//! @{
//
// Copyright (c) 2020 NXP
//
//! \file opus_cci.c
//! \brief Contains OPUS Decoder CCI functions.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "opus_cci_raw.h"
#include "opus.h"
#include "ccidec.h"

/* These status codes returned to CCI layer.  They must match
 * codec_interface_public_api.h */
#define OPUS_SUCCESS             0
#define OPUS_DECODER_INIT_FAILED 1
#define OPUS_DECODE_ERROR        2
#define OPUS_END_OF_DECODE       3

typedef struct _OpusCCI
{
    OpusDecoder *dec;

    pfnReadCallback *pfnRead;
    pfnSeekCallback *pfnSeek;
    pfnFtellCallback *pfnFtell;

    char *indata;
    unsigned char in_buffer[OPUS_PAGE_SIZE];
    int16_t out_buffer[OUTPUT_BUFFER];
} OpusCCI;

/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the minimum input/output frame size for the decoder.
//!
//!
//! \param[out] inSize  Input Frame Size
//! \param[out] outSize Output Frame Size
//!
//! \retval it returns the minimum input/output frame size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int OPUSDecoderGetIOFrameSize(int *inSize, int *outSize)
{
    *inSize  = OPUS_PAGE_SIZE;
    *outSize = OUTPUT_BUFFER;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//!
//! \param[in] None
//!
//! \returns it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int OPUSDecoderGetMemorySize()
{
    int size;

    size = sizeof(OpusCCI) + opus_decoder_get_size(MAX_CHANNEL);

    return size;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the
//! decoder internal structures.
//!
//!
//! \param[in] memory         This is pointer to the decoder memory.
//! \param[in] callbackFn     This is pointer to the decoder callback functions.
//! \param[in] userData        This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    if codec initializes successfully.
//! \retval kCodecInitError     if codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int OPUSDecoderInit(int *memory[], int *pCallbackFn[], int *pUserData)
{
    OpusCCI *oc;
    decoder_callback_FunctionTable_t *pCall;
    int error;

    oc = (OpusCCI *)memory[0];
    memset(oc, 0, sizeof(OpusCCI));

    oc->dec    = (void *)((unsigned char *)memory[0] + sizeof(OpusCCI));
    oc->indata = (char *)pUserData[0];

    pCall        = (decoder_callback_FunctionTable_t *)pCallbackFn;
    oc->pfnRead  = pCall->pfnRead;
    oc->pfnFtell = pCall->pfnFtell;
    oc->pfnSeek  = pCall->pfnSeek;

    error = opus_decoder_init(oc->dec, 48000, 1);
    if (error != OPUS_OK)
    {
        return OPUS_DECODER_INIT_FAILED;
    }

    return OPUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decodes the input bit stream and generates the PCM data in output pcm buffer.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] sampleProudced      This is pointer to the samples produced by the codec.
//! \param[in] bufOuts             This is pointer to the output pcm buffer.
//!
//! \retval kCodecSuccess           if codec decodes frame successfully.
//! \retval kCodecDecodeError       if codec fails to decode frame.
//! \retval kCodeEndOfDecode        if decoding of input file is done.
/////////////////////////////////////////////////////////////////////////////////
int OPUSDecoderDecode(int *pMemory[], int *pSampleToProduced, int *pBufOut)
{
    OpusCCI *oc;
    int decode_ret;
    int in_read;
    uint32_t packet_size;

    *pSampleToProduced = 0;

    oc = (OpusCCI *)pMemory[0];

    /* Read the packet header (4 byte size) */
    in_read = oc->pfnRead(0, (unsigned char *)&packet_size, 4, 0, (void *)oc->indata);
    if (in_read <= 0)
    {
        return OPUS_END_OF_DECODE;
    }
    else if (in_read < 4)
    {
        return OPUS_DECODE_ERROR;
    }

    /* Make sure packet size is valid. */
    if (packet_size > OPUS_PAGE_SIZE)
    {
        return OPUS_DECODE_ERROR;
    }

    /* Read single packet data. */
    in_read = oc->pfnRead(0, oc->in_buffer, packet_size, 0, (void *)oc->indata);
    if (in_read <= 0)
    {
        return OPUS_END_OF_DECODE;
    }
    else if (in_read < packet_size)
    {
        return OPUS_DECODE_ERROR;
    }

    decode_ret =
        opus_decode(oc->dec, (const unsigned char *)oc->in_buffer, packet_size, &oc->out_buffer[0], OUTPUT_BUFFER, 0);
    if (decode_ret < 0)
    {
        return OPUS_DECODE_ERROR;
    }

    *pSampleToProduced = decode_ret * 2;
    *pBufOut           = (int)&oc->out_buffer[0];

    return OPUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] seekOffset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int OPUSDecoderSeek(int *pMemory[], int seekOffset)
{
    return seekOffset;
}

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
