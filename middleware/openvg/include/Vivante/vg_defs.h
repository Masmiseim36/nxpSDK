/****************************************************************************
*
*    Copyright 2022 Vivante Corporation, Santa Clara, California.
*    Copyright 2024 NXP
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

#ifndef __VG_DEFS_H__
#define __VG_DEFS_H__

/*------------------------------------------------------------------------
 * \file
 * \brief    Platform- and compiler-dependent type and macro definitions.
 * \note    
 *-----------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <assert.h>

#if defined (_MSC_VER)
#    define VG_INLINE __forceinline
#elif defined __APPLE__ || defined (__GNUC__) || defined (__GCC32__) || defined(__ICCARM__)
#    define VG_INLINE inline
#endif

#if defined (_MSC_VER) && (_MSC_VER >= 1300)
typedef uintptr_t       VGuintptr;
typedef intptr_t        VGintptr;
#else
typedef unsigned long   VGuintptr;
typedef signed long     VGintptr;
#endif

#define VIV_UINT32_MAX  (0xffffffffu)
#define VIV_INT32_MAX   (0x7fffffff)
#define VIV_INT32_MIN   (-0x7fffffff-1)

/* maximum mantissa is 23 */
#define VIV_MANTISSA_BITS 23

/* maximum exponent is 8 */
#define VIV_EXPONENT_BITS 8

typedef union
{
    VGfloat     f;
    VGuint      i;
} VGfloatInt;

static VG_INLINE float getFloatMax()
{
    VGfloatInt v;
    v.i = (((1<<(VIV_EXPONENT_BITS-1))-1+127) << 23) | (((1<<VIV_MANTISSA_BITS)-1) << (23-VIV_MANTISSA_BITS));
    return v.f;
}
#define VIV_FLOAT_MAX  getFloatMax()

#define VIV_DEBUG   1

#if VIV_DEBUG
#    define VG_ASSERT assert
#else
#    define VG_ASSERT
#endif

#define VG_APIENTRY

#define VIV_MAX_SCISSOR_RECTS               32
#define VIV_MAX_DASH_COUNT                  16
#define VIV_MAX_IMAGE_WIDTH                 4096
#define VIV_MAX_IMAGE_HEIGHT                4096
#define VIV_MAX_IMAGE_PIXELS                (VIV_MAX_IMAGE_WIDTH*VIV_MAX_IMAGE_HEIGHT)
#define VIV_MAX_IMAGE_BYTES                 (4*VIV_MAX_IMAGE_WIDTH*VIV_MAX_IMAGE_HEIGHT)
#define VIV_MAX_COLOR_RAMP_STOPS            256
#define VIV_MAX_KERNEL_SIZE                 256
#define VIV_MAX_SEPARABLE_KERNEL_SIZE       256
#define VIV_MAX_GAUSSIAN_DEVIATION          16.0f
#define VIV_MAX_EDGES                       262144
#define VIV_MAX_SAMPLES                     32
#define VIV_NUM_TESSELLATED_SEGMENTS        256
#define VIV_OBJECTS_HASH                    512

#define VG_FRAMEBUFFER_COUNT                1

#define EGL_ASSERT(a)
#define EGL_UNREF(a)

#endif /* __VG_DEFS_H__ */
