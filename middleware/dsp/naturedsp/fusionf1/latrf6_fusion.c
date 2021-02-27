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

void latrf_process6(struct tag_latrf_t *latr, 
                    float32_t  *  restrict    r,
                    const float32_t*         x, int N )
#if 0
{
    float32_t d0,d1,d2,d3,d4,d5;
          float32_t * restrict delLine;
    const xtfloat *restrict coef;
    xtfloat c0,c1,c2,c3,c4,c5;

    float32_t t0;
    float32_t scale;

    int n;

    NASSERT(latr->M==6);
    delLine = latr->delayLine;
    scale   = latr->scale;
    d0=delLine[0];
    d1=delLine[1];
    d2=delLine[2];
    d3=delLine[3];
    d4=delLine[4];
    d5=delLine[5];
    coef    = (const xtfloat*)latr->coef+5;
    XT_xtfloat_loadip(c5,coef,-4);
    XT_xtfloat_loadip(c4,coef,-4);
    XT_xtfloat_loadip(c3,coef,-4);
    XT_xtfloat_loadip(c2,coef,-4);
    XT_xtfloat_loadip(c1,coef,-4);
    XT_xtfloat_loadxp(c0,coef,4*5);

    for ( n=0; n<N; n++ )
    {
        t0  = x[n]*scale;
        t0 -= d5 * (float32_t)c5;
        t0 -= d4 * (float32_t)c4;
        d5  = d4 + t0 * (float32_t)c4 ;
        t0 -= d3 * (float32_t)c3;
        d4  = d3 + t0 * (float32_t)c3 ;
        t0 -= d2 * (float32_t)c2;
        d3  = d2 + t0 * (float32_t)c2 ;
        t0 -= d1 * (float32_t)c1;
        d2  = d1 + t0 * (float32_t)c1 ;
        t0 -= d0 * (float32_t)c0 ;
        d1  = d0 + t0 * (float32_t)c0 ;
        d0 =      t0;
        r[n] = t0;
    }
    delLine[0]=d0;
    delLine[1]=d1;
    delLine[2]=d2;
    delLine[3]=d3;
    delLine[4]=d4;
    delLine[5]=d5;
} // latrf6_process()
#elif 0
{
        float32_t * restrict  delLine;
  const float32_t * restrict  coef;

  float32_t t0,t1,t2,t3;
  float32_t scale;

  int n;

  NASSERT(latr->M==6);
  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;

  for ( n=0; n<(N&~3); n+=4 )
  {
    // Scale the input sample.
    t0 = x[n+0]*scale;
    t1 = x[n+1]*scale;
    t2 = x[n+2]*scale;
    t3 = x[n+3]*scale;

    t0 -=          delLine[6-1]       * coef[6-1];
    t0 -=          delLine[6-2]       * coef[6-2];
    delLine[6-1] = delLine[6-2] +  t0 * coef[6-2];
    t0 -=          delLine[6-3]       * coef[6-3];
    delLine[6-2] = delLine[6-3] +  t0 * coef[6-3];
    t0 -=          delLine[6-4]       * coef[6-4];
    delLine[6-3] = delLine[6-4] +  t0 * coef[6-4];
    t1 -=          delLine[6-1]       * coef[6-1];
    t1 -=          delLine[6-2]       * coef[6-2];
    delLine[6-1] = delLine[6-2] +  t1 * coef[6-2];
    t1 -=          delLine[6-3]       * coef[6-3];
    delLine[6-2] = delLine[6-3] +  t1 * coef[6-3];
    t2 -=          delLine[6-1]       * coef[6-1];
    t2 -=          delLine[6-2]       * coef[6-2];
    delLine[6-1] = delLine[6-2] +  t2 * coef[6-2];
    t3 -=          delLine[6-1]       * coef[6-1];

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
      x+=n;
      r+=n;
      N&=3;
      __Pragma("loop_count min=1, max=3");
      for (n=0; n<N; n++)
      {
        t0 = x[n]*scale;
        t0 -= delLine[6-1]*coef[6-1];
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
} // latrf6_process()
#else
{
        float32_t * restrict  delLine;
    const float32_t * restrict  coef;

    float32_t t0;
    float32_t scale;

    int n;

    NASSERT(latr->M==6);
    delLine = latr->delayLine;
    coef    = latr->coef;
    scale   = latr->scale;
    __Pragma("loop_count min=1");
    for (n=0; n<N; n++)
    {
        t0 = x[n]*scale;
        t0 -= delLine[6-1]*coef[6-1];
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
} // latrf6_process()
#endif
