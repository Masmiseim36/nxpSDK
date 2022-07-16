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
 * Real Matrix Inversion
 * Optimized code for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, mtx_inv3x3f, (float32_t* x))
#else
/*-------------------------------------------------------------------------
  These functions implement in-place matrix inversion by Gauss elimination 
  with full pivoting

  Precision: 
  f   floating point (real and complex)

  Input:
  x[N*N]      input matrix
  Output:
  x[N*N]      result
  N is 2,3 or 4

  Restrictions:
  none
-------------------------------------------------------------------------*/
#define SZ_F32 (sizeof(float32_t))
#define SZ_2F32 (2*SZ_F32)

void mtx_inv3x3f(float32_t* x)
{
  xtfloatx2 *restrict pk;
  xtfloatx2 *restrict pmax;
  xtfloatx2 *         pA0;
  xtfloatx2 *         pX;
  xtfloatx2 *         pA;
  xtfloatx2 *restrict pA1;
  xtfloatx2 _0, _1;
  xtfloatx2 R, C0, C1;
  xtfloatx2 A00, A01, A02;
  xtfloatx2 A10, A11, A12;
  xtfloatx2 X0, X1, X2, X3, X4;
  xtfloatx2 Amax0, Amax1, Amax2;
  xtfloatx2 Ak0, Ak1, Ak2;
  ae_valign xa;
  int n, k;
  float32_t ALIGN(8) A[18];

  /* Copy the matrix to buffer and
  * initialize identity matrix:
  * 
  * x00 x01 x02 1.0 0.0 0.0
  * x10 x11 x12 0.0 1.0 0.0
  * x20 x21 x22 0.0 0.0 1.0
  */
  C0 = XT_CONST_S(0);
  C1 = XT_CONST_S(1);
  pA0 = (xtfloatx2 *)(x);
  pA1 = (xtfloatx2 *)(A);
  {
    /*
    * Copy the matrix to buffer and
    * initialize identity matrix:
    *
    * x00 x01 x02 1.0 0.0 0.0
    * x10 x11 x12 0.0 1.0 0.0
    * x20 x21 x22 0.0 0.0 1.0
    */
    _0 = XT_CONST_S(0);
    _1 = XT_CONST_S(1);
    pX = (xtfloatx2 *)(x);
    pA = (xtfloatx2 *)(A);
    xa = XT_LASX2PP(pX);
    XT_LASX2IP(X0, xa, pX);
    XT_LASX2IP(X1, xa, pX);
    XT_LASX2IP(X2, xa, pX);
    XT_LASX2IP(X3, xa, pX);
    X4 = XT_LSI((xtfloat *)pX, 0);


    XT_SSX2IP(X0, pA, SZ_2F32);
    R = XT_SEL32_HH_SX2(X1, _1);
    XT_SSX2IP(R, pA, SZ_2F32);
    XT_SSX2IP(_0, pA, SZ_2F32);

    R = XT_SEL32_LH_SX2(X1, X2);
    XT_SSX2IP(R, pA, SZ_2F32);
    R = XT_SEL32_LH_SX2(X2, _0);
    XT_SSX2IP(R, pA, SZ_2F32);
    R = XT_SEL32_LH_SX2(_1, _0);
    XT_SSX2IP(R, pA, SZ_2F32);

    XT_SSX2IP(X3, pA, SZ_2F32);
    R = XT_SEL32_LH_SX2(X4, _0);
    XT_SSX2IP(R, pA, SZ_2F32);
    R = XT_SEL32_LH_SX2(_0, _1);
    XT_SSX2IP(R, pA, SZ_2F32);
  }
  /* Set bounds of the buffer */
  WUR_AE_CBEGIN0((uintptr_t)(A));
  WUR_AE_CEND0  ((uintptr_t)(A+18));

  pk  = (xtfloatx2 *)(A);
  pA0 = (xtfloatx2 *)(A+6);
  pA1 = (xtfloatx2 *)(A+12);
  /* Gauss elimination */

  for(k=0; k<3; k++)
  {
    xtfloat amax, max;
    unsigned int imax;
    /* pivoting */
    imax=k;
    amax=XT_CONST_S(0);
    /* find absolute max value in the k-th column */
    for(n=k; n<3; n++)
    {
      xtbool cond;
      xtfloat a, t;

      t = A[n*6+k];
      a = XT_ABS_S(t);

      cond = /*xtbool2_extract_0*/(XT_OLT_S(amax, a));
      XT_MOVT_S(amax, a, cond);
      XT_MOVT_S(max,  t, cond);
      XT_MOVT  (imax, n, cond);
    }

    R = max;
    R = XT_RECIP_S(R);

    /* permutation of rows */
    pmax = (xtfloatx2 *)(A+imax*6);

    Ak0   = XT_LSX2I(pmax, 0*SZ_2F32);
    Ak1   = XT_LSX2I(pmax, 1*SZ_2F32);
    Ak2   = XT_LSX2I(pmax, 2*SZ_2F32);
    Amax0 = XT_LSX2I(  pk, 0*SZ_2F32);
    Amax1 = XT_LSX2I(  pk, 1*SZ_2F32);
    Amax2 = XT_LSX2I(  pk, 2*SZ_2F32);
    XT_SSX2IP(Amax0, pmax, SZ_2F32);
    XT_SSX2IP(Amax1, pmax, SZ_2F32);
    XT_SSX2IP(Amax2, pmax, SZ_2F32);
    __Pragma("no_reorder")
    /* multiply k-th row by the reciprocal *
    * pivot element during swapping rows  */
    Ak0 = XT_MUL_SX2(Ak0,R);
    Ak1 = XT_MUL_SX2(Ak1,R);
    Ak2 = XT_MUL_SX2(Ak2,R);
    XT_SSX2IP(Ak0, pk, SZ_2F32);
    XT_SSX2IP(Ak1, pk, SZ_2F32);
    XT_SSX2IP(Ak2, pk, SZ_2F32);

    /* elimination */
    /* join forward and back substitution */

    C0 = XT_LSX((xtfloat *)pA0, k*SZ_F32);
    C1 = XT_LSX((xtfloat *)pA1, k*SZ_F32);

    A00 = XT_LSX2I(pA0, 0*SZ_2F32);
    A01 = XT_LSX2I(pA0, 1*SZ_2F32);
    A02 = XT_LSX2I(pA0, 2*SZ_2F32);
    A10 = XT_LSX2I(pA1, 0*SZ_2F32);
    A11 = XT_LSX2I(pA1, 1*SZ_2F32);
    A12 = XT_LSX2I(pA1, 2*SZ_2F32);

    XT_MSUB_SX2(A00, Ak0, C0);
    XT_MSUB_SX2(A01, Ak1, C0);
    XT_MSUB_SX2(A02, Ak2, C0);
    XT_MSUB_SX2(A10, Ak0, C1);
    XT_MSUB_SX2(A11, Ak1, C1);
    XT_MSUB_SX2(A12, Ak2, C1);

    XT_SSX2XC(A00, pA0, SZ_2F32);
    XT_SSX2XC(A01, pA0, SZ_2F32);
    XT_SSX2XC(A02, pA0, SZ_2F32);
    XT_SSX2XC(A10, pA1, SZ_2F32);
    XT_SSX2XC(A11, pA1, SZ_2F32);
    XT_SSX2XC(A12, pA1, SZ_2F32);
  }
  /* copy 4-6 columns to x */
  for(n=0; n<3; n++)
  {
    x[n*3+0]=A[n*6+3];
    x[n*3+1]=A[n*6+4];
    x[n*3+2]=A[n*6+5];
  }
}/* mtx_inv3x3f() */
#endif
