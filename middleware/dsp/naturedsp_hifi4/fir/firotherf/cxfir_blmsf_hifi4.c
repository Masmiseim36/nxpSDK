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
 * Least Mean Squares Complex Data
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
DISCARD_FUN(void,cxfir_blmsf,( complex_float * e, complex_float * h, 
                const complex_float * r,
                const complex_float * x, 
                float32_t norm, float32_t mu, 
                int          N, int       M ))
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
void cxfir_blmsf    ( complex_float * e, complex_float * h, 
                const complex_float * r,
                const complex_float * x, 
                float32_t norm, float32_t mu, 
                int          N, int       M )
{
          xtfloatx2 * restrict pH;
          xtfloatx2 * restrict pE;
    const xtfloatx2 * restrict pX;
    const xtfloatx2 * restrict pR;
    xtfloatx2 b;
    int m,n;
    NASSERT(e);
    NASSERT(h);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(e,8);
    NASSERT_ALIGN(h,8);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT(N>0 && M>0);
    NASSERT(M%8==0 && N%8==0);

    /* estimate error */
    pR=(const xtfloatx2 * )r;
    pE=(      xtfloatx2 * )e;
    for (n=0; n<N; n+=4)
    {
        xtfloatx2 s0,s1,x0,x1,x2,x3,hh,s2,s3;
        xtfloatx2 s4,s5,s6,s7;
        s4=s5=s6=s7=XT_CONST_S(0);
        pH=(      xtfloatx2 *)(h+M-1);
        pX=(const xtfloatx2 *)(x+n);
        XT_LSX2IP(s0,pR,sizeof(complex_float));
        XT_LSX2IP(s1,pR,sizeof(complex_float));
        XT_LSX2IP(s2,pR,sizeof(complex_float));
        XT_LSX2IP(s3,pR,sizeof(complex_float));
        XT_LSX2IP(x0,pX,sizeof(complex_float));
        XT_LSX2IP(x1,pX,sizeof(complex_float));
        XT_LSX2IP(x2,pX,sizeof(complex_float));
        __Pragma("loop_count min=8, factor=2")
        for (m=0; m<M; m++)
        {
            XT_LSX2XP(hh,pH,-(int)sizeof(complex_float));
            XT_LSX2IP(x3,pX,sizeof(complex_float));
            XT_MADDMUX_S(s0,x0,hh,6);
            XT_MADDMUX_S(s4,x0,hh,7);
            XT_MADDMUX_S(s1,x1,hh,6);
            XT_MADDMUX_S(s5,x1,hh,7);
            XT_MADDMUX_S(s2,x2,hh,6);
            XT_MADDMUX_S(s6,x2,hh,7);
            XT_MADDMUX_S(s3,x3,hh,6);
            XT_MADDMUX_S(s7,x3,hh,7);
            x0=x1; x1=x2; x2=x3;
        }
        s0=XT_ADD_SX2(s0,s4);
        s1=XT_ADD_SX2(s1,s5);
        s2=XT_ADD_SX2(s2,s6);
        s3=XT_ADD_SX2(s3,s7);

        XT_SSX2IP(s0,pE,sizeof(complex_float));
        XT_SSX2IP(s1,pE,sizeof(complex_float));
        XT_SSX2IP(s2,pE,sizeof(complex_float));
        XT_SSX2IP(s3,pE,sizeof(complex_float));
    }
    /* update impluse response */
    b=XT_MUL_SX2(mu,XT_RECIP_SX2(norm));
    pH=(xtfloatx2*)(h+M-4);
    for (m=0; m<M; m+=4)
    {
        xtfloatx2 h0,h1,h2,h3;
        xtfloatx2 s0,s1,s2,s3,s4,s5,s6,s7,x0,x1,x2,x3,ee;
        pE=(xtfloatx2*)e;
        pX=(const xtfloatx2*)(x+m);
        s0=s1=s2=s3=XT_CONST_S(0);
        s4=s5=s6=s7=XT_CONST_S(0);
        XT_LSX2IP(x0,pX,sizeof(complex_float));
        XT_LSX2IP(x1,pX,sizeof(complex_float));
        XT_LSX2IP(x2,pX,sizeof(complex_float));
        __Pragma("loop_count min=8, factor=2")
        for (n=0; n<N; n++)
        {
            XT_LSX2IP(ee,pE,sizeof(complex_float));
            XT_LSX2IP(x3,pX,sizeof(complex_float));
            XT_MADDMUX_S(s0,x0,ee,4);
            XT_MADDMUX_S(s4,x0,ee,5);
            XT_MADDMUX_S(s1,x1,ee,4);
            XT_MADDMUX_S(s5,x1,ee,5);
            XT_MADDMUX_S(s2,x2,ee,4);
            XT_MADDMUX_S(s6,x2,ee,5);
            XT_MADDMUX_S(s3,x3,ee,4);
            XT_MADDMUX_S(s7,x3,ee,5);
            x0=x1;
            x1=x2;
            x2=x3;
        }
        s0=XT_ADD_SX2(s0,s4);
        s1=XT_ADD_SX2(s1,s5);
        s2=XT_ADD_SX2(s2,s6);
        s3=XT_ADD_SX2(s3,s7);
        h0=XT_LSX2I(pH,3*sizeof(complex_float));
        h1=XT_LSX2I(pH,2*sizeof(complex_float));
        h2=XT_LSX2I(pH,1*sizeof(complex_float));
        h3=XT_LSX2I(pH,0*sizeof(complex_float));
        XT_MADD_SX2(h0,s0,b);
        XT_MADD_SX2(h1,s1,b);
        XT_MADD_SX2(h2,s2,b);
        XT_MADD_SX2(h3,s3,b);
        XT_SSX2I (h0,pH,3*sizeof(complex_float));
        XT_SSX2I (h1,pH,2*sizeof(complex_float));
        XT_SSX2I (h2,pH,1*sizeof(complex_float));
        XT_SSX2XP(h3,pH,-4*(int)sizeof(complex_float));
    }
} /* cxfir_blmsf() */
#else
/* for scalar FPU */
void cxfir_blmsf    ( complex_float * e, complex_float * h, 
                const complex_float * r,
                const complex_float * x, 
                float32_t norm, float32_t mu, 
                int          N, int       M )
{
          xtfloat* restrict pH;
    const xtfloat* restrict pX;
    const xtfloat* restrict pR;
          xtfloat* restrict pE;
    xtfloat b;
    int m,n;
    NASSERT(e);
    NASSERT(h);
    NASSERT(r);
    NASSERT(x);
    NASSERT_ALIGN(e,8);
    NASSERT_ALIGN(h,8);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT(N>0 && M>0);
    NASSERT(M%8==0 && N%8==0);

    /* estimate error */
    pR=(const xtfloat*)r;
    pE=(      xtfloat*)e;
    for (n=0; n<N; n+=2)
    {
        xtfloat s0_re,s0_im,h0_r,h0_i,x0_r,x0_i;
        xtfloat s1_re,s1_im;
        XT_LSIP(s0_re,pR,sizeof(float32_t));
        XT_LSIP(s0_im,pR,sizeof(float32_t));
        XT_LSIP(s1_re,pR,sizeof(float32_t));
        XT_LSIP(s1_im,pR,sizeof(float32_t));
        pX=(const xtfloat*)(x+n);
        pH=(      xtfloat*)(h+M-1);
        XT_LSIP(x0_r,pX,sizeof(float32_t));
        XT_LSIP(x0_i,pX,sizeof(float32_t));
        __Pragma("loop_count min=8")
        for (m=0; m<M; m++)
        {
            h0_i=XT_LSI (pH,sizeof(float32_t));
            XT_LSXP(h0_r,pH,-2*(int)sizeof(float32_t));
            XT_MSUB_S(s0_re,h0_r,x0_r); XT_MSUB_S(s0_re,h0_i,x0_i);
            XT_MADD_S(s0_im,h0_i,x0_r); XT_MSUB_S(s0_im,h0_r,x0_i);
            XT_LSIP(x0_r,pX,sizeof(float32_t));
            XT_LSIP(x0_i,pX,sizeof(float32_t));
            XT_MSUB_S(s1_re,h0_r,x0_r); XT_MSUB_S(s1_re,h0_i,x0_i);
            XT_MADD_S(s1_im,h0_i,x0_r); XT_MSUB_S(s1_im,h0_r,x0_i);
        }
        XT_SSIP(s0_re,pE,sizeof(float32_t));
        XT_SSIP(s0_im,pE,sizeof(float32_t));
        XT_SSIP(s1_re,pE,sizeof(float32_t));
        XT_SSIP(s1_im,pE,sizeof(float32_t));
    }
    /* update impluse response */
    b=XT_MUL_S(mu,XT_RECIP_S(norm));
    pH=(xtfloat*)(h+(M-2));
    for (m=0; m<M; m+=2)
    {
        xtfloat s0_re,s0_im,x0_r,x0_i,e0_r,e0_i;
        xtfloat s1_re,s1_im;
        xtfloat h0_re,h0_im;
        xtfloat h1_re,h1_im;
        s0_re=0.f;        s0_im=0.f;
        s1_re=0.f;        s1_im=0.f;
        pX=(const xtfloat*)(x+m);
        pE=(      xtfloat*)(e);
        XT_LSIP(x0_r,pX,sizeof(xtfloat));
        XT_LSIP(x0_i,pX,sizeof(xtfloat));
        __Pragma("loop_count min=8")
        for (n=0; n<N; n++)
        {
            XT_LSIP(e0_r,pE,sizeof(xtfloat));
            XT_LSIP(e0_i,pE,sizeof(xtfloat));
            XT_MADD_S(s0_re,x0_r,e0_r); XT_MADD_S(s0_re,x0_i,e0_i);
            XT_MSUB_S(s0_im,x0_r,e0_i); XT_MADD_S(s0_im,x0_i,e0_r);
            XT_LSIP(x0_r,pX,sizeof(xtfloat));
            XT_LSIP(x0_i,pX,sizeof(xtfloat));
            XT_MADD_S(s1_re,x0_r,e0_r); XT_MADD_S(s1_re,x0_i,e0_i);
            XT_MSUB_S(s1_im,x0_r,e0_i); XT_MADD_S(s1_im,x0_i,e0_r);
        }
        h1_re=XT_LSI (pH,0*sizeof(float32_t));
        h1_im=XT_LSI (pH,1*sizeof(float32_t));
        h0_re=XT_LSI (pH,2*sizeof(float32_t));
        h0_im=XT_LSI (pH,3*sizeof(float32_t));
        XT_MADD_S(h1_re,s1_re,b);
        XT_MADD_S(h1_im,s1_im,b);
        XT_MADD_S(h0_re,s0_re,b);
        XT_MADD_S(h0_im,s0_im,b);
        XT_SSI (h1_im,pH,1*sizeof(float32_t));
        XT_SSI (h0_re,pH,2*sizeof(float32_t));
        XT_SSI (h0_im,pH,3*sizeof(float32_t));
        XT_SSXP(h1_re,pH,-4*(int)sizeof(float32_t));
    }
} /* cxfir_blmsf() */
#endif
