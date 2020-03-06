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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "latr32x16_common.h"

/* Order 3 lattice filter data processing function. */
void latr32x16_3_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32   * restrict R;
  const ae_int32   * restrict X;
        ae_int32x2   *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_21, dl_0z, dl_x0;
  ae_f16x4   cf_210z, cf_10z2;
  ae_int16x4 s0, s1, s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1, t_rnd;
  ae_f64     q0, q1, q2, q_rnd;
  ae_int64   u0, u1, u2;

  int n;

  NASSERT( r && x && dline && coef );

  NASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  NASSERT( M == 3 );

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
  s_g = AE_MOVDA16X2( 1, (unsigned)scale );

  // Rounding term for Q17.46 -> Q1.30 conversion.
  t_rnd = AE_MOVDA32X2( 0, (1<<15) );

  q_rnd = AE_MOVF64_FROMINT32X2( t_rnd );

  //
  // Pre-load the reflection coefficients and delay line elements.
  //

  // Q15
  s0 = AE_L16X4_I( C, +0 ); // k2 k1 k0 0

  // Make a skew coefficient quadruple.
  s1 = AE_SEL16_6543( s0, s0 ); // k1 k0 0 k2

  cf_210z = ( s0 );
  cf_10z2 = ( s1 );

  // Load delay line elements d2..d0.
  // Q1.30
  dl_21 = AE_L32X2_I( D, +0 );
  dl_0z = AE_L32X2_I( D, +8 );

  // <don't care>; d0
  select_f32x2_hh( dl_x0, dl_0z, dl_0z );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   p1 = g*x[n] - k2*d2 - k1*d1
  //   p0 = p1 - k0*d0;
  //
  //   d2 = d1 + k1*p1;
  //   d1 = d0 + k0*p0;
  //   d0 = p0;
  //
  //   r[n] = p0;
  //
  // The inner loop is fully unrolled.
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    //--------------------------------------------------------------------------
    // Load the next input sample and prescale it.

    // Q31
    AE_L32_IP( t0, X, +4 );

    // <rounding term> x[n]
    t0 = AE_SEL32_LL( t_rnd, t0 );

    // q0 = <rounding term> + g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MULZAAD32X16_H1_L0( t0, s_g );

    q0 = ( u0 );
    q1 = ( u0 );

    //--------------------------------------------------------------------------
    // Update delay elements d2, d1.

    // q1 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L1( q1, dl_x0, cf_210z );

    // q0 -= k2*d2 + k1*d1
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
    AE_MULSSFD32X16_H3_L2( q0, dl_21, cf_210z );
    AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_210z );

    u1 = ( q0 );
    u2 = ( q1 );

    // p1 = q0; p0 = q1
    // Q1.30 <- Q17.46 + 16 - 32 w/ truncation.
    // Actually we get rounded values due to the rounding term.
    t1 = AE_TRUNCA32X2F64S( u1, u2, 16 );

    p0 = ( t1 );

    // d1 d0
    select_f32x2_ll( dl_21, dl_21, dl_x0,0 );

    // d2 = d1 + k1*p1; d1 = d0 + k0*p0
    // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
    AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_10z2 );

    //--------------------------------------------------------------------------
    // Update delay element d0, format and store response sample.

    // <don't care>; d0 = p0
    dl_x0 = p0;

    // r[n] = q1 - <rounding term>
    q2 = AE_F64_SUBS( q1, q_rnd );

    // Q16.47 <- Q17.46 + 1
    q2 = AE_F64_SLAIS( q2, 1 );

    // Q31 <- Q16.47 - 16 w/ rounding and saturation
    AE_S32RA64S_IP( q2, R, +4 );
  }

  //
  // Store updated delay line elements.
  //

  // d0 0
  select_f32x2_ll( dl_0z, dl_x0, dl_0z,0 );

  // Q1.30
  AE_S32X2_I( dl_21, D, +0 );
  AE_S32X2_I( dl_0z, D, +8 );

} // latr32x16_3_proc()
