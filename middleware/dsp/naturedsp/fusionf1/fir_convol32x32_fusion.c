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
    Real data circular convolution, 32x32-bit
*/

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  x,y,r - aligned on an 8-bytes boundary
  N,M   - multiples of 4 and >0
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

void fir_convol32x32(int32_t * restrict r,
    const int32_t     * restrict x,
    const int32_t     * restrict y,
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
    
    const int32_t  *          xn;
    const ae_f32x2 *          X;
    const ae_f32x2 *          Y;
    ae_f32x2 * restrict R;

    ae_f64     q0, q1, q2, q3;
    ae_int32x2 t0, t1, t2;
    ae_f32x2 x0, x1, x2;
    ae_f32x2 y0;

    int n, m;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r, 8);
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0 && M % 4 == 0);
    NASSERT(N > 0 && N % 4 == 0);

    //
    // Setup pointers and circular addressing for array x[N].
    //

    xn = x + 2;

    R = (ae_f32x2 *)r;

    WUR_AE_CBEGIN0((uintptr_t)(x + 0));
    WUR_AE_CEND0((uintptr_t)(x + N));

    for (n = 0; n < (N >> 2); n++)
    {
        // Set X to &x[4*n+2]
        X = (const ae_f32x2 *)xn;

        xn += 4;

        // Load x[4*n+3]..x[4*n+0]
        AE_L32X2_RIC(t2, X); x2 = (t2);
        AE_L32X2_RIC(t1, X); x1 = (t1);

        // Load x[4*n-1]..x[4*n-2].
        AE_L32X2_RIC(t0, X); x0 = (t0);

        Y = (const ae_f32x2 *)y;

        AE_L32X2_IP(t0, Y, +8); y0 = (t0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q0 = AE_MULF32R_LH(x1, y0);
        AE_MULAF32R_LH(q0, y0, x0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q1 = AE_MULF32R_HH(x1, y0);
        AE_MULAF32R_LL(q1, x1, y0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q2 = AE_MULF32R_LH(x2, y0);
        AE_MULAF32R_LH(q2, y0, x1);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q3 = AE_MULF32R_HH(x2, y0);
        AE_MULAF32R_LL(q3, x2, y0);

        x2 = x1;  x1 = x0;
        AE_L32X2_RIC(t0, X); x0 = (t0);

        __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 1) - 1; m++){
            AE_L32X2_IP(t0, Y, +8);
            y0 = (t0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q0, x1, y0);
            AE_MULAF32R_LH(q0, y0, x0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q1, x1, y0);
            AE_MULAF32R_LL(q1, x1, y0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q2, x2, y0);
            AE_MULAF32R_LH(q2, y0, x1);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q3, x2, y0);
            AE_MULAF32R_LL(q3, x2, y0);

            x2 = x1;  x1 = x0;
            AE_L32X2_RIC(t0, X); x0 = (t0);
        }

        // Convert and save 4 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        AE_S32RA64S_IP(q0, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q1, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q2, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q3, castxcc(ae_f32, R), +4);
    }
} // fir_convol32x32()
