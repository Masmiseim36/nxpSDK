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
Interpolating block real FIR filter, 32x32-bit
*/

/*-------------------------------------------------------------------------
Interpolating Block Real/Complex FIR Filter
Computes a real FIR filter (direct-form) with interpolation using IR stored
in vector h. The real data input is stored in vector x. The filter output
result is stored in vector y. The filter calculates N*D output samples
using M*D coefficients and requires last N+M*D-1 samples on the delay line.
NOTE:
user application is not responsible for management of delay lines

Precision:
16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs
16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs
32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs
32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs
f         floating point

Input:
h[M*D]        filter coefficients; h[0] is to be multiplied with the
newest sample,Q31, Q15, floating point
D             interpolation ratio
N             length of input sample block
M             length of subfilter. Total length of filter is M*D
x[N]          input samples,Q15, Q31 or floating point
Output:
y[N*D]        output samples, Q15, Q31 or floating point

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
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#include "firinterp32x32_common.h"


/* Data processing function for a factor 2 interpolating FIR filter. */
 int firinterp32x32_D2_proc( int32_t * restrict y,
                                  int32_t * delayLine,
                                  int32_t delayLen,
                          const int32_t * restrict x,
                          const int32_t * restrict h,
                          int wrIx, int D, int N, int M )
{
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          X;
        ae_f32     * restrict Y;
  const ae_int32x2 *          C;

  ae_valign D_va;
  ae_int32x2 t01, t23;
  ae_int32x2 t12, t34;
  ae_int32x2 c0, c2;
  ae_f64   q0, q1, q2, q3;
  ae_f64   q4, q5, q6, q7;
  ae_f64     z;

  int m, n;

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
  Y    = (      ae_f32     *)y;

  WUR_AE_CBEGIN0( (uintptr_t)(delayLine            ) );
  WUR_AE_CEND0  ( (uintptr_t)(delayLine + delayLen ) );
  z = AE_ZERO64();
  
  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>2); n++ )
  {
    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_int32x2*)h;

    // Load 4 input samples.
    // Q31
    AE_L32X2_IP( t01, X, +8 );
    AE_L32X2_IP( t23, X, +8 );

    // Store 4 samples to the delay line buffer with circular address update.
    // Q31
    AE_S32X2_XC( t01, D_wr, +8 );
    AE_S32X2_XC( t23, D_wr, +8 );

    {
      Y    = ( ae_f32 *)(y + 4*n*D);
      // Start reading the delay line from the oldest sample, M+8 samples back
      // from the newest one.
      D_rd0 = D_wr;
      D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

      AE_LA32X2POS_PC( D_va, D_rd1 );

      // Load two oldest samples for even/odd accumulators.
      // Q31
      AE_L32X2_XC  ( t01, D_rd0, +8 );
      AE_LA32X2_IC( t12, D_va, D_rd1 );

      // Zero the accumulators.
      q0 = z; q1 = z; q2 = z; q3 = z;
      q4 = z; q5 = z; q6 = z; q7 = z;

      //
      // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
      // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
      // inserted into the impulse response during initialization.
      //
      __Pragma("loop_count min=2")
      for (m = 0; m<(M >> 1) + 2; m++)
      {
        // Load next two samples for even accumulators.
        // Q31
        AE_L32X2_XC( t23, D_rd0, +8 );

        // Load next two samples for odd accumulators. 
        // Q31
        AE_LA32X2_IC( t34, D_va, D_rd1 );

        // Load the next 4 tap coefficients.
        // Q31
        c2 = AE_L32X2_X(C, +4*(M+4));
        AE_L32X2_IP(c0, C, +8);
  
        AE_MULAF32R_HH(q0, t01, c0);
        AE_MULAF32R_LL(q0, t01, c0);
        AE_MULAF32R_HH(q1, t12, c0);
        AE_MULAF32R_LL(q1, t12, c0);
        AE_MULAF32R_HH(q2, t23, c0);
        AE_MULAF32R_LL(q2, t23, c0);
        AE_MULAF32R_HH(q3, t34, c0);
        AE_MULAF32R_LL(q3, t34, c0);

        AE_MULAF32R_HH(q4, t01, c2);
        AE_MULAF32R_LL(q4, t01, c2);
        AE_MULAF32R_HH(q5, t12, c2);
        AE_MULAF32R_LL(q5, t12, c2);
        AE_MULAF32R_HH(q6, t23, c2);
        AE_MULAF32R_LL(q6, t23, c2);
        AE_MULAF32R_HH(q7, t34, c2);
        AE_MULAF32R_LL(q7, t34, c2);
        t01 = t23;
        t12 = t34;      
      }

      //
      // Scale outputs by the interpolation factor 2.
      //
      q0 = AE_ADD64S( q0, q0 );
      q1 = AE_ADD64S( q1, q1 );
      q2 = AE_ADD64S( q2, q2 );
      q3 = AE_ADD64S( q3, q3 );
      q4 = AE_ADD64S( q4, q4 );
      q5 = AE_ADD64S( q5, q5 );
      q6 = AE_ADD64S( q6, q6 );
      q7 = AE_ADD64S( q7, q7 );

      //
      // Store 8 outputs to the output array with 2-samples stride.
      //

      AE_S32RA64S_XP( q0, Y, +D*4 );
      AE_S32RA64S_XP( q1, Y, +D*4 );
      AE_S32RA64S_XP( q2, Y, +D*4 );
      AE_S32RA64S_XP( q3, Y, -3*4*D +4);

      AE_S32RA64S_XP( q4, Y, +D*4 );
      AE_S32RA64S_XP( q5, Y, +D*4 );
      AE_S32RA64S_XP( q6, Y, +D*4 );
      AE_S32RA64S_XP( q7, Y, 0 );
    }
  }
  return (int)((int32_t *)D_wr - delayLine) ;
}
