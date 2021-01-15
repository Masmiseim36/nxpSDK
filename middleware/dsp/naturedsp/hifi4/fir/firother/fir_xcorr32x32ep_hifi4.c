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
    Real data circular cross-correlation, 32x32-bit with 72-bit accumulator
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

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point 


  Input:
  x[N]      input data Q15, Q31 or floating point
  y[M]      input data Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restriction:
  x,y,r     should not overlap
  x,y,r     aligned on an 8-bytes boundary
  N,M       multiples of 4 and >0
-------------------------------------------------------------------------*/

void fir_xcorr32x32ep( int32_t * restrict r,
               const int32_t * restrict x,
               const int32_t * restrict y,
               int N, int M ) 
{
        ae_int32x2 * restrict R;
  const ae_int32x2 * restrict X;
  const ae_int32x2 * restrict Z;
  const ae_int32x2 * restrict Y;
  //
  // Circular cross-correlation algorithm:
  //
  //   r[n] = sum( x[mod(n+m,N)]*y[m] )
  //        m=0..M-1
  //
  //   where n = 0..N-1
  //
  int n, m;

  ae_int32x2 x01,x12,x23,x34,x45,x56,x67,y01,y23,r01,r23;
  ae_int64 q0,q1,q2,q3;
  ae_ep    e0,e1,e2,e3;
  volatile int _0=0;

  NASSERT( r && x && y && N > 0 && M > 0 );
  NASSERT (M%4==0 && N%4==0);
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT_ALIGN8(r);

  R = (ae_int32x2 *)r;
  WUR_AE_CBEGIN0( (uintptr_t)(x) );
  WUR_AE_CEND0  ( (uintptr_t)(x+N) );
  X = (const ae_int32x2 *)(x);
  Y = (const ae_int32x2 *)(y);
  Z=X;
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>2); n++ )
  {
    X = (const ae_int32x2 *)(x+n*4);
    AE_L32X2_XC( x01, X,8 );
    AE_L32X2_XC( x23, X,8 );
    Z=X;

    //XT_MOVEQZ(_0,_0,_0);
    e0=e1=e2=e3=AE_MOVEA(_0);
    q0=q1=q2=q3=AE_ZERO64();
    __Pragma("loop_count min=1")
    for (m=0; m<(M>>2); m++ )
    {
        AE_L32X2_XC( x45, X, 8);
        AE_L32X2_XC( x67, X, 8);
        AE_L32X2_IP( y01, Y, 8);
        AE_L32X2_IP( y23, Y, 8);
        x12=AE_SEL32_LH(x01,x23);
        x34=AE_SEL32_LH(x23,x45);
        x56=AE_SEL32_LH(x45,x67);

        AE_MULAAD32EP_HH_LL(e0,q0,x01,y01);
        AE_MULAAD32EP_HH_LL(e0,q0,x23,y23);

        AE_MULAAD32EP_HH_LL(e1,q1,x12,y01);
        AE_MULAAD32EP_HH_LL(e1,q1,x34,y23);

        AE_MULAAD32EP_HH_LL(e2,q2,x23,y01);
        AE_MULAAD32EP_HH_LL(e2,q2,x45,y23);

        AE_MULAAD32EP_HH_LL(e3,q3,x34,y01);
        AE_MULAAD32EP_HH_LL(e3,q3,x56,y23);
        x01=x45;
        x23=x67;
    }
    X=Z;
    Y-=M/2;
    q0=AE_SRAI72(e0,q0,15);
    q1=AE_SRAI72(e1,q1,15);
    q2=AE_SRAI72(e2,q2,15);
    q3=AE_SRAI72(e3,q3,15);
    r01=AE_ROUND32X2F48SASYM(q0,q1);
    r23=AE_ROUND32X2F48SASYM(q2,q3);
    AE_S32X2_IP(r01,R,8);
    AE_S32X2_IP(r23,R,8);
  }
} // fir_xcorr32x32ep()
