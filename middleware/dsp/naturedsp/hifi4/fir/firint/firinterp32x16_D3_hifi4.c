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


/* Data processing function for a factor 3 interpolating FIR filter. */
 int firinterp32x16_D3_proc( int32_t * restrict y,
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
  const ae_int16x4 *          C1;
  const ae_int16x4 *          C1_;

  ae_valign D_va, C_va, C1_va;
  ae_int32x2 t01, t23, t45;
  ae_int32x2 t12, t34, t56;
  ae_int16x4 c0, c1, c2, c3;
  ae_f64     q0, q1, q2, q3;
  ae_f64     q4, q5, q6, q7;
  ae_int64   w0, w1, w2, w3;
  ae_int64   w4, w5, w6, w7;
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
  Y    = (      ae_int32     *)y;

  if (M>8)
  {
    WUR_AE_CBEGIN0( (uintptr_t)(delayLine            ) );
    WUR_AE_CEND0  ( (uintptr_t)(delayLine + delayLen ) );
    WUR_AE_CBEGIN1( (uintptr_t)( h                   ) );
    WUR_AE_CEND1  ( (uintptr_t)( h + (M + 4+4)*3 ) );
    z = AE_ZERO64();

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_int16x4*)(h+4);

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

      // Start reading the delay line from the oldest sample, M+4 samples back
      // from the newest one.
      D_rd1 = D_wr;
      D_rd0 = (ae_int32x2*)( D_rd1 + 1 );
      C1 = (const ae_int16x4*)(h+ (M + 4+4 +4));
      C_ = (const ae_int16x4*)((uintptr_t)C-2);
      C1_ = (const ae_int16x4*)((uintptr_t)C1-2);
      AE_LA16X4POS_PC1(C_va,C_);
      AE_LA16X4POS_PC1(C1_va,C1_);
      AE_L32X2_XC  ( t01, D_rd1, +16 );
      // Zero the accumulators.
      q0 = z; q1 = z; q2 = z; q3 = z;
      q4 = z; q5 = z; q6 = z; q7 = z;

      //
      // Inner loop: process 4 taps for 8 accumulators on each trip. Actually we 
      // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
      // inserted into the impulse response during initialization.
      //
      AE_LA16X4_IC1( c1, C_va, C_ );
      AE_L16X4_XC1(c0,C,+8);
      __Pragma("loop_count min=2");
      for ( m=0; m<(M>>2)+1; m++ )
      {
        // Load next two samples for even accumulators. Altogether we have
        // 6 samples residing in 3 AE registers.
        // Q31

        AE_L32X2_XC( t23, D_rd0, +16 );
        AE_L32X2_XC( t45, D_rd1, +16 );
       
        AE_LA16X4_IC1( c3, C1_va, C1_ );
        AE_L16X4_XC1(c2,C1,+8);
                                                            
        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c0);
        AE_MULAAAAFQ32X16(q1,t01,t23,c1);
        AE_MULAAAAFQ32X16(q2,t23,t45,c0);
        AE_MULAAAAFQ32X16(q3,t23,t45,c1);


        AE_MULAAAAFQ32X16(q4,t01,t23,c2);
        AE_MULAAAAFQ32X16(q5,t01,t23,c3);
        AE_MULAAAAFQ32X16(q6,t23,t45,c2);
        AE_MULAAAAFQ32X16(q7,t23,t45,c3);

        t01 = t45; 
        AE_LA16X4_IC1( c1, C_va, C_ );
        AE_L16X4_XC1(c0,C,+8);                                      
      }

      w0 = AE_ADD64S(q0,q0);
      w1 = AE_ADD64S(q1,q1);
      w2 = AE_ADD64S(q2,q2);
      w3 = AE_ADD64S(q3,q3);
      w4 = AE_ADD64S(q4,q4);
      w5 = AE_ADD64S(q5,q5);
      w6 = AE_ADD64S(q6,q6);
      w7 = AE_ADD64S(q7,q7);

      q0 = AE_ADD64S(w0,q0);
      q1 = AE_ADD64S(w1,q1);
      q2 = AE_ADD64S(w2,q2);
      q3 = AE_ADD64S(w3,q3);
      q4 = AE_ADD64S(w4,q4);
      q5 = AE_ADD64S(w5,q5);
      q6 = AE_ADD64S(w6,q6);
      q7 = AE_ADD64S(w7,q7);

      //
      // Store 8 outputs to the output array with 2-samples stride.
      //

      AE_S32RA64S_XP( q0, Y, +4 );
      AE_S32RA64S_XP( q4, Y, +2*4 );                                 
      AE_S32RA64S_XP( q1, Y, +4 );
      AE_S32RA64S_XP( q5, Y, +2*4 );
      AE_S32RA64S_XP( q2, Y, +4 );
      AE_S32RA64S_XP( q6, Y, +2*4 );
      AE_S32RA64S_XP( q3, Y, +4 );
      AE_S32RA64S_XP( q7, Y, -3*4*3+4 );


      // Start reading the delay line from the oldest sample, M+4 samples back
      // from the newest one.
      D_rd1 = D_wr;
      D_rd0 = (ae_int32x2*)( D_rd1 + 1 );
      C = C1 +1;
      C_ = (const ae_int16x4*)((uintptr_t)C-2);
      AE_LA16X4POS_PC1(C_va,C_);
      AE_L32X2_XC  ( t01, D_rd1, +16 );
      // Zero the accumulators.
      q0 = z; q1 = z; q2 = z; q3 = z;

      //
      // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
      // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
      // inserted into the impulse response during initialization.
      //
      AE_LA16X4_IC1( c1, C_va, C_ );
      AE_L16X4_XC1(c0,C,+8);
      __Pragma("loop_count min=2");
      for ( m=0; m<(M>>2)+1; m++ )
      {
        // Load next two samples for even accumulators. Altogether we have
        // 6 samples residing in 3 AE registers.
        // Q31

        AE_L32X2_XC( t23, D_rd0, +16 );
        AE_L32X2_XC( t45, D_rd1, +16 );
                                                            
        //Q16.47
        AE_MULAAAAFQ32X16(q0,t01,t23,c0);
        AE_MULAAAAFQ32X16(q1,t01,t23,c1);
        AE_MULAAAAFQ32X16(q2,t23,t45,c0);
        AE_MULAAAAFQ32X16(q3,t23,t45,c1);

        t01 = t45; 
        AE_LA16X4_IC1( c1, C_va, C_ );
        AE_L16X4_XC1(c0,C,+8);                                      
      }
      w0 = AE_ADD64S(q0,q0);
      w1 = AE_ADD64S(q1,q1);
      w2 = AE_ADD64S(q2,q2);
      w3 = AE_ADD64S(q3,q3);

      q0 = AE_ADD64S(w0,q0);
      q1 = AE_ADD64S(w1,q1);
      q2 = AE_ADD64S(w2,q2);
      q3 = AE_ADD64S(w3,q3);

      //
      // Store 4 outputs to the output array with 2-samples stride.
      //

      AE_S32RA64S_XP( q0, Y, +3*4 );
      AE_S32RA64S_XP( q1, Y, +3*4 );
      AE_S32RA64S_XP( q2, Y, +3*4 );
      AE_S32RA64S_XP( q3, Y, +4 );
    }
  }
  else
  {
    WUR_AE_CBEGIN0( (uintptr_t)(delayLine            ) );
    WUR_AE_CEND0  ( (uintptr_t)(delayLine + delayLen ) );
    WUR_AE_CBEGIN1( (uintptr_t)( h + (M + 4)   ) );
    WUR_AE_CEND1  ( (uintptr_t)( h + (M + 4)*3 ) );
    z = AE_ZERO64();

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_int16x4*)( h + (M + 4) );
    //
    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    Y    = ( ae_int32 *)(y);
    __Pragma("loop_count min=1")
    for ( n=0; n<(N>>2); n++ )
    {
      // Load 4 input samples.
      // Q31
      AE_L32X2_IP( t01, X, +8 );
      AE_L32X2_IP( t23, X, +8 );

      // Store 4 samples to the delay line buffer with circular address update.
      // Q31
      AE_S32X2_XC( t01, D_wr, +8 );
      AE_S32X2_XC( t23, D_wr, +8 );

      {
        
        // Start reading the delay line from the oldest sample, M+4 samples back
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
        for ( m=0; m<(M>>2)+1; m++ )
        {
          // Load next two samples for even accumulators. Altogether we have
          // 6 samples residing in 3 AE registers.
          // Q31
          AE_L32X2_XC( t23, D_rd0, +8 );
          AE_L32X2_XC( t45, D_rd0, +8 );

          // Load next two samples for odd accumulators. 
          // Q31
          AE_LA32X2_IC( t34, D_va, D_rd1 );
          AE_LA32X2_IC( t56, D_va, D_rd1 );

          // Load the next 4 tap coefficients.
          // Q31
          c0 = AE_L16X4_X(C, -2*(M+4));
          AE_L16X4_XC1(c1, C, +8);

          //Q16.47
          AE_MULAAAAFQ32X16(q0,t01,t23,c0);
          AE_MULAAAAFQ32X16(q1,t12,t34,c0);
          AE_MULAAAAFQ32X16(q2,t23,t45,c0);
          AE_MULAAAAFQ32X16(q3,t34,t56,c0);

          AE_MULAAAAFQ32X16(q4,t01,t23,c1);
          AE_MULAAAAFQ32X16(q5,t12,t34,c1);
          AE_MULAAAAFQ32X16(q6,t23,t45,c1);
          AE_MULAAAAFQ32X16(q7,t34,t56,c1);

          t01 = t45;
          t12 = t56;
        }

        w0 = AE_ADD64S(q0,q0);
        w1 = AE_ADD64S(q1,q1);
        w2 = AE_ADD64S(q2,q2);
        w3 = AE_ADD64S(q3,q3);
        w4 = AE_ADD64S(q4,q4);
        w5 = AE_ADD64S(q5,q5);
        w6 = AE_ADD64S(q6,q6);
        w7 = AE_ADD64S(q7,q7);

        q0 = AE_ADD64S(w0,q0);
        q1 = AE_ADD64S(w1,q1);
        q2 = AE_ADD64S(w2,q2);
        q3 = AE_ADD64S(w3,q3);
        q4 = AE_ADD64S(w4,q4);
        q5 = AE_ADD64S(w5,q5);
        q6 = AE_ADD64S(w6,q6);
        q7 = AE_ADD64S(w7,q7);

        //
        // Store 4 outputs to the output array with 2-samples stride.
        //

        AE_S32RA64S_XP( q0, Y, +4 );
        AE_S32RA64S_XP( q4, Y, +2*4 );
        AE_S32RA64S_XP( q1, Y, +4 );
        AE_S32RA64S_XP( q5, Y, +2*4 );
        AE_S32RA64S_XP( q2, Y, +4 );
        AE_S32RA64S_XP( q6, Y, +2*4 );
        AE_S32RA64S_XP( q3, Y, +4 );
        AE_S32RA64S_XP( q7, Y, -3*4*3+4 );

        /////////////////////////////////
        // Start reading the delay line from the oldest sample, M+4 samples back
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

        //
        // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
        // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
        // inserted into the impulse response during initialization.
        //
        __Pragma("loop_count min=2")
        for ( m=0; m<(M>>2)+1; m++ )
        {
          // Load next two samples for even accumulators. Altogether we have
          // 6 samples residing in 3 AE registers.
          // Q31
          AE_L32X2_XC( t23, D_rd0, +8 );
          AE_L32X2_XC( t45, D_rd0, +8 );

          // Load next two samples for odd accumulators. 
          // Q31
          AE_LA32X2_IC( t34, D_va, D_rd1 );
          AE_LA32X2_IC( t56, D_va, D_rd1 );


          // Load the next 4 tap coefficients.
          // Q31
          AE_L16X4_XC1(c0, C, +8);

          //Q16.47
          AE_MULAAAAFQ32X16(q0,t01,t23,c0);
          AE_MULAAAAFQ32X16(q1,t12,t34,c0);
          AE_MULAAAAFQ32X16(q2,t23,t45,c0);
          AE_MULAAAAFQ32X16(q3,t34,t56,c0);

          t01 = t45;
          t12 = t56;
        }

        w0 = AE_ADD64S(q0,q0);
        w1 = AE_ADD64S(q1,q1);
        w2 = AE_ADD64S(q2,q2);
        w3 = AE_ADD64S(q3,q3);

        q0 = AE_ADD64S(w0,q0);
        q1 = AE_ADD64S(w1,q1);
        q2 = AE_ADD64S(w2,q2);
        q3 = AE_ADD64S(w3,q3);

        //
        // Store 4 outputs to the output array with 2-samples stride.
        //

        AE_S32RA64S_XP( q0, Y, +3*4 );
        AE_S32RA64S_XP( q1, Y, +3*4 );
        AE_S32RA64S_XP( q2, Y, +3*4 );
        AE_S32RA64S_XP( q3, Y, -3*4*3+4 );
      }
      Y = (ae_int32 *) XT_ADD((uintptr_t)Y,4*3*3);
    }
  }

  return (int)((int32_t *)D_wr - delayLine) ;
}
