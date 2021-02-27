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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
*/

#include "latrf_common.h"


void latrf_process3(struct tag_latrf_t *latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N )
#if 0
{
        float32_t * restrict  delLine;
  const float32_t * restrict  coef;

  float32_t t0,t1,t2,t3,d0,d1,d2;
  float32_t scale;
  const xtfloatx2 *pX=(const xtfloatx2 *)x;
  ae_valign ux=AE_LA64_PP(pX);
  int n;

  NASSERT(latr->M==3);
  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;

  d0=delLine[0];
  d1=delLine[1];
  d2=delLine[2];
  for ( n=0; n<(N&~3); n+=4 )
  {
      xtfloatx2 x01,x23,sscale;
      sscale=(xtfloat)scale;
      XT_LASX2IP(x01,ux,pX);
      XT_LASX2IP(x23,ux,pX);
      x01=XT_MUL_SX2(x01,sscale);
      x23=XT_MUL_SX2(x23,sscale);
      t1=(xtfloat)XT_SEL32_LL_SX2(x01,x01);
      t1=(xtfloat)x01;
      t0=(xtfloat)XT_SEL32_HH_SX2(x01,x01);
      t3=(xtfloat)x23;
      t2=(xtfloat)XT_SEL32_HH_SX2(x23,x23);
        /*t0 = x[n+0]*scale;
        t1 = x[n+1]*scale;
        t2 = x[n+2]*scale;
        t3 = x[n+3]*scale;*/
        
        t0 -=     d2 * coef[2];
        t0 -=     d1 * coef[1];
        d2 = d1 + t0 * coef[1];
        t1 -=     d2 * coef[2];
        t0 -=     d0 * coef[0];
        r[n+0] = t0;
        d1 = d0 + t0 * coef[0];
        t1 -=     d1 * coef[1];
        d2 = d1 + t1 * coef[1];
        t1 -=     t0 * coef[0];
        r[n+1] = t1;
        t0 = t0 + t1 * coef[0];

        t2 -=     d2 * coef[2];
        t2 -=     t0 * coef[1];
        d2 = t0 + t2 * coef[1];
        t3 -=     d2 * coef[2];
        t2 -=     t1 * coef[0];
        r[n+2] = t2;
        t1 = t1 + t2 * coef[0];
        t3 -=     t1 * coef[1];
        d2 = t1 + t3 * coef[1];
        t3 -=     t2 * coef[0];
        d1 = t2 + t3 * coef[0];
        d0 = t3;

        r[n+3] = t3;
  }
  delLine[0]=d0;
  delLine[1]=d1;
  delLine[2]=d2;

  if (N&3)
  {
        N&=3;x+=n;r+=n;
        for ( n=0; n<N; n++)
        {
            t0 = x[n]*scale;
            t0 -=          delLine[2]      * coef[2];
            t0 -=          delLine[1]      * coef[1];
            delLine[1+1] = delLine[1] + t0 * coef[1];
            t0 -=          delLine[0]      * coef[0];
            delLine[0+1] = delLine[0] + t0 * coef[0];
            delLine[0+0] = t0;
            r[n] = t0;
        }
  }
} // latrf3_process()
#else
{
    float32_t d0,d1,d2,c0,c1,c2;
          float32_t * restrict delLine;
    const float32_t * restrict coef;

    float32_t t0;
    float32_t scale;

    int n;

    NASSERT(latr->M==3);
    delLine = latr->delayLine;
    coef    = latr->coef;
    scale   = latr->scale;
    d0=delLine[0];
    d1=delLine[1];
    d2=delLine[2];
    c0=coef[0];
    c1=coef[1];
    c2=coef[2];

    for ( n=0; n<N; n++ )
    {
        t0  = x[n]*scale;
        t0 -= d2 * c2;
        t0 -= d1 * c1;
        d2  = d1 + t0 * c1 ;
        t0 -= d0 * c0 ;
        d1  = d0 + t0 * c0 ;
        d0 =      t0;
        r[n] = t0;
    }
    delLine[0]=d0;
    delLine[1]=d1;
    delLine[2]=d2;
} // latrf3_process()
#endif
