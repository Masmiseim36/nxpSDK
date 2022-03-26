////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opusdec OPUS Decoder
//! @{
//
// Copyright (c) 2020 NXP
//
//! \file opus_cci.h
//! \brief Contains OPUS Decoder CCI APIs definitions.
////////////////////////////////////////////////////////////////////////////////
#ifndef __opus_cci_h__
#define __opus_cci_h__

#include "opusfile.h"
#include "internal.h"
#include "ccidec.h"

#define MAX_CHANNEL 2
#define BYTE_WIDTH 2
#define HEADER_SIZE 1024
#define MAX_FRAME   60
#define MAX_FREQ_KHZ 48
#define FRAMES_PER_PACKET   1
#define INPUT_BUFFER_SIZE MAX_FRAME*MAX_FREQ_KHZ
#define OUTPUT_BUFFER_SIZE MAX_FRAME*FRAMES_PER_PACKET*MAX_CHANNEL*MAX_FREQ_KHZ

typedef struct OpusDec_CCI
{
    OggOpusFile opus_file;                         /*!< OggOpusFile structure */
    OpusHead header;
    decoder_callback_FunctionTable_t callbacks;   /*!< Streamer IO callback functions */
    OpusDecoder *decoder;
    CCIDecInfo cci_dec;
    ogg_page og;
    int packet_decode;
    char *input_buffer;
    int16_t output_buffer[OUTPUT_BUFFER_SIZE];
} OpusDec_CCI_t;

// OPUS Decoder Public API
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//! \fntype Function
//!
//! \param[in] None
//!
//! \returns it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSGetMemorySize (void);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the decoder internal structures.
//!
//! \fntype Function
//!
//! \param[in] *memory         This is pointer to the decoder memory.
//! \param[in] *callbackFn     This is pointer to the decoder callback functions.
//! \param[in] *userData        This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    if codec initializes successfully.
//! \retval kCodecInitError     if codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSInit (int *memory[], int *pCallbackFn[], int *pUserData);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decodes the input bit stream and generates the PCM data in output pcm buffer.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] *sampleProudced      This is pointer to the samples produced by the codec.
//! \param[in] *bufOuts             This is pointer to the output pcm buffer.
//!
//! \retval kCodecSuccess           if codec decodes frame successfully.
//! \retval kCodecDecodeError       if codec fails to decode frame.
//! \retval kCodeEndOfDecode        if decoding of input file is done.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSDecode(int *memory[],int *sampleProudced,int *bufOut);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder reset function to cleanup the decoder structures and free memory.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//!
//! \retval kCodecSuccess    if codec destroys successfully.
//! \retval kCodecInitError     if codec fails to cleanup.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSReset(int *pMemory[]);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] seekOffset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSSeek(int *pMemory[],int seekOffset);

#endif /* __opus_cci_h__ */

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
