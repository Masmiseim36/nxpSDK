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
    Real block FIR filter, 32x16-bit, unaligned data and arbitrary M/N allowed
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

                                                         
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfira32x16_common.h"

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  These functions implement FIR filter with no limitation on size of data
  block, alignment and length of impulse response at the cost of increased
  processing complexity.
  NOTE: 
  User application is not responsible for management of delay lines.

  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point
  Input:
  x[N]     input samples, Q15, Q31, floating point
  h[M]     filter coefficients in normal order, Q15, Q31, floating point
  N        length of sample block
  M        length of filter
  Output:
  y[N]     input samples, Q15, Q31, floating point 

  Restrictions:
  x,y      should not be overlapping
-------------------------------------------------------------------------*/
void bkfira32x16_process( bkfira32x16_handle_t handle, 
                          int32_t * restrict   y,
                    const int32_t * restrict   x, int N)
{
  bkfira32x16_ptr_t bkfir = (bkfira32x16_ptr_t)handle;

  const ae_f16x4   *          C;                            
        ae_int32x2 * restrict D_wr;
        ae_int32   * restrict D_s_wr;
  const ae_int32x2 * restrict D_rd0;
  const ae_int32x2 * restrict D_rd1;
  const ae_int32   * restrict D_s_rd;                
  const ae_int32x2 * restrict X;
  const ae_int32   * restrict X_s;
        ae_int32     * restrict Y;

  ae_valign D_va0, D_va1, X_va;

  ae_int32x2 t0, t1, t2, t3;
  ae_int32x2 t01, t12, t23, t34, t45, t56, t67, t78, t89, t9a;
  ae_f64     q0, q1, q2, q3, q4, q5, q6, q7;
  ae_int16x4 c;
  ae_f64     z; 
  int M;
  int m, n;

  if(N<=0) return;
  ASSERT( bkfir && bkfir->magic == BKFIRA32X16_MAGIC && y && x );

  M = bkfir->M;

  ASSERT( IS_ALIGN( bkfir->delayLine                   ) &&
          IS_ALIGN( bkfir->delayLine + bkfir->delayLen ) &&
          IS_ALIGN( bkfir->coef + bkfir->M + 4 ) &&
          IS_ALIGN( bkfir->coef ) );

  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2*)(bkfir->delayLine + bkfir->wrIx);
  X    = (const ae_int32x2*)x;
  Y    = (      ae_int32    *)y;

  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( bkfir->coef                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( bkfir->coef + bkfir->M + 4 ) );

  
  X_va = AE_LA64_PP( X );
  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_f16x4*)bkfir->coef ;
  //
  // Break the input signal into 8-samples blocks. For each block, store 8
  // samples to the delay line and compute the filter response.
  //
  z = AE_ZERO64();
  for ( n=0; n<(N>>3); n++ )
  {
    // Load 8 input samples.
    // Q31
    AE_LA32X2_IP( t0, X_va, X );
    AE_LA32X2_IP( t1, X_va, X );
    AE_LA32X2_IP( t2, X_va, X );
    AE_LA32X2_IP( t3, X_va, X );

    D_va0 = AE_ZALIGN64();

    // Store 8 samples to the delay line buffer with circular address update.
    // Q31
    AE_SA32X2_IC( t0, D_va0, D_wr );
    AE_SA32X2_IC( t1, D_va0, D_wr );
    AE_SA32X2_IC( t2, D_va0, D_wr );
    AE_SA32X2_IC( t3, D_va0, D_wr );

    AE_SA64POS_FP( D_va0, D_wr );

    // Circular buffer pointer looks at the oldest sample: M+8 samples back from
    // the newest one.
    D_rd0 = D_wr;
    D_s_rd = (      ae_int32*)D_wr;
    AE_L32_XC( t0, D_s_rd, +4 );
    D_rd1 = (ae_int32x2*)( D_s_rd );

    AE_LA32X2POS_PC( D_va0, D_rd0 );
    AE_LA32X2POS_PC( D_va1, D_rd1 );
    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_LA32X2_IC  ( t01, D_va0, D_rd0 );
    AE_LA32X2_IC  ( t23, D_va0, D_rd0 );
    AE_LA32X2_IC  ( t45, D_va0, D_rd0 );
    AE_LA32X2_IC( t12, D_va1, D_rd1 );
    AE_LA32X2_IC( t34, D_va1, D_rd1 );
    AE_LA32X2_IC( t56, D_va1, D_rd1 );

    q0 = z; q1 = z; q2 = z; q3 = z;
    q4 = z; q5 = z; q6 = z; q7 = z;

    //
    // Inner loop: process 4 taps for 8 accumulators on each trip. Totally we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //
    __Pragma("loop_count min=2")   
    __Pragma("no_unroll")
    for ( m=0; m<(M>>2)+1; m++ )
    {
      AE_LA32X2_IC( t67, D_va0, D_rd0 );
      AE_LA32X2_IC( t89, D_va0, D_rd0 );


      // Load next two samples for odd accumulators. 
      // Q31
      AE_LA32X2_IC( t78, D_va1, D_rd1 );
      AE_LA32X2_IC( t9a, D_va1, D_rd1 );

      // Load the next 4 tap coefficients.
      // Q31
      AE_L16X4_XC1(c, C, +8);

      //Q16.47
      AE_MULAAAAFQ32X16(q0,t01,t23,c);
      AE_MULAAAAFQ32X16(q1,t12,t34,c);
      AE_MULAAAAFQ32X16(q2,t23,t45,c);
      AE_MULAAAAFQ32X16(q3,t34,t56,c);

      AE_MULAAAAFQ32X16(q4,t45,t67,c);
      AE_MULAAAAFQ32X16(q5,t56,t78,c);
      AE_MULAAAAFQ32X16(q6,t67,t89,c);
      AE_MULAAAAFQ32X16(q7,t78,t9a,c);

      t01 = t45; t23 = t67; t45 = t89; 
      t12 = t56; t34 = t78; t56 = t9a;
    }

    // Convert and save 8 outputs.
    // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
    AE_S32RA64S_IP( q0, Y, +4 );
    AE_S32RA64S_IP( q1, Y, +4 );
    AE_S32RA64S_IP( q2, Y, +4 );
    AE_S32RA64S_IP( q3, Y, +4 );
    AE_S32RA64S_IP( q4, Y, +4 );
    AE_S32RA64S_IP( q5, Y, +4 );
    AE_S32RA64S_IP( q6, Y, +4 );
    AE_S32RA64S_IP( q7, Y, +4 );
  }

  //
  // Process the last N&7 samples. 
  //


  D_s_wr = (      ae_int32*)D_wr;

  if ( N&7 )
  {
    X_s    = (const ae_int32*)X;

    // Insert 0..7 input samples into the delay line one-by-one.
    for ( n=0; n<(N&7); n++ )
    {
      AE_L32_IP ( t0, X_s   , +4 );
      AE_S32_L_XC( t0, D_s_wr, +4 );
    }

    D_s_rd = D_s_wr;

    // Perform dummy reads to skip 8-(N&7) oldest samples.
    for ( n=0; n<(-N&7); n++ )
    {
      AE_L32_XC( t0, D_s_rd, +4 );
    }

    D_rd0 = (const ae_int32x2*)D_s_rd;
    AE_L32_XC( t0, D_s_rd, +4 );
    D_rd1 = (ae_int32x2*)( D_s_rd );

    AE_LA32X2POS_PC( D_va0, D_rd0 );
    AE_LA32X2POS_PC( D_va1, D_rd1 );
    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_LA32X2_IC  ( t01, D_va0, D_rd0 );
    AE_LA32X2_IC  ( t23, D_va0, D_rd0 );
    AE_LA32X2_IC  ( t45, D_va0, D_rd0 );
    AE_LA32X2_IC( t12, D_va1, D_rd1 );
    AE_LA32X2_IC( t34, D_va1, D_rd1 );
    AE_LA32X2_IC( t56, D_va1, D_rd1 );


    q0 = z; q1 = z; q2 = z; q3 = z;
    q4 = z; q5 = z; q6 = z; q7 = z;

    //
    // Inner loop: process 4 taps for 8 accumulators on each trip. Totally we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //
    __Pragma("loop_count min=2")
    __Pragma("no_unroll")
    for ( m=0; m<(M>>2)+1; m++ )
    {
      AE_LA32X2_IC( t67, D_va0, D_rd0 );
      AE_LA32X2_IC( t89, D_va0, D_rd0 );

      // Load next two samples for odd accumulators. 
      // Q31
      AE_LA32X2_IC( t78, D_va1, D_rd1 );
      AE_LA32X2_IC( t9a, D_va1, D_rd1 );

      // Load the next 4 tap coefficients.
      // Q31
      AE_L16X4_XC1(c, C, +8);

      //Q16.47
      AE_MULAAAAFQ32X16(q0,t01,t23,c);
      AE_MULAAAAFQ32X16(q1,t12,t34,c);
      AE_MULAAAAFQ32X16(q2,t23,t45,c);
      AE_MULAAAAFQ32X16(q3,t34,t56,c);

      AE_MULAAAAFQ32X16(q4,t45,t67,c);
      AE_MULAAAAFQ32X16(q5,t56,t78,c);
      AE_MULAAAAFQ32X16(q6,t67,t89,c);
      AE_MULAAAAFQ32X16(q7,t78,t9a,c);

      t01 = t45; t23 = t67; t45 = t89; 
      t12 = t56; t34 = t78; t56 = t9a;
    }

    Y = (ae_int32*)( y + N-1 );

    // Convert and save 1..7 outputs.
    // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
    switch ( N & 7 )
    {
    case 7: AE_S32RA64S_IP( q6, Y, -4 );
    case 6: AE_S32RA64S_IP( q5, Y, -4 );
    case 5: AE_S32RA64S_IP( q4, Y, -4 );
    case 4: AE_S32RA64S_IP( q3, Y, -4 );
    case 3: AE_S32RA64S_IP( q2, Y, -4 );
    case 2: AE_S32RA64S_IP( q1, Y, -4 );
    }

    AE_S32RA64S_IP( q0, Y, -4 );
  }
  bkfir->wrIx = (int)((int32_t *)D_s_wr - bkfir->delayLine) ;
} // bkfira32x16_process()
