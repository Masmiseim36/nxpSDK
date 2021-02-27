/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, vec_poly4f, (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N))
#else
/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q15 or 
  Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger than 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  16x16  16-bit inputs, 16-bit coefficients, 16-bit output. 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q15, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (5 coefficients for vec_poly4_xxx 
          and 9 coefficients for vec_poly8_xxx), Q15, Q31 
          or floating point
  Output:
  z[N]    result, Q15, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31
-------------------------------------------------------------------------*/
void vec_poly4f      (float32_t * restrict z, const float32_t * restrict x, const float32_t * restrict c, int N )
{
  int n;
  xtfloat c3f, c4f;

  const xtfloatx2* restrict px = (const xtfloatx2 *)x;
  const xtfloat *  restrict pc = (const xtfloat *)c;
  xtfloatx2* restrict pz = (xtfloatx2 *)z;
  xtfloat zf0, zf1, zf2, zf3;
  xtfloat xf0;
  NASSERT(x);
  NASSERT(c);
  NASSERT(z);
  if (N <= 0) return;

  c3f = XT_LSI(pc, 12);
  c4f = XT_LSI(pc, 16);
  for (n = 0; n <(N); n++) 
  {
    zf2 = XT_LSI(pc, 8);
    zf3 = c3f;
    zf0 = XT_LSI(pc, 0);
    zf1 = XT_LSI(pc, 4);
   
    XT_LSIP(xf0, castxcc(xtfloat, px), 4);
    XT_MADD_S(zf3, xf0, c4f);
    XT_MADD_S(zf2, xf0, zf3);
    XT_MADD_S(zf1, xf0, zf2);
    XT_MADD_S(zf0, xf0, zf1);
    XT_SSIP(zf0, castxcc(xtfloat, pz), 4);
  }
} /* vec_poly4f() */
#endif
