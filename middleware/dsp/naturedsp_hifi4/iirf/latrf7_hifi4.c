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
  NatureDSP Signal Processing Library. IIR part
    Lattice Block Real IIR, floating point
    C code optimized for HiFi4

    M=7
  IntegrIT, 2006-2018
*/
#include "latrf_common.h"
#include "common_fpu.h"

#if (HAVE_VFPU || HAVE_FPU)
void latrf_process7(struct tag_latrf_t *latr, 
                    float32_t * restrict     r,
                    const float32_t*         x, int N )
{
        float32_t * restrict  delLine;
  const float32_t * restrict  coef;

  float32_t t0,t1,t2,t3;
  float32_t scale;

  int n;

  NASSERT(latr->M==7);
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

    t0 -=          delLine[7-1]       * coef[7-1];
    t0 -=          delLine[7-2]       * coef[7-2];
    delLine[7-1] = delLine[7-2] +  t0 * coef[7-2];
    t0 -=          delLine[7-3]       * coef[7-3];
    delLine[7-2] = delLine[7-3] +  t0 * coef[7-3];
    t0 -=          delLine[7-4]       * coef[7-4];
    delLine[7-3] = delLine[7-4] +  t0 * coef[7-4];
    t1 -=          delLine[7-1]       * coef[7-1];
    t1 -=          delLine[7-2]       * coef[7-2];
    delLine[7-1] = delLine[7-2] +  t1 * coef[7-2];
    t1 -=          delLine[7-3]       * coef[7-3];
    delLine[7-2] = delLine[7-3] +  t1 * coef[7-3];
    t2 -=          delLine[7-1]       * coef[7-1];
    t2 -=          delLine[7-2]       * coef[7-2];
    delLine[7-1] = delLine[7-2] +  t2 * coef[7-2];
    t3 -=          delLine[7-1]       * coef[7-1];

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
      x+=n;
      r+=n;
      N&=3;
      __Pragma("loop_count min=1, max=3");
      for (n=0 ; n<N; n++ )
      {
        t0 = x[n]*scale;
        t0 -= delLine[7-1]*coef[7-1];
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
} // latrf7_process()
#endif
