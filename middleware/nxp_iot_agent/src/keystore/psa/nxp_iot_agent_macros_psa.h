/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_MACROS_PSA_H_
#define _NXP_IOT_AGENT_MACROS_PSA_H_
 
#include "nxp_iot_agent_status.h"
#include "nxp_iot_agent_log.h"

//
// The following macros simplify and unify error handling for psa calls. They do
// rely on the following variables being available in the current scope:
//    * psa_status_t psa_status
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto exit
// which must be defined in the function scope.

#define PSA_SUCCESS_OR_EXIT_STATUS_MSG(STATUS, ...)      \
if (PSA_SUCCESS != psa_status)       \
{                                            \
	IOT_AGENT_ERROR(__VA_ARGS__);            \
	agent_status = (STATUS);        \
	goto exit;                            \
}

#define PSA_SUCCESS_OR_EXIT_STATUS(STATUS)             \
PSA_SUCCESS_OR_EXIT_STATUS_MSG(STATUS, "psa_status is not success but [0x%08x]", psa_status)

#define PSA_SUCCESS_OR_EXIT_MSG(...)             \
PSA_SUCCESS_OR_EXIT_STATUS_MSG(IOT_AGENT_FAILURE, __VA_ARGS__)

#define PSA_SUCCESS_OR_EXIT()             \
PSA_SUCCESS_OR_EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "psa_status is not success but [0x%08x]", psa_status)

#endif // _NXP_IOT_AGENT_MACROS_PSA_H_
