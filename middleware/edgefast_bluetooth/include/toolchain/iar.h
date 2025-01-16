/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IAR_TOOLCHAIN_H_
#define _IAR_TOOLCHAIN_H_

#include "inttypes.h"
#include "stdbool.h"
#include "stdalign.h"


#if (defined(CONFIG_ARCH_POSIX) && (CONFIG_ARCH_POSIX > 0))
#include <arch/posix/posix_trace.h>
/*let's not segfault if this were to happen for some reason*/
#define CODE_UNREACHABLE \
{\
	posix_print_error_and_exit("CODE_UNREACHABLE reached from %s:%d\n",\
		__FILE__, __LINE__);\
	__builtin_unreachable(); \
}
#else
#define CODE_UNREACHABLE __builtin_unreachable()
#endif

#ifndef FUNC_NORETURN
#define FUNC_NORETURN __attribute__((__noreturn__))
#endif /* FUNC_NORETURN */

#define CONFIG_LIBC_ERRNO

#define __used		//__attribute__((__used__))
#define __unused	__attribute__((__unused__))
#define __maybe_unused	__attribute__((__unused__))

#ifndef __deprecated
#define __deprecated __attribute__((deprecated))
#endif

#ifndef __must_check
#define __must_check __attribute__((warn_unused_result))
#endif

#define __SIZEOF_LONG_LONG__ 64U
#define __SIZEOF_LONG__      32U
#define __CHAR_BIT__         8U

#if !defined(__ssize_t_defined)
#define __ssize_t_defined
typedef int ssize_t;
#endif

#define __fallthrough __attribute__((fallthrough))

#define __asm__ __asm
#define __volatile__ volatile

#ifndef __alignof__
#define __alignof__ __ALIGNOF__
#endif

#ifndef __alignof
#define __alignof __ALIGNOF__
#endif

#define __noinit
#define __noinit_named(x)

#ifndef __ram_aligned
/* disable misra 19.13 */
_Pragma("diag_suppress=Pm120")
#define __aligned_internal(x) _Pragma(#x)
_Pragma("diag_default=Pm120")
#define __ram_aligned(n) __aligned_internal(data_alignment=n)
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((__aligned__(x))) // _Alignas(x)
#endif

#ifndef _OFF_T_DECLARED
typedef int off_t; /* file offset */
#define _OFF_T_DECLARED
#endif

#define __SIZE_TYPE__ size_t

#ifndef __builtin_types_compatible_p
#define __builtin_types_compatible_p(a,b) 0U
#endif

#ifndef __typeof__
#define __typeof__ typeof
#endif

#define __packed __attribute__((__packed__))

#define likely(x)   __builtin_expect((bool)!!(x), true)
#define unlikely(x) __builtin_expect((bool)!!(x), false)

/* Unaligned access */
#define UNALIGNED_GET(p) \
__extension__ ({							\
	struct  __attribute__((__packed__)) {				\
		__typeof__(*(p)) __v;					\
	} *__p = (__typeof__(__p)) (p);					\
	__p->__v;							\
})

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

#define __get_rw_section_start(x, type, name) static __section_define(x) type *name = (type *)__section_begin("."#x);
#define __get_rw_section_end(x, type, name) static __section_define(x) type *name = (type *)__section_end("."#x);

#ifdef __cplusplus
#define BUILD_ASSERT(EXPR, MSG...) static_assert(EXPR, "" MSG)
#define BUILD_ASSERT_MSG(EXPR, MSG) static_assert(EXPR, MSG)
#else
#define BUILD_ASSERT(EXPR, MSG...) _Static_assert(EXPR, "" MSG)
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

#define ARG_UNUSED(x) (void)(x)

#define __ERROR(msg) __ERROR1(error msg)
#define __ERROR1(s) _Pragma(#s)

/* Generic message */
#ifndef __DEPRECATED_MACRO
#define __DEPRECATED_MACRO __ERROR("Macro is deprecated")
#endif

#include <toolchain/common.h>

#endif
