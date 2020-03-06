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
    Complex block FIR filter, 32x32-bit with 72-bit accumulator
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
#include "cxfir32x32ep_common.h"

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  1. User application is not responsible for management of delay lines
  2. User has an option to set IR externally or copy from original location 
     (i.e. from the slower constant memory). In the first case, user is 
     responsible for right alignment, ordering and zero padding of filter 
     coefficients - usually array is composed from zeroes (left padding), 
     reverted IR and right zero padding.

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  h[M]      complex filter coefficients; h[0] is to be multiplied with the 
            newest sample, Q15, Q31, floating point
  x[N]      input samples, Q15, Q31, floating point
  N         length of sample block (in complex samples) 
  M         length of filter 
  extIR     if zero, IR is copied from original location, otherwise not
            but user should keep alignment, order of coefficients 
            and zero padding requirements shown below
  Output:			
  y[N]      output samples, Q15, Q31, floating point

  Alignment, ordering and zero padding for external IR  (extIR!=0)
  -----------------+----------+--------------+--------------+----------------
  Function	       |Alignment,|Left zero     |   Coefficient| Right zero 
                   | bytes    |padding, bytes|   order      | padding, bytes
  -----------------+----------+--------------+--------------+----------------
  cxfir16x16_init, |     8    |    4         |  inverted    |  4
  hifi3            |          |              |              |
  cxfir16x16_init, |     8    |  2 before    |  *           |  6 after
  hifi3z/4         |          |  each copy   |              |  each copy
  cxfir32x16_init  |     8    |    4         |  inverted    |  4
  cxfir32x32_init  |     8    |    0         |  inv,conj    |  0
  cxfir32x32ep_init|     8    |    0         |  inv,conj    |  0
  cxfirf_init      |     8    |    0         |  direct      |  0
  -----------------+----------+--------------+--------------+----------------
  * inverted: conjugated copy and (imaginary; real) copy at 4*(M+4) bytes offset

  Restriction:
  x,y       should not overlap
  x,h       aligned on a 8-bytes boundary
  N,M       multiples of 4
-------------------------------------------------------------------------*/

/* Put a chunk of input signal into the delay line and compute the filter
 * response. */
void cxfir32x32ep_process( cxfir32x32ep_handle_t handle, 
                         complex_fract32 * restrict      y,
                   const complex_fract32 * restrict      x, int N )
{
  cxfir32x32ep_ptr_t cxfir = (cxfir32x32ep_ptr_t)handle;

  const   ae_f32x2 *            C;
        ae_int32x2 * restrict D_wr;
  const ae_int32x2 *          D_rd0;
  const ae_int32x2 *          D_rd1;
  const ae_int32x2 *          X;
        ae_int32     * restrict Y;
  int _0=0;
  ae_valign D_va;

  ae_int32x2 t0, t1, t2, t3;
  ae_f32x2   d0, d1;
  ae_int32x2   c0, c1, c2, c3, c2n, c3n;
  ae_int64     q0r, q1r, q2r, q3r;
  ae_int64     q0i, q1i, q2i, q3i;
  ae_f64     z;

  ae_ep      ep0r, ep1r, ep2r, ep3r;
  ae_ep      ep0i, ep1i, ep2i, ep3i;


  int M, wrIx;
  int m, n;

  if(N<=0) return;
  NASSERT( cxfir && cxfir->magic == CXFIR32X32EP_MAGIC && y && x );
  NASSERT_ALIGN8( x ) ;

  M = cxfir->M;
  wrIx = cxfir->wrIx;
  NASSERT( !( M & 3 ) && !( N & 3 ) );
  NASSERT_ALIGN8(cxfir->delayLine);
  //NASSERT_ALIGN8(cxfir->delayLine + 2*cxfir->delayLen);
  NASSERT_ALIGN8(cxfir->coef);

  //
  // Setup pointers and circular delay line buffer.
  //

  D_wr = (      ae_int32x2*)(cxfir->delayLine + 2*wrIx);
  X    = (const ae_int32x2 *)x;
  Y    = (      ae_int32     *)y;

  WUR_AE_CBEGIN0( (uintptr_t)( cxfir->delayLine                   ) );
  WUR_AE_CEND0  ( (uintptr_t)( cxfir->delayLine + 2*cxfir->delayLen ) );
  WUR_AE_CBEGIN1( (uintptr_t)( cxfir->coef                   ) );
  WUR_AE_CEND1  ( (uintptr_t)( cxfir->coef + 2*cxfir->M  ) );
  z = AE_ZERO64();
  // Reset the coefficients pointer. Now it looks at the tap corresponding
  // to the oldest sample in the delay line.
  C = (const ae_f32x2*)cxfir->coef;
      
  //
  // Break the input signal into 4-samples blocks. For each block, store 4
  // samples to the delay line and compute the filter response.
  //
    __Pragma("loop_count min=1")
  for ( n=0; n<(N>>1); n++ )
  {
    // Load 4 input samples.
    // Q31
    AE_L32X2_IP( t0, X, +8 );
    AE_L32X2_IP( t1, X, +8 );

    // Store 4 samples to the delay line buffer with circular address update.
    // Q31
    AE_S32X2_XC( t0, D_wr, +8 );
    AE_S32X2_XC( t1, D_wr, +8 );

    // Circular buffer write pointer looks at the oldest sample: M+4 samples
    // back from the newest one. To read the delay buffer separately for
    // even/odd accumulators, the two read pointers are set to M-4th and M-3rd
    // samples. Pointer increment is safe in respect to crossing the circular
    // buffer boundary.
    D_rd0 = D_wr;
    // Load two oldest samples for even/odd accumulators.
    // Q31
    AE_L32X2_XC  ( t0, D_rd0, +8 );
    D_rd1 = D_rd0;
    AE_L32X2_XC  ( t1, D_rd0, +8 );
    AE_LA32X2POS_PC( D_va, D_rd1 );

    d0 = ( t0 );
    d1 = ( t1 );

    // Zero the accumulators.
    q0r = z; q1r = z; q2r = z; q3r = z;
    q0i = z; q1i = z; q2i = z; q3i = z;
    XT_MOVEQZ(_0,_0,_0);    // prevent spilling
    ep0r = AE_MOVEA(_0);ep1r = AE_MOVEA(_0);
    ep2r = AE_MOVEA(_0);ep3r = AE_MOVEA(_0);
    ep0i = AE_MOVEA(_0);ep1i = AE_MOVEA(_0);
    ep2i = AE_MOVEA(_0);ep3i = AE_MOVEA(_0);

    //
    // Inner loop: process 4 taps for 4 accumulators on each trip. Actually we 
    // perform M+4 MACs for each accumulator, 4 of which fall on zero taps
    // inserted into the impulse response during initialization.
    //
    __Pragma("loop_count min=2")
    for ( m=0; m<(M>>1); m++ )
    {
      // Load next two samples for even accumulators. Altogether we have
      // 4 samples residing in 8 AE registers.
      // Q31
      AE_L32X2_XC( t2, D_rd0, +8 );
      AE_L32X2_XC( t3, D_rd0, +8 );

      // Load the next 2 tap coefficients.
      // Q31
      AE_L32X2_XC1(c0, C, +8);
      AE_L32X2_XC1(c1, C, +8);

      c2n = AE_NEG32(c0);
      c3n = AE_NEG32(c1);

      //c2 = AE_SEL32_LH(c0,c0);
      //c3 = AE_SEL32_LH(c1,c1);

      c2 = AE_SEL32_LH(c2n,c0);
      c3 = AE_SEL32_LH(c3n,c1);
      // Q9.62 <- Q9.62 + [ Q31*Q31 ] without symmetric rounding
      AE_MULAAD32EP_HH_LL (ep0r,q0r,t0,c0);
      AE_MULAAD32EP_HH_LL (ep0r,q0r,t1,c1);

      AE_MULAAD32EP_HH_LL (ep1r,q1r,t1,c0);
      AE_MULAAD32EP_HH_LL (ep1r,q1r,t2,c1);

      
      AE_MULAAD32EP_HH_LL (ep0i,q0i,t0,c2);
      AE_MULAAD32EP_HH_LL (ep0i,q0i,t1,c3);

      AE_MULAAD32EP_HH_LL (ep1i,q1i,t1,c2);
      AE_MULAAD32EP_HH_LL (ep1i,q1i,t2,c3);
      
      t0 = t2;
      t1 = t3;
    }
    // Convert and save 4 outputs.
    
    //Q17.47 <- Q9.62
    q0r = AE_SRAI72(ep0r,q0r,15);
    q1r = AE_SRAI72(ep1r,q1r,15);
    q0i = AE_SRAI72(ep0i,q0i,15);
    q1i = AE_SRAI72(ep1i,q1i,15);

    AE_S32RA64S_IP( q0r, Y, +4 );
    AE_S32RA64S_IP( q0i, Y, +4 );
    AE_S32RA64S_IP( q1r, Y, +4 );
    AE_S32RA64S_IP( q1i, Y, +4 );
  }

  cxfir->wrIx = (int)((int32_t *)D_wr - cxfir->delayLine)>>1 ;
} // cxfir32x32ep_process()
