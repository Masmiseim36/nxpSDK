/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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

#ifndef __XA_CAPTURER_API_H__
#define __XA_CAPTURER_API_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/

/* ...capturer-specific configuration parameters */
enum xa_config_param_capturer {
    XA_CAPTURER_CONFIG_PARAM_CB             = 0,
    XA_CAPTURER_CONFIG_PARAM_STATE          = 1,
    XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH      = 2,
    XA_CAPTURER_CONFIG_PARAM_CHANNELS       = 3,
    XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE    = 4,
    XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE     = 5,    /* frame size per channel in bytes. Deprecated, use XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES instead. */
    XA_CAPTURER_CONFIG_PARAM_BYTES_PRODUCED  = 6,
    XA_CAPTURER_CONFIG_PARAM_SAMPLE_END      = 7,
    XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES = 8,    /* frame size per channel in samples */
    XA_CAPTURER_CONFIG_PARAM_INTERLEAVE      = 9,
    XA_CAPTURER_CONFIG_PARAM_I2S_INTERFACE  = 10,
    XA_CAPTURER_CONFIG_PARAM_I2S_MASTER_SLAVE  = 11,
    XA_CAPTURER_CONFIG_PARAM_I2S_MODE       = 12,
    XA_CAPTURER_CONFIG_PARAM_I2S_SCK_POLARITY  = 13,
    XA_CAPTURER_CONFIG_PARAM_I2S_WS_POLARITY = 14,
    XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_1 = 15,
    XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_2 = 16,
    XA_CAPTURER_CONFIG_PARAM_NUM            = 17
};

/* ...XA_CAPTURER_CONFIG_PARAM_CB: compound parameters data structure */
typedef struct xa_capturer_cb_s {
    /* ...input buffer completion callback */
    void      (*cb)(struct xa_capturer_cb_s *, WORD32 idx);

}   xa_capturer_cb_t;


/* ...capturer states  */
enum xa_capturer_state {
    XA_CAPTURER_STATE_START = 0,
    XA_CAPTURER_STATE_IDLE  = 1,
    XA_CAPTURER_STATE_RUN   = 2,
    XA_CAPTURER_STATE_PAUSE = 3
};

/* ...component identifier (informative) */
#define XA_CODEC_CAPTURER               7

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/

#define XA_CAPTURER_API_NONFATAL(e)     \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_CAPTURER, (e))

#define XA_CAPTURER_API_FATAL(e)        \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_CAPTURER, (e))

enum xa_error_nonfatal_api_capturer {
    XA_CAPTURER_API_NONFATAL_MAX = XA_CAPTURER_API_NONFATAL(0)
};

enum xa_error_fatal_api_capturer {
    XA_CAPTURER_API_FATAL_MAX = XA_CAPTURER_API_FATAL(0)
};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/

#define XA_CAPTURER_CONFIG_NONFATAL(e)  \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_CAPTURER, (e))

#define XA_CAPTURER_CONFIG_FATAL(e)     \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_CAPTURER, (e))

enum xa_error_nonfatal_config_capturer {
    XA_CAPTURER_CONFIG_NONFATAL_RANGE   = XA_CAPTURER_CONFIG_NONFATAL(0),
    XA_CAPTURER_CONFIG_NONFATAL_STATE   = XA_CAPTURER_CONFIG_NONFATAL(1),
    XA_CAPTURER_CONFIG_NONFATAL_MAX     = XA_CAPTURER_CONFIG_NONFATAL(2)
};

enum xa_error_fatal_config_capturer {
    XA_CAPTURER_CONFIG_FATAL_HW         = XA_CAPTURER_CONFIG_FATAL(0),
    XA_CAPTURER_CONFIG_FATAL_STATE      = XA_CAPTURER_CONFIG_FATAL(1),
    XA_CAPTURER_CONFIG_FATAL_RANGE      = XA_CAPTURER_CONFIG_FATAL(2),
    XA_CAPTURER_CONFIG_FATAL_MAX        = XA_CAPTURER_CONFIG_FATAL(3)
};

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/

#define XA_CAPTURER_EXEC_NONFATAL(e)    \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_CAPTURER, (e))

#define XA_CAPTURER_EXEC_FATAL(e)       \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_CAPTURER, (e))

enum xa_error_nonfatal_execute_capturer {
    XA_CAPTURER_EXEC_NONFATAL_STATE     = XA_CAPTURER_EXEC_NONFATAL(0),
    XA_CAPTURER_EXEC_NONFATAL_INPUT     = XA_CAPTURER_EXEC_NONFATAL(1),
    XA_CAPTURER_EXEC_NONFATAL_MAX       = XA_CAPTURER_EXEC_NONFATAL(2),
    XA_CAPTURER_EXEC_NONFATAL_NO_DATA   = XA_CAPTURER_EXEC_NONFATAL(3)
};

enum xa_error_fatal_execute_capturer {
    XA_CAPTURER_EXEC_FATAL_HW           = XA_CAPTURER_EXEC_FATAL(0),
    XA_CAPTURER_EXEC_FATAL_STATE        = XA_CAPTURER_EXEC_FATAL(1),
    XA_CAPTURER_EXEC_FATAL_INPUT        = XA_CAPTURER_EXEC_FATAL(2),
    XA_CAPTURER_EXEC_FATAL_MAX          = XA_CAPTURER_EXEC_FATAL(3)
};

/*******************************************************************************
 * API function definition (tbd)
 ******************************************************************************/

#if defined(USE_DLL) && defined(_WIN32)
#define DLL_SHARED __declspec(dllimport)
#elif defined (_WINDLL)
#define DLL_SHARED __declspec(dllexport)
#else
#define DLL_SHARED
#endif

#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
DLL_SHARED xa_codec_func_t xa_capturer;
#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif /* __XA_CAPTURER_API_H__ */
