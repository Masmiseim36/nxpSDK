/*
 * Copyright (c) 2009-2017 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
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


#ifndef __XA_SBC_DEC_API_H__
#define __XA_SBC_DEC_API_H__

/*****************************************************************************/
/* SBC Decoder specific API definitions                                      */
/*****************************************************************************/

/* sbc_dec-specific configuration parameters */
enum xa_config_param_sbc_dec {
  XA_SBC_DEC_CONFIG_PARAM_PCM_WDSZ     = 0,
  XA_SBC_DEC_CONFIG_PARAM_SAMP_FREQ    = 1,
  XA_SBC_DEC_CONFIG_PARAM_NUM_CHANNELS = 2,
  XA_SBC_DEC_CONFIG_PARAM_BITRATE      = 3
};

/* commands */
#include "xa_apicmd_standards.h"

/* sbc_dec-specific commands */
/* (none) */

/* sbc_dec-specific command types */
/* (none) */

/* error codes */
#include "xa_error_standards.h"

#define XA_CODEC_SBC_DEC	17

/* sbc_dec-specific error codes */
/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
/* (none) */

/* Fatal Errors */
/* (none) */

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_sbc_dec {
  XA_SBC_DEC_EXECUTE_NONFATAL_BAD_SBC_FRAME  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 0),
  XA_SBC_DEC_EXECUTE_NONFATAL_CRC_ERROR      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 1),
  XA_SBC_DEC_EXECUTE_NONFATAL_BAD_BLOCK      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 2),
  XA_SBC_DEC_EXECUTE_NONFATAL_NEED_MORE_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 3)
};

/* Fatal Errors */
enum xa_error_fatal_execute_sbc_dec {
  XA_SBC_DEC_EXECUTE_FATAL_INVALID_STREAM = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_SBC_DEC, 0)

};

#include "xa_type_def.h"

#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */
xa_codec_func_t xa_sbc_dec;
#if defined(__cplusplus)
}
#endif	/* __cplusplus */
#endif /* __XA_SBC_DEC_API_H__ */
