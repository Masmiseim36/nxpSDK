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

    any M
  IntegrIT, 2006-2018
*/
#include "latrf_common.h"
#include "common_fpu.h"

#if (HAVE_VFPU || HAVE_FPU)
void latrf_processX(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N  )
{
        float32_t * restrict  delLine;
  const float32_t * restrict  coef;

  float32_t t0,t1,t2,t3;
  float32_t scale;

  int M;
  int n, m;

  delLine = latr->delayLine;
  coef    = latr->coef;
  scale   = latr->scale;
  M       = latr->M;

  for ( n=0; n<(N&~3); n+=4 )
  {
    // Scale the input sample.
    t0 = x[n+0]*scale;
    t1 = x[n+1]*scale;
    t2 = x[n+2]*scale;
    t3 = x[n+3]*scale;

    t0 -=          delLine[M-1]       * coef[M-1];
    t0 -=          delLine[M-2]       * coef[M-2];
    delLine[M-1] = delLine[M-2] +  t0 * coef[M-2];
    t0 -=          delLine[M-3]       * coef[M-3];
    delLine[M-2] = delLine[M-3] +  t0 * coef[M-3];
    t0 -=          delLine[M-4]       * coef[M-4];
    delLine[M-3] = delLine[M-4] +  t0 * coef[M-4];
    t1 -=          delLine[M-1]       * coef[M-1];
    t1 -=          delLine[M-2]       * coef[M-2];
    delLine[M-1] = delLine[M-2] +  t1 * coef[M-2];
    t1 -=          delLine[M-3]       * coef[M-3];
    delLine[M-2] = delLine[M-3] +  t1 * coef[M-3];
    t2 -=          delLine[M-1]       * coef[M-1];
    t2 -=          delLine[M-2]       * coef[M-2];
    delLine[M-1] = delLine[M-2] +  t2 * coef[M-2];
    t3 -= delLine[M-1]*coef[M-1];
    for ( m=M-5; m>=0; m-- )
    {
      t0 -=          delLine[m+0]       * coef[m+0];
      delLine[m+1] = delLine[m+0] +  t0 * coef[m+0];
      t1 -=          delLine[m+1]       * coef[m+1];
      delLine[m+2] = delLine[m+1] +  t1 * coef[m+1];
      t2 -=          delLine[m+2]       * coef[m+2];
      delLine[m+3] = delLine[m+2] +  t2 * coef[m+2];
      t3 -=          delLine[m+3]       * coef[m+3];
      delLine[m+4] = delLine[m+3] +  t3 * coef[m+3];
    }
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
        // Scale the input sample.
        t0 = x[n]*scale;
        t0 -= delLine[M-1]*coef[M-1];
        for ( m=M-2; m>=0; m-- )
        {
          t0 -= ( delLine[m] * coef[m] );
          // Update the (m+1)-th delay line element.
          delLine[m+1] = delLine[m] + ( t0 * coef[m] );
        }
        // Update the first delay line element with the resulting sample
        delLine[0] = t0;
        // Make the output sample.
        r[n] = t0;
      }
  }
} // latrf_process()
#endif
