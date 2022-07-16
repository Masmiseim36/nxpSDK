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

void latrf_process8(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N )
{
        float32_t *  delLine;
  const float32_t * restrict  coef;

  float32_t t0;
  float32_t scale;

  int n;

  NASSERT(latr->M==8);
  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
#if 0
  for ( n=0; n<(N&~3); n+=4 )
  {
    float32_t t1,t2,t3;
    //Scale the input sample.
    t0 = x[n+0]*scale;
    t1 = x[n+1]*scale;
    t2 = x[n+2]*scale;
    t3 = x[n+3]*scale;

    t0 -=          delLine[8-1]       * coef[8-1];
    t0 -=          delLine[8-2]       * coef[8-2];
    delLine[8-1] = delLine[8-2] +  t0 * coef[8-2];
    t0 -=          delLine[8-3]       * coef[8-3];
    delLine[8-2] = delLine[8-3] +  t0 * coef[8-3];
    t0 -=          delLine[8-4]       * coef[8-4];
    delLine[8-3] = delLine[8-4] +  t0 * coef[8-4];
    t1 -=          delLine[8-1]       * coef[8-1];
    t1 -=          delLine[8-2]       * coef[8-2];
    delLine[8-1] = delLine[8-2] +  t1 * coef[8-2];
    t1 -=          delLine[8-3]       * coef[8-3];
    delLine[8-2] = delLine[8-3] +  t1 * coef[8-3];
    t2 -=          delLine[8-1]       * coef[8-1];
    t2 -=          delLine[8-2]       * coef[8-2];
    delLine[8-1] = delLine[8-2] +  t2 * coef[8-2];
    t3 -=          delLine[8-1]       * coef[8-1];

    t0 -=          delLine[3+0]       * coef[3+0];
    delLine[3+1] = delLine[3+0] +  t0 * coef[3+0];
    t1 -=          delLine[3+1]       * coef[3+1];
    delLine[3+2] = delLine[3+1] +  t1 * coef[3+1];
    t2 -=          delLine[3+2]       * coef[3+2];
    delLine[3+3] = delLine[3+2] +  t2 * coef[3+2];
    t3 -=          delLine[3+3]       * coef[3+3];
    delLine[3+4] = delLine[3+3] +  t3 * coef[3+3];
    t0 -=          delLine[2+0]       * coef[2+0];
    delLine[2+1] = delLine[2+0] +  t0 * coef[2+0];
    t1 -=          delLine[2+1]       * coef[2+1];
    delLine[2+2] = delLine[2+1] +  t1 * coef[2+1];
    t2 -=          delLine[2+2]       * coef[2+2];
    delLine[2+3] = delLine[2+2] +  t2 * coef[2+2];
    t3 -=          delLine[2+3]       * coef[2+3];
    delLine[2+4] = delLine[2+3] +  t3 * coef[2+3];
    t0 -=          delLine[1+0]       * coef[1+0];
    delLine[1+1] = delLine[1+0] +  t0 * coef[1+0];
    t1 -=          delLine[1+1]       * coef[1+1];
    delLine[1+2] = delLine[1+1] +  t1 * coef[1+1];
    t2 -=          delLine[1+2]       * coef[1+2];
    delLine[1+3] = delLine[1+2] +  t2 * coef[1+2];
    t3 -=          delLine[1+3]       * coef[1+3];
    delLine[1+4] = delLine[1+3] +  t3 * coef[1+3];
    t0 -=          delLine[0+0]       * coef[0+0];
    delLine[0+1] = delLine[0+0] +  t0 * coef[0+0];
    t1 -=          delLine[0+1]       * coef[0+1];
    delLine[0+2] = delLine[0+1] +  t1 * coef[0+1];
    t2 -=          delLine[0+2]       * coef[0+2];
    delLine[0+3] = delLine[0+2] +  t2 * coef[0+2];
    t3 -=          delLine[0+3]       * coef[0+3];
    delLine[0+4] = delLine[0+3] +  t3 * coef[0+3];
    r[n+0] = t0;
    t1 -=        t0       * coef[0];
    r[n+1] = t1;
    t0 =         t0 +  t1 * coef[0];
    t2 -=        t0       * coef[1];
    t0 =         t0 +  t2 * coef[1];
    t2 -=        t1       * coef[0];
    r[n+2] = t2;
    t1 =         t1 +  t2 * coef[0];
    t3 -=        t0       * coef[2];
    delLine[3] = t0 +  t3 * coef[2];
    t3 -=        t1       * coef[1];
    delLine[2] = t1 +  t3 * coef[1];
    t3 -=        t2       * coef[0];
    r[n+3] = t3;
    delLine[1] = t2 +  t3 * coef[0];
    delLine[0] = t3;

  }
  if (N&3)
  {
      for ( ; n<N; n++ )
      {
        t0 = x[n]*scale;
        t0 -= delLine[8-1]*coef[8-1];
        t0 -=         delLine[6]       * coef[6];
        delLine[6+1]= delLine[6] +  t0 * coef[6];
        t0 -=         delLine[5]       * coef[5];
        delLine[5+1]= delLine[5] +  t0 * coef[5];
        t0 -=         delLine[4]       * coef[4];
        delLine[4+1]= delLine[4] +  t0 * coef[4];
        t0 -=         delLine[3]       * coef[3];
        delLine[3+1]= delLine[3] +  t0 * coef[3];
        t0 -=         delLine[2]       * coef[2];
        delLine[2+1]= delLine[2] +  t0 * coef[2];
        t0 -=         delLine[1]       * coef[1];
        delLine[1+1]= delLine[1] +  t0 * coef[1];
        t0 -=         delLine[0]       * coef[0];
        delLine[0+1]= delLine[0] +  t0 * coef[0];
        delLine[0] = t0;
        r[n] = t0;
      }
  }
#else
    {
        xtfloat d0,d1,d2,d3,d4,d5,d6,d7;
        xtfloat c0,c1,c2,c3,c4,c5,c6,c7;
        d0=delLine[0];
        d1=delLine[1];
        d2=delLine[2];
        d3=delLine[3];
        d4=delLine[4];
        d5=delLine[5];
        d6=delLine[6];
        d7=delLine[7];
        c0=coef[0];
        c1=coef[1];
        c2=coef[2];
        c3=coef[3];
        c4=coef[4];
        c5=coef[5];
        c6=coef[6];
        c7=coef[7];
        __Pragma("loop_count min=1");
        for ( n=0; n<N; n++ )
        {
            XT_LSIP(t0,castxcc(xtfloat,x),sizeof(float32_t));
            t0=XT_MUL_S(t0,scale);
            XT_MSUB_S(t0,d7,c7);
            XT_MSUB_S(t0,d6,c6);
            XT_MADD_S(d6,t0,c6); d7=d6;
            XT_MSUB_S(t0,d5,c5);
            XT_MADD_S(d5,t0,c5); d6=d5;
            XT_MSUB_S(t0,d4,c4);
            XT_MADD_S(d4,t0,c4); d5=d4;
            XT_MSUB_S(t0,d3,c3);
            XT_MADD_S(d3,t0,c3); d4=d3;
            XT_MSUB_S(t0,d2,c2);
            XT_MADD_S(d2,t0,c2); d3=d2;
            XT_MSUB_S(t0,d1,c1);
            XT_MADD_S(d1,t0,c1); d2=d1;
            XT_MSUB_S(t0,d0,c0);
            XT_MADD_S(d0,t0,c0); d1=d0;
            d0 =      t0;
            XT_SSIP(t0,castxcc(xtfloat,r),sizeof(float32_t));
        }
        delLine[0]=d0;
        delLine[1]=d1;
        delLine[2]=d2;
        delLine[3]=d3;
        delLine[4]=d4;
        delLine[5]=d5;
        delLine[6]=d6;
        delLine[7]=d7;
    }
#endif

} // latrf_process()

#endif
