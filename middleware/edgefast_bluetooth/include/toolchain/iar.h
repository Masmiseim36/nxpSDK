/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IAR_TOOLCHAIN_H_
#define _IAR_TOOLCHAIN_H_

#include "inttypes.h"

#if !defined(__ssize_t_defined)
#define __ssize_t_defined
typedef int ssize_t;
#endif

#define __asm__ __asm
#define __volatile__ volatile

#ifndef __aligned
#define __aligned(x)
#endif

#define __noinit

#ifndef __ram_aligned
/* disable misra 19.13 */
_Pragma("diag_suppress=Pm120")
#define __aligned_internal(x) _Pragma(#x)
_Pragma("diag_default=Pm120")
#define __ram_aligned(n) __aligned_internal(data_alignment = n)
#endif

#ifndef _OFF_T_DECLARED
typedef int off_t; /* file offset */
#define _OFF_T_DECLARED
#endif

#ifndef STRUCT_PACKED_PRE
#define STRUCT_PACKED_PRE __packed
#endif

#ifndef STRUCT_PACKED_POST
#define STRUCT_PACKED_POST
#endif

#ifndef UNION_PACKED_PRE
#define UNION_PACKED_PRE __packed
#endif

#ifndef UNION_PACKED_POST
#define UNION_PACKED_POST
#endif

#ifndef ENUM_PACKED_PRE
#define ENUM_PACKED_PRE
#endif

#ifndef ENUM_PACKED_POST
#define ENUM_PACKED_POST __packed
#endif

#define __SIZE_TYPE__ size_t

#ifndef __builtin_types_compatible_p
#define __builtin_types_compatible_p(a,b) 0U
#endif

#ifndef __typeof__
#define __typeof__
#endif

#define __used

/* Unaligned access */
#define UNALIGNED_GET(p) sys_get_le32((const uint8_t *)p)

#if 0
#define ___in_section_pragma(str) _Pragma(#str)
#define ___in_section_locate(sec) ___in_section_pragma(location = #sec)
#define ___in_section(a, b, c)                          \
    ___in_section_locate("." _STRINGIFY(a)          \
                "." _STRINGIFY(b)       \
                "." _STRINGIFY(c))
#else
#define ___in_section(a, b, c) @ "."#a"."#b"."#c
#endif

#define __in_section(a, b, c) ___in_section(a, b, c)

#define __in_section_unique(seg) ___in_section(seg, pool, __COUNTER__)

#define __section_define_internal(s) _Pragma(#s)
#define __section_define(x) __section_define_internal(section="."#x)
#define __get_section_start(x, type, name) static __section_define(x) type *name = (type *)__section_begin("."#x);
#define __get_section_end(x, type, name) static __section_define(x) type *name = (type *)__section_end("."#x);

#ifdef __cplusplus
#define BUILD_ASSERT(EXPR) static_assert(EXPR, "")
#define BUILD_ASSERT_MSG(EXPR, MSG) static_assert(EXPR, MSG)
#else
#define BUILD_ASSERT(EXPR) _Static_assert(EXPR, "")
#define BUILD_ASSERT_MSG(EXPR, MSG) _Static_assert(EXPR, MSG)
#endif

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 0
#endif
#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1
#endif
#ifndef __BYTE_ORDER__
#define __BYTE_ORDER__ __LITTLE_ENDIAN__
#endif

#define __deprecated
#define ARG_UNUSED(x) (void)(x)

#define __ERROR(msg) __ERROR1(error msg)
#define __ERROR1(s) _Pragma(#s)

/* Generic message */
#ifndef __DEPRECATED_MACRO
#define __DEPRECATED_MACRO __ERROR("Macro is deprecated")
#endif

#include <toolchain/common.h>

#endif
