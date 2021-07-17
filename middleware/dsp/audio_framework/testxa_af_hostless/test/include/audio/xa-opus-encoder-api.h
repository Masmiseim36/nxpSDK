/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
#ifndef __XA_OPUS_ENC_DEFINITIONS_H__
#define __XA_OPUS_ENC_DEFINITIONS_H__

/* commands */
#include "xa_apicmd_standards.h"

/* error codes */
#include "xa_error_standards.h"

/* ...component identifier (informative) */
#include "xa_opus_codec_api.h"

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/
enum xa_config_param_opus_enc {
  XA_OPUS_ENC_CONFIG_PARAM_CHANNELS 	= 0,
  XA_OPUS_ENC_CONFIG_PARAM_SAMPLE_RATE  = 1,
  XA_OPUS_ENC_CONFIG_PARAM_PCM_WIDTH    = 2,

  XA_OPUS_ENC_CONFIG_PARAM_APPLICATION	= 3,
  XA_OPUS_ENC_CONFIG_PARAM_BITRATE		= 4,
  XA_OPUS_ENC_CONFIG_PARAM_CBR			= 5,
  XA_OPUS_ENC_CONFIG_PARAM_CVBR			= 6,

  XA_OPUS_ENC_CONFIG_PARAM_BW			= 7,
  XA_OPUS_ENC_CONFIG_PARAM_MAX_BW		= 8,
  XA_OPUS_ENC_CONFIG_PARAM_MAX_PAYLOAD	= 9,
  XA_OPUS_ENC_CONFIG_PARAM_COMPLEXITY	= 10,
  XA_OPUS_ENC_CONFIG_PARAM_SILK_INBANDFEC_ENABLE	= 11,
  XA_OPUS_ENC_CONFIG_PARAM_FORCE_NUMCHANNELS		= 12,
  XA_OPUS_ENC_CONFIG_PARAM_SILK_DTX_ENABLE			= 13,
  XA_OPUS_ENC_CONFIG_PARAM_PACKET_LOSS_PERCENTAGE	= 14,
  XA_OPUS_ENC_CONFIG_PARAM_FORCE_MODE	= 15,
  XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE	= 16, /* Deprecated, use XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES instead. */
  XA_OPUS_ENC_CONFIG_PARAM_MAX_FRAMES_PER_PACKET    = 17,
  XA_OPUS_ENC_CONFIG_PARAM_SIGNAL_TYPE  = 18,
  XA_OPUS_ENC_CONFIG_PARAM_RESET_STATE  = 19,
  XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES	= 20,
};

#define XA_CODEC_OPUS_ENC                  0x11

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/
/* Non Fatal Errors */
//enum xa_error_nonfatal_api_opus_enc {};

/* Fatal Errors */
//enum xa_error_fatal_api_opus_enc {};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_config_opus_enc {
    XA_OPUS_ENC_CONFIG_NONFATAL_RANGE  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS_ENC, 0),

};

/* Fatal Errors */
//enum xa_error_fatal_config_opus_enc {};
/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_execute_opus_enc {
    XA_OPUS_ENC_EXEC_NONFATAL_INPUT    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS_ENC, 0),
};

/* Fatal Errors */
//enum xa_error_fatal_execute_opus_enc {};

#endif /* __XA_OPUS_ENC_DEFINITIONS_H__ */
