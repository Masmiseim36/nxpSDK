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

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common helper macros. */
#include "common.h" 
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, mtx_det4x4f, (float32_t * restrict d, const float32_t *restrict x, int L))
#else
/*-------------------------------------------------------------------------
  Operations with Small Matrices
  These functions implement basic operations under the sequence of small 
  square matrices. Fixed point data are interpreted as Q15 or Q31 and 
  results might be saturated.
  NOTE: 
  Determinant is computed recursively via minors of submatrices. So, in 
  the fixed-point routines, intermediate results might be saturated 
  although final result is in range. To avoid this saturation, right shift 
  might be applied at the first stage of computations. It means that final 
  result would be represented in Q(15-rsh) or Q(31-rsh) respectively. 
  Ad-hoc formula for rsh is rsh>=N-2 for real matrices and rsh>=N-1 for 
  complex matrices.

  Precision: 
  16x16  16-bit input, 16-bit output (real and complex)
  32x32  32-bit input, 32-bit output (real and complex)
  f      floating point (real and complex)

  Matrix dimensions are 2x2, 3x3, 4x4

  Input:
  x[L][N*N]      L input matrices
  y[L][N*N]      L input matrices (for addition, subtraction, multiply 
                 functions)
  rsh            right shift for fixed-point multiply and determinant 
                 function
  L              number of matrices
  Output:
  z[L][N*N]      L output matrices (for addition, subtraction, multiply, 
                 transpose functions)
  d[L]           determinants for L matrices (for determinant functions)

  Restrictions:
  rsh should be in range 0..15
  x,y,z should not overlap
-------------------------------------------------------------------------*/
/* real matrix determinant */
void mtx_det4x4f     (float32_t * restrict d, const float32_t *restrict x,          int L)
{
          xtfloat    *  pz = (xtfloat      *)d;
  const xtfloatx2    *  restrict px = (const xtfloatx2      *)(x);
  const xtfloatx2    *  restrict px_ = (const xtfloatx2      *)(x+12);

  int l;
  xtfloatx2 x01, x23, x45, x67;
  xtfloatx2 x89, xab, xcd, xef;
  xtfloat x0, x1, x2, x3;
  xtfloat A0, A1, A2;
  xtfloat B1, B2, C2;
  xtfloat M0, M1, M2, M3;
  ae_valign xa;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  XT_LASX2IP(x89, xa, px);
  XT_LASX2IP(xab, xa, px);
  XT_LASX2IP(xcd, xa, px);
  XT_LASX2IP(xef, xa, px);
  A0 = XT_MUL_LHL_S(xab, xef);
  A1 = XT_MUL_LLL_S(x89, xef);
  B1 = XT_MUL_LHL_S(x89, xef);
  A2 = XT_MUL_LLH_S(x89, xef);
  B2 = XT_MUL_LHH_S(x89, xef);
  C2 = XT_MUL_LHL_S(x89, xcd);

  XT_MSUB_LLH_S(A0, xab, xef);
  XT_MSUB_LLL_S(A1, xcd, xab);
  XT_MSUB_LLH_S(A2, xcd, xab);
  XT_MSUB_LHL_S(B1, xcd, xab);
  XT_MSUB_LHH_S(B2, xcd, xab);
  XT_MSUB_LHL_S(C2, xcd, x89);
  for (l = 0; l<(L - 1 ); l++)
  {
    XT_LASX2IP(x01, xa, px);
    XT_LASX2IP(x23, xa, px);
    XT_LASX2IP(x45, xa, px);
    XT_LASX2IP(x67, xa, px);

    XT_LSIP(x0, castxcc(xtfloat,px_), 4);//45
    XT_LSIP(x1, castxcc(xtfloat,px_), 4);
    XT_LSIP(x2, castxcc(xtfloat,px_), 4);//67
    XT_LSXP(x3, castxcc(xtfloat,px_), -7 * 4);

    M0 = XT_MUL_S(x1, A0);
    XT_MSUB_S(M0, x2, A1);
    XT_MADD_S(M0, x3, A2);

    M1 = XT_MUL_S(x0, A0);
    XT_MSUB_S(M1, x2, B1);
    XT_MADD_S(M1, x3, B2);

    M2 = XT_MUL_S(x0, A1);
    XT_MSUB_S(M2, x1, B1);
    XT_MADD_S(M2, x3, C2);

    M3 = XT_MUL_S(x0, A2);
    XT_MSUB_S(M3, x1, B2);
    XT_MADD_S(M3, x2, C2);

    XT_LSIP(x0, castxcc(xtfloat,px_), 2*4);
    XT_LSXP(x2, castxcc(xtfloat,px_), 18*4);

    A0 = XT_MUL_S(x0, M0);
    A1 = XT_MUL_S((x01), M1);
    XT_MADD_S(A0, x2, M2);
    XT_MADD_S(A1, (x23), M3);
    A0 = XT_SUB_S(A0,A1);

    XT_SSIP(A0, pz, 4);

    XT_LASX2IP(x89, xa, px);
    XT_LASX2IP(xab, xa, px);
    XT_LASX2IP(xcd, xa, px);
    XT_LASX2IP(xef, xa, px);

    A0 = XT_MUL_LHL_S(xab, xef);
    A1 = XT_MUL_LLL_S(x89, xef);
    B1 = XT_MUL_LHL_S(x89, xef);
    A2 = XT_MUL_LLH_S(x89, xef);
    B2 = XT_MUL_LHH_S(x89, xef);
    C2 = XT_MUL_LHL_S(x89, xcd);

    XT_MSUB_LLH_S(A0, xab, xef);
    XT_MSUB_LLL_S(A1, xcd, xab);
    XT_MSUB_LLH_S(A2, xcd, xab);
    XT_MSUB_LHL_S(B1, xcd, xab);
    XT_MSUB_LHH_S(B2, xcd, xab);
    XT_MSUB_LHL_S(C2, xcd, x89);
  }
  XT_LASX2IP(x01, xa, px);
  XT_LASX2IP(x23, xa, px);
  XT_LASX2IP(x45, xa, px);
  XT_LASX2IP(x67, xa, px);

  XT_LSIP(x0, castxcc(xtfloat, px_), 4);//45
  XT_LSIP(x1, castxcc(xtfloat, px_), 4);
  XT_LSIP(x2, castxcc(xtfloat, px_), 4);//67
  XT_LSXP(x3, castxcc(xtfloat, px_), -7 * 4);

  M0 = XT_MUL_S(x1, A0);
  XT_MSUB_S(M0, x2, A1);
  XT_MADD_S(M0, x3, A2);

  M1 = XT_MUL_S(x0, A0);
  XT_MSUB_S(M1, x2, B1);
  XT_MADD_S(M1, x3, B2);

  M2 = XT_MUL_S(x0, A1);
  XT_MSUB_S(M2, x1, B1);
  XT_MADD_S(M2, x3, C2);

  M3 = XT_MUL_S(x0, A2);
  XT_MSUB_S(M3, x1, B2);
  XT_MADD_S(M3, x2, C2);

  XT_LSIP(x0, castxcc(xtfloat, px_), 2 * 4);
  XT_LSXP(x2, castxcc(xtfloat, px_), 18 * 4);

  A0 = XT_MUL_S(x0, M0);
  A1 = XT_MUL_S((x01), M1);
  XT_MADD_S(A0, x2, M2);
  XT_MADD_S(A1, (x23), M3);
  A0 = XT_SUB_S(A0, A1);

  XT_SSIP(A0, pz, 4);
} /* mtx_det4x4f() */
#endif


