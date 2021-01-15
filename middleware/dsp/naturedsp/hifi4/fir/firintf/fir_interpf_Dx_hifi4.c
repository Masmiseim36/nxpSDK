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
    Real interpolating FIR Filter
    C code optimized for HiFi4
    IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#include "fir_interpf_Dx.h"
#include "common_fpu.h"

#if (HAVE_VFPU)

/*-------------------------------------------------------------------------
    Dx interpolator:
    Input/output:
    delay[M*D] - circular delay line
    Input:
    p        - pointer to delay line
    x[N]     - input signal
    h[M*D]   - impulse response
    N        - number of output samples
    Output:
    z[N*D]     - output samples
    Restrictions:
    N>0, M>0
    N   - multiple of 8
    M   - multiple of 4
    D > 1
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
-------------------------------------------------------------------------*/
float32_t * fir_interpf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int D, int N)
{
        xtfloat   *restrict pZ;
        xtfloatx2 *restrict pX;
  const xtfloatx2 *restrict pH0;
  const xtfloatx2 *restrict pH1;
  const xtfloatx2 *restrict pDld;
        xtfloatx2 *restrict pDst;
  xtfloatx2 acc0a, acc0b, acc1a, acc1b;
  xtfloatx2 acc2a, acc2b, acc3a, acc3b;
  xtfloatx2 x01, x23;
  xtfloatx2 x32, x21, x10, x0_1, x_12, x_23, x_34;
  xtfloatx2 h01, h23;
  xtfloat   s0, s1, s2, s3;
  int n, j, m;

  NASSERT(x);
  NASSERT(z);
  NASSERT(N>0);
  NASSERT(M>0);

  /* set bounds of the delay line */
  WUR_AE_CBEGIN0((uintptr_t)(delay));
  WUR_AE_CEND0((uintptr_t)(delay + M));
  /* initialize pointers */
  pDst = (xtfloatx2 *)p;
  pX   = (xtfloatx2 *)x;

  /* Process by 4 input samples (4*D output samples) */
  __Pragma("loop_count min=1")
  for (n = 0; n < (N>>2); n++)
  {
      pH0  = (const xtfloatx2 *)(h);
      pH1  = pH0 + 1;
      pZ   = (xtfloat *)(z+(n<<2)*D);
      __Pragma("loop_count min=1")
      for (j = 0; j < D; j++)
      {
          acc0a = acc0b = acc1a = acc1b = (xtfloatx2)(0.0f);
          acc2a = acc2b = acc3a = acc3b = (xtfloatx2)(0.0f);
          /* preload input samples */
          x10 = XT_LSX2RI(pX, 0);
          x32 = XT_LSX2RI(pX, 2*sizeof(float32_t));
          x21 = XT_SEL32_LH_SX2(x32, x10);
          /* prepare for reverse loading of the delay line */
          pDld = pDst;
          AE_ADDCIRC32X2_XC(castxcc(ae_int32x2,pDld), -2*(int)sizeof(float32_t));

          /* kernel loop: compute by 4 taps for each sample */
          __Pragma("loop_count min=1")
          for (m = 0; m < (M>>2); m++)
          {
            /* load filter coefficients */
            XT_LSX2IP(h01, pH0, 4*sizeof(float32_t));
            XT_LSX2IP(h23, pH1, 4*sizeof(float32_t));
            /* load samples from the delay line */
            XT_LSX2RIC(x_12, pDld);
            XT_LSX2RIC(x_34, pDld);
            x0_1 = XT_SEL32_LH_SX2(x10, x_12);
            x_23 = XT_SEL32_LH_SX2(x_12, x_34);
            /* multiply */
            XT_MADD_SX2(acc0a, h01, x0_1);
            XT_MADD_SX2(acc1a, h01, x10);
            XT_MADD_SX2(acc2a, h01, x21);
            XT_MADD_SX2(acc3a, h01, x32);
            XT_MADD_SX2(acc0b, h23, x_23);
            XT_MADD_SX2(acc1b, h23, x_12);
            XT_MADD_SX2(acc2b, h23, x0_1);
            XT_MADD_SX2(acc3b, h23, x10 );
            /* shift whole delay line */
            x32 = x_12;
            x21 = x_23;
            x10 = x_34;
          }
          /* save computed samples */
          acc0a = acc0a + acc0b;
          acc1a = acc1a + acc1b;
          acc2a = acc2a + acc2b;
          acc3a = acc3a + acc3b;
          s0 = XT_RADD_SX2(acc0a);
          s1 = XT_RADD_SX2(acc1a);
          s2 = XT_RADD_SX2(acc2a);
          s3 = XT_RADD_SX2(acc3a);
          XT_SSXP(s0, pZ, D*sizeof(float32_t));
          XT_SSXP(s1, pZ, D*sizeof(float32_t));
          XT_SSXP(s2, pZ, D*sizeof(float32_t));
          XT_SSXP(s3, pZ, (-3*D+1)*(int)sizeof(float32_t));
      }
      /* update the delay line */
      XT_LSX2IP(x01, pX  , 2*sizeof(float32_t));
      XT_LSX2IP(x23, pX  , 2*sizeof(float32_t));
      XT_SSX2XC(x01, pDst, 2*sizeof(float32_t));
      XT_SSX2XC(x23, pDst, 2*sizeof(float32_t));
  }
  return (float32_t*)pDst;
} /* fir_interpf() */
#elif HAVE_FPU
// for scalar FPU
float32_t * fir_interpf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int D, int N)
{
#define P 4
  int n, m, j, _N;
  const xtfloat*  restrict pX = (const xtfloat*)x;
  const xtfloat*  restrict px = (const xtfloat*)x;
  const xtfloat* restrict pD  = (const xtfloat*)p;
  const xtfloat*   restrict pH = (const xtfloat*)h;
  xtfloat*          pZ = (xtfloat*)z;
  ae_valign ax, az, ad;
  NASSERT(x);
  NASSERT(z);
  M = M;
  NASSERT(N>0);
  NASSERT(M>0);
  NASSERT(D>1);
  NASSERT(M % 4 == 0);
  NASSERT(N % 8 == 0);
  _N = N&(~3);
  p = p;
  ax = AE_LA64_PP(pX);
  az = AE_ZALIGN64();
  WUR_AE_CBEGIN0((uintptr_t)(delay));
  WUR_AE_CEND0((uintptr_t)(delay + M));
  for (n = 0; n<_N; n +=P)
  {
    xtfloat x0, x1, x2, x3, H0;
    xtfloat A0, A1, A2, A3;
    xtfloat s0, s1, s2, s3;
    pH = (const xtfloat*)h;
    __Pragma("loop_count min=5")
      for (j = 0; j<D; j++)
      {
        xtfloat temp;
        pX = (xtfloat*)((uintptr_t)(x + n));
        ax = AE_LA64_PP(pX);
        XT_LSIP(x0, castxcc(xtfloat, pX), 4);
        XT_LSIP(x1, castxcc(xtfloat, pX), 4);
        XT_LSIP(x2, castxcc(xtfloat, pX), 4);
        XT_LSIP(x3, castxcc(xtfloat, pX), 4);
        A0 = 
        A1 = 
        A2 = 
        A3 = XT_CONST_S(0);
        ad = AE_LA64_PP(pD);
        XT_LSXC(temp, castxcc(xtfloat, pD), -4);

        __Pragma("loop_count min=1")
        for (m = 0; m<M; m ++)
        {
          XT_LSIP(H0, pH, 4);
          XT_MADD_S(A0, H0, x0);
          XT_MADD_S(A1, H0, x1);
          XT_MADD_S(A2, H0, x2);
          XT_MADD_S(A3, H0, x3);
          x3=x2;x2=x1;x1=x0;
          XT_LSXC(x0, castxcc(xtfloat, pD), -4);
         
        }
        XT_LSXC(temp, castxcc(xtfloat, pD), 4);
        pZ = (xtfloat*)((uintptr_t*)z + n*D + j);
        XT_SSXP(A0, pZ, 4 * D);
        XT_SSXP(A1, pZ, 4 * D);
        XT_SSXP(A2, pZ, 4 * D);
        XT_SSXP(A3, pZ, -3 * 4 * D + 4);
      }
    
      XT_LSIP(s0, castxcc(xtfloat, px), 4);
      XT_LSIP(s1, castxcc(xtfloat, px), 4);
      XT_LSIP(s2, castxcc(xtfloat, px), 4);
      XT_LSIP(s3, castxcc(xtfloat, px), 4);
    
      XT_SSXC(s0, castxcc(xtfloat, pD), 4);
      XT_SSXC(s1, castxcc(xtfloat, pD), 4);
      XT_SSXC(s2, castxcc(xtfloat, pD), 4);
      XT_SSXC(s3, castxcc(xtfloat, pD), 4);
    }
  p = (float32_t*)pD;
  return (float32_t*)pD;
} 
#endif
