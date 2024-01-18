/*
 * Copyright (c) 2012-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */


#ifndef XA_G722_CODEC_API_H
#define XA_G722_CODEC_API_H

#include <xa_type_def.h>
#include <xa_error_standards.h>

/* Sample Size defination */
#define XA_G722_MAX_NUM_BITS_PER_ENCODED_SAMPLE     4
#define XA_G722_NUM_BITS_PER_PCM_SAMPLE             16

#define XA_CODEC_G722  1

/* G.722 specific error codes */

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_g711_codec {
    XA_G722_CONFIG_NONFATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_G722, 0)
};

/* Fatal Errors */
enum xa_error_fatal_config_g711_codec {
    XA_G722_CONFIG_FATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_G722, 0)
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_g711_codec {
    XA_G722_EXECUTE_NONFATAL_PLACE_HOLDER = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_G722, 0)
};

/* Fatal Errors */
enum xa_error_fatal_execute_g711_codec {
    XA_G722_EXECUTE_FATAL_INVALID_NUM_SAMPLES_TO_PROCESS = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_G722, 0),
    XA_G722_EXECUTE_FATAL_INSUFFICIENT_BYTES_TO_PROCESS  = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_G722, 1)
};

/* Type definitions */


typedef enum
{
    XA_G722_GOOD_FRAME = 0,
    XA_G722_BAD_FRAME
} xa_g722_frame_type_t;

#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */

/*******************************************************************************
 * Encoder
 ******************************************************************************/

XA_ERRORCODE xa_g722_enc_init(xa_codec_handle_t handle,
                              pVOID scratch);

XA_ERRORCODE xa_g722_enc (
        xa_codec_handle_t handle,
        pWORD16 inp_speech,
        pWORD8 enc_speech,
        WORD32 num_samples);

WORD32 xa_g722_enc_get_handle_byte_size();
WORD32 xa_g722_enc_get_scratch_byte_size();

/*******************************************************************************
 * Decoder
 ******************************************************************************/
XA_ERRORCODE xa_g722_dec_init(
        xa_codec_handle_t handle,
        pVOID scratch,
        WORD32 plc_enable);

XA_ERRORCODE xa_g722_dec (
        xa_codec_handle_t handle,
        pWORD8 enc_speech,
        pWORD16 synth_speech,
        WORD32 num_bytes,
        xa_g722_frame_type_t frame_type);

WORD32 xa_g722_dec_get_handle_byte_size();
WORD32 xa_g722_dec_get_scratch_byte_size();


/*******************************************************************************
 * Common API calls
 ******************************************************************************/
const char *xa_g722_get_lib_name_string();
const char *xa_g722_get_lib_version_string();
const char *xa_g722_get_lib_api_version_string();

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif /* XA_G722_CODEC_API_H */
