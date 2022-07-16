/*
 * Copyright 2018, 2019, 2020, 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _NXP_IOT_AGENT_COMMON_H_
#define _NXP_IOT_AGENT_COMMON_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if defined(NXP_IOT_AGENT_CONFIG_FILE)
#include NXP_IOT_AGENT_CONFIG_FILE
#else
#include <nxp_iot_agent_config.h>
#endif

#include <nxp_iot_agent_status.h>
#include <nxp_iot_agent_log.h>

#if defined(SSS_USE_FTR_FILE)
#include <fsl_sss_ftr.h>
#else
#include <fsl_sss_ftr_default.h>
#endif

#include <sm_types.h>

#if AX_EMBEDDED

 // A freescale cpu
#include <fsl_sss_types.h>
#define COMPILE_TIME_ASSERT(condition) SSCP_BUILD_ASSURE(condition,compile_time_assert_failed)

#else

#if defined(__clang__)
// Clang. This can be windows of linux or mingw or .... Attention, clang in those cases ALSO 
// defines _MSC_VER or __GNUC__, so this check MUST BE BEFORE the others.
#define COMPILE_TIME_ASSERT(condition) _Static_assert(condition, "compile_time assert failed")

#elif defined(_MSC_VER)
// Visual studio
#define COMPILE_TIME_ASSERT(condition) static_assert(condition, "compile_time assert failed")

#elif defined(__GNUC__)
// GCC
#define COMPILE_TIME_ASSERT(condition) _Static_assert(condition, "compile_time assert failed")

#endif // compiler selection

#endif // #if AX_EMBEDDED


#ifdef __cplusplus
extern "C" {
#endif


/** Endpoint types. Those need to be aligned to the endpoint
 * types on the protocol layer. This is guarded by compile time
 * assertion in dispatcher.c! */
#define IOT_AGENT_KS_SSS_A71CH_NOT_SUPPORTED    (10U)
#define IOT_AGENT_KS_SSS_SE05X                  (11U)
#define IOT_AGENT_KS_SSS_RPC_NOT_SUPPORTED      (20U)
#define IOT_AGENT_DS_FS                         (50U)
#define IOT_AGENT_DS_PLAIN                      (51U)

/** Cloud specific Serice types. Those need to be aligned to the serivice
* types on the protocol layer. This is guarded by compile time
* assertion in dispatcher.c! */
#define IOT_AGENT_AWSSERVICE	(0)
#define IOT_AGENT_GOOGLESERVICE	(1)
#define IOT_AGENT_AZURESERVICE	(2)
#define IOT_AGENT_IBMSERVICE	(3)
#define IOT_AGENT_CUSTOMSERVICE	(15)

/** Protocol types. This is guarded by compile time
* assertion in dispatcher.c! */
#define IOT_AGENT_PROTOCOL_HTTPS    (1)
#define IOT_AGENT_PROTOCOL_MQTTS    (2)
#define IOT_AGENT_PROTOCOL_AMQPS    (3)
#define IOT_AGENT_PROTOCOL_XMPP     (4)
#define IOT_AGENT_PROTOCOL_DDS      (5)
#define IOT_AGENT_PROTOCOL_COAP     (6)

/** The length of a correlation ID as used by the cloud service. 
 * 
 * These are UUIDs: 32 hex digits + 4 dashes + \0.
 */
#define NXP_IOT_AGENT_CORRELATION_ID_LENGTH (37)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef _NXP_IOT_AGENT_COMMON_H_
