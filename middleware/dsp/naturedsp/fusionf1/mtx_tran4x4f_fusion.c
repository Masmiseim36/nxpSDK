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
DISCARD_FUN(void, mtx_tran4x4f, (float32_t * restrict z, const float32_t *restrict x, int L))
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
/* real matrix transpose */
void mtx_tran4x4f(float32_t * restrict z, const float32_t *restrict x, int L)
{
  int l;
        xtfloatx2    *  pz = (xtfloatx2      *)z;
  const xtfloatx2    *  restrict px = (const xtfloatx2      *)x;
  xtfloatx2 x0, x1, x2, x3, x4, x5, x6, x7;
  xtfloatx2 z0, z1, z2, z3, z4, z5, z6, z7;
  ae_valign xa, za;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(z, sizeof(*z));
  if (L <= 0) return;
  xa = AE_LA64_PP(px);
  za = AE_ZALIGN64();
  for (l = 0; l<(L); l++)
  {
    XT_LASX2IP(x0, xa, px);
    XT_LASX2IP(x1, xa, px);
    XT_LASX2IP(x2, xa, px);
    XT_LASX2IP(x3, xa, px);
    XT_LASX2IP(x4, xa, px);
    XT_LASX2IP(x5, xa, px);
    XT_LASX2IP(x6, xa, px);
    XT_LASX2IP(x7, xa, px);
    z0 = XT_SEL32_HH_SX2(x0, x2);
    z1 = XT_SEL32_HH_SX2(x4, x6);
    z2 = XT_SEL32_LL_SX2(x0, x2);
    z3 = XT_SEL32_LL_SX2(x4, x6);
    z4 = XT_SEL32_HH_SX2(x1, x3);
    z5 = XT_SEL32_HH_SX2(x5, x7);
    z6 = XT_SEL32_LL_SX2(x1, x3);
    z7 = XT_SEL32_LL_SX2(x5, x7);
    XT_SASX2IP(z0, za, pz);
    XT_SASX2IP(z1, za, pz);
    XT_SASX2IP(z2, za, pz);
    XT_SASX2IP(z3, za, pz);
    XT_SASX2IP(z4, za, pz);
    XT_SASX2IP(z5, za, pz);
    XT_SASX2IP(z6, za, pz);
    XT_SASX2IP(z7, za, pz);
  }
  AE_SA64POS_FP(za, pz);
} /* mtx_tran4x4f() */
#endif



