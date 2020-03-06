/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    Real block FIR filter, 24x24-bit, packed delay line and coefficient storage
    C code optimized for HiFi4
  IntegrIT, 2006-2014
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfir24x24p_common.h"

/*-------------------------------------------------------------------------
  Real FIR filter.
  Computes a real FIR filter (direct-form) using IR stored in vector h. The 
  real data input is stored in vector x. The filter output result is stored 
  in vector y. The filter calculates N output samples using M coefficients 
  and requires last M-1 samples in the delay line.
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.


  Precision: 
  16x16    16-bit data, 16-bit coefficients, 16-bit outputs. Ordinary variant 
           and stereo
  32x16    32-bit data, 16-bit coefficients, 32-bit outputs
  32x32    32-bit data, 32-bit coefficients, 32-bit outputs. Ordinary variant 
           and stereo
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point. Ordinary variant and stereo

  Input:
  x[N*S]   input samples, Q31, Q15, floating point
  h[M]     filter coefficients in normal order, Q31, Q15, floating point
  hl[M]    for stereo filters: filter coefficients for left channel
  hr[M]    for stereo filters: filter coefficients for right channel
  N        length of sample block, should be a multiple of 4
  M        length of filter, should be a multiple of 4
  extIR    if zero, IR is copied from original location, otherwise not
           but user should keep alignment, order of coefficients 
           and zero padding requirements shown below
  S        1 for ordinary (single channel) filters, 2 - for stereo variant
  
  Output:
  y[N*S]   output samples, Q31, Q15, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  ------------------------+----------+--------------+--------------+----------------
  Function                |Alignment,|Left zero     |   Coefficient| Right zero 
                          | bytes    |padding, bytes|   order      | padding, bytes
  ------------------------+----------+--------------+--------------+----------------
  bkfir16x16_init         |     8    |      2       |  inverted    |  6
  bkfir32x16_init (M>32)  |     8    |     10       |  inverted    |  6
  bkfir32x16_init (M<=32) |     8    |      2       |  inverted    |  6
  bkfir32x32_init         |     8    |      4       |  inverted    |  12
  bkfir32x32ep_init       |     8    |      4       |  inverted    |  12
  bkfirf_init             |     8    |      0       |  direct      |  0
  stereo_bkfir16x16_init  |     8    |      2       |  inverted    |  6
  stereo_bkfir32x32_init  |     8    |      4       |  inverted    |  12
  stereo_bkfirf_init      |     8    |      0       |  direct      |  0
  ------------------------+----------+--------------+--------------+----------------

  Restrictions:
  x, y     should not be overlapping
  x, h     aligned on a 8-bytes boundary
  N, M     multiples of 4 
-------------------------------------------------------------------------*/
void bkfir24x24p_process( bkfir24x24p_handle_t handle, 
                          f24 * restrict      y,
                    const f24 * restrict      x, int N)
{
  bkfir24x24p_ptr_t bkfir = (bkfir24x24p_ptr_t)handle;

  const void     *          C;
  const void     *          D_rd;
        void     * restrict D_wr;
  const ae_f24x2 *          X;
        ae_f24x2 * restrict Y;

  ae_valign C_va, D_va, Y_va;

  ae_f64     q0, q1, q2, q3;
  ae_f64     q4, q5, q6, q7;
  ae_f24x2   d0, d1, d2, d3;
  ae_f24x2   d4, d5;
  ae_int24x2 t0, t1, t2, t3;
  ae_int24x2 t4, t5;
  ae_f24x2   c0, c1;
  ae_int24x2 h0, h1;
  uintptr_t wrIx;
  int M;
  int m, n;


  if(N<=0) return;
  ASSERT( IS_ALIGN( x ) );

  M = bkfir->M;

  ASSERT( bkfir && bkfir->magic == BKFIR24X24P_MAGIC && y && x );

  ASSERT( IS_ALIGN( x ) );

  //
  // Setup pointers and circular delay line buffer.
  //
  wrIx = bkfir->wrIx;
  D_wr = (      ae_int24x2 *)(((uintptr_t)bkfir->delayLine) + wrIx);
  X    = (const ae_f24x2*)x;
  Y    = (      ae_f24x2*)y;
  
  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );

  Y_va = AE_ZALIGN64();

  //
  // Break the input signal into 8-samples blocks. For each block, store 8
  // samples to the delay line and compute the filter response.
  //

  for ( n=0; n<(N>>3); n++ )
  {
    // Load 8 input samples.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_IP( d0, X, +8 );
    AE_L32X2F24_IP( d1, X, +8 );
    AE_L32X2F24_IP( d2, X, +8 );
    AE_L32X2F24_IP( d3, X, +8 );

    t0 = ( d0 );
    t1 = ( d1 );
    t2 = ( d2 );
    t3 = ( d3 );

    D_va = AE_ZALIGN64();

    // Store 8 samples to the delay line buffer with circular address update.
    // Q(23+8) <- Q23 + 8
    AE_SA24X2_IC( t0, D_va, D_wr );
    AE_SA24X2_IC( t1, D_va, D_wr );
    AE_SA24X2_IC( t2, D_va, D_wr );
    AE_SA24X2_IC( t3, D_va, D_wr );

    AE_SA64POS_FC( D_va, D_wr );

    // Circular buffer pointer looks at the oldest sample: M+8 samples back from
    // the newest one.
    D_rd = D_wr;

    AE_LA24X2POS_PC( D_va, D_rd );

    // Load 8 oldest samples from the delay line.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IC( t0, D_va, D_rd );
    AE_LA24X2_IC( t1, D_va, D_rd );
    AE_LA24X2_IC( t2, D_va, D_rd );
    AE_LA24X2_IC( t3, D_va, D_rd );

    d0 = ( t0 );
    d1 = ( t1 );
    d2 = ( t2 );
    d3 = ( t3 );

    //
    // Inner loop prologue: process the first 4 taps for each of 8 accumulators.
    //

    // Load next 4 samples. Altogether we have 12 samples residing in 6 AE
    // registers.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IC( t4, D_va, D_rd );
    AE_LA24X2_IC( t5, D_va, D_rd );

    d4 = ( t4 );
    d5 = ( t5 );

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f24x2*)bkfir->coef;

    C_va = AE_LA64_PP( C );

    // Load 4 tap coefficients.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IP( h0, C_va, C );
    AE_LA24X2_IP( h1, C_va, C );

    c0 = ( h0 );
    c1 = ( h1 );

    // 2xQ16.47 <- 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULFD24X2_FIR_H( q0, q1, d0, d1, c0 );
    AE_MULFD24X2_FIR_H( q2, q3, d1, d2, c0 );
    AE_MULFD24X2_FIR_H( q4, q5, d2, d3, c0 );
    AE_MULFD24X2_FIR_H( q6, q7, d3, d4, c0 );

    // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULAFD24X2_FIR_H( q0, q1, d1, d2, c1 );
    AE_MULAFD24X2_FIR_H( q2, q3, d2, d3, c1 );
    AE_MULAFD24X2_FIR_H( q4, q5, d3, d4, c1 );
    AE_MULAFD24X2_FIR_H( q6, q7, d4, d5, c1 );

    // First 4 taps are done. Move 4 input samples out of the registers file.
    d0 = d2; d1 = d3; d2 = d4; d3 = d5;

    //
    // Inner loop: process 4 taps for 8 accumulators on each trip. Totally we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //
    __Pragma("loop_count min=1")
    for ( m=0; m<(M>>2); m++ )
    {
      // Load 4 samples from the delay line. Altogether we have 12 samples
      // residing in 6 AE registers.
      // Q23 <- Q(23+8) - 8
      AE_LA24X2_IC( t4, D_va, D_rd );
      AE_LA24X2_IC( t5, D_va, D_rd );

      d4 = ( t4 );
      d5 = ( t5 );

      // Load the next 4 tap coefficients.
      // Q23 <- Q(23+8) - 8
      AE_LA24X2_IP( h0, C_va, C );
      AE_LA24X2_IP( h1, C_va, C );

      c0 = ( h0 );
      c1 = ( h1 );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD24X2_FIR_H( q0, q1, d0, d1, c0 );
      AE_MULAFD24X2_FIR_H( q2, q3, d1, d2, c0 );
      AE_MULAFD24X2_FIR_H( q4, q5, d2, d3, c0 );
      AE_MULAFD24X2_FIR_H( q6, q7, d3, d4, c0 );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD24X2_FIR_H( q0, q1, d1, d2, c1 );
      AE_MULAFD24X2_FIR_H( q2, q3, d2, d3, c1 );
      AE_MULAFD24X2_FIR_H( q4, q5, d3, d4, c1 );
      AE_MULAFD24X2_FIR_H( q6, q7, d4, d5, c1 );

      // 4 taps are done. Move 4 input samples out of the registers file.
      d0 = d2; d1 = d3; d2 = d4; d3 = d5;
    }

    // 2xQ23 <- 2xQ16.47 - 24 w/ rounding and saturation.
    d0 = AE_ROUND24X2F48SASYM( q0, q1 );
    d1 = AE_ROUND24X2F48SASYM( q2, q3 );
    d2 = AE_ROUND24X2F48SASYM( q4, q5 );
    d3 = AE_ROUND24X2F48SASYM( q6, q7 );

    // Store 8 filter outputs.
    // 2xQ(23+8) <- 2xQ23 + 8
    AE_SA32X2F24_IP( d0, Y_va, Y );
    AE_SA32X2F24_IP( d1, Y_va, Y );
    AE_SA32X2F24_IP( d2, Y_va, Y );
    AE_SA32X2F24_IP( d3, Y_va, Y );
  }

  //
  // If the signal chunk length N is not a multiple of 8, process the last
  // quadruple: store it to the delay line and compute the filter response.
  //

  if ( N & 4 )
  {
    // Load 4 input samples.
    // Q23 <- Q(23+8) - 8
    AE_L32X2F24_IP( d0, X, +8 );
    AE_L32X2F24_IP( d1, X, +8 );

    t0 = ( d0 );
    t1 = ( d1 );

    D_va = AE_ZALIGN64();

    // Store 4 samples to the delay line buffer with circular address update.
    // Q(23+8) <- Q23 + 8
    AE_SA24X2_IC( t0, D_va, D_wr );
    AE_SA24X2_IC( t1, D_va, D_wr );

    AE_SA64POS_FC( D_va, D_wr );

    // Circular buffer pointer looks at the oldest sample: M+8 samples back from
    // the newest one. 
    D_rd = D_wr;

    AE_LA24X2POS_PC( D_va, D_rd );

    // Perform dummy reads to jump over 4 oldest 32-bit entries with circular
    // address update. Now the pointer is M+4 samples apart from the newest one.
    AE_LA24X2_IC( t0, D_va, D_rd );
    AE_LA24X2_IC( t1, D_va, D_rd );

    // Load 4 samples from the delay line.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IC( t0, D_va, D_rd );
    AE_LA24X2_IC( t1, D_va, D_rd );

    d0 = ( t0 );
    d1 = ( t1 );

    //
    // Inner loop prologue: process the first 4 taps for each of 4 accumulators.
    //

    // Load next 4 samples. Altogether we have 8 samples residing in 4 AE 
    // registers.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IC( t2, D_va, D_rd );
    AE_LA24X2_IC( t3, D_va, D_rd );

    d2 = ( t2 );
    d3 = ( t3 );

    // Reset the coefficients pointer. Now it looks at the tap corresponding
    // to the oldest sample in the delay line.
    C = (const ae_f24x2*)bkfir->coef;

    C_va = AE_LA64_PP( C );

    // Load 4 tap coefficients.
    // Q23 <- Q(23+8) - 8
    AE_LA24X2_IP( h0, C_va, C );
    AE_LA24X2_IP( h1, C_va, C );

    c0 = ( h0 );
    c1 = ( h1 );

    // 2xQ16.47 <- 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULFD24X2_FIR_H( q0, q1, d0, d1, c0 );
    AE_MULFD24X2_FIR_H( q2, q3, d1, d2, c0 );

    // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULAFD24X2_FIR_H( q0, q1, d1, d2, c1 );
    AE_MULAFD24X2_FIR_H( q2, q3, d2, d3, c1 );

    // First 4 taps are done. Move 4 input samples out of the registers file.
    d0 = d2; d1 = d3;

    //
    // Inner loop: process 4 taps for 4 accumulators on each trip. Totally we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //

    for ( m=0; m<(M>>2); m++ )
    {
      // Load 4 samples from the delay line. Altogether we have 8 samples 
      // residing in 4 AE registers.
      AE_LA24X2_IC( t2, D_va, D_rd );
      AE_LA24X2_IC( t3, D_va, D_rd );

      d2 = ( t2 );
      d3 = ( t3 );

      // Load the next 4 tap coefficients.
      // Q23 <- Q(23+8) - 8
      AE_LA24X2_IP( h0, C_va, C );
      AE_LA24X2_IP( h1, C_va, C );

      c0 = ( h0 );
      c1 = ( h1 );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD24X2_FIR_H( q0, q1, d0, d1, c0 );
      AE_MULAFD24X2_FIR_H( q2, q3, d1, d2, c0 );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD24X2_FIR_H( q0, q1, d1, d2, c1 );
      AE_MULAFD24X2_FIR_H( q2, q3, d2, d3, c1 );

      // 4 taps are done. Move 4 input samples out of the registers file.
      d0 = d2; d1 = d3;
    }

    // 2xQ23 <- 2xQ16.47 - 24 w/ rounding and saturation.
    d0 = AE_ROUND24X2F48SASYM( q0, q1 );
    d1 = AE_ROUND24X2F48SASYM( q2, q3 );

    // Store 4 filter outputs.
    // 2xQ(23+8) <- 2xQ23 + 8
    AE_SA32X2F24_IP( d0, Y_va, Y );
    AE_SA32X2F24_IP( d1, Y_va, Y );
  }

  AE_SA64POS_FP( Y_va, Y );

   bkfir->wrIx = (uintptr_t)D_wr - (uintptr_t)bkfir->delayLine ;
} // bkfir24x24p_process()
