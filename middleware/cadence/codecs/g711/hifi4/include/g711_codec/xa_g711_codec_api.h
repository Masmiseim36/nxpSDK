/*
 * Copyright (c) 2012-2021 Cadence Design Systems, Inc.
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



#ifndef __XA_G711_CODEC_API_H__
#define __XA_G711_CODEC_API_H__

#include "xa_type_def.h"
#include "xa_error_standards.h"

#define XA_G711_MAX_SAMPLES_PER_FRAME 80
#define XA_G711_NUM_BITS_PER_SAMPLE 16

#define XA_G711_MAX_NUM_BITS_PER_FRAME 640
#define XA_G711_MAX_NUM_BYTES_PER_FRAME (XA_G711_MAX_NUM_BITS_PER_FRAME >> 3)

#define XA_CODEC_G711  1

/* G.711 specific error codes */

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_g711_codec {
    XA_G711_CONFIG_NONFATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_G711, 0)
};

/* Fatal Errors */
enum xa_error_fatal_config_g711_codec {
    XA_G711_CONFIG_FATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_G711, 0)
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_g711_codec {
    XA_G711_EXECUTE_NONFATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_G711, 0)
};

/* Fatal Errors */
enum xa_error_fatal_execute_g711_codec {
    XA_G711_EXECUTE_FATAL_PLACE_HOLDER  = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_G711, 0),
};

/* Type definitions */
/* Enconding law */
typedef enum
{
    XA_G711_LAW_A = 0,
    XA_G711_LAW_U
} xa_g711_law_t;

/* Frame type (applies to both Encoder and Decoder) */
typedef enum
{
    XA_G711_FRAME_NORMAL = 0,
    XA_G711_FRAME_SID,
    XA_G711_FRAME_ERASURE
} xa_g711_frame_type_t;


#if defined(__cplusplus)
extern "C" {
#endif    /* __cplusplus */

/***************************************************************
 * Encoder 
 ***************************************************************/
XA_ERRORCODE xa_g711_enc_init (xa_codec_handle_t handle,
                               pVOID scratch );

XA_ERRORCODE xa_g711_enc ( xa_codec_handle_t handle,
                           pWORD16 inp_speech,
                           pUWORD8 enc_speech,
                           xa_g711_law_t law,
                           xa_g711_frame_type_t *p_frame_type);

WORD32 xa_g711_enc_get_handle_byte_size ();
WORD32 xa_g711_enc_get_scratch_byte_size ();

/***************************************************************
 * Decoder       
 ***************************************************************/
XA_ERRORCODE xa_g711_dec_init (xa_codec_handle_t state,
                               pVOID scratch,
                               WORD32 plc_enable );

XA_ERRORCODE xa_g711_dec ( xa_codec_handle_t state,
                           pUWORD8 enc_speech,
                           pWORD16 synth_speech,
                           xa_g711_law_t law,
                           xa_g711_frame_type_t frame_type);

WORD32 xa_g711_dec_get_handle_byte_size ();
WORD32 xa_g711_dec_get_scratch_byte_size ();

/***************************************************************
 * Common API calls
 ***************************************************************/

const char* xa_g711_get_lib_name_string ();
const char* xa_g711_get_lib_version_string ();
const char* xa_g711_get_lib_api_version_string ();

#if defined(__cplusplus)
}
#endif    /* __cplusplus */


#endif /* __XA_G711_CODEC_API_H__ */

