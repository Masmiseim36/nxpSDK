/*
 * Copyright 2018-2021,2024-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_MACROS_H_
#define _NXP_IOT_AGENT_MACROS_H_

#include "nxp_iot_agent_status.h"
#include "nxp_iot_agent_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_utils EdgeLock 2GO agent generic util functions.
 * @ingroup edgelock2go_agent
 *
 * @addtogroup edgelock2go_agent_utils
 * @{
 */

//
// The following macros simplify and unify error handling for agent-internal calls. They do
// rely on the following variables being available in the current scope:
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto cleanup
// which must be defined in the function scope.

#define EXIT_STATUS_MSG(STATUS, ...)       \
	agent_status = (STATUS);                 \
	IOT_AGENT_ERROR(__VA_ARGS__);          \
	goto exit;

#define ASSERT_OR_EXIT_STATUS_MSG(CONDITION, STATUS, ...) \
if (!(CONDITION))                                     \
{                                                     \
	agent_status = (STATUS);                 \
	IOT_AGENT_ERROR(__VA_ARGS__);                     \
	goto exit;                                     \
}

#define ASSERT_OR_EXIT_STATUS_SILENT(CONDITION, STATUS) \
if (!(CONDITION))                                     \
{                                                     \
	agent_status = (STATUS);                 \
	goto exit;                                     \
}

#define ASSERT_OR_EXIT_SILENT(CONDITION) \
ASSERT_OR_EXIT_STATUS_SILENT(CONDITION, IOT_AGENT_FAILURE)

#define ASSERT_OR_EXIT_MSG(CONDITION, ...) \
ASSERT_OR_EXIT_STATUS_MSG(CONDITION, IOT_AGENT_FAILURE, __VA_ARGS__)

#define ASSERT_OR_EXIT_STATUS(CONDITION, STATUS) \
ASSERT_OR_EXIT_STATUS_MSG(CONDITION, STATUS, "assert failed")

#define ASSERT_OR_EXIT(CONDITION) \
ASSERT_OR_EXIT_STATUS_MSG(CONDITION, IOT_AGENT_FAILURE, "assert failed")


//
// The following macros simplify and unify error handling for agent-internal calls. They do
// rely on the following variables being available in the current scope:
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto exit
// which must be defined in the function scope.

#define AGENT_SUCCESS_OR_EXIT_MSG(...)    \
if (IOT_AGENT_SUCCESS != agent_status)       \
{                                            \
	IOT_AGENT_ERROR(__VA_ARGS__);            \
	goto exit;                            \
}

#define AGENT_SUCCESS_OR_EXIT()    \
AGENT_SUCCESS_OR_EXIT_MSG("agent_status is not success but [0x%08x]", agent_status)

//
// The following macros simplify and unify error handling for protobuf encoding/decoding calls which do
// have a boolean return value. They do rely on the following variables being available in the
// current scope:
//    * bool result
// Also, upon error, they want to jump to a label
//    goto exit
// which must be defined in the function scope.

#define RESULT_TRUE_OR_EXIT_MSG(...)      \
if (true != result)                       \
{                                         \
	IOT_AGENT_ERROR(__VA_ARGS__);         \
	result = false;                       \
	goto exit;                            \
}

#define RESULT_TRUE_OR_EXIT()             \
RESULT_TRUE_OR_EXIT_MSG("result is false")

#ifdef _MSC_VER
#define ACCESS _access
#else
#define ACCESS access
#endif

#if ! defined(AX_UNUSED_ARG)
#define AX_UNUSED_ARG(x) (void)(x)
#endif

#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_MACROS_H_
