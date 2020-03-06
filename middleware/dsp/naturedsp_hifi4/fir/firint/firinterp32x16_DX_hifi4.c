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
  NatureDSP Signal Processing Library. FIR part
  Interpolating block real FIR filter, 32x16-bit 
  C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Interpolating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with interpolation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector y. The filter calculates N*D output samples 
  using M*D coefficients and requires last N+M*D-1 samples on the delay line.
  NOTE:
  user application is not responsible for management of delay lines

  Precision: 
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations

  Input:
  h[M*D]        filter coefficients; h[0] is to be multiplied with the 
                newest sample,Q31 or Q15
  D             interpolation ratio
  N             length of input sample block
  M             length of subfilter. Total length of filter is M*D
  x[N]          input samples,Q31 
  Output:
  y[N*D]        output samples,Q31

  Restrictions:
  x,h,y should not overlap
  x,h - aligned on an 8-bytes boundary
  N   - multiple of 8
  M   - multiple of 4
  D should be >1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D   - 2, 3 or 4

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"

#include "firinterp32x16_common.h"


/* Generic data processing function for a decimating FIR filter. */
 int firinterp32x16_DX_proc( int32_t * restrict y,
                                  int32_t * delayLine,
                                  int32_t delayLen,
                          const int32_t * restrict x,
                          const int16_t * restrict h,
                          int wrIx, int D, int N, int M )
{
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          X;
        ae_int32     * restrict Y;
  const ae_int16x4 *          C;
  const ae_int16x4 *          C_;

  ae_valign C_va;
  ae_int32x2 t01, t23, t45;
  ae_int32x2 z0, z1;
  ae_int16x4 c0, c1;
  ae_f64     q0, q1, q2, q3;
  ae_f64     z;
  ae_f32x2   g_frac;

  int m, n, d;

  NASSERT( y && delayLine && x && h 
          &&
          D > 0 && N > 0 && M > 0 );

  NASSERT( !( N & 7 ) && !( M & 3 ) );

  NASSERT( IS_ALIGN( delayLine ) &&
          IS_ALIGN( x      ) &&
          IS_ALIGN( h      ) );

  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2 *)(delayLine+wrIx);
  X    = (const ae_int32x2 *)x;
  Y    = (      ae_int32     *)y;

  WUR_AE_CBEGIN0( (uintptr_t)(delayLine            ) );
  WUR_AE_CEND0  ( (uintptr_t)(delayLine + delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( h                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( h + (M + 4+4)*D ) );
  z = AE_ZERO64();

  {
    int q_frac_int =  D << ( 16);
    g_frac = AE_MOVDA32X2(q_frac_int, q_frac_int);
  }
  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_int16x4*)h;
  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>2); n++ )
  {
    // Load 4 input samples.
    // Q31
    t23 = AE_L32X2_I( X, +8 );
    AE_L32X2_IP( t01, X, +16 );

    // Store 4 samples to the delay line buffer with circular address update.
    // Q31
    AE_S32X2_XC( t01, D_wr, +8 );
    AE_S32X2_XC( t23, D_wr, +8 );
    Y    = ( ae_int32 *)(y + 4*n*D);

    for (d=0;d<D;d++)
    {

      // Start reading the delay line from the oldest sample, M+4 samples back
      // from the newest one.
      D_rd1 = D_wr;
      D_rd0 = (ae_int32x2*)( D_rd1 + 1 );
      C_ = (const ae_int16x4*)((uintptr_t)C+2*3);
      AE_LA16X4POS_PC1(C_va,C_);
      AE_L32X2_XC  ( t01, D_rd1, +16 );
      AE_L16X4_XC(c0, C, +8);
      // Zero the accumulators.
      q0 = z; q1 = z; q2 = z; q3 = z;

      //
      // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
      // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
      // inserted into the impulse response during initialization.
      //
      __Pragma("loop_count min=2");
      for ( m=0; m<(M>>2)+1; m++ )
      {
        // Load next two samples for even accumulators. Altogether we have
        // 6 samples residing in 3 AE registers.
        // Q31

        AE_L32X2_XC( t23, D_rd0, +16 );
        AE_L32X2_XC( t45, D_rd1, +16 );
        AE_LA16X4_IC1( c1, C_va, C_ );
        AE_L16X4_XC1(c0,C,+8);
                                                            
        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c0);
        AE_MULAAAAFQ32X16(q1,t01,t23,c1);
        AE_MULAAAAFQ32X16(q2,t23,t45,c0);
        AE_MULAAAAFQ32X16(q3,t23,t45,c1);
        t01 = t45; 
      }

      z0 = AE_ROUND32X2F48SASYM(q0,q1);
      z1 = AE_ROUND32X2F48SASYM(q2,q3);

      q0 = AE_MUL32_HH(z0,g_frac);
      q1 = AE_MUL32_LL(z0,g_frac);
      q2 = AE_MUL32_HH(z1,g_frac);
      q3 = AE_MUL32_LL(z1,g_frac);

      //
      // Store 4 outputs to the output array with 2-samples stride.
      //

      AE_S32RA64S_XP( q0, Y, +D*4 );
      AE_S32RA64S_XP( q1, Y, +D*4 );
      AE_S32RA64S_XP( q2, Y, +D*4 );
      AE_S32RA64S_XP( q3, Y, -D*4*3+4 );
    }
  }
  return (int)((int32_t *)D_wr - delayLine) ;
}
