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
DISCARD_FUN(void, mtx_inv4x4f, (float32_t* x))
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
void mtx_inv4x4f(float32_t* x)
{
  xtfloatx2 *restrict pk;
  xtfloatx2 *restrict pmax;
  xtfloatx2 *restrict pA0;
  xtfloatx2 *restrict pA1;
  xtfloatx2 *restrict pA2;
  xtfloatx2 R, C0, C1;
  xtfloatx2 A00, A01, A02, A03;
  xtfloatx2 A10, A11, A12, A13;
  xtfloatx2 Amax0, Amax1, Amax2, Amax3;
  xtfloatx2  Ak0, Ak1, Ak2, Ak3;
  ae_valign vX;
  int n, k;
  float32_t ALIGN(8) A[32];
  /* Copy the matrix to buffer and
  * initialize identity matrix:
  *
  * x00 x01 x02 x03 1.0 0.0 0.0 0.0
  * x10 x11 x12 x13 0.0 1.0 0.0 0.0
  * x20 x21 x22 x23 0.0 0.0 1.0 0.0
  * x30 x31 x32 x33 0.0 0.0 0.0 1.0
  */
  C0 = XT_CONST_S(0);
  C1 = XT_CONST_S(1);
  pA0 = (xtfloatx2 *)(x);
  pA1 = (xtfloatx2 *)(A);
  {
    xtfloatx2 C01, C10;
    C01 = XT_SEL32_HH_SX2(C0, C1);
    C10 = XT_SEL32_HH_SX2(C1, C0);
    /* Load input matrix */
    vX = XT_LASX2PP(pA0);
    XT_LASX2IP(A00, vX, pA0);
    XT_LASX2IP(A01, vX, pA0);
    XT_LASX2IP(A02, vX, pA0);
    XT_LASX2IP(A03, vX, pA0);
    XT_LASX2IP(A10, vX, pA0);
    XT_LASX2IP(A11, vX, pA0);
    XT_LASX2IP(A12, vX, pA0);
    XT_LASX2IP(A13, vX, pA0);
    /* Save input and identity matrix */
    /* 1-st row */
    XT_SSX2IP(A00, pA1, SZ_2F32);
    XT_SSX2IP(A01, pA1, SZ_2F32);
    XT_SSX2IP(C10, pA1, SZ_2F32);
    XT_SSX2IP(C0, pA1, SZ_2F32);
    /* 2-nd row */
    XT_SSX2IP(A02, pA1, SZ_2F32);
    XT_SSX2IP(A03, pA1, SZ_2F32);
    XT_SSX2IP(C01, pA1, SZ_2F32);
    XT_SSX2IP(C0, pA1, SZ_2F32);
    /* 3-rd row */
    XT_SSX2IP(A10, pA1, SZ_2F32);
    XT_SSX2IP(A11, pA1, SZ_2F32);
    XT_SSX2IP(C0, pA1, SZ_2F32);
    XT_SSX2IP(C10, pA1, SZ_2F32);
    /* 4-th row */
    XT_SSX2IP(A12, pA1, SZ_2F32);
    XT_SSX2IP(A13, pA1, SZ_2F32);
    XT_SSX2IP(C0, pA1, SZ_2F32);
    XT_SSX2IP(C01, pA1, SZ_2F32);
  }

  /* Set bounds of the buffer */
  WUR_AE_CBEGIN0((uintptr_t)(A));
  WUR_AE_CEND0((uintptr_t)(A + 32));

  pk = (xtfloatx2 *)(A);
  pA0 = (xtfloatx2 *)(A + 8);
  pA1 = (xtfloatx2 *)(A + 16);
  pA2 = (xtfloatx2 *)(A + 24);
  /* Gauss elimination */
  for (k = 0; k<4; k++)
  {
    xtfloat b0, b1, b2, b3, b4, b5, b6, b7;
    xtfloat amax, max;
    unsigned int imax;
    /* pivoting */
    imax = k;
    amax = XT_CONST_S(0);
    /* find absolute max value in the k-th column */
    for (n = k; n<4; n++)
    {
      xtbool cond;
      xtfloat a, t;

      t = A[n * 8 + k];
      a = XT_ABS_S(t);

      cond = /*xtbool2_extract_0*/(XT_OLT_S(amax, a));
      XT_MOVT_S(amax, a, cond);
      XT_MOVT_S(max, t, cond);
      XT_MOVT(imax, n, cond);
    }

    R = max;
    R = XT_RECIP_S(R);
     
    /* permutation of rows */
    pmax = (xtfloatx2 *)(A + imax * 8);
    Ak0 = XT_LSX2I(pmax, 0 * SZ_2F32);
    Ak1 = XT_LSX2I(pmax, 1 * SZ_2F32);
    Ak2 = XT_LSX2I(pmax, 2 * SZ_2F32);
    Ak3 = XT_LSX2I(pmax, 3 * SZ_2F32);
    Amax0 = XT_LSX2I(pk, 0 * SZ_2F32);
    Amax1 = XT_LSX2I(pk, 1 * SZ_2F32);
    Amax2 = XT_LSX2I(pk, 2 * SZ_2F32);
    Amax3 = XT_LSX2I(pk, 3 * SZ_2F32);
    XT_SSX2IP(Amax0, pmax, SZ_2F32);
    XT_SSX2IP(Amax1, pmax, SZ_2F32);
    XT_SSX2IP(Amax2, pmax, SZ_2F32);
    XT_SSX2IP(Amax3, pmax, SZ_2F32);
    __Pragma("no_reorder")
    /* multiply k-th row by the reciprocal * 
    * pivot element during swapping rows  */
    b0 = XT_MUL_LHH_S(Ak0,R);
    b1 = XT_MUL_LLL_S(Ak0,R);
    b2 = XT_MUL_LHH_S(Ak1,R);
    b3 = XT_MUL_LLL_S(Ak1,R);
    b4 = XT_MUL_LHH_S(Ak2,R);
    b5 = XT_MUL_LLL_S(Ak2,R);
    b6 = XT_MUL_LHH_S(Ak3,R);
    b7 = XT_MUL_LLL_S(Ak3,R);

    XT_SSIP(b0, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b1, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b2, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b3, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b4, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b5, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b6, castxcc(xtfloat,pk), SZ_F32);
    XT_SSIP(b7, castxcc(xtfloat,pk), SZ_F32);
    
    /* elimination */
    /* join forward and back substitution */

    {
      xtfloat c0, c1, c2;
      xtfloat a0, a1, a2, a3, a4, a5, a6, a7;
      c0 = XT_LSX((xtfloat *)pA0, k*SZ_F32);
      c1 = XT_LSX((xtfloat *)pA1, k*SZ_F32);
      c2 = XT_LSX((xtfloat *)pA2, k*SZ_F32);

      a0 = XT_LSI((xtfloat *)pA0, 0 * SZ_F32);
      a1 = XT_LSI((xtfloat *)pA0, 1 * SZ_F32);
      a2 = XT_LSI((xtfloat *)pA0, 2 * SZ_F32);
      a3 = XT_LSI((xtfloat *)pA0, 3 * SZ_F32);
      a4 = XT_LSI((xtfloat *)pA0, 4 * SZ_F32);
      a5 = XT_LSI((xtfloat *)pA0, 5 * SZ_F32);
      a6 = XT_LSI((xtfloat *)pA0, 6 * SZ_F32);
      a7 = XT_LSI((xtfloat *)pA0, 7 * SZ_F32);
      XT_MSUB_S(a0, b0, c0);
      XT_MSUB_S(a1, b1, c0);
      XT_MSUB_S(a2, b2, c0);
      XT_MSUB_S(a3, b3, c0);
      XT_MSUB_S(a4, b4, c0);
      XT_MSUB_S(a5, b5, c0);
      XT_MSUB_S(a6, b6, c0);
      XT_MSUB_S(a7, b7, c0);

      XT_SSXC(a0, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a1, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a2, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a3, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a4, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a5, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a6, castxcc(xtfloat, pA0), SZ_F32);
      XT_SSXC(a7, castxcc(xtfloat, pA0), SZ_F32);
      
      a0 = XT_LSI((xtfloat *)pA1, 0 * SZ_F32);
      a1 = XT_LSI((xtfloat *)pA1, 1 * SZ_F32);
      a2 = XT_LSI((xtfloat *)pA1, 2 * SZ_F32);
      a3 = XT_LSI((xtfloat *)pA1, 3 * SZ_F32);
      a4 = XT_LSI((xtfloat *)pA1, 4 * SZ_F32);
      a5 = XT_LSI((xtfloat *)pA1, 5 * SZ_F32);
      a6 = XT_LSI((xtfloat *)pA1, 6 * SZ_F32);
      a7 = XT_LSI((xtfloat *)pA1, 7 * SZ_F32);
      XT_MSUB_S(a0, b0, c1);
      XT_MSUB_S(a1, b1, c1);
      XT_MSUB_S(a2, b2, c1);
      XT_MSUB_S(a3, b3, c1);
      XT_MSUB_S(a4, b4, c1);
      XT_MSUB_S(a5, b5, c1);
      XT_MSUB_S(a6, b6, c1);
      XT_MSUB_S(a7, b7, c1);

      XT_SSXC(a0, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a1, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a2, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a3, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a4, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a5, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a6, castxcc(xtfloat, pA1), SZ_F32);
      XT_SSXC(a7, castxcc(xtfloat, pA1), SZ_F32);
      
      a0 = XT_LSI((xtfloat *)pA2, 0 * SZ_F32);
      a1 = XT_LSI((xtfloat *)pA2, 1 * SZ_F32);
      a2 = XT_LSI((xtfloat *)pA2, 2 * SZ_F32);
      a3 = XT_LSI((xtfloat *)pA2, 3 * SZ_F32);
      a4 = XT_LSI((xtfloat *)pA2, 4 * SZ_F32);
      a5 = XT_LSI((xtfloat *)pA2, 5 * SZ_F32);
      a6 = XT_LSI((xtfloat *)pA2, 6 * SZ_F32);
      a7 = XT_LSI((xtfloat *)pA2, 7 * SZ_F32);
      XT_MSUB_S(a0, b0, c2);
      XT_MSUB_S(a1, b1, c2);
      XT_MSUB_S(a2, b2, c2);
      XT_MSUB_S(a3, b3, c2);
      XT_MSUB_S(a4, b4, c2);
      XT_MSUB_S(a5, b5, c2);
      XT_MSUB_S(a6, b6, c2);
      XT_MSUB_S(a7, b7, c2);
      
      XT_SSXC(a0, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a1, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a2, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a3, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a4, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a5, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a6, castxcc(xtfloat, pA2), SZ_F32);
      XT_SSXC(a7, castxcc(xtfloat, pA2), SZ_F32);
    }
  }
  
  /* copy inverse matrix to x */
  pA0 = (xtfloatx2 *)(x);
  pA1 = (xtfloatx2 *)(A + 4);
  vX = AE_ZALIGN64();
  {
    XT_LSX2IP(A00, pA1, SZ_2F32);
    XT_LSX2IP(A01, pA1, 3 * SZ_2F32);
    XT_LSX2IP(A02, pA1, SZ_2F32);
    XT_LSX2IP(A03, pA1, 3 * SZ_2F32);
    XT_LSX2IP(A10, pA1, SZ_2F32);
    XT_LSX2IP(A11, pA1, 3 * SZ_2F32);
    XT_LSX2IP(A12, pA1, SZ_2F32);
    XT_LSX2IP(A13, pA1, 0);

    XT_SASX2IP(A00, vX, pA0);
    XT_SASX2IP(A01, vX, pA0);
    XT_SASX2IP(A02, vX, pA0);
    XT_SASX2IP(A03, vX, pA0);
    XT_SASX2IP(A10, vX, pA0);
    XT_SASX2IP(A11, vX, pA0);
    XT_SASX2IP(A12, vX, pA0);
    XT_SASX2IP(A13, vX, pA0);
  }
  XT_SASX2POSFP(vX, pA0);
}/* mtx_inv4x4f() */
#endif
