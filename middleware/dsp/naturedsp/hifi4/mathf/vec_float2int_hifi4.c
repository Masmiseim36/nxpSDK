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

/* DSP Library API */
#include "NatureDSP_Signal_math.h"
#include "common_fpu.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(void,vec_float2int,( int32_t * restrict y, const float32_t * restrict x, int t, int N ))
#elif HAVE_VFPU
#define sz_f32    (int)sizeof(float32_t) 
#define sz_i32    (int)sizeof(int32_t)
/*===========================================================================
  Vector matematics:
  vec_float2int            float to integer converion    
===========================================================================*/
/*-------------------------------------------------------------------------
  Float to integer conversion
  routines scale floating point input down by 2^t and convert it to integer 
  with saturation

  Precision: 
  f     single precision floating point

  Input:
  x[N]  input data, floating point
  t     scale factor
  N     length of vector
  Output:
  y[N]  conversion results, integer

  Restriction:
  t should be in range -126...126
-------------------------------------------------------------------------*/
void   vec_float2int (  int32_t * restrict y, const float32_t * restrict x, int t, int N)
{
  /*
  *  ufloat32uint32 s;
  *  ASSERT(t>=-126 && t<=126);
  *  s.u = ((uint32_t)(-t + 127)) << 23;
  *  x=x*s.f;
  *  if (x>= 2147483648.f) return MAX_INT32;
  *  if (x<=-2147483648.f) return MIN_INT32;
  *  return (int32_t) x;
  */
  const xtfloatx2 * restrict X = (const xtfloatx2 *)x;
       ae_int32x2 * restrict Y = (ae_int32x2 *)y;
  int n;
  ae_int32x2 y0, t0;
  xtfloat f0;
  xtfloatx2 x0, y1;
  int32_t h0;
  ae_valign Y_va;
  NASSERT(x);
  NASSERT(y);
  NASSERT(t >= -126 && t <= 126);
  if (N <= 0) return;
  t0 = AE_MOVDA32(((uint32_t)(-t + 127)) << 23);
  y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(t0);

  if ((((uintptr_t)(x)) & 7))
  {
    ae_int32x2 tmp;
    f0 = XT_LSX((xtfloat *)X, 0);
    AE_L32_IP(tmp, castxcc(ae_int32, X), sz_f32);
    f0 = XT_MUL_S(f0, y1);
    h0 = XT_TRUNC_S(f0, 0);
    AE_S32_L_IP(AE_MOVINT32X2_FROMINT32(h0), castxcc(ae_int32,Y), sz_i32);
    N--;
  }
  Y_va = AE_ZALIGN64();
  for (n = 0; n<(N >> 1); n++)
  {
    XT_LSX2IP(x0, X, 2 * sz_f32);
    x0 = XT_MUL_SX2(x0, y1);
    y0 = XT_TRUNC_SX2(x0,0);
    AE_SA32X2_IP(y0, Y_va, Y);
  }
  AE_SA64POS_FP(Y_va, Y);
  if (N & 1)
  { 
    f0 = XT_LSX((xtfloat *)X, 0);
    f0 = XT_MUL_S(f0, y1);
    h0 = XT_TRUNC_S(f0, 0);
    XT_S32I(h0, (int *)Y, 0);
  }
} /* vec_float2int() */
#elif HAVE_FPU
#define sz_f32    (int)sizeof(float32_t) 

void vec_float2int( int32_t * restrict y, const float32_t * restrict x, int t, int N )
{
    int n;
	xtfloat f0, y0;
	int32_t x0;

	NASSERT(x);
	NASSERT(y);
	NASSERT(t >= -126 && t <= 126);
	if (N <= 0) return;
	y0 = XT_WFR((uint32_t)(-t + 127) << 23);
	for (n=0; n<N; n++)
	{
        XT_LSIP(f0, castxcc(xtfloat, x), sz_f32);
        f0 = XT_MUL_S(f0, y0);
        x0 = XT_TRUNC_S(f0, 0);
		XT_S32I(x0, (int *)y, 0);
		y++;
	}
} /* vec_float2int() */
#endif
