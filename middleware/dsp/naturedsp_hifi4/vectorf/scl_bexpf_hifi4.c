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
DISCARD_FUN(int,scl_bexpf,(float32_t x))
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
int scl_bexpf  (float32_t x)
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
  unsigned e;
  xtfloat f0;
  int32_t u0, exp0;
  xtbool2 b0, b1;
  xtbool zero, bnan;
  f0 = x;
  f0 = XT_ABS_S(f0);
  b0 = XT_UN_S(f0, f0);
  b1 = XT_OEQ_S(f0, 0.f);
  zero = xtbool2_extract_0(b0);
  bnan = xtbool2_extract_0(b1);
  zero = XT_ORB(zero, bnan);
  exp0 = 0;
  u0 = XT_RFR(f0);
  u0 = ((uint32_t)u0) >> 23;
  if (u0 == 0)
  {
    f0 = XT_MUL_S(f0, 16777216.f);
    exp0 -= 24;
  }
  u0 = XT_RFR(f0);
  u0 = ((uint32_t)u0) >> 23;
  exp0 += ((int)(u0)-126);
  e = exp0;
  XT_MOVT(e, 0, zero);
  return -(int32_t)e;
} /* scl_bexpf() */
#else
int scl_bexpf  (float32_t x)
{
  unsigned e;
  xtfloat f0;
  int32_t u0, exp0;
  xtbool zero, bnan;
  f0 = x;
  f0 = XT_ABS_S(f0);
  zero = XT_UN_S(f0, f0);
  bnan = XT_OEQ_S(f0, 0.f);
  zero = XT_ORB(zero, bnan);
  exp0 = 0;
  u0 = XT_RFR(f0);
  u0 = ((uint32_t)u0) >> 23;
  XT_MOVT_S(f0,XT_MUL_S(f0, 16777216.f),(xtbool)(u0==0));
  XT_MOVEQZ(exp0,-24,u0);
  u0 = XT_RFR(f0);
  u0 = ((uint32_t)u0) >> 23;
  exp0 += ((int)(u0)-126);
  e = exp0;
  XT_MOVT(e, 0, zero);
  return -(int32_t)e;
}
#endif
