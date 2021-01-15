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
    Power of a Vector
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
/* Signal Processing Library API. */
#include "NatureDSP_Signal_vector.h"
#include "NatureDSP_types.h"
#include "common.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(float32_t ,vec_powerf,( const float32_t * restrict x,int N))
#elif (HAVE_VFPU)

/*===========================================================================
  Vector matematics:
  vec_power            Power of a Vector
===========================================================================*/
/*-------------------------------------------------------------------------
  Power of a Vector
  These routines compute power of vector with scaling output result by rsh 
  bits. Fixed point rountines make accumulation in the 64-bit wide 
  accumulator and output may scaled down with saturation by rsh bits. 
  So, if representation of x input is Qx, result will be represented in 
  Q(2x-rsh) format.
  Two versions of routines are available: regular versions (vec_power32x32, 
  vec_power16x16, vec_powerf) work with arbitrary arguments, faster versions 
  (vec_power32x32_fast, vec_power16x16_fast) apply some restrictions.

  Precision: 
  32x32 32x32-bit data, 64-bit output
  16x16 16x16-bit data, 64-bit output
  f     single precision floating point

  Input:
  x[N]  input data, Q31, Q15 or floating point
  rsh   right shift of result
  N     length of vector
  Returns: 
  Sum of squares of a vector, Q(2x-rsh)

  Restrictions:
  for vec_power32x32(): rsh in range 31...62
  for vec_power16x16(): rsh in range 0...31
  For regular versions (vec_power32x32, vec_power16x16, vec_powerf):
  none
  For faster versions (vec_power32x32_fast, vec_power16x16_fast ):
  x - aligned on 8-byte boundary
  N - multiple of 4
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
float32_t   vec_powerf     ( const float32_t * restrict x,int N)
{
  int n;

  xtfloatx2 vacc0, vacc1, vacc2, vacc3;
  xtfloatx2 vacc4, vacc5, vacc6, vacc7;
  xtfloat xf, zf, acc;
  xtfloatx2 x0, x1;
  const xtfloatx2 * restrict px0 = (const xtfloatx2 *)x;

  NASSERT(x);
  if (N <= 0) return 0;

  vacc0 = XT_MOV_SX2(0.f);
  vacc1 = XT_MOV_SX2(0.f);
  vacc2 = XT_MOV_SX2(0.f);
  vacc3 = XT_MOV_SX2(0.f);
  vacc4 = XT_MOV_SX2(0.f);
  vacc5 = XT_MOV_SX2(0.f);
  vacc6 = XT_MOV_SX2(0.f);
  vacc7 = XT_MOV_SX2(0.f);
  zf = XT_MOV_S(0.f);

  if ((((uintptr_t)(x)) & 7))
  {
    ae_int32x2 tmp;
    xf = XT_LSI((const xtfloat *)px0, 0);
    AE_L32_IP(tmp, castxcc(ae_int32, px0), sz_f32);
    zf = XT_MUL_S(xf, xf);

    N--;
  }
  for (n = 0; n<(N>>4); n ++)
  {
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_LSX2IP(x1, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
    XT_MADD_SX2(vacc1, x1, x1);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_LSX2IP(x1, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc2, x0, x0);
    XT_MADD_SX2(vacc3, x1, x1);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_LSX2IP(x1, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc4, x0, x0);
    XT_MADD_SX2(vacc5, x1, x1);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_LSX2IP(x1, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc6, x0, x0);
    XT_MADD_SX2(vacc7, x1, x1); 
  }
  if (N & 8)
  {
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
  }
  if (N & 4)
  {
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
  }
  if (N & 2)
  {
    XT_LSX2IP(x0, px0, 2 * sz_f32);
    XT_MADD_SX2(vacc0, x0, x0);
  }
  if (N & 1)
  {
    xf = XT_LSI((const xtfloat *)px0, 0);
    XT_MADD_S(zf, xf, xf);
  }
  vacc0 = XT_ADD_SX2(vacc0, vacc1);
  vacc2 = XT_ADD_SX2(vacc3, vacc2);
  vacc4 = XT_ADD_SX2(vacc4, vacc5);
  vacc6 = XT_ADD_SX2(vacc6, vacc7);
  vacc0 = XT_ADD_SX2(vacc0, vacc2);
  vacc4 = XT_ADD_SX2(vacc4, vacc6);
  vacc0 = XT_ADD_SX2(vacc0, vacc4);

  acc = XT_RADD_SX2(vacc0);
  acc = XT_ADD_S(acc, zf);

  return acc;
} /* vec_powerf() */
#else
float32_t   vec_powerf     ( const float32_t * restrict x,int N)
{
  xtfloat acc0, acc1,x0;
  int n;
  const xtfloat  * restrict pX = (const xtfloat  *)x;
  if (N <= 0) return 0.f;
  acc0 = acc1 = XT_CONST_S(0);
  for (n = 0; n<(N&~1); n+=2)
  {
    XT_LSIP(x0, pX, sizeof(xtfloat));
    XT_MADD_S(acc0,x0,x0);
    XT_LSIP(x0, pX, sizeof(xtfloat));
    XT_MADD_S(acc1,x0,x0);
  }
  if (N&1)
  {
    XT_LSIP(x0, pX, sizeof(xtfloat));
    XT_MADD_S(acc0,x0,x0);
  }
  return XT_ADD_S(acc0 , acc1 );
}
#endif
