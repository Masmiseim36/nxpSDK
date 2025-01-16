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
#ifndef __XA_AMR_WB_DEC_DEFINITIONS_H__
#define __XA_AMR_WB_DEC_DEFINITIONS_H__

/* commands */
#include "xa_apicmd_standards.h"

/* error codes */
#include "xa_error_standards.h"

/* ...component identifier (informative) */
#define XA_CODEC_AMR_WB_DEC                  5

/* Non Fatal Errors */
enum xa_error_nonfatal_config_amr_wb_dec {
  XA_AMR_WB_DEC_CONFIG_NONFATAL_RANGE = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AMR_WB_DEC, 0),
  XA_AMR_WB_DEC_CONFIG_NONFATAL_STATE = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AMR_WB_DEC, 1),
  XA_AMR_WB_DEC_CONFIG_NONFATAL_MAX   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_AMR_WB_DEC, 2),
};

enum xa_error_nonfatal_execute_amr_wb_dec {
    XA_AMR_WB_DEC_EXEC_NONFATAL_STATE    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 0),
    XA_AMR_WB_DEC_EXEC_NONFATAL_NO_DATA  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 1),
    XA_AMR_WB_DEC_EXEC_NONFATAL_INPUT    = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 2),
    XA_AMR_WB_DEC_EXEC_NONFATAL_OUTPUT   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 3),
    XA_AMR_WB_DEC_EXEC_NONFATAL_MAX      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 4)
};

/* Fatal Errors */
enum xa_error_fatal_execute_amr_wb_dec {
  XA_AMR_WB_DEC_EXEC_FATAL_INVALID_DATA = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_AMR_WB_DEC, 0)
};

enum xa_config_param_amr_wb_dec {
  XA_AMR_WB_DEC_CONFIG_PARAM_PCM_WDSZ     = 0,
  XA_AMR_WB_DEC_CONFIG_PARAM_SAMP_FREQ    = 1,
  XA_AMR_WB_DEC_CONFIG_PARAM_NUM_CHANNELS = 2,
};

#define XA_AMR_WB_CONFIG_PARAM_NUM_CHANNELS     1
#define XA_AMR_WB_CONFIG_PARAM_SAMP_FREQ        16000
#define XA_AMR_WB_CONFIG_PARAM_PCM_WDSZ         16

#endif /* __XA_AMR_WB_DEC_DEFINITIONS_H__ */
