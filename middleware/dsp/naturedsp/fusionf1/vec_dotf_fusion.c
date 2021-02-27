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
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, vec_dotf, (const float32_t * restrict x, const float32_t * restrict y, int N))
#else
/*===========================================================================
  Vector matematics:
  vec_dot              Vector Dot Product
===========================================================================*/
/*-------------------------------------------------------------------------
  Vector Dot product
  These routines take two vectors and calculates their dot product. Two 
  versions of routines are available: regular versions (vec_dot24x24, 
  vec_dot32x16, vec_dot32x32, vec_dot16x16, vec_dotf) work with arbitrary 
  arguments, faster versions (vec_dot24x24_fast, vec_dot32x16_fast, 
  vec_dot32x32_fast, vec_dot16x16_fast) apply some restrictions.

  Precision: 
  16x16  16x16-bit data, 64-bit output for regular version and 32-bit for 
         fast version
  24x24  24x24-bit data, 64-bit output
  32x16  32x16-bit data, 64-bit output
  32x32  32x32-bit data, 64-bit output
  f      floating point

  Input:
  x[N]  input data, Q31 or floating point
  y[N]  input data, Q31, Q15, or floating point
  N	length of vectors
  Returns: dot product of all data pairs, Q31 or floating point

  Restrictions:
  Regular versions:
  None
  Faster versions:
  x,y - aligned on 8-byte boundary
  N   - multiple of 4

  vec_dot16x16_fast utilizes 32-bit saturating accumulator, so, input data 
  should be scaled properly to avoid erroneous results
-------------------------------------------------------------------------*/
float32_t vec_dotf   (const float32_t * restrict x,const float32_t * restrict y,int N)

{
  int n;

  xtfloat xf0, yf0;
  xtfloat zf0, zf1, zf2, zf3;
  xtfloatx2 vxf0, vyf0;
  xtfloatx2 vxf1, vyf1;
  const xtfloat * restrict px = (const xtfloat *)x;
  const xtfloat * restrict py = (const xtfloat *)y;
  ae_valign x_align, y_align;
  NASSERT(x);
  NASSERT(y);
  if (N <= 0) return 0; 
  x_align = AE_LA64_PP(px);
  y_align = AE_LA64_PP(py);

  zf0 = XT_MOV_S(0);
  zf1 = XT_MOV_S(0);
  zf2 = XT_MOV_S(0);
  zf3 = XT_MOV_S(0);

  for (n = 0; n<(N>>2); n++)
  { 
    XT_LASX2IP(vxf0, x_align, castxcc(xtfloatx2, px));
    XT_LASX2IP(vyf0, y_align, castxcc(xtfloatx2, py));
    XT_MADD_LHH_S(zf0, vyf0, vxf0);
    XT_MADD_LLL_S(zf1, vyf0, vxf0);
    XT_LASX2IP(vxf1, x_align, castxcc(xtfloatx2, px));
    XT_LASX2IP(vyf1, y_align, castxcc(xtfloatx2, py));
    XT_MADD_LHH_S(zf2, vyf1, vxf1);
    XT_MADD_LLL_S(zf3, vyf1, vxf1);
  }
  for (n=4*n; n<N; n++)
  {
    XT_LSIP(xf0, px, 4);
    XT_LSIP(yf0, py, 4);
    XT_MADD_S(zf0, xf0, yf0);
  }
  zf0 = XT_ADD_S(zf0, zf1);
  zf2 = XT_ADD_S(zf2, zf3);
  zf0 = XT_ADD_S(zf0, zf2);
  return zf0;
} /* vec_dotf() */
#endif
