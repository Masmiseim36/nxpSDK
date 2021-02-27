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

#include "NatureDSP_types.h"
#include "NatureDSP_Signal.h"
#include "common.h"
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(float32_t, vec_maxf, (const float32_t* restrict x, int N))
#else
/*-------------------------------------------------------------------------
  Vector Min/Max
  These routines find maximum/minimum value in a vector.
  Two versions of functions available: regular version (vec_min32x32, 
  vec_max32x32, vec_min24x24, vec_max24x24,vec_max16x16, vec_min16x16, 
  vec_maxf, vec_minf) with arbitrary arguments and faster version 
  (vec_min32x32_fast, vec_max32x32_fast, vec_min24x24_fast, 
  vec_max24x24_fast,vec_min16x16_fast, vec_min16x16_fast) that apply some 
  restrictions
  NOTE: functions return zero if N is less or equal to zero

  Precision: 
  32x32 32-bit data, 32-bit output
  24x24 24-bit data, 24-bit output
  16x16 16-bit data, 16-bit output
  f     single precision floating point
  
  Input:
  x[N]  input data
  N     length of vector
  Function return minimum or maximum value correspondingly

  Restriction:
  For regular routines:
  none
  For faster routines:
  x aligned on 8-byte boundary
  N   - multiple of 4
-------------------------------------------------------------------------*/
float32_t vec_maxf     (const float32_t* restrict x, int N)
{
  int n;
  xtfloat xf, maxf;

  const xtfloat * restrict px = (const xtfloat *)x;
  static const union ufloat32uint32 minusInff = { 0xff800000 }; /* -Inf */
  ae_valign x_align;
  NASSERT(x);
  if (N <= 0) return 0;

  x_align = AE_LA64_PP(px);

  maxf = XT_MOV_S(minusInff.f);
  for (n = 0; n<(N); n++)
  {
    XT_LSIP(xf, px, 4);

    maxf = XT_MAX_S(xf, maxf);
  }
  return maxf;
}
#endif
