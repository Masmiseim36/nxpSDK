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
/* Signal Processing Library API. */
#include "common.h"
#include "latr32x16_common.h"


/* Order 1 lattice filter data processing function. */
void latr32x16_1_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32 * restrict R;
  const ae_int32 * restrict X;
        ae_int32x2 *          D;
  const ae_int16x4 *          C;

  ae_f32x2   dl_0z, dl_x0;
  ae_f16x4   cf_0zzz;
  ae_int16x4 s_g;
  ae_f32x2   p0;
  ae_int32x2 t0, t1;
  ae_f64     q0;
  ae_int64   u0;

  int n;

  NASSERT( r && x && dline && coef );

  NASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  NASSERT( M == 1 );

  p0 = 0;

  //
  // Setup pointers.
  //

  R = (      ae_int32 *)r;
  X = (const ae_int32 *)x;
  D = (      ae_int32x2 *)dline;
  C = (const ae_int16x4 *)coef;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16X2( 0, (unsigned)scale );

  //
  // Pre-load the reflection coefficient and delay line element.
  //

  // Q15
  cf_0zzz = AE_L16X4_I( C, +0 );

  // Load delay line element d0.
  // Q1.30
  dl_0z = AE_L32X2_I( D, +0 );

  // <don't care>; d0
  select_f32x2_hh( dl_x0, dl_0z, dl_0z );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   q0 = g*x[n] - k0*d0;
  //   d0 = q0;
  //   r[n] = q0;
  //

  __Pragma("loop_count min=1")
  for ( n=0; n<N; n++ )
  {
    // Q31
    AE_L32_IP( t0, X, +4 );

    // Prescaling, q0 = g*x[n]
    // Q17.46 <- Q15*Q31
    u0 = AE_MUL32X16_L0( t0, s_g );

    q0 = ( u0 );

    // q0 -= k0*d0
    // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
    AE_MULSF32X16_L3( q0, dl_x0, cf_0zzz );

    // <don't care>; d0 = q0
    // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
    AE_PKSR32( dl_x0, q0, 0 );

    // r[n] = q0
    // Q31 <- Q17.46 + 1 - 16 w/ rounding and saturation
    AE_PKSR32( p0, q0, 1 );

    t1 = ( p0 );

    // Q31
    AE_S32_L_IP( t1, R, +4 );
  }

  //
  // Store the updated delay line element.
  //

  // d0 0
  select_f32x2_ll( dl_0z, dl_x0, dl_0z,0 );

  // Q1.30
  AE_S32X2_I( dl_0z, D, +0 );

} // latr32x16_1_proc()
