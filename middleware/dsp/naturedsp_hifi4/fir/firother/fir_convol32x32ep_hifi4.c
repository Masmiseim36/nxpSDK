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
    Real data circular convolution, 32x32-bit with 72-bit accumulator
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
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs 
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  x[N]      input data, Q15, Q31 or floating point
  y[M]      input data, Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restriction:
  x,y,r     should not overlap
  x,y,r     aligned on an 8-bytes boundary
  N,M       multiples of 4 and >0
-------------------------------------------------------------------------*/
void fir_convol32x32ep( int32_t * restrict r,
                const int32_t     * restrict x,
                const int32_t     * restrict y,
                int N, int M )
{
  // Circular convolution algorithm:
  //
  //   r[n] = sum( x[mod(n-m,N)]*y[m] )
  //        m=0..M-1
  //
  //   where n = 0..N-1
        ae_int32x2 * restrict R;
  const ae_int32x2 * restrict X;
  const ae_int32x2 * restrict Y;
  int n, m, circinc;

  ae_int32x2 x0,x1,x2,x3,x4,y0,y1,r01,r23;
  ae_int32x2 z0,z2;
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

  circinc=(M*4+32);
  while(circinc>=(N<<2))circinc-=(N<<2);

  X = (const ae_int32x2 *)(x);    // Set X to &x[n+2]
  AE_ADDCIRC_XC(castxcc(ae_int64,X),8);
  Y = (const ae_int32x2 *)(y);
  __Pragma("loop_count min=1")
  for ( n=0; n<(N>>2); n++ )
  {
    //X = (const ae_int32x2 *)(x+n*4+2);
    AE_L32X2_RIC( z0, X );
    AE_L32X2_RIC( x0, X );
    //XT_MOVEQZ(_0,_0,_0);
    e0=e1=e2=e3=AE_MOVEA(_0);
    q0=q1=q2=q3=AE_ZERO64();
    __Pragma("loop_count min=1")
    for ( m=0; m<(M>>2); m++ )
    {
      AE_L32X2_IP ( y0, Y, 8);
      AE_L32X2_IP ( y1, Y, 8);
      AE_L32X2_RIC( x1, X );
      AE_L32X2_RIC( x3, X );
      x2=AE_SEL32_LH(x0,x1);
      x4=AE_SEL32_LH(x1,x3);
      z2=AE_SEL32_LH(z0,x0);

      AE_MULAAD32EP_HH_LL(e1,q1,x0,y0);
      AE_MULAAD32EP_HH_LL(e0,q0,x2,y0);
      AE_MULAAD32EP_HH_LL(e1,q1,x1,y1);
      AE_MULAAD32EP_HH_LL(e0,q0,x4,y1);

      AE_MULAAD32EP_HH_LL(e3,q3,z0,y0);
      AE_MULAAD32EP_HH_LL(e2,q2,z2,y0);
      AE_MULAAD32EP_HH_LL(e3,q3,x0,y1);
      AE_MULAAD32EP_HH_LL(e2,q2,x2,y1);

      x0=x3;
      z0=x1;
    }
    AE_ADDCIRC_XC(castxcc(ae_int64,X),circinc);
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
} // fir_convol32x32ep()
