/*
 * Copyright (c) 2006-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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


#ifndef __XA_VORBIS_DEC_API_H__
#define __XA_VORBIS_DEC_API_H__

#include <xa_memory_standards.h>

/* vorbis_dec-specific configuration parameters */
enum xa_config_param_vorbis_dec 
{
    XA_VORBISDEC_CONFIG_PARAM_SAMP_FREQ                         = 0,
    XA_VORBISDEC_CONFIG_PARAM_NUM_CHANNELS                      = 1,
    XA_VORBISDEC_CONFIG_PARAM_PCM_WDSZ                          = 2,
    XA_VORBISDEC_CONFIG_PARAM_COMMENT_MEM_PTR                   = 3,
    XA_VORBISDEC_CONFIG_PARAM_COMMENT_MEM_SIZE                  = 4,
    XA_VORBISDEC_CONFIG_PARAM_GET_CUR_BITRATE                   = 5,
    XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_FILE_MODE              = 6,
    XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_LAST_PKT_GRANULE_POS   = 7,
    XA_VORBISDEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE                 = 8,
    XA_VORBISDEC_CONFIG_PARAM_RUNTIME_MEM                       = 9
};

/* commands */
#include <xa_apicmd_standards.h>

/* vorbis_dec-specific command types */
/* (none) */

/* error codes */
#include <xa_error_standards.h>
#define XA_CODEC_VORBIS_DEC 7

/* vorbis_dec-specific error codes */

/*****************************************************************************/
/* Class 1: Configuration Errors                                     */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_config_vorbis_dec
{
    XA_VORBISDEC_CONFIG_NONFATAL_GROUPED_STREAM = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_VORBIS_DEC, 0),
    XA_VORBISDEC_CONFIG_NONFATAL_BAD_PARAM = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_VORBIS_DEC, 1)
};

/* Fatal Errors */
enum xa_error_fatal_config_vorbis_dec
{
    XA_VORBISDEC_CONFIG_FATAL_BADHDR            = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 0),
    XA_VORBISDEC_CONFIG_FATAL_NOTVORBIS         = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 1),
    XA_VORBISDEC_CONFIG_FATAL_BADINFO           = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 2),
    XA_VORBISDEC_CONFIG_FATAL_BADVERSION        = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 3),
    XA_VORBISDEC_CONFIG_FATAL_BADBOOKS          = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 4),
    XA_VORBISDEC_CONFIG_FATAL_CODEBOOK_DECODE   = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 5),
    XA_VORBISDEC_CONFIG_FATAL_INVALID_PARAM     = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VORBIS_DEC, 6)
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_vorbis_dec
{
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_HOLE = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 0),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_NOTAUDIO = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 1),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_BADPACKET = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 2),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_RUNTIME_DECODE_FLUSH_IN_PROGRESS = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 3),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_INVALID_STRM_POS = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 4),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 5),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_UNEXPECTED_IDENT_PKT_RECEIVED = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 6),
    XA_VORBISDEC_EXECUTE_NONFATAL_OV_UNEXPECTED_HEADER_PKT_RECEIVED = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 7)
};
/* Fatal Errors */
enum xa_error_fatal_execute_vorbis_dec
{
    XA_VORBISDEC_EXECUTE_FATAL_PERSIST_ALLOC                = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 0),
    XA_VORBISDEC_EXECUTE_FATAL_SCRATCH_ALLOC                = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 1),
    XA_VORBISDEC_EXECUTE_FATAL_CORRUPT_STREAM               = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 2),
    XA_VORBISDEC_EXECUTE_FATAL_INSUFFICIENT_INP_BUF_SIZE    = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_VORBIS_DEC, 3)
};

#include "xa_type_def.h"

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

    xa_codec_func_t xa_vorbis_dec;

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __XA_VORBIS_DEC_API_H__ */
