/*
 * Copyright (c) 2009-2020 Cadence Design Systems, Inc.
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


#ifndef __XA_SBC_ENC_API_H__
#define __XA_SBC_ENC_API_H__

/*****************************************************************************/
/* SBC Encoder specific API definitions                                      */
/*****************************************************************************/

/* sbc_enc-specific channel modes */
enum xa_sbc_enc_chmode {
  XA_SBC_ENC_CHMODE_MONO =   0,
  XA_SBC_ENC_CHMODE_DUAL =   1,
  XA_SBC_ENC_CHMODE_STEREO = 2,
  XA_SBC_ENC_CHMODE_JOINT =  3
};

/* sbc_enc-specific configuration parameters */
enum xa_config_param_sbc_enc {
  XA_SBC_ENC_CONFIG_PARAM_SAMP_FREQ    = 0,
  XA_SBC_ENC_CONFIG_PARAM_SUBBANDS     = 1,
  XA_SBC_ENC_CONFIG_PARAM_BLOCKS       = 2,
  XA_SBC_ENC_CONFIG_PARAM_CHMODE       = 3,
  XA_SBC_ENC_CONFIG_PARAM_SNR          = 4,
  XA_SBC_ENC_CONFIG_PARAM_BITRATE      = 5,
  XA_SBC_ENC_CONFIG_PARAM_BITPOOL      = 6
};

/* commands */
#include "xa_apicmd_standards.h"

/* sbc_enc-specific commands */
/* (none) */

/* sbc_enc-specific command types */
/* (none) */

/* error codes */
#include "xa_error_standards.h"

#define XA_CODEC_SBC_ENC	19

/* sbc_enc-specific error codes */
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Fatal Errors */
enum xa_error_fatal_api_sbc_enc {
  XA_SBC_ENC_API_FATAL_INVALID_API_SEQ      = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_SBC_ENC, 4)
};

/* Non Fatal Errors */
enum xa_error_nonfatal_api_sbc_enc {
  XA_SBC_ENC_API_NONFATAL_INVALID_API_SEQ      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_SBC_ENC, 0)
};

/* Fatal Errors */
enum xa_error_fatal_config_sbc_enc {
  XA_SBC_ENC_CONFIG_FATAL_INVALID_NUM_SUBBANDS = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_SBC_ENC,  0),
  XA_SBC_ENC_CONFIG_FATAL_INVALID_NUM_BLOCKS   = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_SBC_ENC,  1),
  XA_SBC_ENC_CONFIG_FATAL_INVALID_CHMODE       = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_SBC_ENC,  2),
  XA_SBC_ENC_CONFIG_FATAL_INVALID_SAMP_FREQ    = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_SBC_ENC,  3),
  XA_SBC_ENC_CONFIG_FATAL_INVALID_BITPOOL      = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_SBC_ENC,  4)
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_sbc_enc {
  XA_SBC_ENC_EXECUTE_NONFATAL_NEED_MORE_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_ENC, 0),
};

/* Fatal Errors */
enum xa_error_fatal_execute_sbc_enc {
  XA_SBC_ENC_EXECUTE_FATAL_OUTPUT_BUFFER_OVERFLOW = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_SBC_ENC, 0),
};

#include "xa_type_def.h"

#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */
xa_codec_func_t xa_sbc_enc;
#if defined(__cplusplus)
}
#endif	/* __cplusplus */
#endif /* __XA_SBC_ENC_API_H__ */
