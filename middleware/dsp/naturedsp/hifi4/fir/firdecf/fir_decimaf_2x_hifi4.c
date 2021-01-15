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
    NatureDSP Signal Processing Library. FIR part
    Real FIR Filter with decimation (2x)
    C code optimized for HiFi4
    IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Filters and transformations */
#include "NatureDSP_Signal_fir.h"
#include "fir_decimaf_2x.h"
#include "common_fpu.h"

#if (HAVE_VFPU)

/*-------------------------------------------------------------------------
    2x decimator:
    Input/output:
    delay[M] - circular delay line
    Input:
    p        - pointer to delay line
    x[N*2]   - input signal
    h[M]     - impulse response
    N        - number of output samples
    Output:
    z[N]     - output samples
    Restrictions:
    N>0, M>0
    M multiple of 2
    N multiple of 8
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
-------------------------------------------------------------------------*/
float32_t * fir_decimaf_2x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int N)
{
          xtfloatx2 *restrict pz;
    const xtfloatx2 *restrict ph;
          xtfloatx2 *restrict px;
          xtfloatx2 * pp;
    const xtfloatx2 * pd;
    xtfloatx2 A0, A2, A4, A6, A0_, A2_, A4_, A6_;
    xtfloatx2 X01, X23, X45, X67,
              X65, X43, X21, X10, X0_1, X_12, X_23, X_34;
    xtfloatx2 H01, H23, T0, T1, T2, T3, A02, A46;
    ae_valign al_z;
    int n,m;

    NASSERT(x);
    NASSERT(z);
    NASSERT(h);
    NASSERT(delay);
    NASSERT(p);
    NASSERT_ALIGN(delay,8);
    NASSERT(N>0);
    NASSERT(M>0);
    NASSERT(M%4==0);

    /* set circular buffer boundaries */
    WUR_AE_CBEGIN0( (uintptr_t)( delay + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( delay + M ) );
    /* initialize pointers */
    pp = (xtfloatx2 *)(p);
    pz = (xtfloatx2 *)(z);
    px = (xtfloatx2 *)(x);
    al_z = AE_ZALIGN64();

    /* process by 8 input samples (4 output samples) */
    __Pragma("loop_count min=1")
    for (n = 0; n < (N>>2); n++)
    {
        A0 = A2 = A4 = A6 = (xtfloatx2)(0.0f);
        A0_= A2_= A4_= A6_= (xtfloatx2)(0.0f);
        /* load input samples */
        X01 = XT_LSX2I(px, 0*sizeof(float32_t));
        X23 = XT_LSX2I(px, 2*sizeof(float32_t));
        X45 = XT_LSX2I(px, 4*sizeof(float32_t));
        X67 = XT_LSX2I(px, 6*sizeof(float32_t));
        X65 = XT_SEL32_HL_SX2(X67, X45);
        X43 = XT_SEL32_HL_SX2(X45, X23);
        X21 = XT_SEL32_HL_SX2(X23, X01);
        X10 = XT_SEL32_LH_SX2(X01, X01);

        /* prepare for reading delay line */
        pd = pp;
        AE_ADDCIRC32X2_XC(castxcc(ae_int32x2,pd), -2*(int)sizeof(float32_t));
        ph = (const xtfloatx2*)h;

        /* main loop: process by 4 taps for 4 samples */
        __Pragma("loop_count min=1")
        for (m = 0; m < (M>>2); m++)
        {
            /* load filter coefficients */
            XT_LSX2IP(H01, ph, 2*sizeof(float32_t));
            XT_LSX2IP(H23, ph, 2*sizeof(float32_t));
            /* load samples from the delay line */
            XT_LSX2RIC(X_12, pd);
            XT_LSX2RIC(X_34, pd);
            X0_1 = XT_SEL32_LH_SX2(X10, X_12);
            X_23 = XT_SEL32_LH_SX2(X_12, X_34);
            /* SIMD multiply */
            XT_MADD_SX2(A0, H01, X0_1);
            XT_MADD_SX2(A2, H01, X21);
            XT_MADD_SX2(A4, H01, X43);
            XT_MADD_SX2(A6, H01, X65);
            XT_MADD_SX2(A0_, H23, X_23);
            XT_MADD_SX2(A2_, H23, X0_1);
            XT_MADD_SX2(A4_, H23, X21);
            XT_MADD_SX2(A6_, H23, X43);
            /* shift the delay line */
            X65 = X21;
            X43 = X0_1;
            X21 = X_23;
            X10 = X_34;
        }
        A0 = A0 + A0_;
        A2 = A2 + A2_;
        A4 = A4 + A4_;
        A6 = A6 + A6_;
        T0=XT_SEL32_HL_SX2(A0,A2);
        T1=XT_SEL32_LH_SX2(A0,A2);
        T2=XT_SEL32_HL_SX2(A4,A6);
        T3=XT_SEL32_LH_SX2(A4,A6);
        A02 = T0 + T1;
        A46 = T2 + T3;
        /* save computed samples */
        XT_SASX2IP(A02, al_z, pz);
        XT_SASX2IP(A46, al_z, pz);
        /* update delay line */
        XT_LSX2IP(X01, px, 2*sizeof(float32_t));
        XT_LSX2IP(X23, px, 2*sizeof(float32_t));
        XT_LSX2IP(X45, px, 2*sizeof(float32_t));
        XT_LSX2IP(X67, px, 2*sizeof(float32_t));
        XT_SSX2XC(X01, pp, 2*sizeof(float32_t));
        XT_SSX2XC(X23, pp, 2*sizeof(float32_t));
        XT_SSX2XC(X45, pp, 2*sizeof(float32_t));
        XT_SSX2XC(X67, pp, 2*sizeof(float32_t));
    }
    XT_SASX2POSFP(al_z, pz);

    return (float32_t*)pp;
}
#elif (HAVE_FPU)
// for scalar FPU
float32_t * fir_decimaf_2x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int N)
{
  xtfloat* restrict pZ;
  const xtfloat *restrict pH;
  const xtfloat *restrict pX;
  xtfloat * pp;
  xtfloat* restrict pD;
  xtfloat x0, x1, x2, x3;
  xtfloat s0, s1, s2, s3;
  xtfloat h0, h1;
  int n, m;
  NASSERT(x);
  NASSERT(z);
  NASSERT(h);
  NASSERT(delay);
  NASSERT(p);
  NASSERT_ALIGN(delay, 8);
  NASSERT(N>0);
  NASSERT(M>0);
  NASSERT(M % 2 == 0);
  NASSERT(N % 8 == 0);
  pD = (xtfloat*)p;
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0((uintptr_t)(delay + 0));
  WUR_AE_CEND0((uintptr_t)(delay + M));

  pp = (xtfloat*)p;
  pZ = (xtfloat*)z;
  pX = (const xtfloat*)x;
  
  /* process by 2 input samples */
  for (n=0; n<(N>>1); n++)
  {
    xtfloat A0, A1, A2, A3, xx;
    pH = (const xtfloat*)h;
    pD = (xtfloat*)pp;
    A0=A1=A2=A3=XT_CONST_S(0);
    XT_LSIP(x0, pX, 4);
    XT_LSIP(x1, pX, 4);
    XT_LSIP(x2, pX, 4);
    XT_LSIP(x3, pX, 4);
    s0 = x0;
    s1 = x1;
    s2 = x2;
    s3 = x3;
    { xtfloat dummy;  XT_LSXC(dummy, pD, -4); xx = dummy;}
    { xtfloat dummy;  XT_LSXC(dummy, pD, -4); xx = dummy; }
    for (m=0; m<M; m+=2)
    {
      h0 = pH[m+0];
      h1 = pH[m+1];

      XT_MADD_S(A0, x0, h0);
      XT_MADD_S(A1, x2, h0);
      XT_MADD_S(A2, xx, h1);
      XT_MADD_S(A3, x1, h1);

      x3 = x2;
      x2 = x0;
      x1 = xx;
      XT_LSXC(x0, pD, -4);
      XT_LSXC(xx, pD, -4);
    }
    A0 = XT_ADD_S(A0, A2);
    A1 = XT_ADD_S(A1, A3);
    { xtfloat dummy;  XT_LSXC(dummy, pD, 4); }
    XT_SSXC(s0, pp, 4);
    XT_SSXC(s1, pp, 4);
    XT_SSXC(s2, pp, 4);
    XT_SSXC(s3, pp, 4);

    XT_SSIP(A0, pZ, 4);
    XT_SSIP(A1, pZ, 4);
  }
  return (float32_t*)pp;
}

#endif
