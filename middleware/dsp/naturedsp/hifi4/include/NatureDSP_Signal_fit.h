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
#ifndef __NATUREDSP_SIGNAL_FIT_H__
#define __NATUREDSP_SIGNAL_FIT_H__

#include "NatureDSP_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
  Fitting/Interpolation
  vec_poly      Polynomial Approximation 
===========================================================================*/

/*-------------------------------------------------------------------------
  Polynomial approximation
  Functions calculate polynomial approximation for all values from given 
  vector. Fixed point functions take polynomial coefficients in Q31 precision. 
  NOTE:
  approximation is calculated like Taylor series that is why overflow may 
  potentially occur if cumulative sum of coefficients given from the last 
  to the first coefficient is bigger that 1. To avoid this negative effect,
  all the coefficients may be scaled down and result will be shifted left 
  after all intermediate computations.

  Precision: 
  24x24  24-bit inputs, 24-bit coefficients, 24-bit output. 
  32x32  32-bit inputs, 32-bit coefficients, 32-bit output.
  f      floating point

  Input:
  x[N]    input data, Q31 or floating point
  N       length of vector
  lsh     additional left shift for result
  c[M+1]  coefficients (M=4 coefficients for vec_poly4_xxx 
          and M=8 coefficients for vec_poly8_xxx), Q31 or floating point
  Output:			
  z[N]    result, Q31 or floating point

  Restriction:
  x,c,z should not overlap
  lsh   should be in range 0...31

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x aligned on 8-byte boundary
  N   - multiple of 2
-------------------------------------------------------------------------*/
void vec_poly4_24x24(f24 * z, const f24 * x, const f24 *  c, int lsh, int N);
void vec_poly8_24x24(f24 * z, const f24 * x, const f24 *  c, int lsh, int N);
void vec_poly4_32x32(int32_t *  z, const int32_t *  x, const int32_t * c, int lsh, int N);
void vec_poly8_32x32(int32_t *  z, const int32_t *  x, const int32_t * c, int lsh, int N);
void vec_poly4f(float32_t *  z, const  float32_t *  x, const float32_t * c, int N);
void vec_poly8f(float32_t *  z, const  float32_t *  x, const float32_t * c, int N);

#ifdef __cplusplus
}
#endif

#endif/* __NATUREDSP_SIGNAL_FIT_H__ */
