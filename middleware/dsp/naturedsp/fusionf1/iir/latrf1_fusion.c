/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
*/

#include "latrf_common.h"

#if (XCHAL_HAVE_FUSION_FP)

#define MOVTSX2(inout,in,b)                     \
{                                               \
    ae_int32x2 t0,t1;                           \
    t0=XT_AE_MOVINT32X2_FROMXTFLOATX2(inout);   \
    t1=XT_AE_MOVINT32X2_FROMXTFLOATX2(in);      \
    AE_MOVT32X2(t0,t1,b);                       \
    inout=XT_AE_MOVXTFLOATX2_FROMINT32X2(t0);   \
}

void latrf_process1(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N )
#if 1
{
  float32_t * restrict  delLine;
  float32_t t0,s0,c0;
  float32_t scale;
  int n;

  delLine = latr->delayLine;
  c0 = latr->coef[0];
  scale   = latr->scale;
  s0 = delLine[0];
#if 0
  for ( n=0; n<N; n++ )
  {
    t0 = x[n]*scale;  
    t0 -= s0*c0;
    r[n] = s0 = t0;
  }
  delLine[0] = s0;
#else
      for ( n=0; n<N; n++ )
    {
        XT_LSIP(t0,castxcc(xtfloat,x),sizeof(float32_t));
        t0=XT_MUL_S(t0,scale);
        XT_MSUB_S(t0,s0,c0);
        // XT_MSUB_S(t0,d1,c1);
        // XT_MADD_S(d0,t0,c0);
       // d1=d0;
        s0 = t0;
        XT_SSIP(t0,castxcc(xtfloat,r),sizeof(float32_t));
    }
    delLine[0] = s0;
#endif

}


#else
{
    const xtfloatx2 * restrict pX;
          xtfloatx2 * restrict pR;
    ae_valign ux,ur;
   xtfloatx2 t01,s01,_0,tt,cc0,sscale;

  float32_t * restrict  delLine;
  float32_t c0;
  float32_t scale;
  int n;

  _0=XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVDA32X2(0,0));

  delLine = latr->delayLine;
  c0 = latr->coef[0];
  scale   = latr->scale;
  sscale=(xtfloatx2)(xtfloat)scale;

  pX=(const xtfloatx2*)x;
  pR=(      xtfloatx2*)r;
  ux =AE_LA64_PP(pX);
  ur =AE_ZALIGN64();
  cc0=(xtfloat)c0;
  s01=XT_xtfloat_loadi((const xtfloat*)delLine,0);
  for ( n=0; n<(N & ~1); n+=2 )
  {
    XT_LASX2IP(t01,ux,pX);
    t01=XT_MUL_SX2(t01,sscale);
    s01=XT_SEL32_LH_SX2(s01,_0);
    tt=t01; 
    XT_MSUBN_SX2(tt,s01,cc0);
    s01=XT_SEL32_HH_SX2(s01,tt);
    XT_MSUBN_SX2(t01,s01,cc0);
    s01=t01;
    XT_SASX2IP(t01,ur,pR);
  }
  XT_xtfloat_storei(t01,(xtfloat*)delLine,0);
  AE_SA64POS_FP(ur,pR);
  if(N&1)
  {
    xtfloat t0,s0,c0=(xtfloat)cc0,sscale=(xtfloat)scale;
    t0=XT_xtfloat_loadi((const xtfloat*)pX,0);
    t0=XT_MUL_S(t0,sscale);
    s0=XT_SEL32_LL_SX2(s01,s01);
    XT_MSUBN_S(t0,s0,c0);
    XT_xtfloat_storei(t0,(xtfloat*)pR,0);
    XT_xtfloat_storei(t0,(xtfloat*)delLine,0);
  }
}
#endif
#endif
