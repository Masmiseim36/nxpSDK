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
DISCARD_FUN(void, vec_int2float, (float32_t  * restrict y, const int32_t  * restrict x, int t, int N))
#else
/*===========================================================================
  Vector matematics:
  vec_int2float            integer to float converion    
===========================================================================*/
/*-------------------------------------------------------------------------
  integer to float conversion
  routine converts integer to float and scales result up by 2^t.

  Precision: 
  f     single precision floating point

  Output:
  y[N]  output data
  Input:
  x[N]  input data
  t     scale factor
  N     length of vector

  Restriction:
  t should be in range -126...126
-------------------------------------------------------------------------*/
#define sz_f32    (int)sizeof(float32_t)
void   vec_int2float ( float32_t  * restrict y, const int32_t  * restrict x, int t, int N)
{
  /*
  * float32_t y;
  * ufloat32uint32 s;
  * ASSERT(t>=-126 && t<=126);
  * s.u = ((uint32_t)(t + 127)) << 23;
  * y=((float32_t)x);
  * y=y*s.f;
  * return y; 
  */
  const ae_int32 * restrict X = (const ae_int32 *)x;
        xtfloat * restrict Y = (xtfloat *)y;

  int n;
  xtfloat y0, r0;
  ae_int32 x0, t0;
  NASSERT(x);
  NASSERT(y);
  NASSERT(t >= -126 && t <= 126);
  if (N <= 0) return;
  t0 = AE_MOVDA32(((uint32_t)(t + 127)) << 23);
  r0 = XT_WFR(t0);
  for (n = 0; n<N ; n++)
  {
    ae_int32_loadip(x0, X, sz_f32);
    y0 = XT_FLOAT_S(x0, 0);
    y0 = XT_MUL_S(y0, r0);
    XT_SSIP(y0, Y, sz_f32);
  }
} /* vec_int2float() */
#endif
