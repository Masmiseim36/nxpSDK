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
    Complex data circular convolution, 32x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

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
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

void cxfir_convol32x16(complex_fract32 * restrict r,
  const complex_fract32 * restrict x,
  const complex_fract16 * restrict y,
  int N,
  int M)
{
  int n, m;
  const int32_t    *          xn;
  const ae_int32x2 *          X;
  const ae_f16x4   *          Y;
  ae_int32x2 * restrict R;

  ae_f64     q0r, q0i, q1r, q1i, q2r, q2i, q3r, q3i;
  ae_int32x2 t5, t4, t3, t2, t1, t0;
  ae_f32x2   x5, x4, x3, x2, x1, x0;
  ae_f16x4   y0, y1;

  NASSERT(r);
  NASSERT(x);
  NASSERT(y);
  NASSERT_ALIGN(r,8);
  NASSERT_ALIGN(x,8);
  NASSERT_ALIGN(y,8);
  NASSERT(M%4==0 && N%4==0);

  //
  // Setup pointers and circular addressing for array x[N].
  //

  xn = (const int32_t    *)(x + 3);

  R = (ae_int32x2 *)r;

  WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
  WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

  for ( n=0; n<(N>>2); n++ )
  {
    // Set X to &x[8*n+6]
    X = (const ae_int32x2 *)xn;

    xn += 8;

    // Load x[8*n+7]..x[8*n+0]
    // Q31
    AE_L32X2_XC(t5, X, -8);
    AE_L32X2_XC(t4, X, - 8);
    AE_L32X2_XC(t3, X, - 8);
    AE_L32X2_XC( t2, X, -8 );

    x5 = ( t5 );
    x4 = ( t4 );
    x3 = ( t3 );
    x2 = ( t2 );

    //
    // Inner loop kernel: process next 4 entries of y[M] for 4 accumulators.
    // 
    
    
    // Set Y to &y[0]
    Y = (const ae_f16x4 *)y;
    q0r = q1r = q2r = q3r = AE_ZERO64();
    q0i = q1i = q2i = q3i = AE_ZERO64();
    for ( m=0; m<(M>>1); m++ )
    {
      // Load x[8*n-1]..x[8*n-4].
      // Q31
      AE_L32X2_XC(t1, X, -8);
      AE_L32X2_XC(t0, X, -8);

      x1 = (t1);
      x0 = (t0);

      // Load y[4*m+4]..y[4*m+7].
      // Q15
      AE_L16X4_IP(y0, Y, +8);
      y1 = AE_SEL16_2301(y0, y0);

      AE_MULASFD32X16_H3_L2(q0r, x2, y0);
      AE_MULASFD32X16_H1_L0(q0r, x1, y0);
      AE_MULAAAAFQ32X16(q0i, x2, x1, y1);

      AE_MULASFD32X16_H3_L2(q1r, x3, y0);
      AE_MULASFD32X16_H1_L0(q1r, x2, y0);
      AE_MULAAAAFQ32X16(q1i, x3, x2, y1);

      AE_MULASFD32X16_H3_L2(q2r, x4, y0);
      AE_MULASFD32X16_H1_L0(q2r, x3, y0);
      AE_MULAAAAFQ32X16(q2i, x4, x3, y1);

      AE_MULASFD32X16_H3_L2(q3r, x5, y0);
      AE_MULASFD32X16_H1_L0(q3r, x4, y0);
      AE_MULAAAAFQ32X16(q3i, x5, x4, y1);

      // 4 convolution products are done, move out x[8*n-4*m+3]..x[8*n-4*m+0].
      x5 = x3; x4 = x2; x3 = x1; x2 = x0;
    }
    // Convert and save 4 outputs to the output array.
    // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
    AE_S32X2RA64S_IP( q0r, q0i, R );
    AE_S32X2RA64S_IP( q1r, q1i, R );
    AE_S32X2RA64S_IP( q2r, q2i, R );
    AE_S32X2RA64S_IP( q3r, q3i, R );
  }
} /* cxfir_convol32x16() */
