/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_MACROS_SSS_H_
#define _NXP_IOT_AGENT_MACROS_SSS_H_
 
#include "nxp_iot_agent_status.h"
#include "nxp_iot_agent_log.h"

//
// The following macros simplify and unify error handling for sss calls. They do
// rely on the following variables being available in the current scope:
//    * sss_status_t sss_status
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto exit
// which must be defined in the function scope.

#define SSS_SUCCESS_OR_EXIT_STATUS_MSG(STATUS, ...)      \
if ((sss_status_t)kStatus_SSS_Success != sss_status)       \
{                                            \
	IOT_AGENT_ERROR(__VA_ARGS__);            \
	agent_status = STATUS;        \
	goto exit;                            \
}

#define SSS_SUCCESS_OR_EXIT_STATUS(STATUS)             \
SSS_SUCCESS_OR_EXIT_STATUS_MSG(STATUS, "sss_status is not success but [0x%08x]", sss_status)

#define SSS_SUCCESS_OR_EXIT_MSG(...)             \
SSS_SUCCESS_OR_EXIT_STATUS_MSG(IOT_AGENT_FAILURE, __VA_ARGS__)

#define SSS_SUCCESS_OR_EXIT()             \
SSS_SUCCESS_OR_EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "sss_status is not success but [0x%08x]", sss_status)

#endif // _NXP_IOT_AGENT_MACROS_SSS_H_
