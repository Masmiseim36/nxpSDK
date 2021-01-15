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
    for intermediate computations, unaligned data and arbitrary M/N allowed
    C code optimized for HiFi4
	Integrit, 2006-2018
*/


#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common.h"
#include "bkfira32x32ep_common.h"

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

/* Reserve memory for alignment. */
#define ALIGNED_SIZE( size, align ) \
      ( (size_t)(size) + (align) - 1 )

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
      (void*)( ( (uintptr_t)(addr) + ( (align) - 1 ) ) & ~( (align) - 1 ) )

#define sz_i32   sizeof(int32_t)

void bkfira32x32ep_process( bkfira32x32ep_handle_t handle, 
                          int32_t * restrict       y,
                    const int32_t * restrict       x, int N )
{
  bkfira32x32ep_ptr_t bkfir = (bkfira32x32ep_ptr_t)handle;

  const   ae_int32x2 *            C;
        ae_int32x2 * restrict D_wr;
        ae_int32   * restrict D_s_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32   *          D_s_rd;
  const ae_int32x2 *          X;
  const ae_int32   *          X_s;
        ae_int32   * restrict Y;

  ae_valign D_va0, D_va1;

  ae_int32x2 t01, t23, t45, t12, t34, t56;
  ae_int32x2 c0, c1;
  ae_int64   q0, q1, q2, q3;
  ae_ep      ep0, ep1, ep2, ep3;
  ae_f64     z;
  ae_valign  x_align;
  int M, wrIx;
  int m, n, _0=0;

    if (N<=0) return;
    NASSERT( bkfir && bkfir->magic == BKFIRA32X32EP_MAGIC && y && x );
    NASSERT( IS_ALIGN( bkfir->delayLine                   ) &&
            IS_ALIGN( bkfir->coef                   ) &&
            IS_ALIGN( bkfir->delayLine + bkfir->delayLen ));

    M = bkfir->M;
    wrIx = bkfir->wrIx;

  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2*)(bkfir->delayLine + wrIx);
  X    = (const ae_int32x2 *)x;
  Y    = (      ae_int32   *)y;

  WUR_AE_CBEGIN0( (uintptr_t)( bkfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( bkfir->delayLine + bkfir->delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( bkfir->coef                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( bkfir->coef + M+4 ) );

  z = AE_ZERO64();
  
  x_align = AE_LA64_PP(X);

  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_int32x2*)bkfir->coef;

  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //
  for ( n=0; n<(N>>2); n++ )
  {
    // Load 4 input samples.
    // Q31
    AE_LA32X2_IP( t01, x_align, X );
    AE_LA32X2_IP( t23, x_align, X );
    D_va0 = AE_ZALIGN64();
    // Store 4 samples to the delay line buffer with circular address update.
    // Q31
    AE_SA32X2_IC( t01, D_va0, D_wr );
    AE_SA32X2_IC( t23, D_va0, D_wr );
    AE_SA64POS_FP( D_va0, D_wr );
    // Circular buffer write pointer looks at the oldest sample: M+4 samples
    // back from the newest one. 
    D_rd0 = D_wr;
    D_rd1 = (ae_int32x2*)( (int32_t*)D_wr );
    AE_LA32X2POS_PC( D_va0, D_rd0 );

    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_LA32X2_IC( t01, D_va0, D_rd0 );
    AE_L32_XC(t12,castxcc(ae_int32,D_rd1),+4);
    AE_LA32X2POS_PC( D_va1, D_rd1 );                             
    AE_LA32X2_IC( t12, D_va1, D_rd1 );

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
      // 8 samples residing in 4 AE registers.
      // Q31
      AE_LA32X2_IC( t23, D_va0, D_rd0 );
      AE_LA32X2_IC( t45, D_va0, D_rd0 );
      // Load next two samples for odd accumulators. 
      // Q31
      AE_LA32X2_IC( t34, D_va1, D_rd1 );
      AE_LA32X2_IC( t56, D_va1, D_rd1 );
      // Load the next 4 tap coefficients.
      // Q31
      AE_L32X2_XC1(c0,C, +8);
      AE_L32X2_XC1(c1,C, +8);

      // Q9.62 <- Q9.62 + [ Q31*Q31 ] without symmetric rounding
      AE_MULAAD32EP_HH_LL    (ep0,q0,t01,c0);
      AE_MULAAD32EP_HH_LL    (ep0,q0,t23,c1);
      AE_MULAAD32EP_HH_LL    (ep1,q1,t12,c0);
      AE_MULAAD32EP_HH_LL    (ep1,q1,t34,c1);

      AE_MULAAD32EP_HH_LL    (ep2,q2,t23,c0);
      AE_MULAAD32EP_HH_LL    (ep2,q2,t45,c1);
      AE_MULAAD32EP_HH_LL    (ep3,q3,t34,c0);
      AE_MULAAD32EP_HH_LL    (ep3,q3,t56,c1);

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
  X_s    = (const ae_int32*)X;
  D_s_wr = (      ae_int32*)D_wr;
  if (N&3)
  {
    // Insert 0..7 input samples into the delay line one-by-one.
    for ( n=0; n<(N&3); n++ )
    {
      AE_L32_IP ( t01, X_s   , +4 );
      AE_S32_L_XC( t01, D_s_wr, +4 );
    }
    D_s_rd = D_s_wr;
    // Perform dummy reads to skip 8-(N&7) oldest samples.
    for ( n=0; n<(-N&3); n++ )
    {
      AE_L32_XC( t01, D_s_rd, +4 );
    }

    D_rd0 = (const ae_int32x2*)D_s_rd;
    D_rd1 = (ae_int32x2*)( (int32_t*)D_rd0 );
    AE_LA32X2POS_PC( D_va0, D_rd0 );

    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_LA32X2_IC( t01, D_va0, D_rd0 );
    AE_L32_XC(t12,castxcc(ae_int32,D_rd1),+4);
    AE_LA32X2POS_PC( D_va1, D_rd1 );
    AE_LA32X2_IC( t12, D_va1, D_rd1 );

    // Zero the accumulators.
    q0 = z; q1 = z; q2 = z; q3 = z;
    XT_MOVEQZ(_0,_0,_0);
    ep0 = AE_MOVEA(_0);ep1 = AE_MOVEA(_0);
    ep2 = AE_MOVEA(_0);ep3 = AE_MOVEA(_0);

    __Pragma("loop_count min=2")
    for ( m=0; m<(M>>2)+1; m++ )
    {
      // Load next two samples for even accumulators. Altogether we have
      // 8 samples residing in 4 AE registers.
      // Q31
      AE_LA32X2_IC( t23, D_va0, D_rd0 );
      AE_LA32X2_IC( t45, D_va0, D_rd0 );
      // Load next two samples for odd accumulators. 
      // Q31
      AE_LA32X2_IC( t34, D_va1, D_rd1 );
      AE_LA32X2_IC( t56, D_va1, D_rd1 );
      // Load the next 4 tap coefficients.
      // Q31
      AE_L32X2_XC1(c0,C, +8);
      AE_L32X2_XC1(c1,C, +8);

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
    Y = (ae_int32*)( y + N-1 );
    // Convert and save 1..3 outputs.
    //Q17.47 <- Q9.63
    q0 = AE_SRAI72(ep0,q0,15);
    q1 = AE_SRAI72(ep1,q1,15);
    q2 = AE_SRAI72(ep2,q2,15);

    switch ( N & 3 )
    {
    case 3: AE_S32RA64S_IP( q2, Y, -4 );
    case 2: AE_S32RA64S_IP( q1, Y, -4 );
    }

    AE_S32RA64S_IP( q0, Y, -4 ); 
  }
  bkfir->wrIx = (int)((int32_t *)D_s_wr - bkfir->delayLine) ;

} // bkfira32x32ep_process()
