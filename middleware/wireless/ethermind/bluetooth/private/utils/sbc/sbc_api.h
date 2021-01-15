
/*
 *  \file sbc_api.h
 *
 *   This header file contains all the API declarations and definitions
 *   exported by the SBC codec. These are exported to the applications
 *   and to the profiles that intend to use SBC encoder and decoder.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SBC_API_
#define _H_SBC_API_

/* ----------------------------------- Header File Inclusion */
#include "sbc_os.h"

#define SBC_EXCLUDE_FEATURE             0x00
#define SBC_INCLUDE_FEATURE             0x01

/* #define SBC_VBR */

/*
 * Note:
 * 1. Enable mSBC switch only for msbc codec
 * 2. Enable SBC switch only for SBC codec
 * 3. Disable both mSBC & SBC switches for both mSBC and SBC
 */

/*
 * Compilation switch to enable only modified SBC required for
 * Handsfree Wideband Speech (WBS)
 */
/* #define mSBC */

/*
 * Compilation switch to enable only SBC required for A2DP
 */
/* #define SBC  */

/*
 * This enables the ARM assembly code for a portion of code for
 * the bitunpacking module. This helps to reduce the cycle count
 * by 600 cycles.
 */
#define SBC_PLATFORM_ARM                SBC_EXCLUDE_FEATURE


#define SBC_SPEED                       SBC_EXCLUDE_FEATURE


/* ==================== SBC ERRORS START ================== */
#define SBC_BAD_CRC_RECEIVED              0x1101
#define SBC_BIT_ALLOCATION_FAILED         0x1102
#define SBC_INVALID_BITPOOL               0x1103
#define SBC_MAX_BITRATE_EXCEED            0x1104
#define SBC_BITPACKING_FAILED             0x1105
#define SBC_INVALID_PARAMETERS            0x1106
#define SBC_INSUFFICIENT_INPUT_BUFFER     0x1107
#define SBC_INSUFFICIENT_OUTPUT_BUFFER    0x1108
#define SBC_FAILED_SYNCWORD_MATCH         0x1109
#define SBC_INVALID_SAMPLING_FREQUENCY    0x110A
#define SBC_INVALID_CHANNEL_MODE          0x110B
#define SBC_INVALID_SUBBANDS              0x110C
#define SBC_INVALID_ALLOCATION_METHOD     0x110D
#define SBC_INVALID_BLOCK_LENGTH          0x110E
/* ==================== SBC ERRORS END ================== */


/* ----------------------------------- Global Definitions */
/* SBC, mSBC input and output buffer sizes */
#ifdef mSBC
#define SBC_MAX_INPUT_SIZE                240
#else
#define SBC_MAX_INPUT_SIZE                512
#endif /* mSBC */

#ifdef mSBC
#define SBC_MAX_OUTPUT_SIZE               240
#else
#define SBC_MAX_OUTPUT_SIZE               512
#endif /* mSBC */

/* SBC Sampling Frequency */
#define SBC_SAMPLING_FREQ_16000           0
#define SBC_SAMPLING_FREQ_32000           1
#define SBC_SAMPLING_FREQ_44100           2
#define SBC_SAMPLING_FREQ_48000           3

/* SBC Channel Mode constant */
#define SBC_CHANNEL_MONO                  0
#define SBC_CHANNEL_DUAL                  1
#define SBC_CHANNEL_STEREO                2
#define SBC_CHANNEL_JOINT_STEREO          3

/* SBC Block size constant */
#define SBC_BLOCK_SIZE_4                  4
#define SBC_BLOCK_SIZE_8                  8
#define SBC_BLOCK_SIZE_12                 12
#define SBC_BLOCK_SIZE_16                 16

/* New block size added for modified SBC */
#define SBC_BLOCK_SIZE_15                 15

/* SBC subband constant */
#define SBC_SUBBAND_4                     4
#define SBC_SUBBAND_8                     8

/* SBC Bitallocation method constant */
#define SBC_SNR                           1
#define SBC_LOUDNESS                      0

/* SBC bitrate mode constant */
#define SBC_CBR_MODE                      0
#define SBC_VBR_MODE                      1

/* SBC maximum limits */
#ifdef mSBC
#define SBC_MAX_BLOCK_SIZE                15
#else
#define SBC_MAX_BLOCK_SIZE                16
#endif /* mSBC */

#define SBC_MAX_SUBBAND                   8
#define SBC_MAX_CHANNEL                   2
#define SBC_MAX_FILTER_LENGTH             10 * SBC_MAX_SUBBAND


/* ----------------------------------- Internal Structures/Data Types */

typedef UCHAR SBC_ENCODER_SF_INDEX[SBC_MAX_CHANNEL][SBC_MAX_SUBBAND];

typedef UCHAR SBC_ENCODER_BITS_ALLOCATED[SBC_MAX_CHANNEL][SBC_MAX_SUBBAND];

typedef INT32 SBC_ENCODER_SUBBAND_SAMPLE[SBC_MAX_BLOCK_SIZE][SBC_MAX_CHANNEL]
                                        [SBC_MAX_SUBBAND];

typedef INT16 SBC_ENCODER_INPUT_SAMPLE[(SBC_MAX_FILTER_LENGTH + 128) *
                                       (SBC_MAX_CHANNEL)];

typedef UCHAR SBC_DECODER_SF_INDEX[SBC_MAX_CHANNEL][SBC_MAX_SUBBAND];

typedef UCHAR SBC_DECODER_BITS_ALLOCATED[SBC_MAX_CHANNEL][SBC_MAX_SUBBAND];

typedef INT32 SBC_DECODER_SUBBAND_SAMPLE[SBC_MAX_BLOCK_SIZE][SBC_MAX_CHANNEL]
                                        [SBC_MAX_SUBBAND];

typedef INT32 SBC_DECODER_V[10 * SBC_MAX_SUBBAND + 10 * SBC_MAX_SUBBAND + 120];

typedef INT32 SBC_DECODER_BITNEED[SBC_MAX_CHANNEL][SBC_MAX_SUBBAND];


/**
 *  \struct SBC_CODEC_PARAM
 *  \brief  SBC configuration information.
 *
 *  This data type represents the Configuration of SBC codec.
 *
 *  \note
 *
 *
 */
typedef struct
{
    /**
     *  This is the sampling frequency with which the stream has
     *  to be encoded/decoded. The sampling_frequency should be
     *  0, 1, 2, 3 only.
     *  sampling_frequency value of 0 represents 16.0 KHz.
     *  sampling_frequency value of 1 represents 32.0 KHz.
     *  sampling_frequency value of 2 represents 44.1 KHz.
     *  sampling_frequency value of 3 represents 48.0 KHz.
     */
    UCHAR sampling_frequency;

    /**
     *  This is the block size with which the stream has to be
     *  encoded/decoded. The nrof_blocks should be 4, 8, 12,
     *  and 16 only.
     *
     *  For mSBC nrof_blocks should be 15.
     */
    UCHAR nrof_blocks;

    /**
     *  This is the channel mode that has to be encoded/decoded.
     *  The channel_mode should be 0, 1, 2, 3 only.
     *  channel_mode value of 0 represents MONO channel.
     *  channel_mode value of 1 represents DUAL_CHANNEL channel.
     *  channel_mode value of 2 represents STEREO channel.
     *  channel_mode value of 3 represents JOINT_STEREO channel.
     */
    UCHAR channel_mode;

    /**
     *  This is to the number of channels has to be encoded/decoded.
     *  For MONO channel nrof_channels is 1, otherwise 2
     */
    UCHAR nrof_channels;

    /**
     *  This is the bit allocation method with which the bits are to
     *  be allocated. The allocation_method should be 0, and 1 only.
     *  allocation_method value of 0 represents LOUDNESS based bit
     *  allocation. allocation_method value of 1 represents SNR based
     *  bit allocation.
     */
    UCHAR allocation_method;

    /**
     *  This is the number of subbands with which the stream has
     *  to be encoded/decoded. The nrof_subbands should be 4, 8 only.
     */
    UCHAR nrof_subbands;

    /**
     *  This is the size of the bit allocation pool that has to be
     *  used for encoding/decoding of the stream. The value of the
     *  bitpool field shall not exceed 64 and 128 for four subbands
     *  and eight subbands respectively for the MONO and DUAL_CHANNEL
     *  channel modes and 128 and 256 for four subbands and eight
     *  subbands respectively for the STEREO and JOINT_STEREO channel
     *  modes. The bitpool value should also be ranging from 2 to 250.
     */
    UCHAR bitpool;

#ifdef SBC_VBR

    /**
     *  This is the minimum value of bitpool configured.
     */

    UCHAR bitpool_min;

    /**
     *  This is the maximum value of bitpool configured.
     */

    UCHAR bitpool_max;

    /**
     *  This is the mode of bitrate that has to be used for encoding/decoding
     *  of the stream. The different modes are Variable Bit Rate (VBR) mode
     *  and Constant Bit Rate (CBR). The bitrate_mode value of zero (0)
     *  represents CBR and one (1) represents VBR
     */
    UCHAR bitrate_mode;

#endif /* SBC_VBR */

} SBC_CODEC_PARAM;


/**
 *  \struct SBC_ENCODER_IO_STRUCT
 *  \brief  SBC Encoder IO structure
 *
 *  \note
 */
typedef struct
{
    /* Buffer containing data to be encoded */
    UINT16 * encoder_input_buffer;

    /* Buffer containing encoded data */
    UCHAR  * encoder_output_buffer;

    /* Subband samples array */
    SBC_ENCODER_SUBBAND_SAMPLE encoder_subband_sample;

    /* input audio samples */
    SBC_ENCODER_INPUT_SAMPLE encoder_input_sample;

    /* Holds the maximum bitneed */
    INT16 encoder_max_bitneed;

    /* Frame Length */
    UINT16 encoder_frame_length;

    /* Number of bytes in the output_buffer */
    UINT16 encoder_output_buflen;

    /* Number of bytes in the input_buffer */
    UINT16 encoder_input_buflen ;

    /* Bits allocated */
    SBC_ENCODER_BITS_ALLOCATED encoder_bits_allocated;

    /* Scale factor indexs */
    SBC_ENCODER_SF_INDEX encoder_sf_index;

    /* SBC Encoder configuration elements */
    SBC_CODEC_PARAM encoder_params;

    /*
     * Information about the subbands with are encoded with joint stereo
     * coding
     */
    UCHAR encoder_join;

} SBC_ENCODER_IO_STRUCT;

/**
 *  \struct SBC_DECODER_IO_STRUCT
 *  \brief  SBC Decoder IO structure
 *
 *  \note
 */
typedef struct
{
    /* SF Index */
    SBC_DECODER_SF_INDEX decoder_sf_index;

    /* Number of bits allocated to a subband */
    SBC_DECODER_BITS_ALLOCATED decoder_bits_allocated;

    /* Suband samples */
    SBC_DECODER_SUBBAND_SAMPLE decoder_subband_sample;

    /* Matrixed output */
    SBC_DECODER_V decoder_v;

    /* Bitneed for each channel and subband */
    SBC_DECODER_BITNEED decoder_bitneed;

    /*
     * Information about the subbands with are encoded with
     * joint stereo coding
     */
    UINT32 decoder_join;

    /* Holds the maximum bitneed */
    INT32 decoder_max_bitneed;

    /* Only the below information can be accessed by application. */

    /* Buffer containing data to be encoded */
    UCHAR *  decoder_input_buffer;

    /* Buffer containing decoded data */
    UINT16 * decoder_output_buffer;

    /* Number of bytes in the output_buffer */
    UINT16   decoder_output_buflen;

    /* Number of bytes in the input_buffer */
    UINT16   decoder_input_buflen ;

} SBC_DECODER_IO_STRUCT;


/* ----------------------------------- API Declarations */
#ifdef __cplusplus
extern "C"{
#endif

/**
 *  @name SBC Encoder APIs - Initialize SBC Encoder
 */

/*@{*/

/**
 *  \brief To initialize the SBC encoder.
 *
 *  \par Description:
 *  This API enables applications/profiles to initialize the SBC encoder.
 *  This API does the configuration of the SBC encoder and validates the
 *  configuration parameters.
 *
 *  \param [in,out] sbc_io
 *         SBC encoder IO parameters.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the
 *                      reason for failure.
 *  \note
 *   Application should populate SBC_CODEC_PARAM data type before calling
 *   this API.
 */
API_RESULT sbc_encoder_init
           (
               /* INOUT */ SBC_ENCODER_IO_STRUCT *  sbc_io
           );

/*@}*/

/**
 *  @name SBC Encoder APIs - Encoding of audio data
 */

/*@{*/

/**
 *  \brief To encode PCM audio samples.
 *
 *  \par Description:
 *  This API enables applications/profiles to encode PCM audio stream into
 *  SBC bitstream. This API encodes the PCM audio stream and packs the
 *  encoded data into SBC bitstream.
 *
 *  \param [in,out] sbc_io
 *         SBC encoder IO parameters.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the
 *                      reason for failure.
 *  \note
 *   \li Application should call sbcencoder_init function before calling
 *       this API.
 *   \li Application should provide one complete frame of data to the
 *       SBC encoder.
 *   \li Application should allocated one complete output buffer.
 */
API_RESULT sbc_encoder ( /* INOUT */ SBC_ENCODER_IO_STRUCT *  sbc_io);

/*@}*/

/**
 *  @name SBC Encoder APIs - Calculate SBC frame length
 */

/*@{*/

/**
 *  \brief This routine is used to calculate the SBC frame length.
 *
 *  \par Description:
 *  This API enables applications/profiles to calculate the SBC frame
 *  length.
 *
 *  \param [in] params
 *         The SBC configuration parameters.
 *
 *  \param [out] frame_length
 *         Calculated SBC frame length.
 *
 *  \note
 *   \li Application should populate SBC_CODEC_PARAM data type with
 *       valid configuration parameters before calling this API.
 */
void sbc_calculate_frame_length
     (
         /* IN */   SBC_CODEC_PARAM *  params,
         /* OUT */  UINT16 *           frame_length
     );


/**
 *  \brief This routine is used to calculate the SBC bitrate.
 *
 *  \par Description:
 *  This API enables applications/profiles to calculate the SBC bitrate.
 *
 *  \param [in] params
 *         The SBC configuration parameters.
 *
 *  \param [out] bitrate
 *         Calculated SBC bitrate in kbps.
 *
 *  \note
 *   \li Application should populate SBC_CODEC_PARAM data type with
 *       valid configuration parameters before calling this API.
 *
 *   \li The bitrate will be in kbps units.
 *
 *   \li The application should fill the following information on the
 *       SBC_CODEC_PARAM data type.
 *           - Bitpool
 *           - Channel Mode
 *           - Number of Subbands
 *           - Number of Blocks
 *           - Sampling Frequency
 *           - Number of Channels
 *
 *       Please see the description of
 *       SBC_CODEC_PARAM data type to know how to fill the above elements.
 */
void sbc_calculate_bitrate
     (
         /* IN */   SBC_CODEC_PARAM * params,
         /* OUT */  UINT16 *          bitrate
     );


/**
 *  \brief This routine is used to calculate the SBC bitpool.
 *
 *  \par Description:
 *  This API enables applications/profiles to calculate the SBC bitpool.
 *
 *  \param [in] params
 *         The SBC configuration parameters.
 *
 *  \param [in] bitrate
 *         The SBC bitrate in kbps.
 *
 *  \param [out] bitpool
 *         Calculated SBC bitpool.
 *
 *  \note
 *   \li Application should populate SBC_CODEC_PARAM data type with
 *       valid configuration parameters before calling this API.
 *
 *   \li The bitrate must be specfified in kbps units.
 *
 *   \li The application should fill the following information on the
 *       SBC_CODEC_PARAM data type.
 *           - Channel Mode
 *           - Number of Subbands
 *           - Number of Blocks
 *           - Sampling Frequency
 *           - Number of Channels
 *
 *       Please see the description of
 *       SBC_CODEC_PARAM data type to know how to fill the above elements.
 */
void sbc_calculate_bitpool
     (
         /* IN */   SBC_CODEC_PARAM * params,
         /* IN */   UINT16            bitrate,
         /* OUT */  UINT16 *          bitpool
     );

/*@}*/

/**
 *  @name SBC Decoder APIs - Decoding of SBC bitstream
 */

/*@{*/

/**
 *  \brief To decode SBC bitstream.
 *
 *  \par Description:
 *  This API enables applications/profiles to decode SBC bitstream.
 *  This API decodes the SBC bitstream in to PCM audio bitstream.
 *
 *  \param [in,out] sbc_io
 *         SBC IO parameters.
 *
 *  \return API_RESULT: API_SUCCESS, or an Error Code describing the
 *                      reason for failure.
 *  \note
 *  \li Application should allocate sufficient output buffer and
 *      provide it to SBC decoder. If allocated_buflen is not
 *      sufficient SBC decoder will return error, and output_buflen
 *      will be zero.
 *
 *  \li Application should provide complete SBC frame for decoding.
 *
 *  \li The application should fill the following information on the
 *      SBC_DECODER_IO_T data type.
 *           - Input Buffer
 *           - Input buffer Length
 *           - Output buffer
 *           - Output buffer length
 *      Rest of the information in the SBC_DECODER_IO_T is used by SBC
 *      decoder internally and should not be modified/accessed by application.
 */
API_RESULT sbc_decoder
           (
               /* INOUT */  SBC_DECODER_IO_STRUCT* sbc_io
           );

/**
 *  \brief To extract the SBC frame header information.
 *
 *  \par Description:
 *  This API enables parsing the SBC frame header to find the 
 *  SYNCWORD, sampling frequecy, block size, channel mode,
 *  allocation method, number of subbands, bitpool, CRC-8 recieved.
 *
 *  \param [out] decoder_join
 *         Indicates if it is joint-stereo.
 *
 *  \param [out] params
 *         SBC decoder configuration parameters.
 *
 *  \param [in] encoded_data
 *         Encoded bitstream recieved.
 *
 *  \return End byte till where the SBC header is stored.
 *
 *  \note
 *  \li Since SBC supports variable bitrate, application should extract
 *      frame header and calculate the input frame length using
 *      sbc_calculate_frame_length(). The calculated frame length
 *      to be provided to the SBC decoder while calling sbc_decoder().
 */
UINT16 sbc_decoder_extract_sbc_header
       (
           /* OUT */   UINT32          * decoder_join,
           /* INOUT */ SBC_CODEC_PARAM * params,
           /* IN */    UCHAR           * encoded_data
       );

/*@}*/


/**
 *  @name SBC Decoder APIs - Initialize SBC Decoder
 */

/*@{*/

/**
 *  \brief To initialize the SBC decoder.
 *
 *  \par Description:
 *  This API enables applications/profiles to initialize the SBC decoder.
 *
 *  \param [out] sbc_io
 *         SBC IO parameters.
 *
 *  \return None
 *
 */
void sbc_decoder_init ( /* OUT */ SBC_DECODER_IO_STRUCT* sbc_io);

/*@}*/

#ifdef __cplusplus
};
#endif

#endif /* _H_SBC_API_ */

