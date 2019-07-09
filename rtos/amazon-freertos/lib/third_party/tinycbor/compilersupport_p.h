/****************************************************************************
**
** Copyright (C) 2015 Intel Corporation
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/

#ifndef COMPILERSUPPORT_H
#define COMPILERSUPPORT_H

#include "cbor.h"

#ifndef _BSD_SOURCE
#  define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE
#endif
#include <assert.h>
#include <float.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef __cplusplus
#  include <stdbool.h>
#endif

#ifdef __F16C__
#  include <immintrin.h>
#endif

#if __STDC_VERSION__ >= 201112L || __cplusplus >= 201103L || __cpp_static_assert >= 200410
#  define cbor_static_assert(x)         static_assert(x, #x)
#elif !defined(__cplusplus) && defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406) && (__STDC_VERSION__ > 199901L)
#  define cbor_static_assert(x)         _Static_assert(x, #x)
#else
#  define cbor_static_assert(x)         ((void)sizeof(char[2*!!(x) - 1]))
#endif
#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
/* inline is a keyword */
#else
/* use the definition from cbor.h */
#  define inline    CBOR_INLINE
#endif

#ifndef STRINGIFY
#define STRINGIFY(x)            STRINGIFY2(x)
#endif
#define STRINGIFY2(x)           #x

#if !defined(UINT32_MAX) || !defined(INT64_MAX)
/* C89? We can define UINT32_MAX portably, but not INT64_MAX */
#  error "Your system has stdint.h but that doesn't define UINT32_MAX or INT64_MAX"
#endif

#ifndef DBL_DECIMAL_DIG
/* DBL_DECIMAL_DIG is C11 */
#  define DBL_DECIMAL_DIG       17
#endif
#define DBL_DECIMAL_DIG_STR     STRINGIFY(DBL_DECIMAL_DIG)

#ifndef __has_builtin
#  define __has_builtin(x)  0
#endif

#ifdef __cplusplus
#  define CONST_CAST(t, v)  const_cast<t>(v)
#else
/* C-style const_cast without triggering a warning with -Wcast-qual */
#  define CONST_CAST(t, v)  (t)(uintptr_t)(v)
#endif

#ifdef __GNUC__
#ifndef likely
#  define likely(x)     __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#  define unlikely(x)   __builtin_expect(!!(x), 0)
#endif
#  define unreachable() __builtin_unreachable()
#elif defined(_MSC_VER)
#  define likely(x)     (x)
#  define unlikely(x)   (x)
#  define unreachable() __assume(0)
#else
#  define likely(x)     (x)
#  define unlikely(x)   (x)
#  define unreachable() do {} while (0)
#endif

#if defined(__GNUC__) && !defined(__INTEL_COMPILER) && !defined(__clang__) && \
    (__GNUC__ * 100 + __GNUC_MINOR__ >= 404)
#  pragma GCC optimize("-ffunction-sections")
#endif

static inline bool add_check_overflow(size_t v1, size_t v2, size_t *r)
{
#if ((defined(__GNUC__) && (__GNUC__ >= 5)) && !defined(__INTEL_COMPILER)) || __has_builtin(__builtin_add_overflow)
    return __builtin_add_overflow(v1, v2, r);
#else
    /* unsigned additions are well-defined */
    *r = v1 + v2;
    return v1 > v1 + v2;
#endif
}

static inline unsigned short encode_half(double val)
{
#ifdef __F16C__
    return _cvtss_sh(val, 3);
#else
    uint64_t v;
    memcpy(&v, &val, sizeof(v));
    int sign = v >> 63 << 15;
    int exp = (v >> 52) & 0x7ff;
    int mant = v << 12 >> 12 >> (53-11);    /* keep only the 11 most significant bits of the mantissa */
    exp -= 1023;
    if (exp == 1024) {
        /* infinity or NaN */
        exp = 16;
        mant >>= 1;
    } else if (exp >= 16) {
        /* overflow, as largest number */
        exp = 15;
        mant = 1023;
    } else if (exp >= -14) {
        /* regular normal */
    } else if (exp >= -24) {
        /* subnormal */
        mant |= 1024;
        mant >>= -(exp + 14);
        exp = -15;
    } else {
        /* underflow, make zero */
        return 0;
    }

    /* safe cast here as bit operations above guarantee not to overflow */
    return (unsigned short)(sign | ((exp + 15) << 10) | mant);
#endif
}

#endif /* COMPILERSUPPORT_H */
