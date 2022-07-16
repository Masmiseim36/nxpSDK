/*
 * Copyright 2018, 2019, 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_MACROS_H_
#define _NXP_IOT_AGENT_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_utils EdgeLock 2GO agent generic util functions.
 * @ingroup edgelock2go_agent
 *
 * @addtogroup edgelock2go_agent_utils
 * @{
 */

typedef struct iot_agent_context_t iot_agent_context_t;
typedef struct service_link_t service_link_t;


//
// The following macros simplify and unify error handling for agent-internal calls. They do
// rely on the following variables being available in the current scope:
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto cleanup
// which must be defined in the function scope.

#define EXIT_STATUS_MSG(STATUS, ...)       \
	agent_status = STATUS;                 \
	IOT_AGENT_ERROR(__VA_ARGS__);          \
	goto exit;

#define ASSERT_OR_EXIT_STATUS_MSG(CONDITION, STATUS, ...) \
if (!(CONDITION))                                     \
{                                                     \
	agent_status = STATUS;                 \
	IOT_AGENT_ERROR(__VA_ARGS__);                     \
	goto exit;                                     \
}

#define ASSERT_OR_EXIT_STATUS_SILENT(CONDITION, STATUS) \
if (!(CONDITION))                                     \
{                                                     \
	agent_status = STATUS;                 \
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

#if SSS_HAVE_HOSTCRYPTO_OPENSSL

//
// The following macros simplify and unify error handling for openssl calls. They do
// rely on the following variables being available in the current scope:
//    * int openssl_status
//    * iot_agent_status_t agent_status
// Also, upon error, they want to jump to a label
//    goto exit
// which must be defined in the function scope.

#define OPENSSL_ASSERT_OR_EXIT_STATUS(CONDITION, FUNCTION_NAME, STATUS)       \
if (!(CONDITION))                                         \
{                                                         \
    print_openssl_errors(FUNCTION_NAME);                  \
	agent_status = STATUS;                     \
	goto exit;                                         \
}

#define OPENSSL_ASSERT_OR_EXIT(CONDITION, FUNCTION_NAME)       \
OPENSSL_ASSERT_OR_EXIT_STATUS(CONDITION, FUNCTION_NAME, IOT_AGENT_FAILURE)


#define OPENSSL_SUCCESS_OR_EXIT_STATUS(FUNCTION_NAME, STATUS) \
OPENSSL_ASSERT_OR_EXIT_STATUS(openssl_status == 1, FUNCTION_NAME, STATUS)

#define OPENSSL_SUCCESS_OR_EXIT(FUNCTION_NAME) \
OPENSSL_ASSERT_OR_EXIT(openssl_status == 1, FUNCTION_NAME)

#endif // #if SSS_HAVE_HOSTCRYPTO_OPENSSL

#ifdef __cplusplus
} // extern "C"
#endif
  /*!
  *@}
  */ /* end of edgelock2go_agent_utils */

#endif // #ifndef _NXP_IOT_AGENT_MACROS_H_
