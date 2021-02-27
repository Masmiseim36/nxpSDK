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
DISCARD_FUN(void, cmtx_inv2x2f, (complex_float* x))
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
void cmtx_inv2x2f(complex_float* x)
{
  xtfloatx2 *pX = (xtfloatx2 *) x;
  xtfloatx2 a, b, c, d, rrex2, rimx2;
  xtfloat are, aim, bre, bim, cre, cim, dre, dim;
  xtfloat rre, rim, t0;

  a = XT_LSX2I(pX, 0*sizeof(complex_float));
  b = XT_LSX2I(pX, 1*sizeof(complex_float));
  c = XT_LSX2I(pX, 2*sizeof(complex_float));
  d = XT_LSX2I(pX, 3*sizeof(complex_float));
  /* Find the determinant and its reciprocal */
  /* r = det(x) = a*d-b*c */
  rre = XT_MUL_LHH_S(a, d);
  XT_MSUB_LLL_S(rre, a, d);
  XT_MSUB_LHH_S(rre, b, c);
  XT_MADD_LLL_S(rre, b, c);
  rim = XT_MUL_LLH_S(a, d);
  XT_MADD_LHL_S(rim, a, d);
  XT_MSUB_LLH_S(rim, b, c);
  XT_MSUB_LHL_S(rim, b, c);
  /* r = 1.0/det(x) */
  t0 = XT_MUL_S(rre, rre);
  XT_MADD_S(t0, rim, rim);
  t0 = XT_RECIP_S(t0);
  rre = XT_MUL_S(rre, t0);
  t0 = XT_NEG_S(t0); rim = XT_MUL_S(rim, t0);
  rrex2 = XT_xtfloat_rtor_xtfloatx2(rre);
  rimx2 = XT_xtfloat_rtor_xtfloatx2(rim);
  /* Calculate matrix inversion */
  /* a = a*r */
  are = XT_MUL_LHL_S(a, rrex2);
  XT_MSUB_LLL_S(are, a, rimx2);
  aim = XT_MUL_LLL_S(a, rrex2);
  XT_MADD_LHL_S(aim, a, rimx2);
  /* b = b*(-r) */
  bre = XT_MUL_LLL_S(b, rimx2);
  XT_MSUB_LHL_S(bre, b, rrex2);
  bim = XT_MUL_LLL_S(b, rrex2);
  XT_MADD_LHL_S(bim, b, rimx2);
  bim = XT_NEG_S(bim);
  /* c = c*(-r) */
  cre = XT_MUL_LLL_S(c, rimx2);
  XT_MSUB_LHL_S(cre, c, rrex2);
  cim = XT_MUL_LLL_S(c, rrex2);
  XT_MADD_LHL_S(cim, c, rimx2);
  cim = XT_NEG_S(cim);
  /* d = d*r */
  dre = XT_MUL_LHL_S(d, rrex2);
  XT_MSUB_LLL_S(dre, d, rimx2);
  dim = XT_MUL_LLL_S(d, rrex2);
  XT_MADD_LHL_S(dim, d, rimx2);
  /* Store computed values */
  XT_SSX2_L_IP(dre, dim, pX);
  XT_SSX2_L_IP(bre, bim, pX);
  XT_SSX2_L_IP(cre, cim, pX);
  XT_SSX2_L_IP(are, aim, pX);
}/* cmtx_inv2x2f() */
#endif
