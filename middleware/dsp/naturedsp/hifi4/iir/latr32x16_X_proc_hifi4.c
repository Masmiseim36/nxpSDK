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

/* Data processing function for a lattice filter of arbitrary order. */
void latr32x16_X_proc( int32_t * restrict r,     // r[N]     [out   ] Q31
                  const int32_t * restrict x,     // x[N]     [in    ] Q31
                        int32_t *          dline, // dline[M] [in/out] Q1.30
                  const int16_t *          coef,  // coef[M]  [in    ] Q15
                  int16_t scale, int N, int M )   // scale    [in    ] Q15
{
        ae_int32     * restrict R;
  const ae_int32   *          X;
  const ae_int32x2 *          D_rd;
        ae_int32x2 * restrict D_wr;
  const ae_int32   *          C;

  ae_f32x2   dl_32, dl_10, dl_21;
  ae_f16x4   cf_3232, cf_1010, cf_2101;
  ae_f64     q0, q1, q2;
  ae_int64   u0;
  ae_f32x2   p0, p1;
  ae_int32x2 t0, t1, t2;
  ae_int16x4 s0, s1, s_g;

  int n, m;

  NASSERT( r && x && dline && coef );

  NASSERT( IS_ALIGN( dline ) && IS_ALIGN( coef ) );

  NASSERT( M > 8 );
  
  p0 = 0; p1 = 0;

  //
  // Setup pointers.
  //

  R = (      ae_int32   *)r;
  X = (const ae_int32 *)x;

  //
  // Load the prescaling factor.
  //

  // Q15
  s_g = AE_MOVDA16( (unsigned)scale );

  //
  // Pass the input samples block through the AR lattice. n-th response sample
  // and lattice state update are defined as follows:
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-2; m>=0; m-- )
  //     {
  //       q -= k[m]*d[m];
  //       d[m+1] = d[m] + k[m]*q;
  //     }
  //   }
  //
  //   d[0] = r[n] = q;
  //
  // After unrolling the inner loop (x2):
  //
  //   A) Odd filter order
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q0 = g*x[n];
  //     q1 = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-3; m>=0; m-=2 )
  //     {
  //       q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1];
  //       q1 -= k[m+1]*d[m+1] + k[m+0]*d[m+0];
  //
  //       d[m+2] = d[m+1] + k[m+1]*q0;
  //       d[m+1] = d[m+0] + k[m+0]*q1;
  //     }
  //
  //     d[0] = r[n] = q1;
  //   }
  //
  //
  //   B) Even filter order
  //
  //   for ( n=0; n<N; n++ )
  //   {
  //     q0 = g*x[n];
  //     q1 = g*x[n] - k[M-1]*d[M-1];
  //
  //     for ( m=M-3; m>=0; m-=2 )
  //     {
  //       q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1];
  //       q1 -= k[m+1]*d[m+1] + k[m+0]*d[m+0];
  //
  //       d[m+2] = d[m+1] + k[m+1]*q0;
  //       d[m+1] = d[m+0] + k[m+0]*q1;
  //     }
  //
  //     q0 -= k[1]*d[1] + k[0]*d[0];
  //
  //     d[0] = r[n] = q0;
  //   }
  //
  //
  // Mapping of variable suffixes onto coefficient/delay indices:
  //   cf_/dl_ suffix:  3   2   1   0
  //   k[]/d[] index : m+2 m+1 m+0 m-1
  //
  // Coefficients and delay line elements are stored in reverted order:
  //   dline[M] = { d[M-1], d[M-2], ..., d[0] };
  //   coef [M] = { k[M-1], k[M-2], ..., k[0] };
  //


  if ( M & 1 )
  {
    __Pragma("loop_count min=1")
    for ( n=0; n<N; n++ )
    {
      D_rd = (const ae_int32x2 *)dline;
      D_wr = (      ae_int32x2 *)dline;
      C    = (const ae_int32   *)coef;

      // Q31
      AE_L32_IP( t0, X, +4 );

      // q0 = g*x[n], q1 = g*x[n]
      // Q17.46 <- Q31*Q15
      u0 = AE_MUL32X16_L0( t0, s_g );

      q0 = ( u0 );
      q1 = ( u0 );

      cf_1010 = AE_L16X4_I( (const ae_int16x4*)C, 0 );

      // Load d[M-1], d[M-2]; Q1.30
      AE_L32X2_IP( t1, D_rd, +8 );
      // Load k[M-1], k[M-2]; Q15
      AE_L32_IP( t2, C, +4 );

      dl_32   = ( t1 );
      cf_3232 = AE_MOVF16X4_FROMINT32X2( t2 );

      // q1 -= k[M-1]*d[M-1]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
      AE_MULSF32X16_H3( q1, dl_32, cf_3232 );

      __Pragma("loop_count min=4")
      for ( m=M-3; m>=0; m-=2 )
      {
        // Load d[m], d[m-1]; Q1.30
        AE_L32X2_IP( t0, D_rd, +8 );

        dl_10 = ( t0 );

        // d[m+1] d[m]
        select_f32x2_lh( dl_21, dl_32, dl_10,0 );

        // Load k[m], k[m-1]; Q15
        AE_L32_IP( t1, C, +4 );

        cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

        s0 = ( cf_3232 );
        s1 = ( cf_1010 );

        // Make skew coefficients quadruple: k[m+1] k[m] k[m-1] k[m].
        s0 = AE_SEL16_4321( s0, s1 );

        cf_2101 = ( s0 );

        // q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H1_L0( q0, dl_32, cf_3232 );

        // q1 -= k[m+1]*d[m+1] + k[m]*d[m]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_2101 );

        // q0 q1
        // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
        AE_PKSR32( p0, q0, 0 );
        AE_PKSR32( p0, q1, 0 );

        // d[m+2] = d[m+1] + k[m+1]*q0; d[m+1] = d[m] + k[m]*q1
        // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
        AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2101 );

        t2 = ( dl_21 );

        // Save updated d[m+2], d[m+1]
        AE_S32X2_IP( t2, D_wr, +8 );

        dl_32   = dl_10;
        cf_3232 = cf_1010;
      }

      t0 = ( p0 );

      // d0 = q1; Q1.30
      AE_S32_L_I( t0, (ae_int32*)D_wr, +0 );

      // Q16.47 <- Q17.46 + 1
      q2 = AE_F64_SLAIS( q1, 1 );

      // r[n] = q1
      // Q31 <- Q16.47 - 16
      AE_S32RA64S_IP( q2, R, +4 );

    __Pragma("no_reorder")
    }
  }
  else
  {
    __Pragma("loop_count min=1")
    for ( n=0; n<N; n++ )
    {
      D_rd = (const ae_int32x2 *)dline;
      D_wr = (      ae_int32x2 *)dline;
      C    = (const ae_int32   *)coef;

      // Q31
      AE_L32_IP( t0, X, +4 );

      // q0 = g*x[n], q1 = g*x[n]
      // Q17.46 <- Q31*Q15
      u0 = AE_MUL32X16_L0( t0, s_g );

      q0 = ( u0 );
      q1 = ( u0 );

      cf_1010 = AE_L16X4_I( (const ae_int16x4*)C, 0 );

      // Load d[M-1], d[M-2]; Q1.30
      AE_L32X2_IP( t1, D_rd, +8 );
      // Load k[M-1], k[M-2]; Q15
      AE_L32_IP( t2, C, +4 );

      dl_32   = ( t1 );
      cf_3232 = AE_MOVF16X4_FROMINT32X2( t2 );

      // q1 -= k[M-1]*d[M-1]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ]
      AE_MULSF32X16_H3( q1, dl_32, cf_3232 );

      __Pragma("loop_count min=4")
      for ( m=M-3; m>=0; m-=2 )
      {
        // Load d[m], d[m-1]; Q1.30
        AE_L32X2_IP( t0, D_rd, +8 );

        dl_10 = ( t0 );

        // d[m+1] d[m]
        select_f32x2_lh( dl_21, dl_32, dl_10,0 );

        // Load k[m], k[m-1]; Q15
        AE_L32_IP( t1, C, +4 );

        cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

        s0 = ( cf_3232 );
        s1 = ( cf_1010 );

        // Make skew coefficients quadruple: k[m+1] k[m] k[m-1] k[m].
        s0 = AE_SEL16_4321( s0, s1 );

        cf_2101 = ( s0 );

        // q0 -= k[m+2]*d[m+2] + k[m+1]*d[m+1]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H1_L0( q0, dl_32, cf_3232 );

        // q1 -= k[m+1]*d[m+1] + k[m]*d[m]
        // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
        AE_MULSSFD32X16_H3_L2( q1, dl_21, cf_2101 );

        // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
        AE_PKSR32( p0, q0, 0 );
        AE_PKSR32( p0, q1, 0 );

        // d[m+2] = d[m+1] + k[m+1]*q0; d[m+1] = d[m] + k[m]*q1
        // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
        AE_MULAFP32X16X2RAS_H( dl_21, p0, cf_2101 );

        t2 = ( dl_21 );

        // Save updated d[m+2], d[m+1]
        AE_S32X2_IP( t2, D_wr, +8 );

        dl_32   = dl_10;
        cf_3232 = cf_1010;
      }

      // Reload d[1], d[0]; Q1.30
      t0 = AE_L32X2_I( D_rd, -8 );
      // Reload k[1], k[0]; Q15
      t1 = AE_L32_I( C, -4 );

      dl_10   = ( t0 );
      cf_1010 = AE_MOVF16X4_FROMINT32X2( t1 );

      // q0 -= k[1]*d[1] + k[0]*d[0]
      // Q17.46 <- Q17.46 + [ Q15*Q1.30 + 1 ] + [ Q15*Q1.30 + 1 ]
      AE_MULSSFD32X16_H1_L0( q0, dl_10, cf_1010 );

      // <don't care> q0
      // Q1.30 <- Q17.46 - 16 w/ rounding and saturation
      AE_PKSR32( p1, q0, 0 );

      // <don't care>; d[1] = d[0] + k[0]*q0
      // Q1.30 <- Q1.30 + [ Q15*Q1.30 - 15 ] w/ rounding and saturation
      AE_MULAFP32X16X2RAS_H( dl_10, p1, cf_1010 );

      // d[1]; d[0] = q0
      select_f32x2_ll( dl_10, dl_10, p1,0 );

      t2 = ( dl_10 );

      // Save updated d[1], d[0]
      AE_S32X2_I( t2, D_wr, +0 );

      // Q16.47 <- Q17.46 + 1
      q2 = AE_F64_SLAIS( q0, 1 );

      // r[n] = q0
      // Q31 <- Q16.47 - 16
      AE_S32RA64S_IP( q2, R, +4 );

    __Pragma("no_reorder")

    }
  }

} // latr32x16_X_proc()
