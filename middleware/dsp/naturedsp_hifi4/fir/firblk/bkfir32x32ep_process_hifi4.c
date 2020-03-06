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
    Real block FIR filter, 32x32-bit with 72-bit accumulator
    for intermediate computations
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bkfir32x32ep_common.h"

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

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void bkfir32x32ep_process( bkfir32x32ep_handle_t handle, 
                         int32_t * restrict  y,
                   const int32_t * restrict  x, int N )
{
  bkfir32x32ep_ptr_t bkfir = (bkfir32x32ep_ptr_t)handle;

  const ae_int32x2 *            C;
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          X;
        ae_int32   * restrict Y;

  ae_valign D_va;

  ae_int32x2 t01, t23, t45;
  ae_int32x2 t12, t34, t56;
  ae_int32x2 c0, c1;
  ae_int64   q0, q1, q2, q3;
  ae_f64     z;
  ae_ep      ep0, ep1, ep2, ep3;
  int M, wrIx;
  int m, n, _0=0;

  if(N<=0) return;
  ASSERT( bkfir && bkfir->magic == BKFIR32X32EP_MAGIC && y && x );

  ASSERT( IS_ALIGN( x ) );

  M = bkfir->M;
  wrIx = bkfir->wrIx;
  NASSERT( !( M & 3 ) && !( N & 3 ) );

  NASSERT( IS_ALIGN( bkfir->delayLine                   ) &&
          IS_ALIGN( bkfir->delayLine + bkfir->delayLen ) &&
          IS_ALIGN( bkfir->coef ) );

  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2*)(bkfir->delayLine + wrIx);
  X    = (const ae_int32x2 *)x;
  Y    = (      ae_int32     *)y;

  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( bkfir->coef                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( bkfir->coef + bkfir->M + 4 ) );
  z = AE_ZERO64();
  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_int32x2*)(bkfir->coef);
      
  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //
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

    // Circular buffer write pointer looks at the oldest sample: M+4 samples
    // back from the newest one. To read the delay buffer separately for
    // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
    // samples. Pointer increment is safe in respect to crossing the circular
    // buffer boundary.
    D_rd0 = D_wr;
    D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 + 1 );

    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_L32X2_XC  ( t01, D_rd0, +8 );
    AE_LA32X2POS_PC( D_va, D_rd1 );
    AE_LA32X2_IC( t12, D_va, D_rd1 );

    // Zero the accumulators.
    q0 = z; q1 = z; q2 = z; q3 = z;
    XT_MOVEQZ(_0,_0,_0);
    ep0 = AE_MOVEA(_0);ep1 = AE_MOVEA(_0);
    ep2 = AE_MOVEA(_0);ep3 = AE_MOVEA(_0);

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
      AE_L32X2_XC1(c0, C, +8);                    
      AE_L32X2_XC1(c1, C, +8);

      // Q9.62 <- Q9.62 + [ Q31*Q31 ] without symmetric rounding
      AE_MULAAD32EP_HH_LL (ep0,q0,t01,c0);
      AE_MULAAD32EP_HH_LL (ep0,q0,t23,c1);
      AE_MULAAD32EP_HH_LL (ep1,q1,t12,c0);
      AE_MULAAD32EP_HH_LL (ep1,q1,t34,c1);

      AE_MULAAD32EP_HH_LL (ep2,q2,t23,c0);
      AE_MULAAD32EP_HH_LL (ep2,q2,t45,c1);
      AE_MULAAD32EP_HH_LL (ep3,q3,t34,c0);
      AE_MULAAD32EP_HH_LL (ep3,q3,t56,c1);

      t01 = t45;
      t12 = t56;
    }
    // Convert and save 4 outputs.
    
    //Q17.47 <- Q9.62
    q0 = AE_SRAI72(ep0,q0,15);
    q1 = AE_SRAI72(ep1,q1,15);
    q2 = AE_SRAI72(ep2,q2,15);
    q3 = AE_SRAI72(ep3,q3,15);

    AE_S32RA64S_IP( q0, Y, +4 );
    AE_S32RA64S_IP( q1, Y, +4 );
    AE_S32RA64S_IP( q2, Y, +4 );
    AE_S32RA64S_IP( q3, Y, +4 );
  }

  bkfir->wrIx = (int)((int32_t *)D_wr - bkfir->delayLine) ;
} // bkfir32x32ep_process()
