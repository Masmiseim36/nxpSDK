/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _NXP_IOT_AGENT_COMMON_H_
#define _NXP_IOT_AGENT_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL & NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS)
#        error "Only one between NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL and NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS can be set to 1"
#endif

#if ((NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL | NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS) == 0)
// if the crypto is not choosen externally fall back to mbedtls
#undef NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS
#define NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS 1
#endif

#if NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS

#if (NXP_IOT_AGENT_HAVE_SSS & NXP_IOT_AGENT_HAVE_PSA)
#        error "Only one between NXP_IOT_AGENT_HAVE_SSS and NXP_IOT_AGENT_HAVE_PSA can be set to 1"
#endif
#if ((NXP_IOT_AGENT_HAVE_SSS | NXP_IOT_AGENT_HAVE_PSA) == 0)
// if the crypto is not choosen externally fall back to PSA
#undef NXP_IOT_AGENT_HAVE_PSA
#define NXP_IOT_AGENT_HAVE_PSA 1
#endif

#if NXP_IOT_AGENT_HAVE_PSA

// following defines decide if PSA implementation is handled via TF-M or SMW
#ifndef NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
#define NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM 0
#endif

#ifndef NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW
#define NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW	0
#endif

#ifndef NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL
#define NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL 0
#endif

#if (NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM & NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW & NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL)
#        error "Only one between NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM and NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW can be set to 1"
#endif

#if ((NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM | NXP_IOT_AGENT_HAVE_PSA_IMPL_SMW | NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL) == 0)
// If the build system enables none of the PSA implementations, fall back to
// the software-simulator implementation.
#undef NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL
#define NXP_IOT_AGENT_HAVE_PSA_IMPL_SIMUL 1
#endif

#endif // NXP_IOT_AGENT_HAVE_PSA
#endif // NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS

#if AX_EMBEDDED

 // A freescale cpu

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_types.h>
#define COMPILE_TIME_ASSERT(condition) SSCP_BUILD_ASSURE(condition,compile_time_assert_failed)
#else
#define COMPILE_TIME_ASSERT(condition) extern int msg[1 - 2 * (!(condition))] __attribute__((unused))
#endif

#else

#if defined(__clang__) || defined(__ICCARM__)
// Clang. This can be windows or linux or mingw or .... Attention, clang in those cases ALSO
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
/** Endpoint types. Those need to be aligned to the endpoint
 * types on the protocol layer. This is guarded by compile time
 * assertion in dispatcher.c!
 */
#define IOT_AGENT_KS_SSS_A71CH_NOT_SUPPORTED    (10U)
#define IOT_AGENT_KS_SSS_SE05X                  (11U)
#define IOT_AGENT_KS_SSS_RPC_NOT_SUPPORTED      (20U)
#define IOT_AGENT_KS_PSA                        (21U)
#define IOT_AGENT_DS_FS                         (50U)
#define IOT_AGENT_DS_PLAIN                      (51U)

/** Cloud specific Serice types. Those need to be aligned to the serivice
 * types on the protocol layer. This is guarded by compile time
 * assertion in dispatcher.c!
 */
#define IOT_AGENT_AWSSERVICE	(0)
#define IOT_AGENT_GOOGLESERVICE	(1)
#define IOT_AGENT_AZURESERVICE	(2)
#define IOT_AGENT_IBMSERVICE	(3)
#define IOT_AGENT_CUSTOMSERVICE	(15)

/** Protocol types. This is guarded by compile time
 * assertion in dispatcher.c!
 */
#define IOT_AGENT_PROTOCOL_HTTPS    (1)
#define IOT_AGENT_PROTOCOL_MQTTS    (2)
#define IOT_AGENT_PROTOCOL_AMQPS    (3)
#define IOT_AGENT_PROTOCOL_XMPP     (4)
#define IOT_AGENT_PROTOCOL_DDS      (5)
#define IOT_AGENT_PROTOCOL_COAP     (6)

#define IOT_AGENT_KEYSTORE_ID_SEMSLITE  (1)

/** The length of a correlation ID as used by the cloud service.
 *
 * These are UUIDs: 32 hex digits + 4 dashes + \0.
 */
#define NXP_IOT_AGENT_CORRELATION_ID_LENGTH (37)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef _NXP_IOT_AGENT_COMMON_H_
