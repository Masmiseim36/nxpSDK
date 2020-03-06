/*******************************************************************************
* Copyright (c) 2015-2019 Cadence Design Systems, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and 
* not with any other processors and platforms, subject to
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

******************************************************************************/
#ifndef __XA_OPUS_DEC_DEFINITIONS_H__
#define __XA_OPUS_DEC_DEFINITIONS_H__

/* commands */
#include "xa_apicmd_standards.h"

/* error codes */
#include "xa_error_standards.h"

/* ...component identifier (informative) */
#include "xa_opus_codec_api.h"

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/
enum xa_config_param_opus_dec {
  XA_OPUS_DEC_CONFIG_PARAM_CHANNELS 	= 0,
  XA_OPUS_DEC_CONFIG_PARAM_SAMPLE_RATE  = 1,
  XA_OPUS_DEC_CONFIG_PARAM_PCM_WIDTH    = 2,
  XA_OPUS_DEC_CONFIG_PARAM_SILK_INBANDFEC_ENABLE = 3,
  XA_OPUS_DEC_CONFIG_PARAM_GAIN = 4,
  XA_OPUS_DEC_CONFIG_PARAM_LOST_FLAG = 5,
  XA_OPUS_DEC_CONFIG_PARAM_NO_RANGE_DEC_STATE = 6,
  XA_OPUS_DEC_CONFIG_PARAM_VERSION = 7,
  XA_OPUS_DEC_CONFIG_PARAM_NB_STREAMS = 8,
  XA_OPUS_DEC_CONFIG_PARAM_NB_COUPLED = 9,
  XA_OPUS_DEC_CONFIG_PARAM_CHANNEL_MAPPING = 10,
  XA_OPUS_DEC_CONFIG_PARAM_STREAM_MAP = 11,
  XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE	= 12,
};

#define XA_CODEC_OPUS_DEC                  0x12

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/
/* Non Fatal Errors */
//enum xa_error_nonfatal_api_opus_dec {};

/* Fatal Errors */
//enum xa_error_fatal_api_opus_dec {};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/
/* Non Fatal Errors */
//enum xa_error_nonfatal_config_opus_dec {};

/* Fatal Errors */
//enum xa_error_fatal_config_opus_dec {};
/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_execute_opus_dec {
    XA_OPUS_DEC_EXEC_NONFATAL_INPUT    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS_DEC, 0),
};

/* Fatal Errors */
//enum xa_error_fatal_execute_opus_dec {};

#endif /* __XA_OPUS_DEC_DEFINITIONS_H__ */
