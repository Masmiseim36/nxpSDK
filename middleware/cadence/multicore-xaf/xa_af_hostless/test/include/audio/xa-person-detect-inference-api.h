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
/*******************************************************************************
 * xa-person-detect-inference-api.h
 *
 * Inference component API
 ******************************************************************************/

#ifndef __XA_PERSON_DETECT_INFERENCE_API_H__
#define __XA_PERSON_DETECT_INFERENCE_API_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/

/* ...generic commands */
#include "xa_apicmd_standards.h"

/* ...generic error codes */
#include "xa_error_standards.h"

/* ...common types */
#include "xa_type_def.h"

#include "xa_memory_standards.h"

#include "xa-audio-decoder-api.h"

#include "xa-tflm-inference-api.h"

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/

/* ...Inference specific configuration parameters */
enum xa_config_param_person_detect {
    XA_PERSON_DETECT_INFERENCE_CONFIG_PARAM_CHANNELS          = XA_INFERENCE_CONFIG_PARAM_CHANNELS,
    XA_PERSON_DETECT_INFERENCE_CONFIG_PARAM_FRAME_RATE        = XA_INFERENCE_CONFIG_PARAM_FRAME_RATE,
    XA_PERSON_DETECT_INFERENCE_CONFIG_PARAM_PCM_WIDTH         = XA_INFERENCE_CONFIG_PARAM_PCM_WIDTH,
    XA_PERSON_DETECT_INFERENCE_CONFIG_PARAM_PRODUCED          = XA_INFERENCE_CONFIG_PARAM_PRODUCED,
    XA_PERSON_DETECT_INFERENCE_CONFIG_PARAM_FRAME_SIZE        = XA_INFERENCE_CONFIG_PARAM_FRAME_SIZE
};

/* ...person detect component identifier (informative) */
#define XA_PERSON_DETECT_INFERENCE              6



/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/

#define XA_PERSON_DETECT_INFERENCE_API_NONFATAL(e)        \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_PERSON_DETECT_INFERENCE, (e))


#define XA_PERSON_DETECT_INFERENCE_API_FATAL(e)           \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_PERSON_DETECT_INFERENCE, (e))


enum xa_error_nonfatal_api_person_detect {
    XA_PERSON_DETECT_INFERENCE_API_NONFATAL_PLACE_HOLDER = XA_PERSON_DETECT_INFERENCE_API_NONFATAL(0)
};

enum xa_error_fatal_api_person_detect {
    XA_PERSON_DETECT_INFERENCE_API_FATAL_PLACE_HOLDER = XA_PERSON_DETECT_INFERENCE_API_FATAL(0)
};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/

#define XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL(e)     \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_PERSON_DETECT_INFERENCE, (e))


#define XA_PERSON_DETECT_INFERENCE_CONFIG_FATAL(e)        \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_PERSON_DETECT_INFERENCE, (e))


enum xa_error_nonfatal_config_person_detect {
    XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL_RANGE    = XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL(0),
    XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL_STATE    = XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL(1),
    XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL_READONLY = XA_PERSON_DETECT_INFERENCE_CONFIG_NONFATAL(2)
};

enum xa_error_fatal_config_person_detect {
    XA_PERSON_DETECT_INFERENCE_CONFIG_FATAL_RANGE           = XA_PERSON_DETECT_INFERENCE_CONFIG_FATAL(0),
    XA_PERSON_DETECT_INFERENCE_CONFIG_FATAL_TRACK_STATE     = XA_PERSON_DETECT_INFERENCE_CONFIG_FATAL(1),
};

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/

#define XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL(e)       \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_PERSON_DETECT_INFERENCE, (e))


#define XA_PERSON_DETECT_INFERENCE_EXEC_FATAL(e)          \
    XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_PERSON_DETECT_INFERENCE, (e))


enum xa_error_nonfatal_execute_person_detect {
    XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL_STATE    = XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL(0),
    XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL_NO_DATA  = XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL(1),
    XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL_INPUT    = XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL(2),
    XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL_OUTPUT   = XA_PERSON_DETECT_INFERENCE_EXEC_NONFATAL(3),
};

enum xa_error_fatal_execute_person_detect {
    XA_PERSON_DETECT_INFERENCE_EXEC_FATAL_STATE       = XA_PERSON_DETECT_INFERENCE_EXEC_FATAL(0),
    XA_PERSON_DETECT_INFERENCE_EXEC_FATAL_INPUT       = XA_PERSON_DETECT_INFERENCE_EXEC_FATAL(1),
    XA_PERSON_DETECT_INFERENCE_EXEC_FATAL_OUTPUT      = XA_PERSON_DETECT_INFERENCE_EXEC_FATAL(2),
};

/*******************************************************************************
 * Person Detect compoent API function
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
DLL_SHARED xa_codec_func_t xa_person_detect_inference;
#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif /* __XA_PERSON_DETECT_INFERENCE_API_H__ */

