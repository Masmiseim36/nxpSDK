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

#ifndef __XA_RENDERER_API_H__
#define __XA_RENDERER_API_H__

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

/* ...renderer-specific configuration parameters */
enum xa_config_param_renderer {
    XA_RENDERER_CONFIG_PARAM_CB                    = 0,
    XA_RENDERER_CONFIG_PARAM_STATE                 = 1,
    XA_RENDERER_CONFIG_PARAM_PCM_WIDTH             = 2,
    XA_RENDERER_CONFIG_PARAM_CHANNELS              = 3,
    XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE           = 4,
    XA_RENDERER_CONFIG_PARAM_FRAME_SIZE            = 5,    /* frame size per channel in bytes. Deprecated, use XA_RENDERER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES instead. */
    XA_RENDERER_CONFIG_PARAM_BYTES_PRODUCED        = 6,
    XA_RENDERER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES = 7,    /* frame size per channel in samples */
    XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE         = 8,
    XA_RENDERER_CONFIG_PARAM_I2S_MASTER_SLAVE      = 9,
    XA_RENDERER_CONFIG_PARAM_I2S_MODE              = 10,
    XA_RENDERER_CONFIG_PARAM_I2S_SCK_POLARITY      = 11,
    XA_RENDERER_CONFIG_PARAM_I2S_WS_POLARITY       = 12,
    XA_RENDERER_CONFIG_PARAM_I2S_POSITION          = 13,
    XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1        = 14,
    XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2        = 15,
    XA_RENDERER_CONFIG_PARAM_NUM                   = 16,
    XA_RENDERER_CONFIG_PARAM_CODEC_CHANNELS        = 17,
    XA_RENDERER_CONFIG_PARAM_CODEC_PCM_WIDTH       = 18,
    XA_RENDERER_CONFIG_PARAM_CODEC_FRAME_SIZE      = 19
};

/* ...XA_RENDERER_CONFIG_PARAM_CB: compound parameters data structure */
typedef struct xa_renderer_cb_s {
    /* ...input buffer completion callback */
    void      (*cb)(struct xa_renderer_cb_s *, WORD32 idx);

}   xa_renderer_cb_t;


/* ...renderer states  */
enum xa_randerer_state {
    XA_RENDERER_STATE_START = 0,
    XA_RENDERER_STATE_IDLE  = 1,
    XA_RENDERER_STATE_RUN   = 2,
    XA_RENDERER_STATE_PAUSE = 3
};

/* ...component identifier (informative) */
#define XA_CODEC_RENDERER               6

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/

#define XA_RENDERER_API_NONFATAL(e)     \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_RENDERER, (e))

#define XA_RENDERER_API_FATAL(e)        \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_RENDERER, (e))

enum xa_error_nonfatal_api_renderer {
    XA_RENDERER_API_NONFATAL_MAX = XA_RENDERER_API_NONFATAL(0)
};

enum xa_error_fatal_api_renderer {
    XA_RENDERER_API_FATAL_MAX = XA_RENDERER_API_FATAL(0)
};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/

#define XA_RENDERER_CONFIG_NONFATAL(e)  \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_RENDERER, (e))

#define XA_RENDERER_CONFIG_FATAL(e)     \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_RENDERER, (e))

enum xa_error_nonfatal_config_renderer {
    XA_RENDERER_CONFIG_NONFATAL_RANGE   = XA_RENDERER_CONFIG_NONFATAL(0),
    XA_RENDERER_CONFIG_NONFATAL_STATE   = XA_RENDERER_CONFIG_NONFATAL(1),
    XA_RENDERER_CONFIG_NONFATAL_MAX     = XA_RENDERER_CONFIG_NONFATAL(2)
};

enum xa_error_fatal_config_renderer {
    XA_RENDERER_CONFIG_FATAL_HW         = XA_RENDERER_CONFIG_FATAL(0),
    XA_RENDERER_CONFIG_FATAL_STATE      = XA_RENDERER_CONFIG_FATAL(1),
    XA_RENDERER_CONFIG_FATAL_RANGE      = XA_RENDERER_CONFIG_FATAL(2),
    XA_RENDERER_CONFIG_FATAL_MAX        = XA_RENDERER_CONFIG_FATAL(3)
};

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/

#define XA_RENDERER_EXEC_NONFATAL(e)    \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_RENDERER, (e))

#define XA_RENDERER_EXEC_FATAL(e)       \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_RENDERER, (e))

enum xa_error_nonfatal_execute_renderer {
    XA_RENDERER_EXEC_NONFATAL_STATE     = XA_RENDERER_EXEC_NONFATAL(0),
    XA_RENDERER_EXEC_NONFATAL_INPUT     = XA_RENDERER_EXEC_NONFATAL(1),
    XA_RENDERER_EXEC_NONFATAL_OUTPUT    = XA_RENDERER_EXEC_NONFATAL(2),
    XA_RENDERER_EXEC_NONFATAL_MAX       = XA_RENDERER_EXEC_NONFATAL(3)
};

enum xa_error_fatal_execute_renderer {
    XA_RENDERER_EXEC_FATAL_HW           = XA_RENDERER_EXEC_FATAL(0),
    XA_RENDERER_EXEC_FATAL_STATE        = XA_RENDERER_EXEC_FATAL(1),
    XA_RENDERER_EXEC_FATAL_INPUT        = XA_RENDERER_EXEC_FATAL(2),
    XA_RENDERER_EXEC_FATAL_MAX          = XA_RENDERER_EXEC_FATAL(3)
};

/*******************************************************************************
 * API function definition (tbd)
 ******************************************************************************/

#if 0

#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
#define DLL_SHARED
DLL_SHARED xa_codec_func_t xa_renderer;
#if defined(__cplusplus)
}
#endif  /* __cplusplus */
#endif
#endif /* __XA_RENDERER_API_H__ */
