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
    Real FIR Filter with decimation (4x)
    C code optimized for HiFi4
    IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#include "NatureDSP_Signal_fir.h"
#include "fir_decimaf_4x.h"
#include "common_fpu.h"

#if (HAVE_VFPU)
/*-------------------------------------------------------------------------
    4x decimator:
    Input/output:
    delay[M] - circular delay line
    Input:
    p        - pointer to delay line
    x[N*4]   - input signal
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
float32_t * fir_decimaf_4x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int N)
{
          xtfloatx2 *restrict pz;
          xtfloatx2 *restrict px;
    const xtfloatx2 *restrict ph0;
    const xtfloatx2 *restrict ph1;
          xtfloatx2 *pp;
    const xtfloatx2 *pd;
    xtfloatx2 A0, A1, A2, A3, B0, B1, B2, B3,
              X43, X21, X10, X0_1, X_12, X_23, X_34, X_45, X_56, X_67, X_78,
              H01, H23, H45, H67;
    xtfloatx2 X01, X23, X45, X67;
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

    pz = (xtfloatx2 *)(z);
    pp = (xtfloatx2 *)(p);
    px = (xtfloatx2 *)(x);
    al_z = AE_ZALIGN64();
    /* process by 8 input samples (2 output samples) */
    for (n = 0; n < (N>>1); n++)
    {
        A0 = A1 = A2 = A3 = (xtfloatx2)(0.0f);
        B0 = B1 = B2 = B3 = (xtfloatx2)(0.0f);

        ph0 = (const xtfloatx2*)h;
        ph1 = ph0 + 1;
        /* load input samples */
        X01 = XT_LSX2I(px, 0*sizeof(float32_t));
        X23 = XT_LSX2I(px, 2*sizeof(float32_t));
        X45 = XT_LSX2I(px, 4*sizeof(float32_t));
        X43 = XT_SEL32_HL_SX2(X45, X23);
        X21 = XT_SEL32_HL_SX2(X23, X01);
        X10 = XT_SEL32_LH_SX2(X01, X01);
        /* prepare for reading the delay line */
        pd = (const xtfloatx2*)(((const float32_t*)pp));
        AE_ADDCIRC32X2_XC(castxcc(ae_int32x2,pd), -2*(int)sizeof(float32_t));

        /* process by 8 taps for 2 samples */
        for (m = 0; m < (M>>3); m++)
        {
            /* load filter coefficients */
            XT_LSX2IP(H01, ph0, 4*sizeof(float32_t));
            XT_LSX2IP(H23, ph1, 4*sizeof(float32_t));
            XT_LSX2IP(H45, ph0, 4*sizeof(float32_t));
            XT_LSX2IP(H67, ph1, 4*sizeof(float32_t));
            /* load the delay line */
            XT_LSX2RIC(X_12, pd);
            XT_LSX2RIC(X_34, pd);
            XT_LSX2RIC(X_56, pd);
            XT_LSX2RIC(X_78, pd);
            X0_1 = XT_SEL32_LH_SX2(X10, X_12);
            X_23 = XT_SEL32_LH_SX2(X_12, X_34);
            X_45 = XT_SEL32_LH_SX2(X_34, X_56);
            X_67 = XT_SEL32_LH_SX2(X_56, X_78);
            /* perform multiply */
            XT_MADD_SX2(A0, H01, X0_1);
            XT_MADD_SX2(A1, H23, X_23);
            XT_MADD_SX2(A2, H45, X_45);
            XT_MADD_SX2(A3, H67, X_67);
            XT_MADD_SX2(B0, H01, X43);
            XT_MADD_SX2(B1, H23, X21);
            XT_MADD_SX2(B2, H45, X0_1);
            XT_MADD_SX2(B3, H67, X_23);
            /* shift the delay line */
            X43 = X_45;
            X21 = X_67;
            X10 = X_78;
        }
        /* process last 4 taps if M is not a multiple of 8 */
        if (M & 4)
        {
            XT_LSX2RIC(X_12, pd);
            XT_LSX2RIC(X_34, pd);
            X0_1 = XT_SEL32_LH_SX2(X10, X_12);
            X_23 = XT_SEL32_LH_SX2(X_12, X_34);
            H01 = XT_LSX2I(ph0, 0);
            H23 = XT_LSX2I(ph1, 0);
            XT_MADD_SX2(A0, H01, X0_1);
            XT_MADD_SX2(A1, H23, X_23);
            XT_MADD_SX2(B0, H01, X43);
            XT_MADD_SX2(B1, H23, X21);
        }
        /* save computed samples */
        A0 = A0 + A1;
        A2 = A2 + A3;
        A0 = A0 + A2;
        B0 = B0 + B1;
        B2 = B2 + B3;
        B0 = B0 + B2;
        A1 = XT_SEL32_HL_SX2(A0, B0);
        B1 = XT_SEL32_LH_SX2(A0, B0);
        A0 = A1 + B1;

        XT_SASX2IP(A0, al_z, pz);
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
float32_t * fir_decimaf_4x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int N)
{
  xtfloat* restrict pZ;
  const xtfloat *restrict pH;
  const xtfloat *restrict pX;
  xtfloat * pp;
  xtfloat* restrict pD;
  xtfloat x0, x1, x2, x3, x4, x5, x6, x7;
  xtfloat s0, s1, s2, s3, s4, s5, s6, s7;
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
  NASSERT(M%2==0);
  NASSERT(N%8==0);
  pD = (xtfloat*)p;
  /* set circular buffer boundaries */
  WUR_AE_CBEGIN0((uintptr_t)(delay + 0));
  WUR_AE_CEND0((uintptr_t)(delay + M));

  pp = (xtfloat*)p;
  pZ = (xtfloat*)z;
  pX = (const xtfloat*)x;

  /* process by 2 input samples */
  for (n = 0; n<(N >> 1); n++)
  {
    xtfloat A0, A1, A2, A3, xx;
    pH = (const xtfloat*)h;
    pD = (xtfloat*)pp;
    A0 = A1 = A2 = A3 = XT_CONST_S(0);
    XT_LSIP(x0, pX, 4);
    XT_LSIP(x1, pX, 4);
    XT_LSIP(x2, pX, 4);
    XT_LSIP(x3, pX, 4);
    XT_LSIP(x4, pX, 4);
    XT_LSIP(x5, pX, 4);
    XT_LSIP(x6, pX, 4);
    XT_LSIP(x7, pX, 4);
    s0 = x0;
    s1 = x1;
    s2 = x2;
    s3 = x3;
    s4 = x4;
    s5 = x5;
    s6 = x6;
    s7 = x7;
    { xtfloat dummy;  XT_LSXC(dummy, pD, -4); }
    { xtfloat dummy;  XT_LSXC(dummy, pD, -4); xx = dummy; }
    for (m = 0; m<M; m += 2)
    {
      h0 = pH[m + 0];
      h1 = pH[m + 1];

      XT_MADD_S(A0, x0, h0);
      XT_MADD_S(A1, x4, h0);
      XT_MADD_S(A2, xx, h1);
      XT_MADD_S(A3, x3, h1);

      x4 = x2;
      x3 = x1;
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
    XT_SSXC(s4, pp, 4);
    XT_SSXC(s5, pp, 4);
    XT_SSXC(s6, pp, 4);
    XT_SSXC(s7, pp, 4);

    XT_SSIP(A0, pZ, 4);
    XT_SSIP(A1, pZ, 4);
  }
  return (float32_t*)pp; 
}
#endif
