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
    Real data circular convolution/correlation, floating point, helper file
    Code optimized for HiFi4
    IntegrIT, 2006-2018
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corrf.h"
#include "common_fpu.h"

#if (HAVE_VFPU)

/*
    raw linear correlation: 
    Restrictions:
    x[N+M-1] - aligned on 8 byte boundary
    y[M]     - aligned on 8 byte boundary
    actual length of vector y must be a multiple of 4
    with zeroed excess values;
    N>0 && M>0
    N>=M-1
*/
void raw_corrf(
                      float32_t  * restrict r,
                const float32_t  * restrict x,
                const float32_t  * restrict y,
                int N, int M )
{
    //
    // Circular cross-correlation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //
    const xtfloatx2* restrict px0;
    const xtfloatx2* restrict px1;
    const xtfloatx2* restrict py;
          xtfloatx2* restrict pr_st;
    ae_valign al_rst;
    xtfloatx2 X01,X12,X23,X34,X45,X56,X67,Y01,Y23;
    xtfloatx2 A01;
    xtfloatx2 A23;
    xtfloatx2 A0,A1,A2,A3;
    xtfloatx2 a0,a1,a2,a3;
    int n, m;
    int N_ = N & ~3;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(x,8);
    NASSERT(N>0 && M>0);
    NASSERT(N>=M-1);

    al_rst = AE_ZALIGN64();
    pr_st = (xtfloatx2*)r;
    for ( n=0; n<N_; n+=4 )
    {
        px0=(const xtfloatx2*)(x+n);
        px1=(const xtfloatx2*)(x+n+2);
        py =(const xtfloatx2*)(y);

        A0=A1=A2=A3=(xtfloatx2)(0.0f);
        a0=a1=a2=a3=(xtfloatx2)(0.0f);
        /* preload data from x */
        XT_LSX2IP (X01,px0,4*sizeof(float32_t));
        XT_LSX2IP (X23,px1,4*sizeof(float32_t));
        X12 = XT_SEL32_LH_SX2(X01, X23);
        __Pragma("loop_count min=1")
        for ( m=0; m<((M+3)>>2); m++ )
        {
            /* load data from x */
            XT_LSX2IP(X45, px0, 4*sizeof(float32_t));
            XT_LSX2IP(X67, px1, 4*sizeof(float32_t));
            X34 = XT_SEL32_LH_SX2(X23, X45);
            X56 = XT_SEL32_LH_SX2(X45, X67);
            /* load data from y */
            XT_LSX2IP(Y01, py, 2*sizeof(float32_t));
            XT_LSX2IP(Y23, py, 2*sizeof(float32_t));
            /* compute correlation of 4 values */
            XT_MADD_SX2(A0, X01, Y01);
            XT_MADD_SX2(A1, X12, Y01);
            XT_MADD_SX2(A2, X23, Y01);
            XT_MADD_SX2(A3, X34, Y01);
            
            XT_MADD_SX2(a0, X23, Y23);
            XT_MADD_SX2(a1, X34, Y23);
            XT_MADD_SX2(a2, X45, Y23);
            XT_MADD_SX2(a3, X56, Y23);
            /* shift input line for the next iteration */
            X01 = X45;
            X12 = X56;
            X23 = X67;
        }
        A0 = A0 + a0;
        A1 = A1 + a1;
        A2 = A2 + a2;
        A3 = A3 + a3;
        a0 = XT_SEL32_HL_SX2(A0, A1);
        a1 = XT_SEL32_LH_SX2(A0, A1);
        a2 = XT_SEL32_HL_SX2(A2, A3);
        a3 = XT_SEL32_LH_SX2(A2, A3);
        A01 = a0 + a1;
        A23 = a2 + a3;
        /* Save computed values */
        XT_SASX2IP(A01,al_rst,pr_st);
        XT_SASX2IP(A23,al_rst,pr_st);
    }
    XT_SASX2POSFP(al_rst,pr_st);
    /* Compute last (N%3) values */
    {
        xtfloat r0, r1, r2;
        int tail_N = N&3;

        px0=(const xtfloatx2*)(x+n);
        px1=(const xtfloatx2*)(x+n+2);
        py=(const xtfloatx2*)(y);

        A0=A1=A2=A3=(xtfloatx2)(0.0f);
        a0=a1=a2=a3=(xtfloatx2)(0.0f);

        XT_LSX2IP(X01,px0,4*sizeof(float32_t));
        __Pragma("loop_count min=1")
        for ( m=0; m<((M+3)>>2); m++ )
        {
            /* load data from x */
            XT_LSX2IP(X23,px1,4*sizeof(float32_t));
            XT_LSX2IP(X45,px0,4*sizeof(float32_t));
            X12 = XT_SEL32_LH_SX2(X01, X23);
            X34 = XT_SEL32_LH_SX2(X23, X45);
            /* load data from y */
            XT_LSX2IP(Y01, py, 2*sizeof(float32_t));
            XT_LSX2IP(Y23, py, 2*sizeof(float32_t));
            /* compute correlation of 4 values */
            XT_MADD_SX2(A0, X01, Y01);
            XT_MADD_SX2(A1, X12, Y01);
            XT_MADD_SX2(A2, X23, Y01);
            
            XT_MADD_SX2(a0, X23, Y23);
            XT_MADD_SX2(a1, X34, Y23);
            XT_MADD_SX2(a2, X45, Y23);
            /* shift input line for the next iteration */
            X01 = X45;
        }
        A0 = A0 + a0;
        A1 = A1 + a1;
        A2 = A2 + a2;

        r0 = XT_RADD_SX2(A0);
        r1 = XT_RADD_SX2(A1);
        r2 = XT_RADD_SX2(A2);

        if(tail_N>2) XT_SSI(r2,(xtfloat*)pr_st,2*sizeof(xtfloat));
        if(tail_N>1) XT_SSI(r1,(xtfloat*)pr_st,1*sizeof(xtfloat));
        if(tail_N>0) XT_SSI(r0,(xtfloat*)pr_st,0*sizeof(xtfloat));
    }
}/* raw_corrf() */
#elif HAVE_FPU
// for scalar FPU
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
