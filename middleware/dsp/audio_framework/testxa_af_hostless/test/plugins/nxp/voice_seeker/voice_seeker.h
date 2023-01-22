/*
 * Copyright 2022 NXP
 *
 *  Created on: Sep 20, 2019
 *      Author: ttkrk
 */

#ifndef __VOICE_SEEKER_API_H
#define __VOICE_SEEKER_API_H

#define VOICE_SEEKER_MODEL_ALIGNMENT 64
//#define INSTALLOC_OFFSET           ((PL_UINTPTR) (VOICE_SEEKER_MODEL_ALIGNMENT-1))
//#define INSTALLOC_MASK             ((PL_UINTPTR)~INSTALLOC_OFFSET)
//#define INSTALLOC_ALIGN(ptr)       (((PL_UINTPTR)ptr + INSTALLOC_OFFSET)&INSTALLOC_MASK)

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

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/

/* ...pcm gain specific configuration parameters */
enum xa_config_param_VOICE_SEEKER
{
    XA_VOICE_SEEKER_CONFIG_PARAM_CHANNELS             = 0x0,
    XA_VOICE_SEEKER_CONFIG_PARAM_SAMPLE_RATE          = 0x1,
    XA_VOICE_SEEKER_CONFIG_PARAM_PCM_WIDTH            = 0x2,
    XA_VOICE_SEEKER_CONFIG_PARAM_PRODUCED             = 0x3,
    XA_VOICE_SEEKER_CONFIG_PARAM_INPUT_FRAME_SIZE_US  = 0x4,
    XA_VOICE_SEEKER_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US = 0x5,
    XA_VOICE_SEEKER_CONFIG_PARAM_VOICE_SEEKER         = 0x6
};

/* ...component identifier (informative) */
#define XA_CODEC_VOICE_SEEKER (0x42)

#ifdef CPU_MIMXRT685SFVKB_dsp
#define VOICE_SEEKER_MAX_CHANNELS (4)
#else
#define VOICE_SEEKER_MAX_CHANNELS (2)
#endif

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/

#define XA_VOICE_SEEKER_API_NONFATAL(e) XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_VOICE_SEEKER, (e))

#define XA_VOICE_SEEKER_API_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_VOICE_SEEKER, (e))

enum xa_error_nonfatal_api_VOICE_SEEKER
{
    XA_VOICE_SEEKER_API_NONFATAL_MAX = XA_VOICE_SEEKER_API_NONFATAL(0)
};

enum xa_error_fatal_api_VOICE_SEEKER
{
    XA_VOICE_SEEKER_API_FATAL_MAX = XA_VOICE_SEEKER_API_FATAL(0)
};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/

#define XA_VOICE_SEEKER_CONFIG_NONFATAL(e) \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_VOICE_SEEKER, (e))

#define XA_VOICE_SEEKER_CONFIG_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_VOICE_SEEKER, (e))

enum xa_error_nonfatal_config_VOICE_SEEKER
{
    XA_VOICE_SEEKER_CONFIG_NONFATAL_RANGE = XA_VOICE_SEEKER_CONFIG_NONFATAL(0),
    XA_VOICE_SEEKER_CONFIG_NONFATAL_STATE = XA_VOICE_SEEKER_CONFIG_NONFATAL(1),
    XA_VOICE_SEEKER_CONFIG_NONFATAL_MAX   = XA_VOICE_SEEKER_CONFIG_NONFATAL(2)
};

enum xa_error_fatal_config_VOICE_SEEKER
{
    XA_VOICE_SEEKER_CONFIG_FATAL_RANGE       = XA_VOICE_SEEKER_CONFIG_FATAL(0),
    XA_VOICE_SEEKER_CONFIG_FATAL_TRACK_STATE = XA_VOICE_SEEKER_CONFIG_FATAL(0 + XA_VOICE_SEEKER_CONFIG_NONFATAL_MAX),
    XA_VOICE_SEEKER_CONFIG_FATAL_MAX         = XA_VOICE_SEEKER_CONFIG_FATAL(1)
};

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/

#define XA_VOICE_SEEKER_EXEC_NONFATAL(e) \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_VOICE_SEEKER, (e))

#define XA_VOICE_SEEKER_EXEC_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_VOICE_SEEKER, (e))

enum xa_error_nonfatal_execute_VOICE_SEEKER
{
    XA_VOICE_SEEKER_EXEC_NONFATAL_STATE   = XA_VOICE_SEEKER_EXEC_NONFATAL(0),
    XA_VOICE_SEEKER_EXEC_NONFATAL_NO_DATA = XA_VOICE_SEEKER_EXEC_NONFATAL(1),
    XA_VOICE_SEEKER_EXEC_NONFATAL_INPUT   = XA_VOICE_SEEKER_EXEC_NONFATAL(2),
    XA_VOICE_SEEKER_EXEC_NONFATAL_OUTPUT  = XA_VOICE_SEEKER_EXEC_NONFATAL(3),
    XA_VOICE_SEEKER_EXEC_NONFATAL_MAX     = XA_VOICE_SEEKER_EXEC_NONFATAL(4)
};

enum xa_error_fatal_execute_VOICE_SEEKER
{
    XA_VOICE_SEEKER_EXEC_FATAL_STATE  = XA_VOICE_SEEKER_EXEC_FATAL(0),
    XA_VOICE_SEEKER_EXEC_FATAL_INPUT  = XA_VOICE_SEEKER_EXEC_FATAL(1),
    XA_VOICE_SEEKER_EXEC_FATAL_OUTPUT = XA_VOICE_SEEKER_EXEC_FATAL(2),
    XA_VOICE_SEEKER_EXEC_FATAL_MAX    = XA_VOICE_SEEKER_EXEC_FATAL(3)
};

enum AFE_VOICESEEKER_RETURN_STATUS
{
	AFE_VOICESEEKER_ERROR = -1,
	AFE_VOICESEEKER_EOF,
	AFE_VOICESEEKER_NULLADDRESS,
	AFE_VOICESEEKER_SUCCESS
};

/*******************************************************************************
 * API function definition (tbd)
 ******************************************************************************/

#if defined(USE_DLL) && defined(_WIN32)
#define DLL_SHARED __declspec(dllimport)
#elif defined(_WINDLL)
#define DLL_SHARED __declspec(dllexport)
#else
#define DLL_SHARED
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
DLL_SHARED xa_codec_func_t xa_VOICE_SEEKER;
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __VOICE_SEEKER_API_H */
