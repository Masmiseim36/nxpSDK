
/**
 *  \file LC3_api.h
 *
 *  \brief This file contains interface of LC3
 *  (Low Complexity Communication Codec) encoder,decoder.
 */

/*
 *  Copyright (C) 2021. Mindtree Ltd.
 *  All rights reserved.
 */
/* LC3_DSP Macro defines LC3 Feature as below,
 * LC3_DSP = 0 : LC3 NODSP Feature Enabled
 * LC3_DSP = 1 : LC3 DSP Default Feature Enabled
 * LC3_DSP = 2 : LC3 DSP OPT Feature Enabled
 */

#if defined(LC3_DSP) && (LC3_DSP == 1)

#ifndef _H_LC3_API_
#define _H_LC3_API_

/* --------------------------------------------- Header File Inclusion */
#include "LC3_os.h"
#include "LC3_limits.h"

/* --------------------------------------------- Global Definitions */

/**
 * \defgroup lc3_module Low Complexity Communication Codec (LC3)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Low Complexity Communication Codec (LC3) module to the Application
 *  and other upper layers of the stack.
 */

/**
 * \defgroup lc3_defines Defines
 * \{
 * Describes defines for the LC3 Module.
 * \brief This section describes the various Defines in EtherMind
 * Low Complexity Communication Codec (LC3) Layer.
 */

/**
 * \defgroup lc3_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Low Complexity Communication Codec (LC3) Layer.
 */

/**
 * \name LC3 Encoder Error Codes
 * \{
 */
/** LC3 Encoder Success */
#define LC3_ENCODER_SUCCESS                           0x00
/** LC3 Encoder Size Allocation Error */
#define LC3_ENCODER_SIZE_ALLOCATE_ERROR               0x01
/** LC3 Encoder Core Buffer Null Pointer Error */
#define LC3_ENCODER_CORE_BUFFER_NULL_POINTER          0x02
/** LC3 Encoder Setup Parameter Error */
#define LC3_ENCODER_SETUP_PARAM_ERROR                 0x03
/** LC3 Encoder Bitrate Update Error */
#define LC3_ENCODER_BITRATE_UPDATE_ERROR              0x04
/** LC3 Encoder Work Buffer NULL Pointer Error */
#define LC3_ENCODER_WORK_BUFFER_NULL_POINTER          0x05
/** LC3 Encoder Invalid Sample Rate Error */
#define LC3_ENCODER_INVALID_SAMPLE_RATE               0x06
/** LC3 Encoder Invalid Frame MS Error */
#define LC3_ENCODER_INVALID_FRAME_MS                  0x07
/** LC3 Encoder Invalid Channel Count Error */
#define LC3_ENCODER_INVALID_CHANNEL_COUNT             0x08
/** LC3 Encoder Invalid Target Bytes Error */
#define LC3_ENCODER_INVALID_TARGET_BYTES              0x09
/** LC3 Encoder Invalid Context Error */
#define LC3_ENCODER_INVALID_CONTEXT                   0x0A
/** \} */

/**
 * \name LC3 Decoder Error Codes
 * \{
 */
/** LC3 Decoder Success */
#define LC3_DECODER_SUCCESS                           0x00
/** LC3 Decoder Size Allocation Error */
#define LC3_DECODER_SIZE_ALLOCATE_ERROR               0x01
/** LC3 Decoder Core Buffer Null Pointer Error */
#define LC3_DECODER_CORE_BUFFER_NULL_POINTER          0x02
/** LC3 Decoder Setup Parameter Error */
#define LC3_DECODER_SETUP_PARAM_ERROR                 0x03
/** LC3 Decoder Work Buffer NULL Pointer Error */
#define LC3_DECODER_WORK_BUFFER_NULL_POINTER          0x04
/** LC3 Decoder Invalid Sample Rate Error */
#define LC3_DECODER_INVALID_SAMPLE_RATE               0x05
/** LC3 Decoder Invalid Frame MS Error */
#define LC3_DECODER_INVALID_FRAME_MS                  0x06
/** LC3 Decoder Invalid Channel Count Error */
#define LC3_DECODER_INVALID_CHANNEL_COUNT             0x07
/** LC3 Decoder Invalid PLC Type Error */
#define LC3_DECODER_INVALID_PLC_TYPE                  0x08
/** LC3 Decoder Invalid Context Error */
#define LC3_DECODER_INVALID_CONTEXT                   0x09
/** \} */

/** \} */

/**
 * \defgroup lc3_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind
 * Low Complexity Communication Codec (LC3) Layer.
 */

/**
 * \name LC3 Common Constants
 * \{
 */
/** LC3 maximum number of supported samples */
#define LC3_INPUT_FRAME_SIZE_MAX                     480
/** LC3 maximum supported frame-size */
#define LC3_FRAME_SIZE_MAX                           400
/** \} */

/**
 * \name LC3 Encoder Constants
 * \{
 */
/** LC Encoder Maximum Core Buffer Size - 1 channel, 64 bit system */
#define LC3_ENCODER_CORE_BUFFER_SIZE_MAX             3528   /* 3512 */
/** LC Encoder Maximum Work Buffer Size */
#define LC3_ENCODER_WORK_BUFFER_SIZE_MAX             6848   /* 6784 */
/** \} */

/**
 * \name LC3 Decoder Constants
 * \{
 */
/** LC Decoder Maximum Core Buffer Size - 1 channel, 64 bit system */
#define LC3_DECODER_CORE_BUFFER_SIZE_MAX             5224   /* 5216 */
/** LC Decoder Maximum Work Buffer Size */
#define LC3_DECODER_WORK_BUFFER_SIZE_MAX             6528   /* 6512 */
/** \} */

/** \} */

/* --------------------------------------------- Data Types/ Structures */

/**
 *  \defgroup lc3_structures Structures
 *  \{
 *  \brief This section describes the various Data-Types and Structures in
 *  EtherMind Low Complexity Communication Codec (LC3) Layer.
 */

/** LC3 Encoder Session Configuration Parameters */
typedef struct _LC3_ENCODER_SESSION_CONFIG
{
    /**
     *  The sampling frequency of the audio stream that has to be
     *  encoded/decoded.
     *
     *  The sampling_frequency value shall be one of the following
     *  - 8000 (8 KHz), 16000 (16 KHz), 24000 (24 KHz),
     *    32000 (32 KHz), 44100 (44.1 KHz), 48000 (48 kHz)
     */
    UINT16 sampling_frequency;

    /**
     *  The number of audio channels.
     *  For MONO channel nrof_channels is 1, for Stereo 2, etc.
     */
    UINT16 nrof_channels;

    /**
     *  Frame Duration in milliseconds.
     *
     *  The valid values are : 100 (represents 10 ms), 75 (represents 7.5 ms)
     */
    UINT16 frame_duration;

    /**
     *  The bits per audio sample for the input/output PCM signal.
     *
     *  The valid values are : 16-bit, 24-bit or 32-bit
     */
    UINT16 bps;

} LC3_ENCODER_SESSION_CONFIG;

/** LC3 Encoder frame structure */
typedef struct _LC3_ENCODER_FRAME
{
    /**
     *  byte count value to be used for frame encoding
     *  of each audio channel
     */
    UINT16 byte_count;

    /**
     *  Input audio data for frames.
     *  16-, 24- or 32-bits per sample.
     */
    INT32  *input_pcm;

    /** LC3 encoded output buffer */
    UINT8  *output_enc;

}LC3_ENCODER_FRAME;

/** LC3 Encoder context structure */
typedef struct _LC3_ENCODER_CNTX
{
    /** LC3 Encoder Session Configuration Parameter */
    LC3_ENCODER_SESSION_CONFIG    session;

    /** LC3 Encoder Frame Buffer descriptors */
    LC3_ENCODER_FRAME             encframe[LC3_CHANNELS_MAX];

    /** Memory for core buffer */
    void                         *encoder_core;

    /** Memory for work/scratch buffer */
    UINT8                        *workbuffer;

}LC3_ENCODER_CNTX;

/** LC3 Decoder Session Configuration Parameters */
typedef struct _LC3_DECODER_SESSION_CONFIG
{
    /**
     *  The sampling frequency of the audio stream that has to be
     *  encoded/decoded.
     *
     *  The sampling_frequency value shall be one of the following
     *  - 8000 (8 KHz), 16000 (16 KHz), 24000 (24 KHz),
     *    32000 (32 KHz), 44100 (44.1 KHz), 48000 (48 kHz)
     */
    UINT16 sampling_frequency;

    /**
     *  The number of audio channels.
     *  For MONO channel nrof_channels is 1, for Stereo 2, etc.
     */
    UINT16 nrof_channels;

    /**
     *  Frame Duration in milliseconds.
     *
     *  The valid values are : 100 (represents 10 ms), 75 (represents 7.5 ms)
     */
    UINT16 frame_duration;

    /**
     *  The bits per audio sample for the input/output PCM signal.
     *
     *  The valid values are : 16-bit, 24-bit or 32-bit
     */
    UINT16 bps;

    /* external rate control : the maximum byte count for the session */
    /* UINT16 byte_count_max; */

} LC3_DECODER_SESSION_CONFIG;

/** LC3 Decoder frame structure */
typedef struct _LC3_DECODER_FRAME
{
    /**
     *  byte count value to be used for decoding the frames
     *  of each audio channel
     */
    UINT16 byte_count;

    /** LC3 encoded input buffer */
    UINT8  *input_dec;

    /**
     *  Output audio data for frames.
     *  16-, 24- or 32-bits per sample.
     */
    INT32  *output_pcm;

}LC3_DECODER_FRAME;

/** LC3 Decoder context structure */
typedef struct _LC3_DECODER_CNTX
{
    /** LC3 Decoder Session Configuration Parameter */
    LC3_DECODER_SESSION_CONFIG    session;

    /** LC3 Decoder Frame Buffer descriptors */
    LC3_DECODER_FRAME             decframe[LC3_CHANNELS_MAX];

    /** Memory for core buffer */
    void                         *decoder_core;

    /** Memory for work/scratch buffer */
    UINT8                        *workbuffer;

    /** Packet Loss Concealment Type */
    UINT8                         plctype;

}LC3_DECODER_CNTX;

/** \} */

/** \} */

/* --------------------------------------------- Function */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup lc3_api_defs API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind Low Complexity Communication Codec (LC3) Layer to the Application
 * and other upper layers of the stack.
 */

/**
 * \defgroup lc3_encoder_api_defs LC3 Encoder APIs
 * \{
 * This section describes the EtherMind LC3 Encoder APIs.
 */

/**
 *  \brief API to create LC3 Encoder Context
 *
 *  \par Description
 *  This function creates the LC3 encoder context.
 *
 *  \param [in] lc3_enc         LC3 Encoder pointer used for context creation
 *  \param [in] sample_rate     Sampling Frequency of input audio samples, in hz
 *  \param [in] bps_in          Bits per sample of input audio samples
 *  \param [in] channels_count  Input channels count, 1=mono, 2=stereo, etc.
 *  \param [in] frame_ms        Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 *  \param [in] bytes_count     Target bytes [w.r.t bitrate] for each channel
 *  \param [in] core_buffer     Core buffer pointer
 *  \param [in] work_buffer     Work buffer pointer
 *  \param [in] pcm_buf_in      PCM buffer containing input audio samples
 *  \param [in] enc_buf_out     Encoded buffer to be filled by the encoder
 *
 *  \return \ref LC3_ENCODER_SUCCESS or an error code indicating
 *  reason for failure
 *
 *  \note
 *  \li This function does not allocate core, work/scratch buffers
 */
INT32  LC3_encoder_create
       (
           LC3_ENCODER_CNTX *lc3_enc,
           INT32             sample_rate,
           INT32             bps_in,
           INT32             channels_count,
           INT32             frame_ms,
           INT32            *bytes_count,
           UINT8            *core_buffer,
           UINT8            *work_buffer,
           INT32           **pcm_buf_in,
           UINT8           **enc_buf_out
       );

/**
 *  \brief API to clear LC3 encoder context
 *
 *  \par Description
 *  This function clears encoder context
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *
 *  \return None
 */
void LC3_encoder_delete(LC3_ENCODER_CNTX *lc3_enc);

/**
 *  \brief API to perform LC3 encoding
 *
 *  \par Description
 *  This is the main encoding function of one raw input frame.
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *
 *  \return on success returns number of encoded bytes, else zero
 */
INT32 LC3_encoder_process(LC3_ENCODER_CNTX *lc3_enc);

/** utility functions */
/**
 *  \brief API to get LC3 Encoder frame length
 *
 *  \par Description
 *  This gets the numbers of input samples or frame length according
 *  to encoder context [Fs, Nms].
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *
 *  \return number of samples count or frame length
 */
INT32 LC3_encoder_get_frame_length(LC3_ENCODER_CNTX *lc3_enc);

/**
 *  \brief API to get LC3 Encoder encoded bitrate
 *
 *  \par Description
 *  This function calculates the actual bitrate used in encoding
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *
 *  \return bitrate in bps
 */
INT32 LC3_encoder_get_encoded_bitrate(LC3_ENCODER_CNTX *lc3_enc);

/**
 *  \brief API to get I/O data buffers used by encoder
 *
 *  \par Description
 *  This gets the encoder's I/O data buffers
 *
 *  \param [in]  lc3_enc         LC3 Encoder context
 *  \param [out] pcm_buf_in      Pointer to PCM buffer containing input audio samples
 *  \param [out] enc_buf_out     Pointer to Encoded output buffer filled by the encoder
 *
 *  \return buffer size in bytes
 */
INT32 LC3_encoder_get_io_data_buffer
      (
          LC3_ENCODER_CNTX *lc3_enc,
          INT32**pcm_buf_in,
          UINT8**enc_buf_out
      );

/**
 *  \brief API to get work/scratch buffer size required by encoder
 *
 *  \par Description
 *  This gets the encoder's work/scratch buffer size
 *
 *  \param [in] sample_rate    Sampling Frequency of input audio samples, in Hz
 *  \param [in] frame_ms       Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 *
 *  \return buffer size in bytes
 */
INT32 LC3_encoder_get_work_buffer_size
      (
          INT32 sample_rate,
          INT32 frame_ms
      );

/**
 *  \brief API to clear LC3 Encoder Input Buffer
 *
 *  \par Description
 *  This clears the input buffer of encoder from sample index
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *  \param [in] n_samples_idx   Audio Sample index
 *
 *  \return \ref LC3_ENCODER_SUCCESS or an error code indicating reason
 *  for failure
 */
INT32 LC3_encoder_clear_io_data_buffer
      (
          LC3_ENCODER_CNTX *lc3_enc,
          INT32 n_samples_idx
      );

/**
 *  \brief API to get core buffer size required by LC3 Encoder
 *
 *  \par Description:
 *  This gets the encoder's core context buffer size
 *
 *  \param [in] sample_rate     Sampling Frequency of input audio samples, in Hz
 *  \param [in] channels_count  Input channels count, 1=mono, 2=stereo, etc.
 *
 *  \return buffer size in bytes
 */
INT32 LC3_encoder_get_core_context_buffer_size
      (
          INT32 sample_rate,
          INT32 channels_count
      );

/**
 *  \brief API to get number of bytes for a given bitrate
 *
 *  \par Description
 *  This function converts bitrate to bytes [mono channel]
 *
 *  \param [in] bitrate           Bitrate in bps
 *  \param [in] sample_rate       Sampling Frequency of input audio samples, in Hz
 *  \param [in] frame_duration    Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 *
 *  \return bytes
 */
INT32 LC3_encoder_convert_bitrate_to_bytes
      (
          INT32 bitrate,
          INT32 sample_rate,
          INT32 frame_duration
      );

/**
 *  \brief API to get bitrate for given number of bytes
 *
 *  \par Description:
 *  This function converts bytes to bitrate  [single channel]
 *
 *  \param [in] nbytes            No. of bytes
 *  \param [in] sample_rate       Sampling Frequency of input audio samples, in Hz
 *  \param [in] frame_duration    Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 *
 *  \return bitrate in bps
 */
INT32 LC3_encoder_convert_bytes_to_bitrate
      (
          INT32 nbytes,
          INT32 sample_rate,
          INT32 frame_duration
      );

/**
 *  \brief API to set target number of bytes
 *
 *  \par Description
 *  This function set the bytes in rate switching without changing
 *  buffer context
 *
 *  \param [in] lc3_enc         LC3 Encoder context
 *  \param [in] nbytes          Target bytes of channels
 *
 *  \return \ref LC3_ENCODER_SUCCESS or an error code indicating
 *  reason for failure
 */
INT32 LC3_encoder_set_target_bytes
      (
          LC3_ENCODER_CNTX *lc3_enc,
          INT32 *nbytes
      );

/** \} */

/**
 * \defgroup lc3_decoder_api_defs LC3 Decoder APIs
 * \{
 * This section describes the EtherMind LC3 Decoder APIs.
 */

/**
 *  \brief API to create LC3 Decoder Context
 *
 *  \par Description
 *  This function creates the decoder context.
 *
 *  \param [in] lc3_dec           LC3 Decoder pointer used for context creation
 *  \param [in] sample_rate       Sampling frequency of encoded samples, in hz
 *  \param [in] bps               Bits per sample of decoded audio samples
 *  \param [in] channels_count    Input channels count, 1=mono, 2=stereo, etc.
 *  \param [in] frame_ms          Frame duration in ms [100 for 10.0ms, 75 for 7.5ms]
 *  \param [in] plctype           Type of PLC method, 0=default
 *  \param [in] core_buffer       Core buffer pointer
 *  \param [in] work_buffer       Work buffer pointer
 *  \param [in] enc_buf_in        Encoded buffer to be used as input
 *  \param [in] dec_buf_out       PCM buffer to be filled with decoded audio samples
 *
 *  \return \ref LC3_DECODER_SUCCESS or an error code indicating reason for failure
 *
 *  \note
 *  \li This function does not allocate core, work/scratch buffers
 */
INT32 LC3_decoder_create
      (
          LC3_DECODER_CNTX *lc3_dec,
          INT32             sample_rate,
          INT32             bps,
          INT32             channels_count,
          INT32             frame_ms,
          INT32             plctype,
          UINT8            *core_buffer,
          UINT8            *work_buffer,
          UINT8           **enc_buf_in,
          INT32           **dec_buf_out
      );

/**
 *  \brief API to clear LC3 decoder context
 *
 *  \par Description:
 *  This function clears the decoder context.
 *
 *  \param [in] lc3_dec       LC3 Decoder context
 *
 *  \return None
 */
void LC3_decoder_delete(LC3_DECODER_CNTX *lc3_dec);

/**
 *  \brief API to perform LC3 decoding
 *
 *  \par Description:
 *  This is the main decoding function of one encoded input frame.
 *
 *  \param [in] lc3_dec       LC3 Decoder context
 *  \param [in] flg_bfi       External bad frame index [G192 support]
 *                            0=>G192_GOOD_FRAME,1=>G192_BAD_FRAME,2=>G192_REDUNDANCY_FRAME
 *  \param [in] byte_count    per channel encoded bytes count [>=0].
 *                            If 0 => lost packet, decoder does PLC
 *
 *  \return \ref LC3_DECODER_SUCCESS or an error code indicating reason for failure
 */
INT32 LC3_decoder_process
      (
          LC3_DECODER_CNTX *lc3_dec,
          INT32 *flg_bfi,
          INT32 *byte_count
      );

/** Utility functions */
/**
 *  \brief API to get LC3 Decoder frame length
 *
 *  \par Description
 *  This gets the numbers of samples or frame length according to
 *  decoder context [Fs,Nms].
 *
 *  \param [in] lc3_dec       LC3 Decoder context
 *
 *  \return
 *    number of samples count or frame length
 */
INT32 LC3_decoder_get_frame_length(LC3_DECODER_CNTX *lc3_dec);

/**
 *  \brief API to get LC3 Decoder delay
 *
 *  \par Description
 *  This gets the decoder delay in terms of samples
 *
 *  \param [in] lc3_dec       LC3 Decoder context
 *
 *  \return
 *     Number of samples
 */
INT32 LC3_decoder_get_delay_length(LC3_DECODER_CNTX *lc3_dec);

/**
 *  \brief API to get work/scratch buffer size required by LC3 Decoder
 *
 *  \par Description:
 *  This gets the decoder's work/scratch buffer size
 *
 *  \param [in] sample_rate       Sampling frequency of encoded samples, in Hz
 *
 *  \return
 *     buffer size [in bytes]
 */
INT32 LC3_decoder_get_work_buffer_size(INT32 sample_rate);

/**
 *  \brief API to get core buffer size required by LC3 Decoder
 *
 *  \par Description
 *  This gets the decoder's core context buffer size
 *
 *  \param [in] sample_rate     Sampling Frequency of input audio samples, in Hz
 *  \param [in] channels_count  Input channels count, 1=mono, 2=stereo, etc.
 *
 *  \return
 *     buffer size in bytes
 */
INT32 LC3_decoder_get_core_context_buffer_size
      (
          INT32 sample_rate,
          INT32 channels_count
      );

/** \} */

/** \} */

#ifdef __cplusplus
};
#endif

/** \} */

#endif /* _H_LC3_API_ */
#endif /* defined(LC3_DSP) && (LC3_DSP == 0) */
