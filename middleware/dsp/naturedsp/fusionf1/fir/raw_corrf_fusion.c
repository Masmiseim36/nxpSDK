/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
    NatureDSP Signal Processing Library. FIR part
    Real data circular convolution/correlation, floating point, helper file
    Code optimized for Fusion
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#if (XCHAL_HAVE_FUSION_FP)
#include "raw_corrf.h"

/*
    raw linear correlation: 
    Restrictions:
    x - aligned on 8 byte boundary
    N>0 && M>0
    N>=M-1
*/
void raw_corrf(
                      float32_t  * restrict r,
                const float32_t  * restrict x,
                const float32_t  * restrict y,
                int N, int M )
{
  /*
  * Circular cross-correlation algorithm:
  *
  *   r[n] = sum( x[mod(n+m,N)]*y[m] )
  *        m=0..M-1
  *
  *   where n = 0..N-1
  */

  xtfloat A0,A1,A2,A3,X0,X1,X2,X3,X4,Y0,Y1;
  const xtfloat * restrict pX;
  const xtfloat* restrict pY;
  xtfloat * restrict pR;
  int n, m;

  NASSERT(r);
  NASSERT(x);
  NASSERT(y);
  NASSERT_ALIGN(x, 8);
  NASSERT(N>0 && M>0);
  NASSERT(N >= M - 1);

  pR=(      xtfloat*)r;
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
  WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
  for (n = 0; n<(N&~3); n += 4)
  {
    A0=A1=A2=A3=XT_CONST_S(0);
    pX = (const xtfloat*)(x + n);
    pY = (const xtfloat*)y;
    XT_LSXC(X0, pX, 4);
    XT_LSXC(X1, pX, 4);
    XT_LSXC(X2, pX, 4);
    for ( m=0; m<(M>>1); m++ )
    {
      XT_LSXC(X3, pX, 4);
      XT_LSXC(X4, pX, 4);

      XT_LSIP(Y0, pY, 4);
      XT_LSIP(Y1, pY, 4);
      XT_MADD_S(A0, X0, Y0);
      XT_MADD_S(A1, X1, Y0);
      XT_MADD_S(A2, X2, Y0);
      XT_MADD_S(A3, X3, Y0);
      XT_MADD_S(A0, X1, Y1);
      XT_MADD_S(A1, X2, Y1);
      XT_MADD_S(A2, X3, Y1);
      XT_MADD_S(A3, X4, Y1);
      X0 = X2;
      X1 = X3;
      X2 = X4;
    }
    if (M & 1)
    {
      XT_LSXC(X3, pX, 4);
      XT_LSXC(X4, pX, 4);

      XT_LSIP(Y0, pY, 4);
      XT_MADD_S(A0, X0, Y0);
      XT_MADD_S(A1, X1, Y0);
      XT_MADD_S(A2, X2, Y0);
      XT_MADD_S(A3, X3, Y0);
    }
    XT_SSIP(A0,pR,4);
    XT_SSIP(A1,pR,4);
    XT_SSIP(A2,pR,4);
    XT_SSIP(A3,pR,4);
  }
  /* last 1...3 iterations */
  N&=3;
  if (N)
  {
    A0 = A1 = A2 = XT_CONST_S(0);
    pX = (const xtfloat*)(x+n);
    pY = (const xtfloat*)y;
    XT_LSXC(X0, pX, 4);
    XT_LSXC(X1, pX, 4);
    for (m = 0; m<(M >> 1); m++)
    {
      XT_LSXC(X2, pX, 4);
      XT_LSXC(X3, pX, 4);

      XT_LSIP(Y0, pY, 4);
      XT_LSIP(Y1, pY, 4);
      XT_MADD_S(A0, X0, Y0);
      XT_MADD_S(A1, X1, Y0);
      XT_MADD_S(A2, X2, Y0);
      XT_MADD_S(A0, X1, Y1);
      XT_MADD_S(A1, X2, Y1);
      XT_MADD_S(A2, X3, Y1);
      X0 = X2;
      X1 = X3;
    }
    if (M & 1)
    {
      XT_LSXC(X2, pX, 4);
      XT_LSIP(Y0, pY, 4);
      XT_MADD_S(A0, X0, Y0);
      XT_MADD_S(A1, X1, Y0);
      XT_MADD_S(A2, X2, Y0);
    }
    if (N>2) XT_SSI(A2, pR, 2 * 4);
    if (N>1) XT_SSI(A1, pR, 1 * 4);
    if (N>0) XT_SSI(A0, pR, 0 * 4);
  }
} /* raw_corrf() */
#endif

