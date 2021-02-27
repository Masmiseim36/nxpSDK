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
  Real data circular cross-correlation, 32x32-bit
  */

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  x[N]          input data Q31,Q15 or floating point
  y[M]          input data Q31,Q15 or floating point
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


void fir_xcorr32x32(int32_t * restrict r,
    const int32_t * restrict x,
    const int32_t * restrict y,
    int N, int M)
{
    //
    // Circular cross-correlation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //

    const ae_f32x2 *          X;
    const ae_f32x2 *          S;
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

    X = (const ae_f32x2 *)x;
    R = (ae_f32x2 *)r;

    WUR_AE_CBEGIN0((uintptr_t)(x + 0));
    WUR_AE_CEND0((uintptr_t)(x + N));

    for (n = 0; n < (N >> 2); n++)
    {
        AE_L32X2_XC(t0, X, +8); x0 = (t0);
        AE_L32X2_XC(t1, X, +8); x1 = (t1);
        S = X;
        AE_L32X2_XC(t2, S, +8); x2 = (t2);

        Y = (const ae_f32x2 *)y;

        AE_L32X2_IP(t0, Y, +8); y0 = (t0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q0 = AE_MULF32R_HH(x0, y0);
        AE_MULAF32R_LL(q0, x0, y0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q1 = AE_MULF32R_LH(x0, y0);
        AE_MULAF32R_LH(q1, y0, x1);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q2 = AE_MULF32R_HH(x1, y0);
        AE_MULAF32R_LL(q2, x1, y0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q3 = AE_MULF32R_LH(x1, y0);
        AE_MULAF32R_LH(q3, y0, x2);

        x0 = x1; x1 = x2;
        AE_L32X2_XC(t2, S, +8); x2 = (t2);

        __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 1) - 1; m++){
            AE_L32X2_IP(t0, Y, +8);
            y0 = (t0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q0, x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q1, x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_HH(q2, x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
            AE_MULAF32R_LH(q3, x1, y0);
            AE_MULAF32R_LH(q3, y0, x2);

            x0 = x1; x1 = x2;
            AE_L32X2_XC(t2, S, +8); x2 = (t2);
        }

        // Convert and save 4 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        AE_S32RA64S_IP(q0, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q1, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q2, castxcc(ae_f32, R), +4);
        AE_S32RA64S_IP(q3, castxcc(ae_f32, R), +4);
    }

} // fir_xcorr32x32()
