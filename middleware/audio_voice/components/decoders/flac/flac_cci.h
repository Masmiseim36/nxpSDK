/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

////////////////////////////////////////////////////////////////////////////////
//! \addtogroup flacdec FLAC Decoder
//! @{
//
// Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
//
//! \file flac_cci.h
//! \brief Contains FLAC Decoder CCI APIs definitions.
////////////////////////////////////////////////////////////////////////////////
#ifndef __flac_cci_h__
#define __flac_cci_h__

#include <stdint.h>

// FLAC Decoder Public API
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//! \fntype Function
//!
//! \param[in] None
//!
//! \retval it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int32_t FLACDecoderGetMemorySize();

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the
//! decoder internal structures.
//!
//! \fntype Function
//!
//! \param[in] *memory         This is pointer to the decoder memory.
//! \param[in] *callback_fn    This is pointer to the decoder callback functions.
//! \param[in] *user_data      This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    if codec initializes successfully.
//! \retval kCodecInitError     if codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////
int32_t FLACDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decodes the input bit stream and generates the PCM data in output pcm buffer.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] *sample_produced     This is pointer to the samples produced by the codec.
//! \param[in] *buf_out             This is pointer to the output pcm buffer.
//!
//! \retval kCodecSuccess           if codec decodes frame successfully.
//! \retval kCodecDecodeError       if codec fails to decode frame.
//! \retval kCodeEndOfDecode        if decoding of input file is done.
/////////////////////////////////////////////////////////////////////////////////
int32_t FLACDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder reset function.
//!
//! \fntype Function
//!
//! \param[in] *memory             This is pointer to the decoder memory.
//!
//! \retval kCodecSuccess          if codec resets successfully.
//! \retval kCodecResetError       if codec fails to reset.
/////////////////////////////////////////////////////////////////////////////////
int32_t FLACDecoderReset(int32_t *memory[]);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//! \fntype Function
//!
//! \param[in] *memory              This is pointer to the decoder memory.
//! \param[in] seek_offset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int32_t FLACDecoderSeek(int32_t *memory[], int32_t seek_offset);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder get IO frame size.
//!
//!
//! \param[in] inSize            This is pointer to inSize.
//! \param[in] outSize           This is pointer to outSize.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////

int32_t FLACDecoderGetIOFrameSize(int32_t *inSize, int32_t *outSize);

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder get Metadata.
//!
//!
//! \param[in] *memory           This is pointer to the decoder memory.
//! \param[in] *callback_fn      This is pointer to the decoder callback functions.
//! \param[in] *user_data        This is pointer to the user specific data.
//!
//! \retval It returns metadata.
/////////////////////////////////////////////////////////////////////////////////

int32_t FLACDecodergetMetaData(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);

#endif /* __flac_cci_h__ */

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
