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

#include "common.h"
#include "NatureDSP_types.h"

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
int vec_bexp16 (const int16_t * restrict x, int N)
{
  ae_int32x2                  vA0, vA1, vB0, vB1;
  ae_int64                    vL;
  int                         k, exp, a, b;
  const ae_p16x2s * restrict  px   = (const ae_p16x2s *)(x-2);
  const ae_p16s * restrict    px16 = (const ae_p16s *)  (x-1);

  exp = 31-8;

  vB0 = AE_MOVI(0);
  vB1 = AE_MOVI(0);

  if (N<=0) return 0;

  if (((uintptr_t)px)&3)
  {
    px = (const ae_p16x2s *)px16;
    AE_L16M_IU(vA0, px16, sizeof(*px16));
    --N;
    vB0 = AE_MIN32(vB0, vA0);
    vB1 = AE_MAX32(vB1, vA0);
  }

  AE_L16X2M_IU(vA0, px, sizeof(*px));

  for (k=0; k<N-1; k+=2)
  {
    vB0 = AE_MIN32(vB0, vA0);
    vB1 = AE_MAX32(vB1, vA0);
    AE_L16X2M_IU(vA0, px, sizeof(*px));
  }

  if (N&1)
  {
    vA1 = AE_SEL32_HH(vA0, vA0);
    vB0 = AE_MIN32(vB0, vA1);
    vB1 = AE_MAX32(vB1, vA1);
  }

  vA0 = AE_SEL32_LH(vB0, vB0);
  vA1 = AE_SEL32_LH(vB1, vB1);
  vB0 = AE_MIN32(vA0, vB0);
  vB1 = AE_MAX32(vA1, vB1);
  vL = AE_MOVINT64_FROMINT32X2(vB0);
  a = AE_NSA64(vL);
  vL = AE_MOVINT64_FROMINT32X2(vB1);
  b = AE_NSA64(vL);
  exp = XT_MIN(exp, a);
  exp = XT_MIN(exp, b);
  exp += 8;

  return exp;
}
