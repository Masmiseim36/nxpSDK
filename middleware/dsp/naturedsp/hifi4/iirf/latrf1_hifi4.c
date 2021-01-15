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

    M=1
  IntegrIT, 2006-2018
*/
#include "latrf_common.h"
#include "common_fpu.h"

#if (HAVE_VFPU)
#define MOVTSX2(inout,in,b)                     \
{                                               \
    ae_int32x2 t0,t1;                           \
    t0=XT_AE_MOVINT32X2_FROMXTFLOATX2(inout);   \
    t1=XT_AE_MOVINT32X2_FROMXTFLOATX2(in);      \
    AE_MOVT32X2(t0,t1,b);                       \
    inout=XT_AE_MOVXTFLOATX2_FROMINT32X2(t0);   \
}

void latrf_process1(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N )
{
  float32_t * restrict  delLine;
  float32_t t0,s0,c0;
  float32_t scale;
  int n;

  delLine = latr->delayLine;
  c0 = latr->coef[0];
  scale   = latr->scale;
  s0 = delLine[0];
  for ( n=0; n<N; n++ )
  {
    t0 = x[n]*scale;  
    t0 -= s0*c0;
    r[n] = s0 = t0;
  }
  delLine[0] = s0;
}
#else
// for scalar FPU
void latrf_process1(struct tag_latrf_t *latr, float32_t * restrict r,const float32_t*  x, int N )
{
  float32_t * restrict  delLine;
  float32_t t0,s0,c0;
  float32_t scale;
  int n;

  delLine = latr->delayLine;
  c0 = latr->coef[0];
  scale   = latr->scale;
  s0 = delLine[0];
  for ( n=0; n<N; n++ )
  {
    t0 = x[n]*scale;  
    t0 -= s0*c0;
    r[n] = s0 = t0;
  }
  delLine[0] = s0;
}
#endif
