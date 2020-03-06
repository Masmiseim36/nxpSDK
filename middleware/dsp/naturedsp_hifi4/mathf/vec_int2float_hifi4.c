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
/* Common helper macros. */
#include "common_fpu.h"

#if !HAVE_VFPU && !HAVE_FPU
DISCARD_FUN(void, vec_int2float,( float32_t * restrict y, const int32_t * restrict x, int t, int N ))
#elif HAVE_VFPU
/*===========================================================================
  Vector matematics:
  vec_int2float            integer to float converion    
===========================================================================*/
/*-------------------------------------------------------------------------
  Integer to float conversion
  Routines convert integer to float and scale result up by 2^t.

  Precision: 
  f     32-bit input, floating point output

  Input:
  x[N]  input data, integer
  t     scale factor
  N     length of vector
  Output:
  y[N]  conversion results, floating point

  Restriction:
  t should be in range -126...126
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
#define sz_i32    (int)sizeof(int32_t)
void   vec_int2float ( float32_t  * restrict y, const int32_t  * restrict x, int t, int N)
{
  /*
  *  float32_t y;
  *  ufloat32uint32 s;
  *  ASSERT(t>=-126 && t<=126);
  *  s.u = ((uint32_t)(t + 127)) << 23;
  *  y=((float32_t)x);
  *  y=y*s.f;
  *  return y;
  */
   const ae_int32x2 * restrict X = (const ae_int32x2 *)x;
          xtfloatx2 * restrict Y = ( xtfloatx2 *)y;
  
  int n;
  xtfloatx2 y0, y1;
  xtfloat f0;
  ae_int32x2 x0, t0;
  int32_t h0; 
  ae_valign Y_va;
  NASSERT(x);
  NASSERT(y);
  NASSERT(t >= -126 && t <= 126);
  if (N <= 0) return;
  t0 = AE_MOVDA32(((uint32_t)(t + 127)) << 23);
  y1 = XT_AE_MOVXTFLOATX2_FROMINT32X2(t0);
  Y_va = AE_ZALIGN64();
  if ((((uintptr_t)(x)) & 7))
  {
    ae_int32x2 tmp; 
    h0 = XT_L32I((int *)X, 0);
    AE_L32_IP(tmp, castxcc(ae_int32, X), sz_f32);
    f0 = XT_FLOAT_S(h0, 0);
    f0 = XT_MUL_S(f0, y1);
    XT_SSIP(f0, castxcc(xtfloat,Y), sz_f32);
    N--;
  }
  for (n = 0; n<(N >> 1); n++)
  {
    AE_L32X2_IP(x0, X, 2 * sz_i32);
    y0 = XT_FLOAT_SX2(x0, 0);
    y0 = XT_MUL_SX2(y0, y1);
    XT_SASX2IP(y0, Y_va, Y);
  }
  XT_SASX2POSFP(Y_va, Y); 
  if (N & 1)
  {
    h0 = XT_L32I((int *)X, 0);
    f0 = XT_FLOAT_S(h0, 0);
    f0 = XT_MUL_S(f0, y1);
    XT_SSI(f0, (xtfloat *)Y, 0);
  }
}/* vec_int2float() */
#elif HAVE_FPU
#define sz_f32    (int)sizeof(float32_t) 

void vec_int2float( float32_t * restrict y, const int32_t * restrict x, int t, int N )
{
	int n;
	xtfloat f0, y0;
	int32_t x0;

	NASSERT(x);
	NASSERT(y);
	NASSERT(t >= -126 && t <= 126);
	if (N <= 0) return;
	y0 = XT_WFR((uint32_t)(t + 127) << 23);
	for (n=0; n<N; n++)
	{
		x0 = XT_L32I((const int *)x, 0);
		x++;
		f0 = XT_FLOAT_S(x0, 0);
		f0 = XT_MUL_S(f0, y0);
		XT_SSIP(f0, castxcc(xtfloat, y), sz_f32);
	}
} /* vec_int2float() */
#endif
