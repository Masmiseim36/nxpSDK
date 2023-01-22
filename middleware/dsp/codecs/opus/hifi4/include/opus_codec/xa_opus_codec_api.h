/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */



#ifndef __XA_OPUS_CODEC_API_H__
#define __XA_OPUS_CODEC_API_H__

#include "xa_type_def.h"
#include "xa_error_standards.h"

/* OPUS constants */
#define XA_OPUS_MAX_NUM_STREAMS                     8     /* Max. number of streams for multistream opus packet */
#define XA_OPUS_MAX_NUM_CHANNELS                    8     /* Max. number of channels */

#define XA_OPUS_MAX_FRAMES_PER_PACKET               6     /* Max. number of 20ms frames in a packet in CELT mode */
#define XA_OPUS_MAX_SAMPLES_PER_FRAME_CELT          960  /* Max. number of samples in a frame in CELT mode */

/* According to Opus standard, one packet can contain data worth 120 ms at max */
/* Max. number of bytes in one channel of a packet, 20*6 = 120 ms of samples, samples are of type WORD16 */
#define XA_OPUS_MAX_BYTES_CHANNEL_PACKET            (XA_OPUS_MAX_SAMPLES_PER_FRAME_CELT * sizeof(WORD16) * XA_OPUS_MAX_FRAMES_PER_PACKET)



/* Under normal conditions, one complete packet is required in the input buffer. 
 * In case of lost packet, the decoderDecoder extracts the FEC data from the next 
 * packet and processes it. Thus, in case of packet loss, the application should 
 * put 2 packets in the input buffer in order for FEC to operate correctly. 
*/

/* Latency in packets in case of data lost */
#define XA_OPUS_MAX_FEC_DELAY                       1

/* Number of packets in input buffer in case of packets lost */
#define XA_OPUS_MAX_PACKET_IN_INP_BUF               (XA_OPUS_MAX_FEC_DELAY + 1)

/* Max. number of bytes in one encoded packet / max. number of bytes in input buffer if no packets lost */
#define XA_OPUS_MAX_BYTES_ENCODED_PACKET            1504

/* Max. number of data bytes in input buffer in case of packet loss */
#define XA_OPUS_MAX_DEC_INP_BYTES                   (XA_OPUS_MAX_BYTES_ENCODED_PACKET*XA_OPUS_MAX_PACKET_IN_INP_BUF)


/* Setting for most VoIP/videoconference applications where listening quality and intelligibility matter most */
#define XA_OPUS_APPLICATION_VOIP                    2048
/* Setting for broadcast/high-fidelity application where the decoded audio should be as close as possible to the input */
#define XA_OPUS_APPLICATION_AUDIO                   2049
/* Setting only use when lowest-achievable latency is what matters most. Voice-optimized modes cannot be used */
#define XA_OPUS_APPLICATION_RESTRICTED_LOWDELAY     2051

#define XA_OPUS_AUTO                                0      /* Auto/default setting */

#define XA_OPUS_BANDWIDTH_NARROWBAND                1101   /*  4 kHz bandwidth */
#define XA_OPUS_BANDWIDTH_MEDIUMBAND                1102   /*  6 kHz bandwidth */
#define XA_OPUS_BANDWIDTH_WIDEBAND                  1103   /*  8 kHz bandwidth */
#define XA_OPUS_BANDWIDTH_SUPERWIDEBAND             1104   /* 12 kHz bandwidth */
#define XA_OPUS_BANDWIDTH_FULLBAND                  1105   /* 20 kHz bandwidth */

#define XA_OPUS_MODE_SILK_ONLY                      1000   /* SILK mode   */
#define XA_OPUS_MODE_HYBRID                         1001   /* Hybrid mode */
#define XA_OPUS_MODE_CELT_ONLY                      1002   /* CELT mode   */

#define XA_OPUS_SIGNAL_VOICE                        3001   /* Signal being encoded is voice */
#define XA_OPUS_SIGNAL_MUSIC                        3002   /* Signal being encoded is music */

/***********************************************/
/* Structure for controlling encoder operation */
/***********************************************/
typedef struct 
{
    /* I:   Application coding mode; 
             - XA_OPUS_APPLICATION_VOIP 
             - XA_OPUS_APPLICATION_AUDIO 
             - XA_OPUS_APPLICATION_RESTRICTED_LOWDELAY                                      */
    WORD32 application;

    /* I:   Input signal sampling rate in Hertz; 8000/12000/16000/24000/48000               */
    WORD32 API_sampleRate;

    /* I:   Number of channels of output signal; 1/2                                        */
    WORD32 API_numChannels;

    /* I:   Target bitrate; (0-510000)                                                      */
    WORD32 bitRate;                        

    /* I:   Flag to enable constant bitrate; 0/1                                            */
    WORD32 cbr;                        

    /* I:   Flag to enable constrained variable bitrate; 0/1                                */
    WORD32 cvbr;                        

    /* I:   Audio bandwidth (from narrowband to fullband);
             - XA_OPUS_AUTO(0)
             - XA_OPUS_BANDWIDTH_NARROWBAND
             - XA_OPUS_BANDWIDTH_MEDIUMBAND
             - XA_OPUS_BANDWIDTH_WIDEBAND
             - XA_OPUS_BANDWIDTH_SUPERWIDEBAND
             - XA_OPUS_BANDWIDTH_FULLBAND                                                   */
    WORD32 bandwidth;                        

    /* I:   Audio bandwidth (from narrowband to fullband);
             - XA_OPUS_AUTO(0)
             - XA_OPUS_BANDWIDTH_NARROWBAND
             - XA_OPUS_BANDWIDTH_MEDIUMBAND
             - XA_OPUS_BANDWIDTH_WIDEBAND
             - XA_OPUS_BANDWIDTH_SUPERWIDEBAND
             - XA_OPUS_BANDWIDTH_FULLBAND                                                   */
    WORD32 max_bandwidth;                        

    /* I:   max payload size in bytes; [XA_OPUS_AUTO(0), XA_OPUS_MAX_BYTES_PER_PACKET] range  */
    WORD32 max_payload;

    /* I:   Complexity mode; (0-10), 0 is lowest, 5 is medium and 10 is highest complexity  */
    WORD32 complexity;

    /* I:   Flag to enable SILK inband FEC;  0/1                                            */
    WORD32 SILK_InBandFEC_enabled;

    /* I:   Configures mono/stereo forcing in the encoder; 0/1/2 (0 - forcing disabled)     */
    WORD32 force_numChannels;

    /* I:   Flag to enable discontinuous transmission (DTX); 0/1                            */
    WORD32 SILK_DTX_enabled;

    /* I:   Uplink packet loss in percent (0-100)                                           */
    WORD32 packetLossPercentage;

    /* I:   Force encode mode;
             - XA_OPUS_AUTO(0)
             - XA_OPUS_MODE_SILK_ONLY
             - XA_OPUS_MODE_HYBRID
             - XA_OPUS_MODE_CELT_ONLY                                                       */
    WORD32 force_mode;

    /* I:   Configures the type of signal being encoded; 
             - XA_OPUS_AUTO(0)
             - XA_OPUS_SIGNAL_VOICE
             - XA_OPUS_SIGNAL_MUSIC                                                         */
	WORD32 signal_type;

    /* I/O:   Resets the encoder state to be equivalent to a freshly initialized state; 0/1  
    Note: Non-zero values are treated as 1                                                  */
	WORD32 reset_state;

    /* Won't have any effect, processing related to this is not applicable in ref code used, 
    (fixed point opus-1.2.1 ref with DISABLE_FLOAT_API), keeping it for backward compatibility */
    int variable_duration;

} xa_opus_enc_control_t;

/**************************************************************************/
/* Structure for controlling decoder operation and reading decoder status */
/**************************************************************************/
typedef struct 
{
    /* I:   Output signal sampling rate in Hertz; 8000/12000/16000/24000/48000              */
    WORD32 API_sampleRate;

    /* I:   Number of channels of output signal; 
    for channel_mapping 0, 1/2
    for channel_mapping 1, 1-XA_OPUS_MAX_NUM_CHANNELS                                       */
    WORD32 API_numChannels;
	
	/* I:   Flag to enable SILK inband FEC;                                                 */
    WORD32 SILK_InBandFEC_enabled;

    /* I:   Decoder gain adjustment, [-32768, 32767] range, default:0;                      */
    WORD32 gain;

    /* I:   0: no loss, 1 loss                                                              */
    WORD32 lostFlag;       
	
    /* O:   Frames per packet 1, 2, 3, 4, 5, 6                                              */
    WORD32 framesPerPacket;

    /* O:   Flag to indicate that the decoder has remaining payloads internally             */
    WORD32 moreInternalDecoderFrames;

    /* Not in use, won't have any effect on decoding, may be supported in future releases   */
	WORD32 reset_state;

    /* I: 'Range decoder state' conformance check variable control (Ref: RFC6716 - Section 6) 
          1: Excluded 'Range decoder state' variable in input payload 
          0: Included 'Range decoder state' variable in input payload                       */
    WORD32 no_range_dec_state;

    /* Following elements are added to aid Ogg support                                      */

    /* I: Bitstream version from Opus Header                                                */
    WORD32 version;

    /* I: Total number of streams; 1-XA_OPUS_MAX_NUM_STREAMS                                */
    WORD32 nb_streams;

    /* I: Total number of coupled streams; 0-(nb_streams-1)                                 */
    WORD32 nb_coupled;

    /* I: Channel mapping family; 0/1                                                       */
    WORD32 channel_mapping;

    /* I: Output channel map; Array of XA_OPUS_MAX_NUM_CHANNELS unsigned chars with range
    0 to (nb_streams+nb_coupled-1) : route corresponding stream
    255                            : output silence (Ref: RFC7845 - Section 5.1.1)          */
    UWORD8 stream_map[XA_OPUS_MAX_NUM_CHANNELS];

} xa_opus_dec_control_t;


#define XA_CODEC_OPUS  1

/* Opus specific error codes */

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_opus_codec 
{
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_FORCE_CH_SETTING            = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 0),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_MAX_BANDWIDTH_SETTING       = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 1),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_BANDWIDTH_SETTING           = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 2),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_COMPLEXITY_SETTING          = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 3),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_PACKET_LOSS_PERC_SETTING    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 4),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_FORCE_MODE_SETTING          = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 5),
    XA_OPUS_CONFIG_NONFATAL_ENC_INVALID_SIGNAL_TYPE_SETTING         = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 6),
    XA_OPUS_CONFIG_NONFATAL_DEC_INVALID_GAIN_SETTING                = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 7),
    XA_OPUS_CONFIG_NONFATAL_DEC_STREAM_MAP_IGNORED                  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS, 8),
};

/* Fatal Errors */
enum xa_error_fatal_config_opus_codec 
{
    XA_OPUS_CONFIG_FATAL_SAMP_FREQ_NOT_SUPPORTED                  = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 0),
    XA_OPUS_CONFIG_FATAL_NUM_CHANNEL_NOT_SUPPORTED                = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 1),
    XA_OPUS_CONFIG_FATAL_ENC_INVALID_APP_SETTING                  = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 2),
    XA_OPUS_CONFIG_FATAL_ENC_INVALID_BITRATE_SETTING              = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 3),
    XA_OPUS_CONFIG_FATAL_INVALID_PARAM_COMBINATION                = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 4),
    XA_OPUS_CONFIG_FATAL_DEC_NUM_STREAMS_NOT_SUPPORTED            = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 5),
    XA_OPUS_CONFIG_FATAL_DEC_OPUS_BITSTREAM_VERSION_NOT_SUPPORTED = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 6),
    XA_OPUS_CONFIG_FATAL_DEC_UNKNOWN_CHANNEL_MAPPING              = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 7),
    XA_OPUS_CONFIG_FATAL_DEC_BAD_STREAM_MAP                       = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_OPUS, 8),
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_opus_codec 
{
    XA_OPUS_EXECUTE_NONFATAL_BAD_ARG                      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS, 0),
    XA_OPUS_EXECUTE_NONFATAL_INSUFFICIENT_DATA            = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS, 1),
    XA_OPUS_EXECUTE_NONFATAL_INTERNAL_ERROR               = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS, 2),
    XA_OPUS_EXECUTE_NONFATAL_INVALID_PACKET               = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS, 3),
    XA_OPUS_EXECUTE_NONFATAL_INVALID_FORCED_MODE_SETTINGS = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS, 4),
};

/* Fatal Errors */
enum xa_error_fatal_execute_opus_codec 
{
    XA_OPUS_EXECUTE_FATAL_INIT_FAILURE                 = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 0),
    XA_OPUS_EXECUTE_FATAL_NOT_INITIALIZED              = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 1),
    XA_OPUS_EXECUTE_FATAL_PARAMS_CHANGED               = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 2),
    XA_OPUS_EXECUTE_FATAL_ENC_OUT_BUF_TOO_SHORT        = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 3),
    XA_OPUS_EXECUTE_FATAL_DEC_PAYLOAD_ERROR            = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 4),
    XA_OPUS_EXECUTE_FATAL_ENC_INVALID_NUM_INP_SAMPLES  = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_OPUS, 5),
};


#if defined(__cplusplus)
extern "C" {
#endif    /* __cplusplus */


/******************************************************************************
 *         Encoder function                                                   *
 ******************************************************************************/
XA_ERRORCODE xa_opus_enc_init 
( 
    xa_codec_handle_t handle,                           /* I/O: State handle                            */
    xa_opus_enc_control_t *enc_control                  /* I/O: Control Structure                       */
);

XA_ERRORCODE xa_opus_enc
(    
    xa_codec_handle_t       handle,                     /* I/O: State handle                            */
    pWORD16                 inp_speech,                 /* I:   Input speech samples                    */
    pUWORD8                 enc_speech,                 /* O:   Encoded output speech bytes             */
    WORD16                  inp_samples,                /* I:   Number of input samples                   */
    xa_opus_enc_control_t  *enc_control,                /* I/O: Control Structure                       */
    WORD16                 *out_bytes,                  /* I/O: Number of samples (vector/decoded)      */
    pVOID                   scratch                     /* I/O: Scratch memory                          */
);

WORD32 xa_opus_enc_get_handle_byte_size( WORD32 numChannels );
WORD32 xa_opus_enc_get_scratch_byte_size();

/******************************************************************************
 *         Decoder function                                                   *
 *****************************************************************************/

XA_ERRORCODE xa_opus_dec_init 
( 
    xa_codec_handle_t handle,                           /* I/O: State handle                            */
	xa_opus_dec_control_t *dec_control                  /* I/O: Control Structure                       */
);

XA_ERRORCODE xa_opus_dec 
(
    xa_codec_handle_t       handle,                     /* I/O: State handle                            */
    pUWORD8                 enc_speech,                 /* I:   Encoded input speech bytes              */
    pWORD16                 synth_speech,               /* O:   Decoded output speech samples           */
    const WORD32            num_bytes_in,               /* I:   Number of input bytes                   */
    xa_opus_dec_control_t  *dec_control,                /* I/O: Control Structure                       */
    WORD16                 *num_samples_out,            /* I/O: Number of samples (vector/decoded)      */
    pVOID                   scratch                     /* I/O: Scratch memory                          */
);

WORD32 xa_opus_dec_get_handle_byte_size( WORD32 nb_streams, WORD32 nb_coupled );
WORD32 xa_opus_dec_get_scratch_byte_size( WORD32 channel_mapping );

/****************************************************************
 * Common API calls
 ****************************************************************/
const char * xa_opus_get_lib_name_string();
const char * xa_opus_get_lib_version_string();
const char * xa_opus_get_lib_api_version_string();

#if defined(__cplusplus)
}
#endif    /* __cplusplus */

#endif /* __XA_OPUS_CODEC_API_H__ */
