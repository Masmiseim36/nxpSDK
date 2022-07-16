/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*
 * Least Mean Squares
 * C code optimized for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of input samples x[N+M-1],
  computation of error e[N] over a block of reference data r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the input signal times N - the number of samples in a
     data block. This can be calculated using the vec_power24x24() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. this algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 rountine will give better results although 
     convergence rate on bigger errors is the same

  Precision: 
  16x16   16-bit coefficients, 16-bit data, 16-bit output
  24x24   24-bit coefficients, 24-bit data, 32-bit output
  16x32   32-bit coefficients, 16-bit data, 16-bit output
  32x32   32-bit coefficients, 32-bit data, 32-bit output
  f       floating point
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]     reference data vector (near end). First in time value is in 
           r[0], 24-bit or 16-bit, Qx or floating point
  x[N+M-1] input vector (far end). First in time value is in x[0],  
           24-bit or 16-bit, Qx or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, 
           Q31 or floating point
           Fixed-point format for the 24x24-bit variant: Q(2*x-31-bias)
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q31,Q15 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e - should not overlap
  x,r,h,e - aligned on a 8-bytes boundary
  N,M     - multiples of 8 and >0
-------------------------------------------------------------------------*/
#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_blmsf, ( float32_t * e, float32_t * h, const float32_t * r,
  const float32_t * x, float32_t norm, float32_t mu, int N, int M))
#else

void fir_blmsf( float32_t * e, float32_t * h, const float32_t * r,
                const float32_t * x, float32_t norm, float32_t mu, int N, int M )
{
  float32_t b;
  int m, n;

  const xtfloat*  restrict pX = (const xtfloat*)x;
  const xtfloat*  restrict pR = (const xtfloat*)r;
  xtfloat*  restrict pE = (      xtfloat*)e;
  const xtfloat*  restrict pe = (      xtfloat*)e;
  const xtfloat*    restrict ph = (const xtfloat*)h;
  xtfloat*    restrict pH = (xtfloat*)h;
  xtfloat*    restrict pH_wr = (xtfloat*)h;
  NASSERT(e);
  NASSERT(h);
  NASSERT(r);
  NASSERT(x);
  NASSERT_ALIGN(e, 8);
  NASSERT_ALIGN(h, 8);
  NASSERT_ALIGN(r, 8);
  NASSERT_ALIGN(x, 8);
  NASSERT(N>0 && M>0);
  NASSERT(M % 8 == 0 && N % 8 == 0);
  
  if (N <= 0 || M <= 0) return;

  /* estimate error */
  for (n = 0; n<(N); n+=4)
  {
    xtfloat r0, r1, r2, r3;
    xtfloat s0, s1, s2, s3;
    xtfloat x0, x1, x2, x3,x4;
    s0 = XT_CONST_S(0);
    s1 = XT_CONST_S(0);
    s2 = XT_CONST_S(0);
    s3 = XT_CONST_S(0);
    pX = (xtfloat*)((uintptr_t)(x+n));
    XT_LSIP(x0, pX, 4);
    XT_LSIP(x1, pX, 4);
    XT_LSIP(x2, pX, 4);
    XT_LSIP(x3, pX, 4);
    for (m = 0; m<M; m+=2)
    {
      xtfloat hm;
      hm = ph[M - 1 - m];
      XT_LSIP(x4, pX, 4);
      XT_MADD_S(s0, hm, x0);
      XT_MADD_S(s1, hm, x1);
      XT_MADD_S(s2, hm, x2);
      XT_MADD_S(s3, hm, x3);
      hm = ph[M - 1 - m - 1];
      XT_MADD_S(s0, hm, x1);
      XT_MADD_S(s1, hm, x2);
      XT_MADD_S(s2, hm, x3);
      XT_MADD_S(s3, hm, x4);
      x0 = x2; x1 = x3; x2 = x4; 
      XT_LSIP(x3, pX, 4);
    }
    XT_LSIP(r0, pR, 4);
    XT_LSIP(r1, pR, 4);
    XT_LSIP(r2, pR, 4);
    XT_LSIP(r3, pR, 4);
    s0=XT_SUB_S(r0, s0);
    s1=XT_SUB_S(r1, s1);
    s2=XT_SUB_S(r2, s2);
    s3=XT_SUB_S(r3, s3);
    XT_SSIP(s0, pE, 4);
    XT_SSIP(s1, pE, 4);
    XT_SSIP(s2, pE, 4);
    XT_SSIP(s3, pE, 4);
  }
  /* update impluse response */
  b = mu / norm;
  for (m = 0; m<M; m+=4)
  {
    xtfloat h0, h1, h2, h3;
    xtfloat s0, s1, s2, s3;
    xtfloat x0, x1, x2, x3, x4;
    s0 = XT_CONST_S(0);
    s1 = XT_CONST_S(0);
    s2 = XT_CONST_S(0);
    s3 = XT_CONST_S(0);
    pX = (xtfloat*)((uintptr_t)(x + m));
    XT_LSIP(x0, pX, 4);
    XT_LSIP(x1, pX, 4);
    XT_LSIP(x2, pX, 4);
    XT_LSIP(x3, pX, 4);

    for (n = 0; n<N; n+=2)
    {     
      xtfloat en;
      en = pe[n];
      XT_LSIP(x4, pX, 4);
      XT_MADD_S(s0, en, x0);
      XT_MADD_S(s1, en, x1);
      XT_MADD_S(s2, en, x2);
      XT_MADD_S(s3, en, x3);
      en = pe[n+1];
      XT_MADD_S(s0, en, x1);
      XT_MADD_S(s1, en, x2);
      XT_MADD_S(s2, en, x3);
      XT_MADD_S(s3, en, x4);
      x0 = x2; x1 = x3; x2 = x4;
      XT_LSIP(x3, pX, 4);
    } 
    pH = (xtfloat*)((uintptr_t)(h + M - 1 - m));
    XT_LSIP(h0, pH, -4);
    XT_LSIP(h1, pH, -4);
    XT_LSIP(h2, pH, -4);
    XT_LSIP(h3, pH, -4);
    XT_MADD_S(h0, b, s0);
    XT_MADD_S(h1, b, s1);
    XT_MADD_S(h2, b, s2);
    XT_MADD_S(h3, b, s3);
    pH_wr = (xtfloat*)((uintptr_t)(h + M - 1 - m));
    XT_SSIP(h0, pH_wr, -4);
    XT_SSIP(h1, pH_wr, -4);
    XT_SSIP(h2, pH_wr, -4);
    XT_SSIP(h3, pH_wr, -4);
  }
} /* fir_blmsf() */

#endif

