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
    Real block FIR filter, 32x32-bit
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

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

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Filter instance structure. */
#include "stereo_bkfir32x32_common.h"

void stereo_bkfir32x32_process( stereo_bkfir32x32_handle_t handle, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N )
{
  stereo_bkfir32x32_t * bkfir = (stereo_bkfir32x32_ptr_t)handle;
  const ae_f32x2 *          Cl;
  const ae_f32x2 *          Cr;
  const ae_f32x2 *          D_rdl;
  const ae_f32x2 *          D_rdr;
        ae_f32x2 * restrict D_wrl;
        ae_f32x2 * restrict D_wrr;
  const ae_f32x2 *          X;
        ae_f32x2 * restrict Y;

  ae_valign Y_va;

  ae_f64     ql0, ql1, ql2, ql3;
  ae_f64     qr0, qr1, qr2, qr3;
  ae_int32x2 dl0, dl1, dl2, dl3;
  ae_int32x2 dr0, dr1, dr2, dr3;
  ae_int32x2 cl0, cl1;
  ae_int32x2 cr0, cr1;
  ae_int32x2 x0, x1, x2, x3;
  ae_int32x2 y0, y1, y2, y3;

  int M;
  int m, n;

//  ASSERT( bkfir && bkfir->magic == MAGIC && y && x );
  if(N<=0) return;
  ASSERT( IS_ALIGN( x ) );

  M = bkfir->M;

  ASSERT( !( M & 3 ) && !( N & 3 ) );

  ASSERT( IS_ALIGN( bkfir->delayLineLeft                   ) &&
          IS_ALIGN( bkfir->delayLineLeft + bkfir->delayLen ) &&
          IS_ALIGN( bkfir->delayPosLeft                    ) &&
          IS_ALIGN( bkfir->coefLeft ) );

  //
  // Setup pointers and circular delay line buffer.
  //

  X    = (const ae_f32x2*)x;
  Y    = (      ae_f32x2*)y;

  D_wrl = (      ae_f32x2*)bkfir->delayPosLeft;
  D_wrr = (      ae_f32x2*)bkfir->delayPosRight;

  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLineLeft                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLineLeft + bkfir->delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( bkfir->delayLineRight                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( bkfir->delayLineRight + bkfir->delayLen ) );

  Y_va = AE_ZALIGN64();

  for ( n=0; n<(N>>2); n++ )
  {
    // Load 4 input stereo-samples.
    // Q23 <- Q(23+8) - 8
    AE_L32X2_IP( x0, X, 2*sizeof(int32_t) );
    AE_L32X2_IP( x1, X, 2*sizeof(int32_t) );
    AE_L32X2_IP( x2, X, 2*sizeof(int32_t) );
    AE_L32X2_IP( x3, X, 2*sizeof(int32_t) );
	dl0 = AE_SEL32_HH( x0, x1 );
	dl1 = AE_SEL32_HH( x2, x3 );
	dr0 = AE_SEL32_LL( x0, x1 );
	dr1 = AE_SEL32_LL( x2, x3 );

    // Store 2x4 samples to the delay line buffers with circular address update.
    // Q(23+8) <- Q23 + 8
    AE_S32X2_XC ( dl0, D_wrl, 2*sizeof(int32_t) );
    AE_S32X2_XC ( dl1, D_wrl, 2*sizeof(int32_t) );
    AE_S32X2_XC1( dr0, D_wrr, 2*sizeof(int32_t) );
    AE_S32X2_XC1( dr1, D_wrr, 2*sizeof(int32_t) );

    // Circular buffer pointer looks at the oldest sample: M+8 samples back from
    // the newest ones.
	D_rdl = D_wrl;
	D_rdr = D_wrr;

    // Perform dummy reads to jump over 2x4 oldest 32-bit entries with circular
    // address update. Now the pointers are M+4 samples apart from the newest ones.
    AE_L32X2_XC ( dl0, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC ( dl1, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr0, D_rdr, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr1, D_rdr, 2*sizeof(int32_t) );

    // Load 2x4 samples from the delay line.
    // Q23 <- Q(23+8) - 8
    AE_L32X2_XC ( dl0, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC ( dl1, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr0, D_rdr, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr1, D_rdr, 2*sizeof(int32_t) );

    //
    // Inner loop prologue: process the first 2x4 taps for each of 2x4 accumulators.
    //

    // Load next 2x4 samples. Altogether we have 8 stereo-samples residing in 2x4 AE 
    // registers.
    // Q23 <- Q(23+8) - 8
    AE_L32X2_XC ( dl2, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC ( dl3, D_rdl, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr2, D_rdr, 2*sizeof(int32_t) );
    AE_L32X2_XC1( dr3, D_rdr, 2*sizeof(int32_t) );

	// Reset the coefficients pointers. Now they look at the taps corresponding
    // to the oldest samples in the delay lines.
    Cl = (const ae_f32x2*)bkfir->coefLeft;
    Cr = (const ae_f32x2*)bkfir->coefRight;

	// Load 2x4 tap coefficients.
    // Q23 <- Q(23+8) - 8
    AE_L32X2_IP( cl0, Cl, 2*sizeof(int32_t) );
    AE_L32X2_IP( cl1, Cl, 2*sizeof(int32_t) );
    AE_L32X2_IP( cr0, Cr, 2*sizeof(int32_t) );
    AE_L32X2_IP( cr1, Cr, 2*sizeof(int32_t) );

    // 2xQ16.47 <- 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULFD32X2RA_FIR_H( ql0, ql1, dl0, dl1, cl0 );
    AE_MULFD32X2RA_FIR_H( ql2, ql3, dl1, dl2, cl0 );
    AE_MULFD32X2RA_FIR_H( qr0, qr1, dr0, dr1, cr0 );
    AE_MULFD32X2RA_FIR_H( qr2, qr3, dr1, dr2, cr0 );

    // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
    AE_MULAFD32X2RA_FIR_H( ql0, ql1, dl1, dl2, cl1 );
    AE_MULAFD32X2RA_FIR_H( ql2, ql3, dl2, dl3, cl1 );
    AE_MULAFD32X2RA_FIR_H( qr0, qr1, dr1, dr2, cr1 );
    AE_MULAFD32X2RA_FIR_H( qr2, qr3, dr2, dr3, cr1 );

	// First 2x4 taps are done. Move 2x4 input samples out of the registers file.
    dl0 = dl2; dl1 = dl3;
    dr0 = dr2; dr1 = dr3;

    //
    // Inner loop: process 2x4 taps for 2x4 accumulators on each trip. Totally we 
    // perform 2*(M+4) MACs for each accumulator, 2x4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //
    for ( m=0; m<(M>>2); m++ )
    {
      // Load 2x4 samples from the delay line. Altogether we have 2x8 samples 
      // residing in 2x4 AE registers.
      AE_L32X2_XC ( dl2, D_rdl, 2*sizeof(int32_t) );
      AE_L32X2_XC ( dl3, D_rdl, 2*sizeof(int32_t) );
      AE_L32X2_XC1( dr2, D_rdr, 2*sizeof(int32_t) );
      AE_L32X2_XC1( dr3, D_rdr, 2*sizeof(int32_t) );

      // Load the next 2x4 tap coefficients.
      // Q23 <- Q(23+8) - 8
      AE_L32X2_IP( cl0, Cl, 2*sizeof(int32_t) );
      AE_L32X2_IP( cl1, Cl, 2*sizeof(int32_t) );
      AE_L32X2_IP( cr0, Cr, 2*sizeof(int32_t) );
      AE_L32X2_IP( cr1, Cr, 2*sizeof(int32_t) );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD32X2RA_FIR_H( ql0, ql1, dl0, dl1, cl0 );
      AE_MULAFD32X2RA_FIR_H( ql2, ql3, dl1, dl2, cl0 );
      AE_MULAFD32X2RA_FIR_H( qr0, qr1, dr0, dr1, cr0 );
      AE_MULAFD32X2RA_FIR_H( qr2, qr3, dr1, dr2, cr0 );

      // 2xQ16.47 <- 2xQ16.47 + 2x[ Q23*Q23 + 1 ] + 2x[ Q23*Q23 + 1 ]
      AE_MULAFD32X2RA_FIR_H( ql0, ql1, dl1, dl2, cl1 );
      AE_MULAFD32X2RA_FIR_H( ql2, ql3, dl2, dl3, cl1 );
      AE_MULAFD32X2RA_FIR_H( qr0, qr1, dr1, dr2, cr1 );
      AE_MULAFD32X2RA_FIR_H( qr2, qr3, dr2, dr3, cr1 );

      // 2x4 taps are done. Move 2x4 input samples out of the registers file.
      dl0 = dl2; dl1 = dl3;
      dr0 = dr2; dr1 = dr3;
    }

    // 2xQ23 <- 2xQ16.47 - 24 w/ rounding and saturation.
    dl0 = AE_ROUND32X2F48SASYM( ql0, ql1 );
    dl1 = AE_ROUND32X2F48SASYM( ql2, ql3 );
    dr0 = AE_ROUND32X2F48SASYM( qr0, qr1 );
    dr1 = AE_ROUND32X2F48SASYM( qr2, qr3 );

	y0 = AE_SEL32_HH( dl0, dr0 );
	y1 = AE_SEL32_LL( dl0, dr0 );
	y2 = AE_SEL32_HH( dl1, dr1 );
	y3 = AE_SEL32_LL( dl1, dr1 );

    // Store 4 filter outputs.
    // 2xQ(23+8) <- 2xQ23 + 8
    AE_SA32X2_IP( y0, Y_va, Y );
    AE_SA32X2_IP( y1, Y_va, Y );
    AE_SA32X2_IP( y2, Y_va, Y );
    AE_SA32X2_IP( y3, Y_va, Y );
  }

  AE_SA64POS_FP( Y_va, Y );

  bkfir->delayPosLeft  = (int32_t*)D_wrl;
  bkfir->delayPosRight = (int32_t*)D_wrr;
} /* stereo_bkfir32x32_process() */
