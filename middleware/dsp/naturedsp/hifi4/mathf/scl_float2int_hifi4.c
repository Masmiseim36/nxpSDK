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
DISCARD_FUN(int32_t,scl_float2int,( float32_t x, int t ))
#else
/*===========================================================================
  Scalar matematics:
  scl_float2int             float to integer converion    
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
int32_t scl_float2int( float32_t x, int t )
{
  /*
  * ufloat32uint32 s;
  * ASSERT(t>=-126 && t<=126);
  * s.u = ((uint32_t)(-t + 127)) << 23;
  * x=x*s.f;
  * if (x>= 2147483648.f) return MAX_INT32;
  * if (x<=-2147483648.f) return MIN_INT32;
  * return (int32_t) x;
  */
  xtfloat f0, y0;
  int32_t x0;
  NASSERT(t >= -126 && t <= 126);
  y0 = XT_WFR((uint32_t)(-t + 127) << 23);
  f0 = x;
  f0 = XT_MUL_S(f0, y0);
  x0 = XT_TRUNC_S(f0, 0);
  return x0;
} /* scl_float2int() */
#endif
