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
    Real FIR Filter with decimation
    C code optimized for Fusion
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#if (XCHAL_HAVE_FUSION_FP)
#include "fir_decimaf_Dx.h"

/*
    universal decimator:
    Input/output:
    delay[M] - circular delay line
    Input:
    p        - pointer to delay line
    x[N*D]   - input signal
    h[M]     - impulse response
    D          decimation factor
    N        - number of output samples
    Output:
    z[N]     - output samples
    Restrictions:
    N>0, M>0
    D>4
    M multiple of 2
    N multiple of 8
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
*/
float32_t * fir_decimaf_Dx(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x,  int M, int D, int N)
{
  const xtfloat   *px;
  xtfloat   *pz;
  const xtfloatx2 *ph;
  xtfloat   *pp;
  const xtfloat *pD;
  xtfloat A0, A1, A2, A3, XX;
  xtfloatx2 H0, H1;
  ae_valign ah,ad;

  int m,n,d;
  xtfloatx2 p0, p1, p2;
  NASSERT(N>0 && M>0);
  NASSERT(M%2==0);
  NASSERT(N%8==0);
  NASSERT(D>4);
  NASSERT_ALIGN(delay,8);
  WUR_AE_CBEGIN0( (uintptr_t)( delay + 0 ) );
  WUR_AE_CEND0  ( (uintptr_t)( delay + M ) );
  px=(const xtfloat*)x;
  pz=(      xtfloat*)z;
  pp=(xtfloat*)p;
  for (n=0; n<N; n++)
  {
    ph=(const xtfloatx2*)h;
    ah=AE_LA64_PP(ph);

    A0=A1=A2=A3=XT_CONST_S(0);
    pD=(const xtfloat*)pp;
    XX=XT_LSI(px,0);
    XT_LASX2NEGPC(ad, castxcc(xtfloatx2,pD));
    XT_LASX2RIC(p0, ad, castxcc(xtfloatx2, pD));
    XT_LASX2RIC(p1, ad, castxcc(xtfloatx2, pD));
    p2 = XX;
    p0 = XT_SEL32_HL_SX2(p2, p0);
    for (m=0; m<(M&~3); m+=4)
    {
      XT_LASX2IP(H0, ah, ph);
      XT_LASX2IP(H1, ah, ph);

      XT_MADD_LHH_S(A0, H0, p0);
      XT_MADD_LLL_S(A1, H0, p0);
      XT_MADD_LHH_S(A2, H1, p1);
      XT_MADD_LLL_S(A3, H1, p1);
      XT_LASX2RIC(p0, ad, castxcc(xtfloatx2, pD));
      XT_LASX2RIC(p1, ad, castxcc(xtfloatx2, pD));
    }
    if(M&2)
    {
      XT_LASX2IP(H0, ah, ph); 
      XT_MADD_LHH_S(A0, H0, p0);
      XT_MADD_LLL_S(A1, H0, p0);
    }
    A0 = XT_ADD_S(A0, A2);
    A1 = XT_ADD_S(A1, A3);
    A0 = XT_ADD_S(A0, A1);
    __Pragma("loop_count min=4");
    for(d=0; d<D; d++) 
    {
      xtfloat t;
      XT_LSIP(t,px,sizeof(float32_t));
      XT_SSXC(t,pp,sizeof(float32_t));

    }
    XT_SSIP(A0,pz,sizeof(float32_t));
  }
  return (float32_t*)pp;
}
#endif

