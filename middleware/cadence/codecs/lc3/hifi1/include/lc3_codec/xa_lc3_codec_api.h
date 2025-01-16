/*
 * Copyright (c) 2019-2023 Cadence Design Systems, Inc.
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

#ifndef __XA_LC3_CODEC_API_H__
#define __XA_LC3_CODEC_API_H__

#include "xa_error_standards.h"
#include "xa_type_def.h"

/*Maximum number of supported channels.*/
#define XA_LC3_MAX_CHANNELS     2

/*No channels define.*/
#define XA_LC3_NO_CHANNELS      0

/*One channel define.*/
#define XA_LC3_ONE_CHANNEL      1

/*Two channels define.*/
#define XA_LC3_TWO_CHANNELS      2

/*Maximum number of samples per channel that can be stored in one LC3 frame*/
#define XA_LC3_MAX_SAMPLES      480

/* Maximum number of bytes of one LC3 frame. */
#define XA_LC3_MAX_BYTES        870


/* Bits per sample values */
#define XA_LC3_BIPS_VAL_16      16
#define XA_LC3_BIPS_VAL_24      24
#define XA_LC3_BIPS_VAL_32      32

/* Sample rates supported values */
#define XA_LC3_SAMPLE_RATE_8KHZ    8000
#define XA_LC3_SAMPLE_RATE_16KHZ   16000
#define XA_LC3_SAMPLE_RATE_24KHZ   24000
#define XA_LC3_SAMPLE_RATE_32KHZ   32000
#define XA_LC3_SAMPLE_RATE_44_1KHZ 44100
#define XA_LC3_SAMPLE_RATE_48KHZ   48000

/* Default bits per sample value for decoder output*/
#define XA_LC3_BIPS_OUT_DEF_VAL XA_LC3_BIPS_VAL_16

/* Default bits per sample value for encoder input*/
#define XA_LC3_BIPS_IN_DEF_VAL XA_LC3_BIPS_VAL_16


/* Default Sample Rate value*/
#define XA_LC3_SAMPLE_RATE_DEF_VAL XA_LC3_SAMPLE_RATE_8KHZ

/* Default number of channel value*/
#define XA_LC3_NUM_OF_CH_DEF_VAL XA_LC3_ONE_CHANNEL

/* Default bitrate value*/
#define XA_LC3_BITRATE_DEF_VAL 32000

#define XA_CODEC_LC3 6

/* Maximum number of scratch bytes for Decoder. */
#define XA_LC3_DEC_MAX_USER_SYSTEM_SCRATCH_SIZE 28498

/* Maximum number of scratch bytes for Decoder. */
#define XA_LC3_ENC_MAX_USER_SYSTEM_SCRATCH_SIZE 40000

/*

Commands for encoder:

XA_LC3_ENC_PARAM_FRAME_LENGTH   - get frame length from encoder
XA_LC3_ENC_PARAM_BITRATE        - set encoder bit rate
XA_LC3_ENC_PARAM_REAL_BITRATE   - get real bit rate from encoder
*/
typedef enum
{
    XA_LC3_ENC_PARAM_FRAME_LENGTH = 0,
    XA_LC3_ENC_PARAM_BITRATE = 1, 
    XA_LC3_ENC_PARAM_REAL_BITRATE = 2, 
    XA_LC3_ENC_PARAM_FRAME_DMS = 3, 
    XA_LC3_ENC_PARAM_DELAY_COMPENSATION = 4 

}xa_lc3_enc_param_id_t;

/*

Commands for decoder:
 
XA_LC3_DEC_PARAM_FRAME_LENGTH   - get frame length from decoder

*/
typedef enum
{
    XA_LC3_DEC_PARAM_FRAME_LENGTH = 0,
    XA_LC3_DEC_PARAM_FRAME_DMS = 1, 
    XA_LC3_DEC_PARAM_DELAY_COMPENSATION = 2 

}xa_lc3_dec_param_id_t;


/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_lc3_codec
{
    XA_LC3_CONFIG_NONFATAL_INVALID_PARAM = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_LC3, 0),
    XA_LC3_CONFIG_NONFATAL_DEC_PLC_MODE_NOT_SUPPORTED =    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_LC3, 1)
};

/* Fatal Errors */
enum xa_error_fatal_config_lc3_codec
{
    XA_LC3_CONFIG_FATAL_SAMP_FREQ_NOT_SUPPORTED =       XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_LC3, 0),
    XA_LC3_CONFIG_FATAL_NUM_CHANNEL_NOT_SUPPORTED =     XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_LC3, 1),
    XA_LC3_CONFIG_FATAL_ENC_BITRATE_NOT_SUPPORTED =     XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_LC3, 2),
    XA_LC3_CONFIG_FATAL_BITDEPTH_NOT_SUPPORTED =        XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_LC3, 3),
    XA_LC3_FRAMEMS_NOT_SUPPORTED =                      XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_LC3, 4)
};

/*****************************************************************************/
/* Class 2: Execution Class Errors                                           */
/*****************************************************************************/
/* Nonfatal Errors */ 
enum xa_error_nonfatal_execute_lc3_codec
{
    XA_LC3_EXECUTE_NONFATAL_DECODE_ERROR = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_LC3, 0)
};

/* Fatal Errors */
enum xa_error_fatal_execute_lc3_codec
{
    XA_LC3_EXECUTE_FATAL_DEC_NUMBYTES_ERROR = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_LC3, 0),
    XA_LC3_EXECUTE_FATAL_NOT_INITIALIZED = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_LC3, 1)
};

/***********************************************/
/* Structure for controlling encoder operation */
/***********************************************/
typedef struct
{
    /* I:   Output signal sampling rate, Hertz; 8000/16000/24000/32000/44100/48000 */
    UWORD32 sampleRate;

    /* I:   Number of channels of output signal; 1/2 */
    WORD16 nChannels;

    /* I:   Target bitrate, kbps; (16-320) */
    WORD32 bitrate;

    /* I:   Bits per sample of input signal; 16/24 */
    WORD16 bipsIn;

} xa_lc3_enc_init_cfg_t;

/***********************************************/
/* Structure for controlling decoder operation */
/***********************************************/
typedef struct
{
    /* I:   Output signal sampling rate in Hertz; 8000/16000/24000/32000/44100/48000 */
    UWORD32 sampleRate;

    /* I:   Number of channels of output signal; 1/2 */
    WORD16 nChannels;

    /* I:   Bits per sample of output signal; 16/24 */
    WORD32 bipsOut;

} xa_lc3_dec_init_cfg_t;


/****************************************************************
* Common API calls
****************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

    /****************************************************************************
    *         Common API calls                                                  *
    *****************************************************************************/
    const char * xa_lc3_get_lib_name_string();
    const char * xa_lc3_get_lib_version_string();
    const char * xa_lc3_get_lib_api_version_string();

    /****************************************************************************
    *         Encoder function                                                  *
    *****************************************************************************/
    WORD32 xa_lc3_enc_get_handle_byte_size(
        xa_lc3_enc_init_cfg_t *p_cfg    /* I: Initial configuration parameters.*/
    );
    
    WORD32 xa_lc3_enc_get_scratch_byte_size(
        xa_codec_handle_t handle    /* I: State handle.*/
    );
    
    XA_ERRORCODE xa_lc3_enc_init(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        pWORD32 scratch,                /* I: Scratch memory. */
        xa_lc3_enc_init_cfg_t *p_cfg,    /* I: Initial configuration parameters. */
        WORD16 frame_dms                /* I: frame_size in DMS. */
    );

    XA_ERRORCODE xa_lc3_enc_process(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        pVOID p_in_data,                /* I: Input buffer. */
        pVOID p_out_data,               /* O: Output buffer. */
        pWORD32 scratch,                  /* I: Scratch buffer. */
        pUWORD32 p_in_samples,          /* I/O: Number of input samples. */
        pUWORD32 p_out_bytes,            /* I/O: Number of output bytes. */
        WORD32 scratch_size
    );

    XA_ERRORCODE xa_lc3_enc_get_param(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        xa_lc3_enc_param_id_t param_id, /* I: Parameter to be read. */
        pVOID p_param_value             /* O: Parameter value. */
    );

    XA_ERRORCODE xa_lc3_enc_set_param(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        xa_lc3_enc_param_id_t param_id, /* I: Parameter to be written. */
        pVOID p_param_value             /* I: Parameter value. */
    );

    /*****************************************************************************
    *         Decoder function                                                   *
    *****************************************************************************/
    WORD32 xa_lc3_dec_get_handle_byte_size(
        xa_lc3_dec_init_cfg_t *p_cfg    /* I: Initial configuration parameters.*/
    );

    WORD32 xa_lc3_dec_get_scratch_byte_size(
        xa_codec_handle_t handle    /* I/O: State handle*/
    );

    XA_ERRORCODE xa_lc3_dec_init(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        pWORD32 scratch,                /* I: Scratch memory. */
        xa_lc3_dec_init_cfg_t *p_cfg,    /* I: Initial configuration parameters. */
        WORD32 out_samples,             /* I: Number of Samples */
        WORD16 frame_dms);              /* I: Frame size in dms */

    XA_ERRORCODE xa_lc3_dec_process(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        pVOID p_in_data,                /* I: Input buffer. */
        pVOID p_out_data,               /* O: Output buffer. */
        pWORD32 scratch,               /*  I: Scratch buffer. */
        pUWORD32 p_in_bytes,            /* I/O: Number of input bytes. */
        pUWORD32 p_out_samples,          /* I/O: Number of output samples. */
        WORD32 scratch_size,
        WORD32 bfi_ext
    );

    XA_ERRORCODE xa_lc3_dec_get_param(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        xa_lc3_dec_param_id_t param_id, /* I: Parameter to be read. */
        pVOID p_param_value             /* O: Parameter value. */
    );
    
    XA_ERRORCODE xa_lc3_dec_set_param(
        xa_codec_handle_t handle,       /* I/O: State handle. */
        xa_lc3_dec_param_id_t param_id, /* I: Parameter to be written. */
        pVOID p_param_value             /* I: Parameter value. */
    );

#if defined(__cplusplus)
}
#endif


#endif /* __XA_LC3_CODEC_API_H__ */
