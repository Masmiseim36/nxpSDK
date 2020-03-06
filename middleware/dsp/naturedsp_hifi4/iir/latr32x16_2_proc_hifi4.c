/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x16-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*
    NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, 32x16-bit
    C code optimized for HiFi4
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "latr32x16_common.h"

/* Order 2 lattice filter data processing function. */
void latr32x16_2_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_int32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_10;
  ae_f16x4   cf_10zz;
  ae_f32x2   cf_22;
  ae_int16x4 s0, s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1;
  ae_f64     q0;
  ae_int64   u0;

  int n;

  NASSERT( r && x && dline && coef );

  NASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  NASSERT( M == 2 );

  p0 = 0;

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32   *)x;
  D = (      ae_int32x2   *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k1 k0 0 0

  cf_10zz = ( s0 );

  // k1*k0; k1*k0
  // Q31 <- Q15*Q15 + 1
  cf_22 = AE_MULF16SS_32( cf_10zz, cf_10zz );

  // Load delay line elements d1,d0.
  // Q1.30
  dl_10 = AE_L32X2_I( D, +0 );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //   q0 = g*x[n] - k1*d1 - k0*d0
  //   d1 = d0 + k0*q0; d0 = q0;
  //   r[n] = q0;
  //
  // In order to shorten the dependency path, we reorder the calculations:
  //   q0 = g*x[n] - k1*d1 - k0*(1+k1)*d0
  //   d1 = d0; d0 = q0;
  //   r[n] = q0;
  //
  // This modified algorithm is not bit exact with the original one, but it
  // is faster and provides at least the same performance in terms of the 
  // round-off error.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.

    // Q31
    AE_L32_IP( t0, X, +4 );

    // q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d1, d0.

    // q0 -= k1*d1 + k0*d0 
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_10, cf_10zz );

    // q0 -= k1*k0*d0
    // Q16.46 <- Q16.46 + [ Q31*Q1.30 - 15 ] w/ symmetric rounding
    AE_MULSF32R_LL( q0, dl_10, cf_22 );

    // d1 = d0; d0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( dl_10, q0, 0 );

    //--------------------------------------------------------------------------
    // Format and store response sample.

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 1 );

    t1 = ( p0 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // Q1.30
  AE_S32X2_I( dl_10, D, +0 );

} // latr32x16_2_proc()
