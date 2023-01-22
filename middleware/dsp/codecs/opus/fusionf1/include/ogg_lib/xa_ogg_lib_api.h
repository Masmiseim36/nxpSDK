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


#ifndef __XA_OGG_LIB_API_H__
#define __XA_OGG_LIB_API_H__

#include "xa_type_def.h"
#include "xa_error_standards.h"

/* Ogg Lib constants */

#if 0
/************************************************/
/* Structure for controlling ogg-pack operation */
/************************************************/
typedef struct
{
    WORD32 input_Fs_fx;                         /* input signal sampling frequency in Hz */

} xa_ogg_pack_init_cfg_t;

typedef struct
{
    WORD32 total_brate_fx;                      /* total bitrate in bps of the codec */

} xa_ogg_pack_dynamic_control_t;
#endif

/*************************************************/
/* Structure for controlling ogg-parse operation */
/*************************************************/

typedef struct
{
    WORD32 max_page_size;                        /* Maximum page size to be allocatein kB */

} xa_ogg_parse_init_cfg_t;


#define XA_LIB_OGG  4

/* Ogg specific error codes */

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_ogg_lib
{
    XA_OGG_CONFIG_NONFATAL_INVALID_PARAM                     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_LIB_OGG, 0),
};

/* Fatal Errors */
enum xa_error_fatal_config_ogg_lib
{
    XA_OGG_CONFIG_FATAL_PLACE_HOLDER                         = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_LIB_OGG, 0),
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_ogg_lib
{
    XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA                = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_LIB_OGG, 0),
    XA_OGG_EXECUTE_NONFATAL_PAGE_IGNORED                     = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_LIB_OGG, 1),
    XA_OGG_EXECUTE_NONFATAL_STREAM_CHANGE                    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_LIB_OGG, 2),
};

/* Fatal Errors */
enum xa_error_fatal_execute_ogg_lib
{
    XA_OGG_EXECUTE_FATAL_CORRUPTED_STREAM                    = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 0),
    XA_OGG_EXECUTE_FATAL_OUT_BUF_TOO_SMALL                   = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 1),
    XA_OGG_EXECUTE_FATAL_PAGE_SIZE_TOO_SMALL                 = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 2),
    XA_OGG_EXECUTE_FATAL_NOT_INITIALIZED                     = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 3),
    XA_OGG_EXECUTE_FATAL_EXTRA_PKTS_ON_HEADER_PAGE           = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 4),
    XA_OGG_EXECUTE_FATAL_EXTRA_PKTS_ON_TAGS_PAGE             = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_LIB_OGG, 5),
};


#if defined(__cplusplus)
extern "C" {

#endif    /* __cplusplus */

#if 0
/******************************************************************************
 *         Ogg Pack functions                                                 *
 ******************************************************************************/
XA_ERRORCODE xa_ogg_pack_init
(
    xa_codec_handle_t handle,
    pVOID scratch,
    xa_ogg_pack_init_cfg_t *p_cfg
);

XA_ERRORCODE xa_ogg_pack_process
(
    xa_codec_handle_t handle,                       /* I/O: State handle */
    pWORD16 inp_speech,                             /* I  : Input speech samples */
    pUWORD8 enc_speech,                             /* O  : Encoded speech in packed format */
    WORD16  *inp_samples,                           /* I/O: Number of input samples. Returns number of samples processed */
    WORD16  *out_bits                               /* I/O: Space available in bits. Returns number of bits encoded */
);

WORD32 xa_ogg_pack_get_handle_byte_size( xa_ogg_pack_init_cfg_t *p_cfg );
WORD32 xa_ogg_pack_get_scratch_byte_size( xa_ogg_pack_init_cfg_t *p_cfg );
#endif

/******************************************************************************
 *         Ogg Parse functions                                                *
 *****************************************************************************/

XA_ERRORCODE xa_ogg_parse_init
(
    xa_codec_handle_t handle,
    pVOID scratch,
    xa_ogg_parse_init_cfg_t *cfg
);

XA_ERRORCODE xa_ogg_parse_process 
(   xa_codec_handle_t   handle,                 /* I/O: State handle */
    pUWORD8             in_data,                /* I:   Input Ogg stream data */
    pUWORD8             out_data,               /* O:   Output Packet data */
    WORD32              *num_bytes_in,          /* I:O  Number of input bytes to be parsed. Returns same value currently */
    WORD32              *num_bytes_out          /* I/O: Space available in bytes. Returns number of bytes generated */
);

WORD32 xa_ogg_parse_get_handle_byte_size ( xa_ogg_parse_init_cfg_t *p_cfg );
WORD32 xa_ogg_parse_get_scratch_byte_size ( xa_ogg_parse_init_cfg_t *p_cfg );

/****************************************************************
 * Common API calls
 ****************************************************************/

#if 0
typedef enum
{
    XA_OGG_PACK_PARAM_FIRST_PARAM                               = 0x0,
    XA_OGG_PACK_PARAM_LAST_PARAM

} xa_ogg_pack_param_type ;

typedef union xa_ogg_pack_param_id_t
{
    WORD32                      val;            /* For general referencing */
    xa_ogg_pack_param_type      common;
} xa_ogg_pack_param_id_t;

/* Sets Ogg module parameters */
XA_ERRORCODE
xa_ogg_pack_set_param (
                          xa_codec_handle_t handle,               /* Handle to codec object */
                          xa_ogg_pack_param_id_t param_id,        /* Identifies parameter to be written */
                          pVOID p_param_value);                   /* Buffer containing the parameter value */

/* Get Ogg module parameters */
XA_ERRORCODE
xa_ogg_pack_get_param (
                          xa_codec_handle_t handle,               /* Handle to codec object */
                          xa_ogg_pack_param_id_t param_id,        /* Identifies parameter to be written */
                          pVOID p_param_value);                   /* Buffer containing the parameter value */
#endif

typedef enum
{
    XA_OGG_PARSE_PARAM_FIRST_PARAM                               = 0x0,
    XA_OGG_PARSE_PARAM_PAGE_GRANULE,
    XA_OGG_PARSE_PARAM_END_OF_STREAM,
    XA_OGG_PARSE_PARAM_LAST_PARAM

} xa_ogg_parse_param_type;

typedef union xa_ogg_parse_param_id_t
{
    WORD32                      val;            /* For general referencing */
    xa_ogg_parse_param_type     type;
} xa_ogg_parse_param_id_t;

XA_ERRORCODE
xa_ogg_parse_set_param (
                          xa_codec_handle_t handle,               /* Handle to codec object */
                          xa_ogg_parse_param_id_t param_id,       /* Identifies parameter to be written */
                          pVOID p_param_value);                   /* Buffer containing the parameter value */

/* Get Ogg module parameters */
XA_ERRORCODE
xa_ogg_parse_get_param (
                          xa_codec_handle_t handle,               /* Handle to codec object */
                          xa_ogg_parse_param_id_t param_id,       /* Identifies parameter to be written */
                          pVOID p_param_value);                   /* Buffer containing the parameter value */

#if defined(__cplusplus)
}
#endif    /* __cplusplus */

#endif /* __XA_OGG_LIB_API_H__ */

