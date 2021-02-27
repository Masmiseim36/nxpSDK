/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FIR part
    Real data linear cross-correlation, 16x16-bit, no requirements on vectors 
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Linear Correlation
  Functions estimate the linear cross-correlation between vectors x (of 
  length N) and y (of length M) resulting in vector r of length N+M-1. It 
  is similar to convolution but y is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_LXCORRA16X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X32_SCRATCH_SIZE(N, M) 
                FIR_LXCORRAF_SCRATCH_SIZE(N, M)     
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N+M-1]      output data, Q31, Q15  or floating point

  Restrictions:
  x,y,r,s should not overlap
  s should be aligned on 8-byte boundary
  N>0, M>0
  N>=M-1
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#if 0//!(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
DISCARD_FUN(void, fir_lxcorra16x16, ( void    * restrict s,
                       int16_t * restrict r,
                 const int16_t * restrict x,
                 const int16_t * restrict y,
                       int N, int M))
#else
#include "raw_lcorr16x16.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_lxcorra16x16( void    * restrict s,
                       int16_t * restrict r,
                 const int16_t * restrict x,
                 const int16_t * restrict y,
                       int N, int M)
{
    void     * s_ptr;
    int16_t  * x_buf;
    int16_t  * y_buf;
    const ae_int16x4 *          S;
    ae_int16x4       * restrict D;
    ae_int16         * restrict D_s;
    ae_valign S_va;
    ae_int16x4 y0, x0;
    ae_int16x4 z_f16x4;

    int n, m;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(s, 8);
    NASSERT(N>0 && M>0);
    NASSERT(N >= M - 1);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.
    s_ptr = s;
    x_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + N + 6;
    y_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = y_buf + M + 6;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_LXCORRA16X16_SCRATCH_SIZE(N, M));
    //----------------------------------------------------------------------------
    // Copy x[N].
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)x_buf;
    S_va = AE_LA64_PP(S);
    for (n = 0; n<((N + 3) >> 2); n++)
    {
        AE_LA16X4_IP(x0, S_va, S);
        AE_S16X4_IP(x0, D, +8);
    }

    D_s = (ae_int16 *)((int16_t *)x_buf + N);
    z_f16x4 = AE_ZERO16();
    for (m = 0; m<6; m++)
    {
        AE_S16_0_IP(z_f16x4, D_s, +2);
    }

    //----------------------------------------------------------------------------
    // Copy y[M] data into the aligned buffer and append 6 zeros:
    S = (const ae_int16x4 *)y;
    D = (ae_int16x4 *)y_buf;
    S_va = AE_LA64_PP(S);
    for (m = 0; m<((M + 3) >> 2); m++)
    {
        AE_LA16X4_IP(y0, S_va, S);
        AE_S16X4_IP(y0, D, +8);
    }

    D_s = (ae_int16 *)((int16_t *)y_buf + M);
    z_f16x4 = AE_ZERO16();
    for (m = 0; m<6; m++)
    {
        AE_S16_0_IP(z_f16x4, D_s, +2);
    }

    raw_lcorr16x16(r, x_buf, y_buf, N, M);
} // fir_lxcorra16x16()
#endif
