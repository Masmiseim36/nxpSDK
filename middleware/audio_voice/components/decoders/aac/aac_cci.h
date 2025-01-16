/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __aac_cci_h__
#define __aac_cci_h__

#include <stdint.h>

// AAC Decoder Public API
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//! \fntype Function
//!
//! \retval It returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderGetMemorySize();

/////////////////////////////////////////////////////////////////////////////////
//! \brief Get metadata
//!
//! \fntype Function
//!
//! \param[in] *memory      This is pointer to store metadata.
//! \param[in] *callback_fn This is pointer to the decoder callback functions.
//! \param[in] *user_data   This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    If codec initializes successfully.
//! \retval kCodecInitError  If codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecodergetMetaData(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the
//! decoder internal structures.
//!
//! \fntype Function
//!
//! \param[in] *memory      This is pointer to the decoder memory.
//! \param[in] *callback_fn This is pointer to the decoder callback functions.
//! \param[in] *user_data   This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    If codec initializes successfully.
//! \retval kCodecInitError  If codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decodes the input bit stream and generates the PCM data in output pcm buffer.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] *sample_produced     This is pointer to the samples produced by the codec.
//! \param[in] *buf_out             This is pointer to the output pcm buffer.
//!
//! \retval kCodecSuccess           If codec decodes frame successfully.
//! \retval kCodecDecodeError       If codec fails to decode frame.
//! \retval kCodeEndOfDecode        If decoding of input file is done.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder reset function.
//!
//! \fntype Function
//!
//! \param[in] *memory             This is pointer to the decoder memory.
//!
//! \retval kCodecSuccess          If codec resets successfully.
//! \retval kCodecResetError       If codec fails to reset.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderReset(int32_t *memory[]);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] seek_offset          It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderSeek(int32_t *memory[], int32_t seek_offset);

/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the minimum input/output frame size for the decoder.
//!
//! \fntype Function
//!
//! \param[out] in_size	 Input Frame Size
//! \param[out] out_size Output Frame Size
//!
//! \retval It returns the minimum input/output frame size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t AACDecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);

#endif /* __wav_cci_h__ */

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
