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
                                                             
#include "common.h"
#include "NatureDSP_types.h"
/* Library API */
#include "NatureDSP_Signal_vector.h"

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
//#include <stdio.h>
int vec_bexp16_fast (const int16_t * restrict x, int N)
{
  ae_int32x2                  vA0, vB0, vB1;
  int                         k, a, b;
  const ae_p16x2s * restrict  px   = (const ae_p16x2s *)(x-2);

  if (N<=0) return 0;
  NASSERT_ALIGN8(x);
  NASSERT((N&3)==0);

  vB0 = AE_MOVDA32(0x80);
  vB1 = AE_MOVDA32(0x80);
  if(N&4)
  {
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB0 = AE_MAX32(vB0, vA0);
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB1 = AE_MAX32(vB1, vA0);
  }
  __Pragma("no_unroll")
  for (k=0; k<(N>>3); ++k)
  {
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB0 = AE_MAX32(vB0, vA0);
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB1 = AE_MAX32(vB1, vA0);
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB0 = AE_MAX32(vB0, vA0);
    AE_L16X2M_IU(vA0, px, sizeof(*px));
    vA0 = AE_ABS32S(vA0);
    vB1 = AE_MAX32(vB1, vA0);
  }
  vB0=AE_MAXABS32S(vB0,vB1);
  vB0=AE_SLAI32S(vB0,8);
  a = AE_NSAZ32_L(vB0);
  b = AE_NSAZ32_L(AE_SEL32_LH(vB0,vB0));
  return XT_MIN(a, b)+16;
}
