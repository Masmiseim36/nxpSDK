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
DISCARD_FUN(void, mtx_det3x3f, (float32_t * restrict d, const float32_t *restrict x, int L))
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
void mtx_det3x3f(float32_t * restrict d, const float32_t *restrict x, int L)
{
        xtfloatx2    *  pz = (xtfloatx2      *)d;
  const xtfloatx2    *  restrict px = (const xtfloatx2      *)(x);

  int l;
  xtfloatx2 x01, x23, x45, x67, x80, x12, x34, x56, x78;
 
  xtfloat A0, A1, A2;
  xtfloat B0, B1, B2;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(d, sizeof(*d));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L >> 1); l++)
  {
    XT_LASX2IP(x01, xa, px);
    XT_LASX2IP(x23, xa, px);
    XT_LASX2IP(x45, xa, px);
    XT_LASX2IP(x67, xa, px);
    XT_LASX2IP(x80, xa, px);
    XT_LASX2IP(x12, xa, px);
    XT_LASX2IP(x34, xa, px);
    XT_LASX2IP(x56, xa, px);
    XT_LASX2IP(x78, xa, px);

    A0 = XT_MUL_LHH_S(x45, x80);
    XT_MSUB_LLL_S(A0, x45, x67);

    B0 = XT_MUL_LLL_S(x34, x78);
    XT_MSUB_LHH_S(B0, x56, x78);

    A1 = XT_MUL_LLH_S(x23, x80);
    XT_MSUB_LLH_S(A1, x45, x67);

    B1 = XT_MUL_LHL_S(x34, x78);
    XT_MSUB_LHL_S(B1, x56, x56);

    A2 = XT_MUL_LLL_S(x23, x67);
    XT_MSUB_LHH_S(A2, x45, x67);

    B2 = XT_MUL_LHH_S(x34, x78);
    XT_MSUB_LLL_S(B2, x34, x56);


    A0 = XT_MUL_S(XT_HIGH_S(x01), A0);
    XT_MSUB_S(A0, x01, A1);
    XT_MADD_S(A0, XT_HIGH_S(x23), A2);

    B0 = XT_MUL_S(x80, B0);
    XT_MSUB_S(B0, XT_HIGH_S(x12), B1);
    XT_MADD_S(B0, x12, B2);

    XT_SSIP(A0, castxcc(xtfloat, pz), 4);
    XT_SSIP(B0, castxcc(xtfloat, pz), 4);
  }
  if (L&1)
  {
    XT_LASX2IP(x01, xa, px);
    XT_LASX2IP(x23, xa, px);
    XT_LASX2IP(x45, xa, px);
    XT_LASX2IP(x67, xa, px);
    XT_LASX2IP(x80, xa, px);

    A0 = XT_MUL_LHH_S(x45, x80);
    XT_MSUB_LLL_S(A0, x45, x67);

    A1 = XT_MUL_LLH_S(x23, x80);
    XT_MSUB_LLH_S(A1, x45, x67);
    A2 = XT_MUL_LLL_S(x23, x67);
    XT_MSUB_LHH_S(A2, x45, x67);
    A0 = XT_MUL_S(XT_HIGH_S(x01), A0);
    XT_MSUB_S(A0, x01, A1);
    XT_MADD_S(A0, XT_HIGH_S(x23), A2);
    XT_SSIP(A0, castxcc(xtfloat, pz), 4);
  }
} /*mtx_det3x3f()*/

#endif

