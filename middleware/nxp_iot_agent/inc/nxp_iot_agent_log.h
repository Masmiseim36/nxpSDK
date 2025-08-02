/* 
 * Copyright 2018-2021,2023-2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#ifndef _NXP_IOT_AGENT_LOG_H_
#define _NXP_IOT_AGENT_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <nxp_iot_agent_config.h>

# ifdef __cplusplus
extern "C" {
# endif /* ifdef __cplusplus */


#if defined ( _WIN32 )
#define __func__ __FUNCTION__
#endif

// Log macros use printf function; only for Freertos target the printf function
// should be forwarded to the PRINTF function implemented in debug console
#if defined(SDK_OS_FREE_RTOS)
#include "fsl_debug_console.h"
#if defined(printf)
#undef printf
#endif
#define printf PRINTF
#endif
	
/**
 * @brief Debug level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
# if defined(ENABLE_IOT_AGENT_DEBUG) && ENABLE_IOT_AGENT_DEBUG
#  define IOT_AGENT_DEBUG(...)                       \
  do {                                               \
    printf("DEBUG:   %s L#%d ", __func__, __LINE__); \
    printf(__VA_ARGS__);                             \
    printf("\n");                                    \
  } while(false)
# else  /* ifdef ENABLE_IOT_AGENT_DEBUG */
#  define IOT_AGENT_DEBUG(...)
# endif /* ifdef ENABLE_IOT_AGENT_DEBUG */

/**
 * @brief Debug level trace logging macro.
 *
 * Macro to print message function entry and exit
 */
# if defined(ENABLE_IOT_AGENT_TRACE) && ENABLE_IOT_AGENT_TRACE
#  define IOT_AGENT_FUNC_ENTRY                              \
  do {                                                      \
    printf("FUNC_ENTRY:   %s L#%d \n", __func__, __LINE__); \
  } while(false)
#  define IOT_AGENT_FUNC_EXIT                               \
  do {                                                      \
    printf("FUNC_EXIT:   %s L#%d \n", __func__, __LINE__);  \
  } while(false)
#  define IOT_AGENT_FUNC_EXIT_RC(x)                                            \
  do {                                                                         \
    printf("FUNC_EXIT:   %s L#%d return code : %d \n", __func__, __LINE__, x); \
    return x;                                                                  \
  } while(false)
#  define IOT_AGENT_TRACE(...)                       \
  do {                                               \
    printf("TRACE:   %s L#%d ", __func__, __LINE__); \
    printf(__VA_ARGS__);                             \
    printf("\n");                                    \
  } while(false)

# else  /* ifdef ENABLE_IOT_AGENT_TRACE */
#  define IOT_AGENT_FUNC_ENTRY

#  define IOT_AGENT_FUNC_EXIT
#  define IOT_AGENT_FUNC_EXIT_SUCCESS(x) do { return x; } while(false)
#  define IOT_AGENT_TRACE(...)
# endif /* ifdef ENABLE_IOT_AGENT_TRACE */

/**
 * @brief Info level logging macro.
 *
 * Macro to expose desired log message.  Info messages do not include automatic
 *function names and line numbers.
 */
# if defined(ENABLE_IOT_AGENT_INFO) && ENABLE_IOT_AGENT_INFO
#  define IOT_AGENT_INFO(...) \
  for (;;) {                        \
    printf(__VA_ARGS__);      \
    printf("\r\n");             \
    break;                  \
  }
# else  /* ifdef ENABLE_IOT_AGENT_INFO */
#  define IOT_AGENT_INFO(...)
# endif /* ifdef ENABLE_IOT_AGENT_INFO */

/**
 * @brief Warn level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
# if defined(ENABLE_IOT_AGENT_WARN) && ENABLE_IOT_AGENT_WARN
#  define IOT_AGENT_WARN(...)                      \
  for (;;) {                                             \
    printf("WARN:  %s L#%d ", __func__, __LINE__); \
    printf(__VA_ARGS__);                           \
    printf("\r\n");                                  \
    break;                                          \
  }
# else  /* ifdef ENABLE_IOT_AGENT_WARN */
#  define IOT_AGENT_WARN(...)
# endif /* ifdef ENABLE_IOT_AGENT_WARN */

/**
 * @brief Error level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
# if defined(ENABLE_IOT_AGENT_ERROR) && ENABLE_IOT_AGENT_ERROR
#  define IOT_AGENT_ERROR(...)                     \
  for (;;) {                                                \
    printf("ERROR: %s L#%d ", __func__, __LINE__); \
    printf(__VA_ARGS__);                           \
    printf("\r\n");                                  \
    break;                                         \
  }

#  define IOT_AGENT_FUNC_EXIT_ERROR(x)                                             \
  for (;;) {                                                                             \
    printf("FUNC_EXIT_ERROR: %s L#%d return code : %d \r\n", __func__, __LINE__, x); \
    return x;                                                                      \
  }

# else  /* ifdef ENABLE_IOT_AGENT_ERROR */
#  define IOT_AGENT_ERROR(...)
#  define IOT_AGENT_FUNC_EXIT_ERROR(x)
# endif /* ifdef ENABLE_IOT_AGENT_ERROR */


#if defined(IOT_AGENT_TEST)
#    define IOT_AGENT_TEST_LOG(ID, ...)    \
  for (;;) {                                 \
        printf("TEST_LOG(%s): ", ID);          \
		printf(__VA_ARGS__);           \
		printf("\n");                  \
		break;      \
  }

#else
#    define IOT_AGENT_TEST_LOG(...)
#endif

# ifdef __cplusplus
}
# endif /* ifdef __cplusplus */

#endif // _NXP_IOT_AGENT_LOG_H_
