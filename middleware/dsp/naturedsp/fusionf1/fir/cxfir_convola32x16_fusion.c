/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    Complex data circular convolution, 32x16-bit
    C code optimized for Fusion
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

#include "baseop.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
  (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void cxfir_convola32x16(void    * restrict s,
                        complex_fract32 * restrict r,
                        const complex_fract32 * restrict x,
                        const complex_fract16 * restrict y,
                        int N,
                        int M)
{
    void    * s_ptr;
    int32_t * x_buf;
    int16_t * y_buf;

    const ae_int32x2 *          S;
        ae_int32x2 * restrict D;
    const ae_int16x4 *          S16;
        ae_int32x2 * restrict D32;
        ae_int16   * restrict D16_s;
    ae_f64 ar,ai;
    const ae_int32x2 *pX;
    const ae_int16x4 *pY;
    const ae_f32x2 *pR;
    ae_int32x2 X0,X1;
    ae_int16x4 Y;
    int m,n;
    ae_valign S_va, D_va;
    ae_int32x2 t;
    ae_int16x4 p;
    ae_int32x2 tmp;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(s,8);
    NASSERT(M>0 && N>0);
    NASSERT(N >= M-1);

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.

    s_ptr = s;
    x_buf = (int32_t*)ALIGNED_ADDR( s_ptr, 8 );
    s_ptr = x_buf + 2*(M-1 + N + 3);
    y_buf = (int16_t*)ALIGNED_ADDR( s_ptr, 8 );
    s_ptr = y_buf + 2*(M + 3);

    ASSERT( (int8_t *)s_ptr - (int8_t *)s <= 
            (int)CXFIR_CONVOLA32X16_SCRATCH_SIZE( N, M ) );

    //----------------------------------------------------------------------------
    // Copy x[N] data into the aligned buffer in a way that simplifies the
    // convolution calculation:
    //  x_buf[M-1+N+3] = { x[N-(M-1)]..x[N-1] x[0] x[1]..x[N-1] X X X }
    // Three X locations are reserved to allow for block-4 processing.
    // Copy last M-1 entries of x{N].
    S = (const ae_int32x2 *)( x + (N - (M-1)) );
    D = (      ae_int32x2 *)x_buf;
    S_va = AE_LA64_PP( S );
    for ( m=0; m<M - (-M &1); m++ )
    {
        AE_LA32X2_IP( t, S_va, S );
        AE_S32X2_IP( t, D, +8 );
    }
    // Copy x[N].
    S = (const ae_int32x2 *)x;
    D = (      ae_int32x2 *)( (int32_t *)D - 2*(!( M & 1 )) );
    S_va = AE_LA64_PP( S );
    D_va = AE_ZALIGN64();
    for ( n=0; n<N; n++ )
    {
        AE_LA32X2_IP( t, S_va, S );
        AE_SA32X2_IP( t, D_va, D );
    }
    AE_SA64POS_FP( D_va, D );
    //----------------------------------------------------------------------------
    // Copy reverted y[M] data into the aligned buffer and append 3 zeros:
    //  y_buf[M+3] = { y[M-1]..y[0] 0 0 0 }
    // Copy y[M] in reverted order.
    S16 = (const ae_int16x4 *)(((const int16_t*) y) + 2*M - 1);
    D32 = (      ae_int32x2 *)y_buf;
    S_va = AE_LA64_PP(S16);
    for ( m=0; m<((M+1)>>1); m++ )
    {
        AE_LA16X4_RIP(p, S_va, S16);
        tmp = AE_MOVINT32X2_FROMF16X4(p);
        AE_S32X2_IP( tmp, D32, 8);
    }
    // Append three zeros to allow for block-4 processing.
    D16_s = (ae_int16 *)((int16_t *)D32 - 2*(-M & 1));
    p = AE_ZERO16();
    for (m = 0; m<1; m++)
    {
        AE_S16_0_IP(p, D16_s, +2);
        AE_S16_0_IP(p, D16_s, +2);
    }
    __Pragma("no_reorder")
    /* convolution */
    x=(const complex_fract32*)x_buf;
    y=(const complex_fract16*)y_buf;
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);

    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    pR=(ae_f32x2*)r;
    for ( n=0; n<N; n++ )
    {
        ar=ai=AE_ZERO();
        pX=(const ae_int32x2*)(x+n);
        pY=(const ae_int16x4 *)y;
       // __Pragma("loop_count min=2")
        for (m = 0; m<M; m+=2)
        {
            AE_L32X2_XC(X0,pX,8);
            AE_L32X2_XC(X1,pX,8);
            AE_L16X4_IP(Y,pY,8);
            AE_MULASFD32X16_H3_L2(ar, X0, Y);
            AE_MULAAFD32X16_H2_L3(ai, X0, Y);
            AE_MULASFD32X16_H1_L0(ar, X1, Y);
            AE_MULAAFD32X16_H0_L1(ai, X1, Y);
        }
        AE_S32X2RA64S_IP(ar, ai, castxcc(ae_f32x2, pR));
    }
} // cxfir_convola32x16()
