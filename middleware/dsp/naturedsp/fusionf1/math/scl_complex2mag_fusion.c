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
DISCARD_FUN(float32_t, scl_complex2mag, (complex_float x))
#else
/*===========================================================================
  Scalar matematics:
  scl_complex2invmag             Complex magnitude (reciprocal) 
===========================================================================*/
/*-------------------------------------------------------------------------
  Complex magnitude
  routines compute complex magnitude or its reciprocal

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input complex data
  N     length of vector

  Restriction:
  none
-------------------------------------------------------------------------*/
float32_t  scl_complex2mag (complex_float x)
{
  /*
  float32_t x_re,x_im;
  float32_t mnt_re, mnt_im, mnt_abs;
  int exp_re, exp_im, exp_abs;
  const int minexp = FLT_MIN_EXP - FLT_MANT_DIG;

  x_re = fabsf( crealf(x) );
  x_im = fabsf( cimagf(x) );

  exp_re = ( x_re != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_re) ) : minexp );
  exp_im = ( x_im != 0 ? (int)STDLIB_MATH(ceilf)( log2f(x_im) ) : minexp );

  exp_abs = ( exp_re > exp_im ? exp_re : exp_im );

  mnt_re = STDLIB_MATH(ldexpf)( x_re, -exp_abs );
  mnt_im = STDLIB_MATH(ldexpf)( x_im, -exp_abs );

  mnt_abs = sqrtf( mnt_re*mnt_re + mnt_im*mnt_im );

  return STDLIB_MATH(ldexpf)( mnt_abs, exp_abs );
  */
  xtfloat xl,xh;
  xtfloatx2 x0, y0;
  xtfloat xf;
  complex_float ALIGN(8) x_tmp;
  ae_int32x2 t0, nsa, u0;
  ae_int32x2 e0, e1;
  ae_int32x2 nsa0;
  xtbool2 b0;
  x_tmp = x;
  x0 = XT_LSX2I((xtfloatx2 *)&x_tmp, 0);
  x0 = XT_ABS_SX2(x0);

  t0 = XT_AE_MOVINT32X2_FROMXTFLOATX2(x0);

  u0 = AE_SRLI32(t0, 23);
  u0 = AE_AND32(u0, 0xFF);
  e0 = AE_SUB32(u0, 127);
  e1 = AE_SEL32_LH(e0, e0);
  nsa = AE_MAX32(e0, e1);
  b0 = AE_LT32(127, nsa);
  AE_MOVT32X2(nsa, 127, b0);

  e0 = AE_SUB32(127, nsa);
  b0 = AE_EQ32(e0, 0);
  nsa0 = AE_SLLI32(e0, 23);
  AE_MOVT32X2(nsa0, 0x00400000, b0);
  y0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
  xl=XT_MUL_LLL_S(x0,y0);
  xh=XT_MUL_LHH_S(x0,y0);
  xf = XT_MUL_S(xl, xl);
  XT_MADD_S(xf,xh,xh);
  xf = XT_SQRT_S(xf);
  e0 = AE_ADD32(127, nsa);
  b0 = AE_EQ32(e0, 0);
  nsa0 = AE_SLLI32(e0, 23);
  AE_MOVT32X2(nsa0, 0x00400000, b0);
  x0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(nsa0);
  xf = XT_MUL_S(xf, XT_LOW_S(x0));

  return (float32_t)xf;
}
#endif
