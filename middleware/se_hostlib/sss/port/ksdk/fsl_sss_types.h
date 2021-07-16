/*
 * Copyright 2018,2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef FSL_SSS_TYPES_H
#define FSL_SSS_TYPES_H

#include <fsl_common.h>

/*! @brief Compile time sizeof() check */
#define SSCP_BUILD_ASSURE(condition, msg) extern int msg[1 - 2 * (!(condition))] __attribute__((unused))

#define SSS_ASSERT_LINE(condition, LINE) extern int msg_##LINE[1 - 2 * (!(condition))] __attribute__((unused))

#define SSS_ASSERT(condition) SSS_ASSERT_LINE(condition, __LINE__)

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

/* snprintf definition for MSVisualC */
#ifndef SNPRINTF
#define SNPRINTF snprintf
#endif /*SNPRINTF */

#define STRNICMP strncasecmp

/* doc:start:sss-heap_mgmt */
#if defined(USE_RTOS) && USE_RTOS == 1
#include "FreeRTOS.h"

void *pvPortCalloc(size_t num, size_t size); /*Calloc for Heap3/Heap4.*/

#ifndef SSS_MALLOC
#define SSS_MALLOC pvPortMalloc
#endif // SSS_MALLOC

#ifndef SSS_FREE
#define SSS_FREE vPortFree
#endif // SSS_FREE

#ifndef SSS_CALLOC
#define SSS_CALLOC pvPortCalloc
#endif // SSS_CALLOC

#else // !USE_RTOS

#include <stdlib.h>

#ifndef SSS_MALLOC
#define SSS_MALLOC malloc
#endif // SSS_MALLOC

#ifndef SSS_FREE
#define SSS_FREE free
#endif // SSS_FREE

#ifndef SSS_CALLOC
#define SSS_CALLOC calloc
#endif // SSS_CALLOC

#endif // USE_RTOS
/* doc:end:sss-heap_mgmt */

#endif /* FSL_SSS_TYPES_H */
