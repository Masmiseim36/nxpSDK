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
    Real data circular convolution, 32x32-bit, no requirements on vectors 
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M) resulting in vector r of length N.
  These functions implement the circular convolution algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  s[]           Scratch memory,
                FIR_CONVOLA16X16_SCRATCH_SIZE( N, M )  
                FIR_CONVOLA24X24_SCRATCH_SIZE( N, M )   
                FIR_CONVOLA32X16_SCRATCH_SIZE( N, M )  
                CXFIR_CONVOLA32X16_SCRATCH_SIZE( N, M )
                FIR_CONVOLA32X32_SCRATCH_SIZE( N, M )  
                FIR_CONVOLAF_SCRATCH_SIZE( N, M )      
                bytes
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr32x32.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_convola32x32(void * restrict s,
    int32_t  * restrict r,
    const int32_t  * restrict x,
    const int32_t  * restrict y,
    int N, int M)
{

    //
    // Circular convolution algorithm:
    //
    //   r[n] = sum( x[mod(n-m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //

    void     * s_ptr;
    int32_t  * x_buf;
    int32_t  * y_buf;

    const ae_f32x2  * S;
    ae_f32x2        * restrict D;

    ae_valign S_va, D_va;
    ae_f32x2   x0, y0;
    ae_int32x2 t0;
    int n, m;

    ASSERT(s && r && x && y && N > 0 && M > 0 && N >= M - 1);

    ASSERT(IS_ALIGN(s));

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.

    s_ptr = s;
    x_buf = (int32_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + M - 1 + N + 3;
    y_buf = (int32_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = y_buf + M + 3;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_CONVOLA32X32_SCRATCH_SIZE(N, M));

    //----------------------------------------------------------------------------
    // Copy x[N] data into the aligned buffer in a way that simplifies the
    // convolution calculation:
    //  x_buf[M-1+N+3] = { x[N-(M-1)]..x[N-1] x[0] x[1]..x[N-1] X X X }
    // Three X locations are reserved to allow for block-4 processing.
    // Copy last M-1 entries of x{N].
    S = (const ae_f32x2 *)(x + N - (M - 1));
    D = (ae_f32x2 *)x_buf;
    S_va = AE_LA64_PP(S);

    for (m = 0; m<(M >> 1); m++)
    {
        AE_LA32X2_IP(t0, S_va, S);  x0 = (t0);
        AE_S32X2_IP(x0, D, +8);
    }
    // Copy x[N].
    S = (const ae_f32x2 *)x;
    D = (ae_f32x2 *)((int32_t *)D - !(M & 1));
    S_va = AE_LA64_PP(S);
    D_va = AE_ZALIGN64();
#ifdef COMPILER_XTENSA
#pragma concurrent
#endif
    for (n = 0; n<((N + 1) >> 1); n++)
    {
        AE_LA32X2_IP(t0, S_va, S); x0 = (t0);
        AE_SA32X2_IP(x0, D_va, D);
    }
    AE_SA64POS_FP(D_va, D);
    D = (      ae_f32x2 *)(x_buf+N + M-1);
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 0*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 1*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 2*sizeof(int32_t) );
    //----------------------------------------------------------------------------
    // Copy reverted y[M] data into the aligned buffer and append 3 zeros:
    //  y_buf[M+3] = { y[M-1]..y[0] 0 0 0 }
    // Copy y[M] in reverted order.
    S = (const ae_f32x2 *)(y + M - 1);
    D = (ae_f32x2 *)y_buf;
    S_va = AE_LA64_PP(S);
    for (m = 0; m<((M + 1) >> 1); m++)
    {
        AE_LA32X2_RIP(t0, S_va, S); y0 = (t0);
        AE_S32X2_IP(y0, D, +8);
    }
    // Append three zeros to allow for block-4 processing.
    D = (ae_f32x2 *)(y_buf+M);
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 0*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 1*sizeof(int32_t) );
    AE_S32_L_I( AE_ZERO32(),(ae_int32*) D, 2*sizeof(int32_t) );
    raw_corr32x32(r, x_buf, y_buf, N, (M + 1)&~1);
}
