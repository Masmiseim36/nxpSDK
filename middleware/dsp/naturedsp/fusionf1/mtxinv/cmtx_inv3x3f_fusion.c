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
 * Complex Matrix Inversion
 * Optimized code for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, cmtx_inv3x3f, (complex_float* x))
#else
#define SZ_F32 (sizeof(float32_t))
#define SZ_CF32 (2*SZ_F32)

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
void cmtx_inv3x3f(complex_float* x)
{
    xtfloatx2 * restrict pX;
    xtfloatx2 * restrict pA;
    xtfloatx2 * pA0;
    xtfloatx2 * pA1;
    xtfloatx2 * pmax;
          xtfloatx2 * restrict pk_st;
    const xtfloatx2 * restrict pk_ld;
    xtfloatx2 Ak, Amax, A0, A1;
    xtfloatx2 X0, X1, X2;
    xtfloatx2 R, C0, C1, C2;
    xtfloatx2 _0, _1;
    int n,k;
    complex_float ALIGN(16) A[18];
    /* Copy the matrix to buffer and
     * initialize identity matrix:
     * 
     * x00 x01 x02 1.0 0.0 0.0
     * x10 x11 x12 0.0 1.0 0.0
     * x20 x21 x22 0.0 0.0 1.0
     */
    pX = (xtfloatx2 *)x;
    pA = (xtfloatx2 *)A;
    _0 = XT_CONST_S(0);
    _1 = XT_CONST_S(1);
    _1 = XT_SEL32_LL_SX2(_1, _0);
    C0 = _1; C1 = _0; C2 = _0;
    /* save by 1 row per iteration */
    for (n=0; n<3; n++)
    {
        XT_LSX2IP(X0, pX, SZ_CF32);
        XT_LSX2IP(X1, pX, SZ_CF32);
        XT_LSX2IP(X2, pX, SZ_CF32);
        XT_SSX2IP(X0, pA, SZ_CF32);
        XT_SSX2IP(X1, pA, SZ_CF32);
        XT_SSX2IP(X2, pA, SZ_CF32);
        XT_SSX2I (C2, pA, 2*SZ_CF32);
        XT_SSX2I (C1, pA, SZ_CF32);
        XT_SSX2IP(C0, pA, 3*SZ_CF32);
        R = C2;
        C2 = C1;
        C1 = C0;
        C0 = R;
    }
    /* Set bounds of the buffer */
    WUR_AE_CBEGIN0((uintptr_t)(A));
    WUR_AE_CEND0  ((uintptr_t)(A+18));

    pk_ld = (xtfloatx2 *)(A);
    pk_st = (xtfloatx2 *)(A);
    pA0 = (xtfloatx2 *)(A+6);
    pA1 = (xtfloatx2 *)(A+12);
    /* Gauss elimination */
    for(k=0; k<3; k++)
    {
        xtfloat t0, re, im, amax, maxre, maxim;
        unsigned int imax;
        /* pivoting */
        imax=k;
        amax=maxre=maxim=XT_CONST_S(0);
        /* find absolute max value in the k-th column */
        pA = (xtfloatx2 *)(A+k*7);
        for(n=k; n<3; n++)
        {
          xtbool cond;
          xtfloatx2 t;
          xtfloat a;

          XT_LSX2IP(t, pA, 6*SZ_CF32);
          a = XT_MUL_LHH_S(t, t);
          XT_MADD_LLL_S(a, t, t);

          re = XT_HIGH_S(t);
          im = XT_LOW_S(t);
          cond = XT_OLT_S(amax, a);
          XT_MOVT_S(amax, a, cond);
          XT_MOVT_S(maxre, re, cond);
          XT_MOVT_S(maxim, im, cond);
          XT_MOVT  (imax, n, cond);
        }
        /* find the reciprocal of the pivot value */
        t0 = XT_MUL_S(maxre, maxre);
        XT_MADD_S(t0, maxim, maxim);
        t0 = XT_RECIP_S(t0);
        maxre = XT_MUL_S(maxre, t0);
        maxim = XT_NEG_S(maxim);
        maxim = XT_MUL_S(maxim, t0);
        R = XT_SEL32_LL_SX2((xtfloatx2)maxre, (xtfloatx2)maxim);

        /* permutation of rows */
        pmax = (xtfloatx2 *)(A+imax*6);
        for (n=0; n<6; n++)
        {
            Ak = XT_LSX2I(pmax, 0);
            XT_LSX2IP(Amax, pk_ld, SZ_CF32);
            XT_SSX2IP(Amax, pmax, SZ_CF32);
            /* multiply k-th row by the reciprocal *
             * pivot element during swapping rows  */
            re = XT_MUL_LHH_S(Ak,R);    XT_MSUB_LLL_S(re,Ak,R);
            im = XT_MUL_LLH_S(Ak,R);    XT_MADD_LHL_S(im,Ak,R);
            XT_SSX2_L_IP(re, im, pk_st);
        }

        /* elimination */
        /* join forward and back substitution */
        C0 = XT_LSX2X(pA0, k*SZ_CF32);
        C1 = XT_LSX2X(pA1, k*SZ_CF32);
        pk_ld -= 6;   
        for (n=0; n<6; n++)
        {
            XT_LSX2IP(Ak, pk_ld, SZ_CF32);
            A0 = XT_LSX2I(pA0, 0);
            A1 = XT_LSX2I(pA1, 0);

            re = XT_HIGH_S(A0);    im = XT_LOW_S(A0);
            XT_MSUB_LHH_S(re, Ak, C0);    XT_MADD_LLL_S(re, Ak, C0);
            XT_MSUB_LLH_S(im, Ak, C0);    XT_MSUB_LHL_S(im, Ak, C0);
            XT_SSXC(re, castxcc(xtfloat, pA0), SZ_F32);
            XT_SSXC(im, castxcc(xtfloat, pA0), SZ_F32);
            re = XT_HIGH_S(A1);    im = XT_LOW_S(A1);
            XT_MSUB_LHH_S(re, Ak, C1);    XT_MADD_LLL_S(re, Ak, C1);
            XT_MSUB_LLH_S(im, Ak, C1);    XT_MSUB_LHL_S(im, Ak, C1);
            XT_SSXC(re, castxcc(xtfloat, pA1), SZ_F32);
            XT_SSXC(im, castxcc(xtfloat, pA1), SZ_F32);
        }
    }
    /* copy 4-6 columns to x */
    pX = (xtfloatx2 *)(x);
    pA = (xtfloatx2 *)(A+3);
    for(n=0; n<3; n++)
    {
        XT_LSX2IP(X0, pA, SZ_CF32);
        XT_LSX2IP(X1, pA, SZ_CF32);
        XT_LSX2IP(X2, pA, 4*SZ_CF32);
        XT_SSX2IP(X0, pX, SZ_CF32);
        XT_SSX2IP(X1, pX, SZ_CF32);
        XT_SSX2IP(X2, pX, SZ_CF32);
    }
}/* cmtx_inv3x3f() */
#endif
