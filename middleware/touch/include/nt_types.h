/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_TYPES_H_
#define _NT_TYPES_H_

#include <stdint.h>
#include <stdlib.h>

/**
 * \defgroup types General Types
 * \ingroup ntapi
 * The standard types used in the whole NXP Touch software library. The code is
 * built on the standard library types, such as uint32_t, int8_t, and so on, loaded from
 * "stdint.h", and it defines just few additional types needed to run the NT.
 *
 * \{
 */

#ifndef NULL
/**
* \brief Standard NULL pointer.
*              There is a definition in the NT, in case that NULL is not defined in the project
              previously.
*/
#define NULL (void *)0
#endif

/** Generic flags for NT processing. */
#define NT_FLAGS_SYSTEM_SHIFT(x)   ((x) + 0)
#define NT_FLAGS_SPECIFIC_SHIFT(x) ((x) + 16)

/**
 * \brief The NXP Touch result of most operations / API. The standard API function
 *  returns the result of the finished operation if needed, and it can have
 *  the following values.
 */
enum nt_result
{
    NT_SUCCESS          = 0,  /*!< Successful result - Everything is all right. */
    NT_FAILURE          = -1, /*!< Something is wrong, unspecified error.*/
    NT_OUT_OF_MEMORY    = -2, /*!< The NT does not have enough memory.*/
    NT_SCAN_IN_PROGRESS = -3, /*!< The scan is currently in progress.*/
    NT_NOT_SUPPORTED    = -4, /*!< The feature is not supported.*/
    NT_INVALID_RESULT   = -5, /*!< The function ends with an invalid result.*/
};

/**
 * \brief NT_FREEMASTER_SUPPORT enables the support of FreeMASTER for the NXP Touch
 *  project. When it is enabled, the NT starts using / including the freemaster.h file.
 *  NT_FREEMASTER_SUPPORT is enabled by default.
 */
#ifndef NT_FREEMASTER_SUPPORT
#define NT_FREEMASTER_SUPPORT 1
#endif

/**
 * \brief NT_SAFETY_SUPPORT enables the support of safety function for the NXP Touch
 *  project. When it is enabled, the NT starts using / including the safety files.
 *  NT_SAFETY_SUPPORT is enabled by default.
 */
#ifndef NT_SAFETY_SUPPORT
#define NT_SAFETY_SUPPORT 1
#endif

/**
 * \brief NT_DEBUG enables the debugging code that caused the assert states in the NT.
 *  For the release compilation, this option should be disabled.
 *  NT_DEBUG is enabled by default, which enables the NT ASSERTS.
 */
#ifndef NT_DEBUG
#define NT_DEBUG 1
#endif

#if (NT_DEBUG == 0)
#define NT_ASSERT(expr) ((void)0)
#else
#define NT_ASSERT(expr)                     \
    {                                       \
        if (!(bool)(expr))                  \
        {                                   \
            char p[] = "__FILE__";          \
            nt_error((char *)&p, __LINE__); \
        }                                   \
    }
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/** \} */ /* end of types group */

#endif /* _NT_TYPES_H_ */
