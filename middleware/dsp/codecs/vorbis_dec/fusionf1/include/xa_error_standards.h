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

  
#ifndef __XA_ERROR_STANDARDS_H__
#define __XA_ERROR_STANDARDS_H__

/*****************************************************************************/
/* File includes                                                             */
/*  xa_type_def.h                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
#define XA_NO_ERROR	0
#define XA_FATAL_ERROR	0x80000000

enum xa_error_severity {
  xa_severity_nonfatal = 0,
  xa_severity_fatal    = 0xffffffff
};

enum xa_error_class {
  xa_class_api     = 0,
  xa_class_config  = 1,
  xa_class_execute = 2
};

#define XA_CODEC_GENERIC	0

#define XA_ERROR_CODE(severity, class, codec, index)	((severity << 15) | (class << 11) | (codec << 6) | index)
#define XA_ERROR_SEVERITY(code)	(((code) & XA_FATAL_ERROR) != 0)
#define XA_ERROR_CLASS(code)	(((code) >> 11) & 0x0f)
#define XA_ERROR_CODEC(code)    (((code) >>  6) & 0x1f)
#define XA_ERROR_SUBCODE(code)	(((code) >>  0) & 0x3f)

/* Our convention is that only api-class errors can be generic ones. */

/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Non Fatal Errors */
/* (none) */
/* Fatal Errors */
enum xa_error_fatal_api_generic {
  XA_API_FATAL_MEM_ALLOC        = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 0),
  XA_API_FATAL_MEM_ALIGN        = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 1),
  XA_API_FATAL_INVALID_CMD      = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 2),
  XA_API_FATAL_INVALID_CMD_TYPE = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 3)
};

#endif /* __XA_ERROR_STANDARDS_H__ */
