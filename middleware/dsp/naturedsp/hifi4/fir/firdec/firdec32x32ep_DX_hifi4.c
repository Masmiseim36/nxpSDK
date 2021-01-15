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
    Decimating block real FIR filter, 32x32ep-bit with 72-bit accumulator
    for intermediate computations
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Decimating Block Real FIR Filter
  Computes a real FIR filter (direct-form) with decimation using IR stored 
  in vector h. The real data input is stored in vector x. The filter output 
  result is stored in vector r. The filter calculates N output samples using
  M coefficients and requires last D*N+M-1 samples on the delay line.
  NOTE:
  - To avoid aliasing IR should be synthesized in such a way to be narrower 
    than input sample rate divided to 2D.
  - user application is not responsible for management of delay lines

  Precision: 
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations

  Input:
  h[M]          filter coefficients; h[0] is to be multiplied with the newest 
                sample, Q31 or Q15
  D             decimation factor (has to be 2, 3 or 4)
  N             length of output sample block
  M             length of filter
  x[D*N]        input samples, Q31
  Output:
  y[N]          output samples, Q31

  Restriction:
  x,h,r should not overlap
  x, h - aligned on an 8-bytes boundary
  N - multiple of 8
  D should exceed 1

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  D - 2, 3 or 4
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"

#include "firdec32x32ep_common.h"

/* Generic data processing function for a decimating FIR filter. */
 int fir32x32ep_DX_proc( int32_t * restrict y,
                                  int32_t * delayLine,
                                  int32_t delayLen,
                          const int32_t * restrict x,
                          const int32_t * restrict h,
                          int wrIx, int D, int N, int M )
{
  const ae_int32   *          D_tmp;
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          D_rd2;
  const ae_int32x2 *          D_rd3;
  const ae_int32x2 *          X;
        ae_int32     * restrict Y;
  const ae_int32x2   *        C;

  ae_valign D_va1, D_va3;
  ae_int32x2 t0, t1, t2, t3, t4, t5, t6, t7;
  ae_int32x2 c0, c1;
  ae_int64   q0, q1, q2, q3;
  ae_f64     z;

  ae_ep      ep0, ep1, ep2, ep3;
  ae_f32x2   g_frac;

  int m, n, _0 = 0;

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

  WUR_AE_CBEGIN0( (uintptr_t)(delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)(delayLine + delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( h                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( h + M + 4 ) );
  z = AE_ZERO64();
  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_int32x2*)h;
  {
    int q_frac_int =  D << (16);
    g_frac = AE_MOVDA32X2(q_frac_int, q_frac_int);
  }

  //
  // Break the input signal into 4*D-samples blocks. For each block, store
  // 4*D samples to the delay line buffer, and compute 4 samples of decimated
  // response signal.
  //
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>2); n++ )
  {
    //__Pragma("loop_count min=2")
    for ( m=0; m<2*D; m++ )
    {
      AE_L32X2_IP( t0, X, +8 ); //Q31
      AE_S32X2_XC( t0, D_wr, +8 ); //Q31
    }

    //
    // Setup 4-way delay line reading pointers, one per an accumulator.
    //

    D_tmp = (const ae_int32   *)D_wr;
    D_rd0 = (const ae_int32x2 *)D_wr;

    AE_L32_XC( t0, D_tmp, 4*D );
    D_rd1 = (const ae_int32x2*)D_tmp;

    AE_L32_XC( t0, D_tmp, 4*D );
    D_rd2 = (const ae_int32x2*)D_tmp;

    AE_L32_XC( t0, D_tmp, 4*D );
    D_rd3 = (const ae_int32x2*)D_tmp;

    AE_LA32X2POS_PC( D_va1, D_rd1 );
    AE_LA32X2POS_PC( D_va3, D_rd3 );


    // Zero the accumulators.
    q0 = z; q1 = z; q2 = z; q3 = z;
    XT_MOVEQZ(_0,_0,_0);
    ep0 = AE_MOVEA(_0);ep1 = AE_MOVEA(_0);
    ep2 = AE_MOVEA(_0);ep3 = AE_MOVEA(_0);

    __Pragma("loop_count min=2")
    for ( m=0; m<(M>>2)+1; m++ )
    {
      // Load the next 4 tap coefficients.
      // Q31
      AE_L32X2_XC1(c0, C, +8);
      AE_L32X2_XC1(c1, C, +8);

      // Load 2 samples for each even-numbered accumulator.
      // Q31

      AE_L32X2_XC( t0, D_rd0, +8 );
      AE_L32X2_XC( t2, D_rd2, +8 );
      AE_L32X2_XC( t4, D_rd0, +8 );
      AE_L32X2_XC( t6, D_rd2, +8 );

      AE_LA32X2_IC( t1, D_va1, D_rd1 );
      AE_LA32X2_IC( t3, D_va3, D_rd3 );
      AE_LA32X2_IC( t5, D_va1, D_rd1 );
      AE_LA32X2_IC( t7, D_va3, D_rd3 );
      
      // Q9.62 <- Q9.62 + [ Q31*Q31 ] without symmetric rounding
      AE_MULAAD32EP_HH_LL (ep0,q0,t0,c0);
      AE_MULAAD32EP_HH_LL (ep0,q0,t4,c1);
      AE_MULAAD32EP_HH_LL (ep1,q1,t1,c0);
      AE_MULAAD32EP_HH_LL (ep1,q1,t5,c1);
      AE_MULAAD32EP_HH_LL (ep2,q2,t2,c0);
      AE_MULAAD32EP_HH_LL (ep2,q2,t6,c1);
      AE_MULAAD32EP_HH_LL (ep3,q3,t3,c0);
      AE_MULAAD32EP_HH_LL (ep3,q3,t7,c1);

    }
    //Q17.47 <- Q9.62
    q0 = AE_SRAI72(ep0,q0,15);
    q1 = AE_SRAI72(ep1,q1,15);
    q2 = AE_SRAI72(ep2,q2,15);      
    q3 = AE_SRAI72(ep3,q3,15);
                                               
    // Convert and store filter outputs.
    // Q31 <- Q16.47 - 16 w/ rounding and saturation.
    AE_S32RA64S_IP( q0, Y, +4 );
    AE_S32RA64S_IP( q1, Y, +4 );
    AE_S32RA64S_IP( q2, Y, +4 );
    AE_S32RA64S_IP( q3, Y, +4 );
  }
  return (int)((int32_t *)D_wr - delayLine) ;
}
