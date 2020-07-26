/*
 * client_proxy_api.h
 *
 *  Created on: Sep 20, 2019
 *      Author: ttkrk
 */

#ifndef __CLIENT_PROXY_API_H
#define __CLIENT_PROXY_API_H

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
enum xa_config_param_client_proxy
{
    XA_CLIENT_PROXY_CONFIG_PARAM_CHANNELS             = 0x0,
    XA_CLIENT_PROXY_CONFIG_PARAM_SAMPLE_RATE          = 0x1,
    XA_CLIENT_PROXY_CONFIG_PARAM_PCM_WIDTH            = 0x2,
    XA_CLIENT_PROXY_CONFIG_PARAM_PRODUCED             = 0x3,
    XA_CLIENT_PROXY_CONFIG_PARAM_INPUT_FRAME_SIZE_US  = 0x4,
    XA_CLIENT_PROXY_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US = 0x5,
    XA_CLIENT_PROXY_CONFIG_PARAM_EAP                  = 0x6,
};

/* ...component identifier (informative) */
#define XA_CODEC_CLIENT_PROXY (0x41)

#define CLIENT_PROXY_MAX_CHANNELS (8)

/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/

#define XA_CLIENT_PROXY_API_NONFATAL(e) XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_CLIENT_PROXY, (e))

#define XA_CLIENT_PROXY_API_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_CLIENT_PROXY, (e))

enum xa_error_nonfatal_api_client_proxy
{
    XA_CLIENT_PROXY_API_NONFATAL_MAX = XA_CLIENT_PROXY_API_NONFATAL(0)
};

enum xa_error_fatal_api_client_proxy
{
    XA_CLIENT_PROXY_API_FATAL_MAX = XA_CLIENT_PROXY_API_FATAL(0)
};

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/

#define XA_CLIENT_PROXY_CONFIG_NONFATAL(e) \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_config, XA_CODEC_CLIENT_PROXY, (e))

#define XA_CLIENT_PROXY_CONFIG_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_config, XA_CODEC_CLIENT_PROXY, (e))

enum xa_error_nonfatal_config_client_proxy
{
    XA_CLIENT_PROXY_CONFIG_NONFATAL_RANGE = XA_CLIENT_PROXY_CONFIG_NONFATAL(0),
    XA_CLIENT_PROXY_CONFIG_NONFATAL_STATE = XA_CLIENT_PROXY_CONFIG_NONFATAL(1),
    XA_CLIENT_PROXY_CONFIG_NONFATAL_MAX   = XA_CLIENT_PROXY_CONFIG_NONFATAL(2)
};

enum xa_error_fatal_config_client_proxy
{
    XA_CLIENT_PROXY_CONFIG_FATAL_RANGE       = XA_CLIENT_PROXY_CONFIG_FATAL(0),
    XA_CLIENT_PROXY_CONFIG_FATAL_TRACK_STATE = XA_CLIENT_PROXY_CONFIG_FATAL(0 + XA_CLIENT_PROXY_CONFIG_NONFATAL_MAX),
    XA_CLIENT_PROXY_CONFIG_FATAL_MAX         = XA_CLIENT_PROXY_CONFIG_FATAL(1)
};

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/

#define XA_CLIENT_PROXY_EXEC_NONFATAL(e) \
    XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_CLIENT_PROXY, (e))

#define XA_CLIENT_PROXY_EXEC_FATAL(e) XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_CLIENT_PROXY, (e))

enum xa_error_nonfatal_execute_client_proxy
{
    XA_CLIENT_PROXY_EXEC_NONFATAL_STATE   = XA_CLIENT_PROXY_EXEC_NONFATAL(0),
    XA_CLIENT_PROXY_EXEC_NONFATAL_NO_DATA = XA_CLIENT_PROXY_EXEC_NONFATAL(1),
    XA_CLIENT_PROXY_EXEC_NONFATAL_INPUT   = XA_CLIENT_PROXY_EXEC_NONFATAL(2),
    XA_CLIENT_PROXY_EXEC_NONFATAL_OUTPUT  = XA_CLIENT_PROXY_EXEC_NONFATAL(3),
    XA_CLIENT_PROXY_EXEC_NONFATAL_MAX     = XA_CLIENT_PROXY_EXEC_NONFATAL(4)
};

enum xa_error_fatal_execute_client_proxy
{
    XA_CLIENT_PROXY_EXEC_FATAL_STATE  = XA_CLIENT_PROXY_EXEC_FATAL(0),
    XA_CLIENT_PROXY_EXEC_FATAL_INPUT  = XA_CLIENT_PROXY_EXEC_FATAL(1),
    XA_CLIENT_PROXY_EXEC_FATAL_OUTPUT = XA_CLIENT_PROXY_EXEC_FATAL(2),
    XA_CLIENT_PROXY_EXEC_FATAL_MAX    = XA_CLIENT_PROXY_EXEC_FATAL(3)
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
DLL_SHARED xa_codec_func_t xa_client_proxy;
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __CLIENT_PROXY_API_H */
