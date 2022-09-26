////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opusdec OPUS Decoder
//! @{
//
// Copyright (c) 2020,2022 NXP
//
//! \file opus_cci.h
//! \brief Contains OPUS Decoder CCI APIs definitions.
////////////////////////////////////////////////////////////////////////////////
#ifndef __opus_cci_h__
#define __opus_cci_h__

#ifdef OGG_OPUS_DEC

#include "opusfile.h"
#include "internal.h"
#include "ccidec.h"

#define MAX_CHANNEL        2
#define BYTE_WIDTH         2
#define HEADER_SIZE        1024
#define MAX_FRAME          60
#define MAX_FREQ_KHZ       48
#define FRAMES_PER_PACKET  1
#define INPUT_BUFFER_SIZE  MAX_FRAME *MAX_FREQ_KHZ
#define OUTPUT_BUFFER_SIZE MAX_FRAME *FRAMES_PER_PACKET *MAX_CHANNEL *MAX_FREQ_KHZ

/**
 * @brief Opus decoder CCI structure
 *
 */
typedef struct OpusDec_CCI
{
    OggOpusFile *opus_file;                     /*!< OggOpusFile structure */
    decoder_callback_FunctionTable_t callbacks; /*!< Streamer IO callback functions */
    CCIDecInfo *cci_dec;                        /*!< CCIDecInfo structure */
    int16_t output_buffer[OUTPUT_BUFFER_SIZE];  /*!< Buffer to store decoded data */
} OpusDec_CCI_t;

// OPUS Decoder Public API
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the minimum input/output frame size for the decoder.
//!
//!
//! \param[out] inSize  Input Frame Size
//! \param[out] outSize Output Frame Size
//!
//! \retval it returns the minimum input/output frame size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t OggOPUSGetIOFrameSize(int32_t *in_size, int32_t *out_size);

/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//!
//! \param[in] None
//!
//! \returns it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t OggOPUSGetMemorySize(void);

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
int32_t OggOPUSInit(int32_t *memory[], int32_t *pCallbackFn[], int32_t *pUserData);

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
int32_t OggOPUSDecode(int32_t *memory[], int32_t *sampleProudced, int32_t *bufOut);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder reset function to cleanup the decoder structures and free memory.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//!
//! \retval kCodecSuccess    if codec destroys successfully.
//! \retval kCodecInitError     if codec fails to cleanup.
/////////////////////////////////////////////////////////////////////////////////
int32_t OggOPUSReset(int32_t *pMemory[]);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] seekOffset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int32_t OggOPUSSeek(int32_t *pMemory[], int32_t seekOffset);

#endif /* OGG_OPUS_DEC */

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}

#endif /* __opus_cci_h__ */
