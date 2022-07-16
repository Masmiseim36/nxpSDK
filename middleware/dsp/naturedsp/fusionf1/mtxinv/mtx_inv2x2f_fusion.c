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
DISCARD_FUN(void, mtx_inv2x2f, (float32_t* x))
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
void mtx_inv2x2f(float32_t* x)
{
  xtfloat *pX = (xtfloat *) x;
  xtfloat a, b, c, d, r, rn;
  a = XT_LSI(pX, 0*sizeof(float32_t));
  b = XT_LSI(pX, 1*sizeof(float32_t));
  c = XT_LSI(pX, 2*sizeof(float32_t));
  d = XT_LSI(pX, 3*sizeof(float32_t));
  /* Find the determinant and its reciprocal */
  r = XT_MUL_S(a, d);
  XT_MSUB_S(r, b, c);
  r = XT_RECIP_S(r); rn = XT_NEG_S(r);
  /* Calculate matrix inversion */
  a = XT_MUL_S(a, r);
  b = XT_MUL_S(b, rn);
  c = XT_MUL_S(c, rn);
  d = XT_MUL_S(d, r);
  XT_SSI(d, pX, 0 * sizeof(float32_t));
  XT_SSI(b, pX, 1 * sizeof(float32_t));
  XT_SSI(c, pX, 2 * sizeof(float32_t));
  XT_SSI(a, pX, 3 * sizeof(float32_t));
} /* mtx_inv2x2f() */
#endif
