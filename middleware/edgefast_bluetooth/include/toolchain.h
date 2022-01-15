/*
 * Copyright 2021 NXP
 * Copyright (c) 2010-2014, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Macros to abstract toolchain specific capabilities
 *
 * This file contains various macros to abstract compiler capabilities that
 * utilize toolchain specific attributes and/or pragmas.
 */

#ifndef ZEPHYR_INCLUDE_TOOLCHAIN_H_
#define ZEPHYR_INCLUDE_TOOLCHAIN_H_

#ifndef __ASSERT
#define __ASSERT(test, fmt, ...) assert(test)
#endif /* __ASSERT */

#ifndef __ASSERT_NO_MSG
#define __ASSERT_NO_MSG(test) assert(test)
#endif /* __ASSERT_NO_MSG */

/**
 * @def HAS_BUILTIN(x)
 * @brief Check if the compiler supports the built-in function \a x.
 *
 * This macro is for use with conditional compilation to enable code using a
 * builtin function that may not be available in every compiler.
 */
#ifdef __has_builtin
#define HAS_BUILTIN(x) __has_builtin(x)
#else
/*
 * The compiler doesn't provide the __has_builtin() macro, so instead we depend
 * on the toolchain-specific headers to define HAS_BUILTIN_x for the builtins
 * supported.
 */
#define HAS_BUILTIN(x) HAS_BUILTIN_##x
#endif

#if defined(__XCC__)
#include <toolchain/xcc.h>
#elif defined(__CCAC__)
#include <toolchain/mwdt.h>
#elif defined(__CC_ARM) || (defined(__ARMCC_VERSION))
#include <toolchain/armcc.h>
#elif defined(__GNUC__) || (defined(_LINKER) && defined(__GCC_LINKER_CMD__))
#include <toolchain/gcc.h>
#elif defined(__ICCARM__)
#include <toolchain/iar.h>
#else
/* This include line exists for off-tree definitions of compilers,
 * and therefore this header is not meant to exist in-tree
 */
#include <toolchain/other.h>
#endif

/*
 * Ensure that __BYTE_ORDER__ and related preprocessor definitions are defined,
 * as these are often used without checking for definition and doing so can
 * cause unexpected behaviours.
 */
#ifndef _LINKER
#if !defined(__BYTE_ORDER__) || !defined(__ORDER_BIG_ENDIAN__) || \
    !defined(__ORDER_LITTLE_ENDIAN__)

#error "__BYTE_ORDER__ is not defined"

#endif
#endif /* !_LINKER */

#endif /* ZEPHYR_INCLUDE_TOOLCHAIN_H_ */
