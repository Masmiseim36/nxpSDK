/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
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
#ifndef __XA_OPUS_DEC_DEFINITIONS_H__
#define __XA_OPUS_DEC_DEFINITIONS_H__

/* commands */
#include "xa_apicmd_standards.h"

/* error codes */
#include "xa_error_standards.h"

/* ...component identifier (informative) */
#ifndef PACK_WS_DUMMY
#include "xa_opus_codec_api.h"
#endif //PACK_WS_DUMMY

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/
enum xa_config_param_opus_dec {
  XA_OPUS_DEC_CONFIG_PARAM_CHANNELS 	= 0,
  XA_OPUS_DEC_CONFIG_PARAM_SAMPLE_RATE  = 1,
  XA_OPUS_DEC_CONFIG_PARAM_PCM_WIDTH    = 2,

  XA_OPUS_DEC_CONFIG_PARAM_GAIN     = 3,
  XA_OPUS_DEC_CONFIG_PARAM_NUM_STREAMS = 4,
  XA_OPUS_DEC_CONFIG_PARAM_NUM_COUPLED_STREAMS = 5,
  XA_OPUS_DEC_CONFIG_PARAM_CHAN_MAPPING = 6,
  XA_OPUS_DEC_CONFIG_PARAM_LOST_PACKET = 7,
  XA_OPUS_DEC_CONFIG_PARAM_EXCLUDE_RANGE_DEC_STATE = 8,
  XA_OPUS_DEC_CONFIG_PARAM_BITSTREAM_VERSION = 9,
  XA_OPUS_DEC_CONFIG_PARAM_STREAM_TYPE = 10,
  XA_OPUS_DEC_CONFIG_PARAM_SILK_INBANDFEC_ENABLE	= 11,
  XA_OPUS_DEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE = 12,
  XA_OPUS_DEC_CONFIG_PARAM_STREAM_MAP = 13,
  XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE	= 16, /* Deprecated, use XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES instead. */
  XA_OPUS_DEC_CONFIG_PARAM_FRAMES_PER_PACKET    = 17,
  XA_OPUS_DEC_CONFIG_PARAM_RESET_STATE  = 19,
  XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES	= 20,
};

#define XA_CODEC_OPUS_DEC                  0x12
#define XA_OPUS_OGG_OUTPUT_BUFFER_SIZE     (3*1024)
#define XA_OPUS_OGG_INPUT_BUFFER_SIZE      (4*1024)

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
enum xa_error_nonfatal_config_opus_dec {
    XA_OPUS_DEC_CONFIG_NONFATAL_RANGE  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_OPUS_DEC, 0),

};

/* Fatal Errors */
//enum xa_error_fatal_config_opus_dec {};
/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_execute_opus_dec {
    XA_OPUS_DEC_EXEC_NONFATAL_INPUT    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_OPUS_DEC, 0),
};


typedef enum
{
    XA_RAW_OPUS_STREAM = 0,
    XA_OGG_OPUS_STREAM = 1
} xa_opus_stream_type_t;

/* Fatal Errors */
//enum xa_error_fatal_execute_opus_dec {};

#endif /* __XA_OPUS_DEC_DEFINITIONS_H__ */
