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
/*
    Real interpolating FIR Filter
    C code optimized for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
/* Filters and transformations */
#include "NatureDSP_Signal.h"
#if (XCHAL_HAVE_FUSION_FP)
#include "fir_interpf_2x.h"

/*
    2x interpolator:
    Input/output:
    delay[M*2] - circular delay line
    Input:
    p        - pointer to delay line
    x[N]     - input signal
    h[M*2]   - impulse response
    N        - number of output samples
    Output:
    z[N*2]     - output samples
    Restrictions:
    N>0, M>0
    N   - multiple of 8
    M   - multiple of 4
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
*/
float32_t * fir_interpf_2x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int N)
{
  int n, m;
  const xtfloatx2*  restrict pX = (const xtfloatx2*)x;
  const xtfloatx2*  restrict px = (const xtfloatx2*)x;
  const xtfloatx2* restrict pD = (const xtfloatx2*)p;
  const xtfloat*   restrict pH = (const xtfloat*)h;
  xtfloat*          pZ = (xtfloat*)z;
  NASSERT(x);
  NASSERT(z);
  NASSERT(N>0);
  NASSERT(M>0);
  NASSERT(M % 4 == 0);
  NASSERT(N % 8 == 0);
  WUR_AE_CBEGIN0((uintptr_t)(delay));
  WUR_AE_CEND0((uintptr_t)(delay + M));
  for (n = 0; n<N; n +=2)
  {
    xtfloat x0, x1;
    xtfloat H0, H1;
    xtfloat A0, A1, A2, A3;
    xtfloat s0, s1;
    pH = (const xtfloat*)h;

    {
      xtfloat temp;
      XT_LSIP(x0, castxcc(xtfloat, pX), 4);
      XT_LSIP(x1, castxcc(xtfloat, pX), 4);
      A0 = A1 = XT_CONST_S(0);
      A2 = A3 = XT_CONST_S(0);
      XT_LSXC(temp, castxcc(xtfloat, pD), -4);
      __Pragma("loop_count min=1")
        for (m = 0; m<M; m ++)
        {
          H0 = pH[m + 0*M];
          H1 = pH[m + 1*M];
          XT_MADD_S(A0, H0, x0);
          XT_MADD_S(A1, H0, x1);
          XT_MADD_S(A2, H1, x0);
          XT_MADD_S(A3, H1, x1);
          x1 = x0;
          XT_LSXC(x0, castxcc(xtfloat, pD), -4);
        }

      XT_LSXC(temp, castxcc(xtfloat, pD), 4);
      XT_SSXP(A0, pZ, 4 );
      XT_SSXP(A2, pZ, 4);
      XT_SSXP(A1, pZ, 4);
      XT_SSXP(A3, pZ, 4);
    }
    XT_LSIP(s0, castxcc(xtfloat, px), 4);
    XT_LSIP(s1, castxcc(xtfloat, px), 4);
    XT_SSXC(s0, castxcc(xtfloat, pD), 4);
    XT_SSXC(s1, castxcc(xtfloat, pD), 4);
  }
  return (float32_t*)pD;
} 
#endif
