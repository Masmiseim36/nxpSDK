/****************************************************************************
*
*    Copyright 2022 Vivante Corporation, Santa Clara, California.
*    All Rights Reserved.
*
*    Permission is hereby granted, free of charge, to any person obtaining
*    a copy of this software and associated documentation files (the
*    'Software'), to deal in the Software without restriction, including
*    without limitation the rights to use, copy, modify, merge, publish,
*    distribute, sub license, and/or sell copies of the Software, and to
*    permit persons to whom the Software is furnished to do so, subject
*    to the following conditions:
*
*    The above copyright notice and this permission notice (including the
*    next paragraph) shall be included in all copies or substantial
*    portions of the Software.
*
*    THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
*    IN NO EVENT SHALL VIVANTE AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
*    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#ifndef __VG_MATH_H__
#define __VG_MATH_H__

#include "vg_defs.h"

#define    PI  3.141592654f

static VG_INLINE VGint VG_ISNAN(float a)
{
    VGfloatInt p;
    p.f = a;
    unsigned int exponent = (p.i>>23) & 0xff;
    unsigned int mantissa = p.i & 0x7fffff;
    if (exponent == 255 && mantissa)
        return 1;
    return 0;
}

typedef float VGfloat;

static VG_INLINE VGfloat   VG_MAX(VGfloat a, VGfloat b)                { return (a > b) ? a : b; }
static VG_INLINE VGfloat   VG_MIN(VGfloat a, VGfloat b)                { return (a < b) ? a : b; }
static VG_INLINE VGfloat   VG_CLAMP(VGfloat a, VGfloat l, VGfloat h)   { if (VG_ISNAN(a)) return l; VG_ASSERT(l <= h); return (a < l) ? l : (a > h) ? h : a; }
static VG_INLINE VGfloat   VG_ABS(VGfloat a)                           { return (a < 0.0f) ? -a : a; }
static VG_INLINE VGfloat   VG_SQR(VGfloat a)                           { return a * a; }
static VG_INLINE VGfloat   VG_DEG_TO_RAD(VGfloat a)                    { return a * PI / 180.0f; }
static VG_INLINE VGfloat   VG_RAD_TO_DEG(VGfloat a)                    { return a * 180.0f/ PI; }
static VG_INLINE VGfloat   VG_MOD(VGfloat a, VGfloat b)                { if (VG_ISNAN(a) || VG_ISNAN(b)) return 0.0f; VG_ASSERT(b >= 0.0f); if (b == 0.0f) return 0.0f; VGfloat f = (VGfloat)fmod(a, b); if (f < 0.0f) f += b; VG_ASSERT(f >= 0.0f && f <= b); return f; }

static VG_INLINE VGint     VG_INT_MAX(VGint a, VGint b)                { return (a > b) ? a : b; }
static VG_INLINE VGint     VG_INT_MIN(VGint a, VGint b)                { return (a < b) ? a : b; }
static VG_INLINE VGint     VG_INT_MOD(VGint a, VGint b)                { VG_ASSERT(b >= 0); if (!b) return 0; VGint i = a % b; if (i < 0) i += b; VG_ASSERT(i >= 0 && i < b); return i; }
static VG_INLINE VGint     VG_INT_ADDSATURATE(VGint a, VGint b)        { VG_ASSERT(b >= 0); VGint r = a + b; return (r >= a) ? r : VIV_INT32_MAX; }


typedef struct {
    VGfloat matrix[3][3];
} Matrix3x3;

typedef struct {
    VGfloat x;
    VGfloat y;
} Vector2;

typedef struct {
    VGfloat x;
    VGfloat y;
    VGfloat z;
} Vector3;


#endif /* __VG_MATH_H__ */
