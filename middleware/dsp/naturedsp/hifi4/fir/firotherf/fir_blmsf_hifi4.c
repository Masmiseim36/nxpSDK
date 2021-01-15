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
 * Least Mean Squares
 * C code optimized for HiFi4
  IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,fir_blmsf,( float32_t * e, float32_t * h, const float32_t * r,
                const float32_t * x, float32_t norm, float32_t mu, int N, int M ))
#elif (HAVE_VFPU)

/*-------------------------------------------------------------------------
  Blockwise Adaptive LMS Algorithm for Real Data
  Blockwise LMS algorithm performs filtering of reference samples x[N+M-1],
  computation of error e[N] over a block of input samples r[N] and makes
  blockwise update of IR to minimize the error output.
  Algorithm includes FIR filtering, calculation of correlation between the 
  error output e[N] and reference signal x[N+M-1] and IR taps update based
  on that correlation.
NOTES: 
  1. The algorithm must be provided with the normalization factor, which is
     the power of the reference signal times N - the number of samples in a
     data block. This can be calculated using the vec_power32x32() or 
     vec_power16x16() function. In order to avoid the saturation of the 
     normalization factor, it may be biased, i.e. shifted to the right.
     If it's the case, then the adaptation coefficient must be also shifted
     to the right by the same number of bit positions.
  2. This algorithm consumes less CPU cycles per block than single 
     sample algorithm at similar convergence rate.
  3. Right selection of N depends on the change rate of impulse response:
     on static or slow varying channels convergence rate depends on
     selected mu and M, but not on N.
  4. 16x16 routine may converge slower on small errors due to roundoff 
     errors. In that cases, 16x32 routine will give better results although
     convergence rate on bigger errors is the same.

  Precision: 
  16x16    16-bit coefficients, 16-bit data, 16-bit output
  16x32    32-bit coefficients, 16-bit data, 16-bit output
  32x32    32-bit coefficients, 32-bit data, 32-bit output, complex and real
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point, complex and real
  Input:
  h[M]     impulse response, Q15, Q31 or floating point
  r[N]	   input data vector (near end). First in time value is in 
           r[0], Q15, Q31 or floating point
  x[N+M-1] reference data vector (far end). First in time value is in x[0],  
           Q15, Q31 or floating point
  norm     normalization factor: power of signal multiplied by N, Q15, Q31  
           or floating point
           Fixed-point format for the 32x16-bit variant: Q(2*x+1-bias)
  mu       adaptation coefficient (LMS step), Q(31-bias) or Q(15-bias)
  N        length of data block
  M        length of h
  Output:
  e[N]     estimated error, Q15, Q31 or floating point
  h[M]     updated impulse response, Q15, Q31 or floating point

  Restriction:
  x,r,h,e  should not overlap
  x,r,h,e  aligned on a 8-bytes boundary
  N,M      multiples of 8 and >0
-------------------------------------------------------------------------*/
void fir_blmsf( float32_t * e, float32_t * h, const float32_t * r,
                const float32_t * x, float32_t norm, float32_t mu, int N, int M )
{
  float32_t b;
  int m, n;

  const xtfloatx2 *restrict pX1;
  const xtfloatx2 *restrict pX0;
  const xtfloatx2 *restrict pR;
        xtfloatx2 *restrict pE;
  const xtfloatx2 *restrict pH;
        xtfloatx2 *restrict pH_wr;
  xtfloatx2 s0, s1, s2, s3;
  xtfloatx2 s0_, s1_, s2_, s3_;
  xtfloatx2 x01, x12, x23, x34, x45, x56, x67;
  xtfloatx2 h01, h23, s01, s23;

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

  /* estimate error */
  pR = (const xtfloatx2*)r;
  pE = (      xtfloatx2*)e;
  __Pragma("loop_count min=1");
  for (n = 0; n < N; n += 4)
  {
    xtfloatx2 r01, r23;
    pX0 = (const xtfloatx2 *)(x+n);
    pX1 = (const xtfloatx2 *)(x+n+2);
    pH  = (const xtfloatx2 *)(h+M-2);

    s0 =s1 =s2 =s3 =(xtfloatx2)(0.0f);
    s0_=s1_=s2_=s3_=(xtfloatx2)(0.0f);
    /* preload data from x */
    XT_LSX2IP(x01, pX0, 4*sizeof(float32_t));
    XT_LSX2IP(x23, pX1, 4*sizeof(float32_t));
    x12 = XT_SEL32_LH_SX2(x01, x23);

    __Pragma("loop_count min=1");
    for (m = 0; m < (M>>2); m++)
    {
        /* load data from x */
        XT_LSX2IP(x45, pX0, 4*sizeof(float32_t));
        XT_LSX2IP(x67, pX1, 4*sizeof(float32_t));
        x34 = XT_SEL32_LH_SX2(x23, x45);
        x56 = XT_SEL32_LH_SX2(x45, x67);
        /* load data from h */
        XT_LSX2RIP(h01, pH, -2*(int)sizeof(float32_t));
        XT_LSX2RIP(h23, pH, -2*(int)sizeof(float32_t));
        /* compute convolution of 4 values */
        XT_MADD_SX2(s0, x01, h01);
        XT_MADD_SX2(s1, x12, h01);
        XT_MADD_SX2(s2, x23, h01);
        XT_MADD_SX2(s3, x34, h01);
        
        XT_MADD_SX2(s0_, x23, h23);
        XT_MADD_SX2(s1_, x34, h23);
        XT_MADD_SX2(s2_, x45, h23);
        XT_MADD_SX2(s3_, x56, h23);
        /* shift input line for the next iteration */
        x01 = x45;
        x12 = x56;
        x23 = x67;
    }
    s0 = s0 + s0_;
    s1 = s1 + s1_;
    s2 = s2 + s2_;
    s3 = s3 + s3_;
    s0_ = XT_SEL32_HL_SX2(s0, s1);
    s1_ = XT_SEL32_LH_SX2(s0, s1);
    s2_ = XT_SEL32_HL_SX2(s2, s3);
    s3_ = XT_SEL32_LH_SX2(s2, s3);
    s01 = s0_ + s1_;
    s23 = s2_ + s3_;
    /* compute and save error */
    XT_LSX2IP(r01, pR, 2*sizeof(float32_t));
    XT_LSX2IP(r23, pR, 2*sizeof(float32_t));
    s01 = XT_SUB_SX2(r01, s01);
    s23 = XT_SUB_SX2(r23, s23);
    XT_SSX2IP(s01, pE, 2*sizeof(float32_t));
    XT_SSX2IP(s23, pE, 2*sizeof(float32_t));
  }
  /* update impluse response */
  b = mu / norm;
  pH_wr = (xtfloatx2*)((uintptr_t)(h + M - 2));
  pH = pH_wr;
  __Pragma("loop_count min=1");
  for (m = 0; m < M; m += 4)
  {
    xtfloatx2 err01, err23;
    pX0 = (const xtfloatx2 *)(x+m);
    pX1 = (const xtfloatx2 *)(x+m+2);
    pE  = (      xtfloatx2 *)(e);

    s0 =s1 =s2 =s3 =(xtfloatx2)(0.0f);
    s0_=s1_=s2_=s3_=(xtfloatx2)(0.0f);
    /* preload data from x */
    XT_LSX2IP(x01, pX0, 4*sizeof(float32_t));
    XT_LSX2IP(x23, pX1, 4*sizeof(float32_t));
    x12 = XT_SEL32_LH_SX2(x01, x23);

    __Pragma("loop_count min=1");
    for (n = 0; n < (N>>2); n++)
    {
        /* load data from x */
        XT_LSX2IP(x45, pX0, 4*sizeof(float32_t));
        XT_LSX2IP(x67, pX1, 4*sizeof(float32_t));
        x34 = XT_SEL32_LH_SX2(x23, x45);
        x56 = XT_SEL32_LH_SX2(x45, x67);
        /* load data from e */
        XT_LSX2IP(err01, pE, 2*(int)sizeof(float32_t));
        XT_LSX2IP(err23, pE, 2*(int)sizeof(float32_t));
        /* compute correlation of 4 values */
        XT_MADD_SX2(s0, x01, err01);
        XT_MADD_SX2(s1, x12, err01);
        XT_MADD_SX2(s2, x23, err01);
        XT_MADD_SX2(s3, x34, err01);
        
        XT_MADD_SX2(s0_, x23, err23);
        XT_MADD_SX2(s1_, x34, err23);
        XT_MADD_SX2(s2_, x45, err23);
        XT_MADD_SX2(s3_, x56, err23);
        /* shift input line for the next iteration */
        x01 = x45;
        x12 = x56;
        x23 = x67;
    }
    s0 = s0 + s0_;
    s1 = s1 + s1_;
    s2 = s2 + s2_;
    s3 = s3 + s3_;
    s0_ = XT_SEL32_HL_SX2(s0, s1);
    s1_ = XT_SEL32_LH_SX2(s0, s1);
    s2_ = XT_SEL32_HL_SX2(s2, s3);
    s3_ = XT_SEL32_LH_SX2(s2, s3);
    s01 = s0_ + s1_;
    s23 = s2_ + s3_;
    /* update and save IR */
    XT_LSX2RIP(h01, pH, -2*(int)sizeof(float32_t));
    XT_LSX2RIP(h23, pH, -2*(int)sizeof(float32_t));
    XT_MADD_SX2(h01, (xtfloatx2)b, s01);
    XT_MADD_SX2(h23, (xtfloatx2)b, s23);
    XT_SSX2RIP(h01, pH_wr, -2*(int)sizeof(float32_t));
    XT_SSX2RIP(h23, pH_wr, -2*(int)sizeof(float32_t));
  }
} /* fir_blmsf() */
#else
// for scalar FPU

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
    XT_LSXP(h0, pH, -4);
    XT_LSXP(h1, pH, -4);
    XT_LSXP(h2, pH, -4);
    XT_LSXP(h3, pH, -4);
    XT_MADD_S(h0, b, s0);
    XT_MADD_S(h1, b, s1);
    XT_MADD_S(h2, b, s2);
    XT_MADD_S(h3, b, s3);
    pH_wr = (xtfloat*)((uintptr_t)(h + M - 1 - m));
    XT_SSXP(h0, pH_wr, -4);
    XT_SSXP(h1, pH_wr, -4);
    XT_SSXP(h2, pH_wr, -4);
    XT_SSXP(h3, pH_wr, -4);
  }
} /* fir_blmsf() */
#endif
