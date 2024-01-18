/*
 * Copyright (c) 2008-2022 Cadence Design Systems, Inc.
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


#ifndef __XA_AAC_DEC_API_H__
#define __XA_AAC_DEC_API_H__

/* aac_dec-specific configuration parameters */
enum xa_config_param_aac_dec {
  XA_AACDEC_CONFIG_PARAM_EXTERNALSAMPLINGRATE = 2,
  XA_AACDEC_CONFIG_PARAM_EXTERNALBSFORMAT     = 3,
  XA_AACDEC_CONFIG_PARAM_TO_STEREO            = 4,
  XA_AACDEC_CONFIG_PARAM_SAMP_FREQ            = 5,
  XA_AACDEC_CONFIG_PARAM_NUM_CHANNELS         = 6,
  XA_AACDEC_CONFIG_PARAM_PCM_WDSZ             = 7,
  XA_AACDEC_CONFIG_PARAM_AAC_SAMPLERATE       = 9, 
  XA_AACDEC_CONFIG_PARAM_DATA_RATE            = 10,
  XA_AACDEC_CONFIG_PARAM_OUTNCHANS            = 11,
  XA_AACDEC_CONFIG_PARAM_CHANMAP              = 14,
  XA_AACDEC_CONFIG_PARAM_ACMOD                = 15,
  XA_AACDEC_CONFIG_PARAM_AAC_FORMAT           = 16, /* This API is not implemented */
  XA_AACDEC_CONFIG_PARAM_ZERO_UNUSED_CHANS    = 17,
  XA_AACDEC_CONFIG_PARAM_DECODELAYERS         = 18,
  XA_AACDEC_CONFIG_PARAM_EXTERNALCHCONFIG     = 19,
};

/* Types of channel modes (acmod) */
typedef enum {
  XA_AACDEC_CHANNELMODE_UNDEFINED = 0,
  XA_AACDEC_CHANNELMODE_MONO,
  XA_AACDEC_CHANNELMODE_DUAL_CHANNEL=3,
  XA_AACDEC_CHANNELMODE_STEREO,
} XA_AACDEC_CHANNELMODE;

/* Types of bitstreams */
typedef enum {
  /* The bitstream type has not (yet) been successfully determined. */
  XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN = 0,
  /* Raw bsac bitstream. it needs to be signaled explicitely. */
  XA_AACDEC_EBITSTREAM_TYPE_BSAC_RAW = 7,

} XA_AACDEC_EBITSTREAM_TYPE;

/* commands */
#include "xa_apicmd_standards.h"

/* error codes */
#include "xa_error_standards.h"

#define XA_CODEC_AAC_DEC 3

/* aac_dec-specific error codes */
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_api_aac_dec {
  XA_AACDEC_API_NONFATAL_CMD_TYPE_NOT_SUPPORTED = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_AAC_DEC, 0)
};
/* Fatal Errors */
enum xa_error_fatal_api_aac_dec {
  XA_AACDEC_API_FATAL_INVALID_API_SEQ = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_AAC_DEC, 4)
};

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_aac_dec {
  XA_AACDEC_CONFIG_NONFATAL_PARAMS_NOT_SET          = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AAC_DEC, 0),
  XA_AACDEC_CONFIG_NONFATAL_DATA_RATE_NOT_SET       = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AAC_DEC, 1),
  XA_AACDEC_CONFIG_NONFATAL_INVALID_GEN_STRM_POS    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AAC_DEC, 2)
};
/* Fatal Errors */
enum xa_error_fatal_config_aac_dec {
  XA_AACDEC_CONFIG_FATAL_INVALID_EXTERNALSAMPLINGRATE = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 0),
  XA_AACDEC_CONFIG_FATAL_INVALID_EXTERNALBSFORMAT     = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 1),
  XA_AACDEC_CONFIG_FATAL_INVALID_TO_STEREO            = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 2),
  XA_AACDEC_CONFIG_FATAL_INVALID_OUTNCHANS            = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 3),
  XA_AACDEC_CONFIG_FATAL_INVALID_PCM_WDSZ             = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 4),
  XA_AACDEC_CONFIG_FATAL_INVALID_ZERO_UNUSED_CHANS    = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 5),
  /* Code For Invalid Number of input channels */	
  XA_AACDEC_CONFIG_FATAL_INVALID_EXTERNALCHCONFIG    = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 6),
  XA_AACDEC_CONFIG_FATAL_INVALID_DECODELAYERS        = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 7),
  XA_AACDEC_CONFIG_FATAL_INVALID_EXTERNALBITRATE     = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_AAC_DEC, 8)
};
/*****************************************************************************/
/* Class 2: Execution Class Errors                                           */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_aac_dec {
  XA_AACDEC_EXECUTE_NONFATAL_INSUFFICIENT_FRAME_DATA   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AAC_DEC, 0),
  XA_AACDEC_EXECUTE_NONFATAL_RUNTIME_INIT_RAMP_DOWN    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AAC_DEC, 1)
  ,XA_AACDEC_EXECUTE_NONFATAL_RAW_FRAME_PARSE_ERROR     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AAC_DEC, 2)
};

#define XA_AACDEC_EXECUTE_FATAL_PARSING_ERROR XA_AACDEC_EXECUTE_FATAL_INIT_FAILED
/* Fatal Errors */
enum xa_error_fatal_execute_aac_dec {
  XA_AACDEC_EXECUTE_FATAL_INIT_FAILED                    = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 0),
  XA_AACDEC_EXECUTE_FATAL_RAW_FRAME_PARSE_ERROR          = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 1),
  XA_AACDEC_EXECUTE_FATAL_BAD_INPUT_FAILURE              = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 2), //This error is deprecated
  XA_AACDEC_EXECUTE_FATAL_UNSUPPORTED_FORMAT             = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 3), //This error is deprecated
  XA_AACDEC_EXECUTE_FATAL_ERROR_IN_CHANROUTING           = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 4),
  XA_AACDEC_EXECUTE_FATAL_ZERO_FRAME_LENGTH              = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 5),
  XA_AACDEC_EXECUTE_FATAL_EMPTY_INPUT_BUFFER             = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AAC_DEC, 6)
};

#include "xa_type_def.h"

#if defined(USE_DLL) && defined(_WIN32)
#define DLL_SHARED __declspec(dllimport)
#elif defined (_WINDLL)
#define DLL_SHARED __declspec(dllexport)
#else
#define DLL_SHARED
#endif

#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */
xa_codec_func_t xa_bsac_dec;
#if defined(__cplusplus)
}
#endif	/* __cplusplus */

#endif /* __XA_AAC_DEC_API_H__ */
