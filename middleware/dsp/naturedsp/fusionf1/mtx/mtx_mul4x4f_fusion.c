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
DISCARD_FUN(void, mtx_mul4x4f, (float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L))
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
/* real matrix multiply */
void mtx_mul4x4f(float32_t * restrict z, const float32_t *restrict x, const float32_t *restrict y, int L)
{
  const xtfloatx2    *  restrict py = (const xtfloatx2    *)y;
  const xtfloatx2    *  restrict py_ = (const xtfloatx2    *)y;
          xtfloat    *  pz = (xtfloat      *)z;
  const xtfloatx2    *  restrict px = (const xtfloatx2      *)x;
  const xtfloatx2    *  restrict px_ = (const xtfloatx2      *)x;
  int l, m;
  xtfloatx2 vx0, vy0, vx1, vy1;
  xtfloatx2 vy2, vy3;
  xtfloatx2 vy4, vy5;
  xtfloatx2 vy6, vy7;
  xtfloat A0, A1, A2, A3;
  ae_valign      y_align, x_align, z_align, y_align_, x_align_;
  NASSERT_ALIGN(x, sizeof(*x));
  NASSERT_ALIGN(y, sizeof(*y));
  NASSERT_ALIGN(z, sizeof(*z));
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);
  x_align_ = AE_LA64_PP(px_);
  y_align_ = AE_LA64_PP(py_);
  z_align = AE_ZALIGN64();
  for (l = 0; l<L; l++, y += 4 * 4)
  {
    py = (const xtfloatx2    *)y;
    y_align = AE_LA64_PP(py);
    XT_LASX2IP(vy0, y_align, py);
    XT_LASX2IP(vy1, y_align, py);
    XT_LASX2IP(vy2, y_align, py);
    XT_LASX2IP(vy3, y_align, py);
    XT_LASX2IP(vy4, y_align, py);
    for (m = 0; m<4; m++)
    {
      py_ = (const xtfloatx2    *)y;
      y_align_ = AE_LA64_PP(py_);

      XT_LASX2IP(vx0, x_align, px);
      XT_LASX2IP(vx1, x_align, px);
   
      XT_LASX2IP(vy5, y_align, py);
      XT_LASX2IP(vy6, y_align, py);
      XT_LASX2IP(vy7, y_align, py);

      A0 = XT_MUL_LHH_S(vx0, vy0);
      XT_MADD_LLH_S(A0, vx0, vy2);
      XT_MADD_LHH_S(A0, vx1, vy4);
      XT_MADD_LLH_S(A0, vx1, vy6);

      A1 = XT_MUL_LHL_S(vx0, vy0);
      XT_MADD_LLL_S(A1, vx0, vy2);
      XT_MADD_LHL_S(A1, vx1, vy4);
      XT_MADD_LLL_S(A1, vx1, vy6);

      XT_SSIP(A0, pz, 4);
      XT_SSIP(A1, pz, 4);
      XT_LASX2IP(vy0, y_align_, py_);
      XT_LASX2IP(vy1, y_align_, py_);
      XT_LASX2IP(vy2, y_align_, py_);
      XT_LASX2IP(vy3, y_align_, py_);

      A2 = XT_MUL_LHH_S(vx0, vy1);
      XT_MADD_LLH_S(A2, vx0, vy3);
      XT_MADD_LHH_S(A2, vx1, vy5);
      XT_MADD_LLH_S(A2, vx1, vy7);

      A3 = XT_MUL_LHL_S(vx0, vy1);
      XT_MADD_LLL_S(A3, vx0, vy3);
      XT_MADD_LHL_S(A3, vx1, vy5);
      XT_MADD_LLL_S(A3, vx1, vy7);
      py = (const xtfloatx2    *)y;
      y_align = AE_LA64_PP(py);
      XT_LASX2IP(vy0, y_align, py);
      XT_LASX2IP(vy1, y_align, py);
      XT_LASX2IP(vy2, y_align, py);
      XT_LASX2IP(vy3, y_align, py);
      XT_LASX2IP(vy4, y_align, py);

      XT_SSIP(A2, pz, 4);
      XT_SSIP(A3, pz, 4);
    }
  }
  AE_SA64POS_FP(z_align, pz);
} /* mtx_mul4x4f() */
#endif


