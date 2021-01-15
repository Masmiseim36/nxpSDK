/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
    NatureDSP Signal Processing Library. FIR part
    Real data circular convolution, 32x32-bit
    C code optimized for HiFi4
    IntegrIT, 2006-2018
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs 
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  x[N]      input data, Q15, Q31 or floating point
  y[M]      input data, Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restriction:
  x,y,r     should not overlap
  x,y,r     aligned on an 8-bytes boundary
  N,M       multiples of 4 and >0
-------------------------------------------------------------------------*/
void fir_convol32x32( int32_t * restrict r,
                const int32_t     * restrict x,
                const int32_t     * restrict y,
                int N, int M )
{
    // Circular convolution algorithm:
    //
    //   r[n] = sum( x[mod(n-m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    const int32_t  *          xn;
    const ae_int32x2 *          X;
    const ae_f32x2 *          Y;
          ae_f32x2 * restrict R;

    ae_f64   q0, q1, q2, q3, q4, q5, q6, q7;
    ae_int32x2 x0, x1, x2, x3, x4, x5;
    ae_int32x2 y0, y1;
    int n, m, circinc;

    NASSERT ( r && x && y && N > 0 && M > 0 );
    NASSERT ( !( N & 3 ) && !( M & 3 ) );
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(r);

    // Setup pointers and circular addressing for array x[N].
    R = (ae_f32x2 *)r;
    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    WUR_AE_CBEGIN1( (uintptr_t)( r + 0 ) );
    WUR_AE_CEND1  ( (uintptr_t)( r + N ) );
    xn = x;
    circinc=24;
    if(circinc >= (N<<2)) circinc -= (N<<2);
    AE_ADDCIRC_XC(castxcc(ae_int64,xn),circinc);
    circinc = (M<<2)+64;
    while(circinc >= (N<<2)) circinc -= (N<<2);   //circinc = circinc % (N<<2);
    X = (const ae_int32x2 *)xn;
    Y = (const ae_f32x2 *)y;

  /* 
    Process by 8 samples. If number of samples is not a multiple of 4
    extra 4 samples will be computed and first 4 samples will be rewritten 
    using circular addressing of the output buffer
  */
    for ( n=0; n<((N+4)>>3); n++ )
    {
        // Load x[8*n+7]..x[8*n+0]
        // Q23 <- Q(23+8) - 8
        AE_L32X2_RIC( x5, X );
        AE_L32X2_RIC( x4, X );
        AE_L32X2_RIC( x3, X );
        AE_L32X2_RIC( x2, X );

        // Inner loop prologue: process first 4 entries of y[M] for 8 accumulators.
        q0=q1=q2=q3=q4=q5=q6=q7=AE_ZERO64();
        __Pragma("loop_count min=1")
        for ( m=0; m<(M>>2); m++ )
        {
            // Load x[8*n-4*m-5]..x[8*n-4*m-8].
            AE_L32X2_RIC( x1, X );
            AE_L32X2_RIC( x0, X );

            // Load y[4*m+4]..y[4*m+7].
            AE_L32X2_IP( y0, Y, +8 );
            AE_L32X2_IP( y1, Y, +8 );
            AE_MULAFD32X2RA_FIR_H( q1, q0, x2, x1, y0 );
            AE_MULAFD32X2RA_FIR_H( q3, q2, x3, x2, y0 );
            AE_MULAFD32X2RA_FIR_H( q5, q4, x4, x3, y0 );
            AE_MULAFD32X2RA_FIR_H( q7, q6, x5, x4, y0 );
            AE_MULAFD32X2RA_FIR_H( q1, q0, x1, x0, y1 );
            AE_MULAFD32X2RA_FIR_H( q3, q2, x2, x1, y1 );
            AE_MULAFD32X2RA_FIR_H( q5, q4, x3, x2, y1 );
            AE_MULAFD32X2RA_FIR_H( q7, q6, x4, x3, y1 );

            // 4 convolution products are done, move out x[8*n-4*m+3]..x[8*n-4*m+0].
            x5 = x3; x4 = x2; x3 = x1; x2 = x0;
        }
        Y-=M/2;
        x0=AE_ROUND32X2F48SASYM(q0, q1);
        x1=AE_ROUND32X2F48SASYM(q2, q3);
        x2=AE_ROUND32X2F48SASYM(q4, q5);
        x3=AE_ROUND32X2F48SASYM(q6, q7);
        AE_S32X2_XC1( x0, R, 8 );
        AE_S32X2_XC1( x1, R, 8 );
        AE_S32X2_XC1( x2, R, 8 );
        AE_S32X2_XC1( x3, R, 8 );
        AE_ADDCIRC_XC(castxcc(ae_int64,X),circinc); 
    }
} // fir_convol32x32()
