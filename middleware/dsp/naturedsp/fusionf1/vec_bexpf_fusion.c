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
DISCARD_FUN(int, vec_bexpf, (const float32_t * restrict x, int N))
#else
/*-------------------------------------------------------------------------
  Common Exponent 
  These functions determine the number of redundant sign bits for each value 
  (as if it was loaded in a 32-bit register) and returns the minimum number 
  over the whole vector. This may be useful for a FFT implementation to 
  normalize data.  
  NOTES:
  Faster version of functions make the same task but in a different manner - 
  they compute exponent of maximum absolute value in the array. It allows 
  faster computations but not bitexact results - if minimum value in the 
  array will be -2^n , fast function returns max(0,30-n) while non-fast 
  function returns (31-n).
  Floating point function returns 0-floor(log2(max(abs(x)))). Returned 
  result will be always in range [-129...146]. 
  Special cases
  x       | result
  --------+-------
  0       |    0
  +/-Inf  | -129
  NaN     |    0

  If dimension N<=0 functions return 0

  Precision: 
  32 32-bit inputs 
  24 24-bit inputs 
  16 16-bit inputs 
  f  single precision floating point

  Input:
  x[N] input data
  N    length of vector
  Returned value: minimum exponent

  Restriction:
  Regular versions (vec_bexp16, vec_bexp24, vec_bexp32, vec_bexpf):
  none

  Faster versions (vec_bexp16_fast, vec_bexp24_fast, vec_bexp32_fast):
  x,y - aligned on 8-byte boundary
  N   - a multiple of 4
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
int vec_bexpf  (const float32_t * restrict x, int N)
{
  /*
  * ufloat32uint32 ux;
  * int e;
  * x=fabsf(x);
  * if(isnan(x) || x==0.f) return 0;
  * ux.f=x;
  * e=0;
  * if ((ux.u>>23)==0)  multiply denorm numbers by 2^24
  * {
  * x = x*16777216.f;
  * e -= 24;
  * }
  * ux.f = x;
  * e += ((int)(ux.u >> 23) - 126);
  * return -e;
  */
  const xtfloat * restrict X = (const xtfloat *)x;
  unsigned e;
  int32_t exp0, u0;
  xtfloat f, f0, f1, fmax0, fmax1;
  xtbool zero, bnan;
  ae_valign x_align;
  int n;
  NASSERT(x);
  if (N <= 0) return 0;
  fmax0 = fmax1 = XT_MOV_S(0.f);
  x_align = AE_LA64_PP(X);
  for (n = 0; n<(N >> 1); n++)
  {
    XT_LSIP(f0, X, sz_f32);
    XT_LSIP(f1, X, sz_f32);
    f0 = XT_ABS_S(f0);
    f1 = XT_ABS_S(f1);
    fmax0 = XT_MAX_S(f0, fmax0);
    fmax1 = XT_MAX_S(f1, fmax1);
  }
  fmax0 = XT_MAX_S(fmax1, fmax0);
  f = fmax0;
  if (N & 1)
  {
    f1 = XT_LSI((xtfloat *)X, 0);
    f1 = XT_ABS_S(f1);
    f = XT_MAX_S(f1, f);
  }
  zero = XT_UN_S(f, f);
  bnan = XT_OEQ_S(f, 0.f);
  zero = XT_ORB(zero, bnan);
  exp0 = 0;
  u0 = XT_RFR(f); 
  u0 = ((uint32_t)u0) >> 23;
  if (u0 == 0)
  {
    f = XT_MUL_S(f, 16777216.f);
    exp0 -= 24;
  }
  u0 = XT_RFR(f);
  u0 = ((uint32_t)u0) >> 23;
  exp0 += ((int)(u0)-126);
  e = exp0;
  XT_MOVT(e, 0, zero);
  return -(int32_t)e;
} /*vec_bexpf()*/
#endif
