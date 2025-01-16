/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __mp3_cci_h__
#define __mp3_cci_h__

#include <stdint.h>

// AAC Decoder Public API
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//!
//! \param[in] None
//!
//! \retval it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t MP3DecoderGetMemorySize ();

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the decoder internal structures.
//!
//!
//! \param[in] memory         This is pointer to the decoder memory.
//! \param[in] callbackFn     This is pointer to the decoder callback functions.
//! \param[in] userData        This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    if codec initializes successfully.
//! \retval kCodecInitError     if codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int32_t MP3DecoderInit (int32_t *pMemory[], int32_t *pCallbackFn[], int32_t *pUserData);

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
int32_t MP3DecoderDecode(int32_t *memory[],int32_t *sampleProudced,int32_t *bufOut);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder reset function.
//!
//!
//! \param[in] memory             This is pointer to the decoder memory.
//!
//! \retval kCodecSuccess          if codec resets successfully.
//! \retval kCodecResetError       if codec fails to reset.
/////////////////////////////////////////////////////////////////////////////////
int32_t MP3DecoderReset(int32_t *memory[]);
/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] seekOffset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int32_t MP3DecoderSeek(int32_t *memory[],int32_t seekOffset);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder get IO frame size.
//!
//!
//! \param[in] inSize            This is pointer to inSize.
//! \param[in] outSize           This is pointer to outSize.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////

int32_t MP3DecoderGetIOFrameSize(int32_t *inSize, int32_t *outSize);

#endif /* __wav_cci_h__ */

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
