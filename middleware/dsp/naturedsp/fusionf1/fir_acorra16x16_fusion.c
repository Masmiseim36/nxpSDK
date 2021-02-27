/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    Real data circular auto-correlation, 16x16-bit, no requirements on vectors
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.
  These functions implement the circular autocorrelation algorithm described
  in the previous chapter with no limitations on x vector length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16   16-bit data, 16-bit outputs
  24x24   24-bit data, 24-bit outputs
  32x32   32-bit data, 32-bit outputs
  f       floating point

  Input:
  s[]       scratch area of
            FIR_ACORRA16X16_SCRATCH_SIZE( N )
            FIR_ACORRA24X24_SCRATCH_SIZE( N )
            FIR_ACORRA32X32_SCRATCH_SIZE( N )
            FIR_ACORRAF_SCRATCH_SIZE( N )    
            bytes
  x[N]      input data Q31, Q15 or floating point
  N         length of x
  Output:
  r[N]      output data, Q31, Q15 or floating point

  Restrictions:
  x,r,s should not overlap
  N >0
  s   - aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr16x16.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_acorra16x16(void           * restrict s,
                     int16_t        * restrict r,
                     const int16_t  * restrict x, int N)
{
    //
    // Circular autocorrelation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*x[m] )
    //        m=0..N-1
    //
    // where n = 0..N-1. Throughout the algorithm implementation, x[mod(n+m,N)],
    // m = 0..N-1 is referred to as the left-hand array, while x[m] is called
    // the right-hand array, and it is accessed through the Y pointer.
    void     * s_ptr;
    int16_t  * x_buf;
    int16_t  * y_buf;
    const ae_int16x4 * S;
          ae_int16x4 * restrict D;
          ae_int16   * restrict D_s;

    ae_valign S_va, D_va;
    ae_int16x4 z_f16x4;

    ae_int16x4 x0, y0;

    int n, m;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(s, 8);
    NASSERT(N>0);

    //
    // Partition the scratch memory.
    s_ptr = s;
    x_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + 2 * N + 3;
    y_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = y_buf + N + 3;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_ACORRA16X16_SCRATCH_SIZE(N));
    //----------------------------------------------------------------------------
    // Copy x[N] into the scratch are in a way that simplifies the 
    // autocorrelation computation:
    //   x[0]..x[N-1] x[0]..x[N-1] 0 0 0
    // First copy of x[N].
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)x_buf;
    S_va = AE_LA64_PP(S);
#ifdef COMPILER_XTENSA
#pragma concurrent
#endif
    for (n = 0; n<((N + 3) >> 2); n++)
    {
        AE_LA16X4_IP(x0, S_va, S);
        AE_S16X4_IP(x0, D, +8);
    }
    // Second copy of x[N].
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)((int16_t *)x_buf + N);
    S_va = AE_LA64_PP(S);
    D_va = AE_ZALIGN64();
#ifdef COMPILER_XTENSA
#pragma concurrent
#endif
    for (n = 0; n<((N + 3) >> 2); n++)
    {
        AE_LA16X4_IP(x0, S_va, S);
        AE_SA16X4_IP(x0, D_va, D);
    }
    AE_SA64POS_FP(D_va, D);
    // Append 3 zeros to allow block-4 processing for the inner loop of the 
    // autocorrelation algorithm.
    D_s = (ae_int16 *)((int16_t *)x_buf + (N<<1));
    z_f16x4 = AE_ZERO16();
    for (n = 0; n<4; n++)
    {
        AE_S16_0_IP(z_f16x4, D_s, +2);
    }

    //----------------------------------------------------------------------------
    // Copy y[M] data into the aligned buffer and append 3 zeros:
    //  y_buf[M+3] = { y[0]..y[M-1] 0 0 0 }
    S = (const ae_int16x4 *)x;
    D = (ae_int16x4 *)y_buf;
    S_va = AE_LA64_PP(S);
    for (m = 0; m<((N + 3) >> 2); m++)
    {
        AE_LA16X4_IP(y0, S_va, S);
        AE_S16X4_IP(y0, D, +8);
    }
    // Append three zeros to allow for block-4 processing.
    D_s = (ae_int16 *)((int16_t *)y_buf + N);
    z_f16x4 = AE_ZERO16();
    for (m = 0; m<3; m++)
    {
        AE_S16_0_IP(z_f16x4, D_s, +2);
    }

    raw_corr16x16(r, x_buf, y_buf, N, (N + 3)&~3);
} // fir_acorra16x16()
