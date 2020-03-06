/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. Vector Operations
    Common Exponent 
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
/* Signal Processing Library API. */
#include "NatureDSP_Signal_vector.h"
#include "common.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(int,vec_bexpf,(const float32_t * restrict x, int N))
#elif (HAVE_VFPU)
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
  16 16-bit inputs 
  f  single precision floating point

  Input:
  x[N]    input data
  N       length of vector
  Returned value:
  minimum exponent

  Restriction:
  Regular versions (vec_bexp16, vec_bexp32, vec_bexpf):
  none
  Faster versions (vec_bexp16_fast, vec_bexp32_fast):
  x   - aligned on 8-byte boundary
  N   - a multiple of 4
-------------------------------------------------------------------------*/
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
  *   x = x*16777216.f;
  *   e -= 24;
  * }
  * ux.f = x;
  * e += ((int)(ux.u >> 23) - 126);
  * return -e;
  */
  const xtfloatx2 * restrict X = (const xtfloatx2 *)x;
  unsigned e;
  int32_t exp0, u0;
  xtfloatx2 f0, fmax0;
  xtfloat f, f1;
  xtbool2 b0, b1; 
  xtbool zero, bnan;
  ae_valign x_align;
  int n;
  NASSERT(x);
  if (N <= 0) return 0;
  fmax0 = XT_MOV_SX2(0.f);
  x_align = AE_LA64_PP(X);
  for (n = 0; n<(N >> 1); n++)
  {
    ae_int32x2 tmp;
    XT_LASX2IP(f0, x_align, X);
    /* x=fabsf(x) */
    tmp = XT_AE_MOVINT32X2_FROMXTFLOATX2(f0);
    tmp = AE_AND32(tmp, 0x7fffffff); 
    f0 = XT_AE_MOVXTFLOATX2_FROMINT32X2(tmp);
    fmax0 = XT_MAX_SX2(f0, fmax0);
  }
  f = XT_RMAX_S(fmax0);
  if (N & 1)
  {
    f1 = XT_LSI((xtfloat *)X,0);
    f1 = XT_ABS_S(f1);
    f = XT_MAX_S(f1, f);
  }
  b0 = XT_UN_S(f, f);
  b1 = XT_OEQ_S(f, 0.f);
  zero = xtbool2_extract_0(b0);
  bnan = xtbool2_extract_0(b1); 
  zero = XT_ORB(zero, bnan);
  exp0 = 0;
  u0 = XT_RFR(f);
  u0 = ((uint32_t)u0) >> 23;
  if (u0 == 0)
  {
    /* multiply denorm numbers by 2^24 */
    f = XT_MUL_S(f, 16777216.f);
    exp0 -= 24;
  }
  u0 = XT_RFR(f);
  u0 = ((uint32_t)u0)>>23;
  exp0 += ((int)(u0)-126);
  e = exp0;
  XT_MOVT(e, 0, zero);
  return -(int32_t)e;
} /* vec_bexpf() */
#else
int vec_bexpf  (const float32_t * restrict x, int N)
{
  const xtfloat * restrict X = (const xtfloat *)x;
  unsigned e;
  int32_t exp0, u0;
  xtfloat f0, fmax0;
  xtfloat f;
  xtbool zero, bnan;
  int n;
  NASSERT(x);
  if (N <= 0) return 0;
  fmax0 = XT_CONST_S(0);
  for (n = 0; n<N; n++)
  {
    XT_LSIP(f0, X, sizeof(xtfloat));
    f0 = XT_ABS_S(f0);
    fmax0 = XT_MAX_S(f0, fmax0);
  }
  f = (fmax0);
  zero = XT_UN_S(f, f);
  bnan = XT_OEQ_S(f, 0.f);
  zero = XT_ORB(zero, bnan);
  exp0 = 0;
  u0 = XT_RFR(f);
  u0 = ((uint32_t)u0) >> 23;
  XT_MOVT_S(f,XT_MUL_S(f, 16777216.f),(xtbool)(u0==0));
  XT_MOVEQZ(exp0,-24,u0);
  u0 = XT_RFR(f);
  u0 = ((uint32_t)u0)>>23;
  exp0 += ((int)(u0)-126);
  e = exp0;
  XT_MOVT(e, 0, zero);
  return -(int32_t)e;
} /* vec_bexpf() */
#endif
