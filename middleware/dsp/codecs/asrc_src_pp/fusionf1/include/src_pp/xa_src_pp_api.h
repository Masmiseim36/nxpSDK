/*
 * Copyright (c) 2011-2020 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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


#ifndef _XA_SRC_PP_API_H_
#define _XA_SRC_PP_API_H_

#include "xa_type_def.h"

/* Sample Rate Convrter configuration parameters */
enum xa_config_param_src_pp
{
    XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE    =  0,
    XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE   =  1,
    XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE     =  2,
    XA_SRC_PP_CONFIG_PARAM_OUTPUT_CHUNK_SIZE    =  3,    
    XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS       =  4,
    XA_SRC_PP_CONFIG_PARAM_SET_INPUT_BUF_PTR    =  5,
    XA_SRC_PP_CONFIG_PARAM_SET_OUTPUT_BUF_PTR   =  6,
    XA_SRC_PP_CONFIG_PARAM_GET_NUM_STAGES       =  7,
	XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE     =  8,
    XA_SRC_PP_CONFIG_PARAM_ENABLE_ASRC          =  9,        // Only available when the library is build with ASRC_ENABLE
	XA_SRC_PP_CONFIG_PARAM_DRIFT_ASRC           = 10,        // Only available when the library is build with ASRC_ENABLE
    XA_SRC_PP_CONFIG_PARAM_GET_DRIFT_FRACT_ASRC = 11,        // Only available when the library is build with ASRC_ENABLE
    XA_SRC_PP_CONFIG_PARAM_ENABLE_CUBIC         = 12,        // Only available when the library is build with POLYPHASE_CUBIC_INTERPOLATION 
    XA_SRC_PP_CONFIG_PARAM_CUSTOM_MODE          = 20         /* Only available for HiFi2 based libraries built with compiler switch "XA_CUSTOM_SRC_IMPL" on */ 
};

#define XA_SRC_PP 3

/* Define for the number of PCM Width */
#define TWO_BYTES_PER_SAMPLE        2
#define THREE_BYTES_PER_SAMPLE      3

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Non-Fatal Errors */
enum xa_error_nonfatal_config_src_pp
{
  XA_SRC_PP_CONFIG_NON_FATAL_INVALID_CONFIG_TYPE        = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_SRC_PP, 0),
  XA_SRC_PP_CONFIG_NON_FATAL_INVALID_ENABLE_ASRC        = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_SRC_PP, 1),
  XA_SRC_PP_CONFIG_NON_FATAL_INVALID_DRIFT_ASRC         = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_SRC_PP, 2),
  XA_SRC_PP_CONFIG_NON_FATAL_INVALID_ENABLE_CUBIC       = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_SRC_PP, 3),  
};
/* Fatal Errors */
enum xa_error_fatal_config_src_pp
{
  XA_SRC_PP_CONFIG_FATAL_INVALID_INPUT_RATE             = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 0),
  XA_SRC_PP_CONFIG_FATAL_INVALID_OUTPUT_RATE            = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 1),
  XA_SRC_PP_CONFIG_FATAL_INVALID_INPUT_CHUNK_SIZE       = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 2),
  XA_SRC_PP_CONFIG_FATAL_INVALID_INPUT_CHANNELS         = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 3),
  XA_SRC_PP_CONFIG_FATAL_INVALID_INPUT_PTR              = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 4),
  XA_SRC_PP_CONFIG_FATAL_INVALID_OUTPUT_PTR             = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 5),
  XA_SRC_PP_CONFIG_FATAL_INVALID_BYTES_PER_SAMPLE       = XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_SRC_PP, 6),
};

/*****************************************************************************/
/* Class 2: Execution Class Errors                                           */
/*****************************************************************************/
/* Non Fatal Errors */
enum xa_error_nonfatal_execute_src_pp
{
  XA_SRC_PP_EXECUTE_NON_FATAL_INVALID_CONFIG_SEQ         = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_SRC_PP, 0),
  XA_SRC_PP_EXECUTE_NON_FATAL_INVALID_API_SEQ            = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_SRC_PP, 1),
};

/* Fatal Errors */
enum xa_error_fatal_execute_src_pp
{
  XA_SRC_PP_EXECUTE_FATAL_ERR_POST_CONFIG_INIT          = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_SRC_PP, 0),
  XA_SRC_PP_EXECUTE_FATAL_ERR_INIT                      = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_SRC_PP, 1),
  XA_SRC_PP_EXECUTE_FATAL_ERR_EXECUTE                   = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_SRC_PP, 2),
};

#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */
    /* Either one of the following is available in any library */
xa_codec_func_t xa_src_pp; // Standard SRC / SRCPlus
xa_codec_func_t xa_src384k_pp; // Trimmed src384k
#if defined(__cplusplus)
}
#endif	/* __cplusplus */

#endif  /*_XA_SRC_PP_API_H_*/
