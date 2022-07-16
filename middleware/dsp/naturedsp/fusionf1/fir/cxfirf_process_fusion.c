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
  NatureDSP Signal Processing Library. FIR part
    Complex block FIR filter, floating point
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Complex Block FIR Filter
  Computes a complex FIR filter (direct-form) using complex IR stored in 
  vector h. The complex data input is stored in vector x. The filter output
  result is stored in vector r. The filter calculates N output samples using 
  M coefficients and requires last M-1 samples in the delay line. Real and
  imaginary parts are interleaved and real parts go first (at even indexes).
  NOTE: 
  User application is not responsible for management of delay lines

  Precision: 
  16x16     16-bit data, 16-bit coefficients, 16-bit outputs
  24x24     24-bit data, 24-bit coefficients, 24-bit outputs
  32x16     32-bit data, 16-bit coefficients, 32-bit outputs
  32x32     32-bit data, 32-bit coefficients, 32-bit outputs
  f         floating point
  Input:
  h[M] complex filter coefficients; h[0] is to be multiplied with the newest 
       sample , Q31, Q15, floating point
  x[N] input samples , Q15, Q31 or floating point
  N    length of sample block (in complex samples) 
  M    length of filter 
  Output:
  y[N] output samples , Q15, Q31 or floating point

  Restriction:
  x,y - should not overlap
  x,h - aligned on a 8-bytes boundary
  N,M - multiples of 4
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "cxfirf.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, cxfirf_process, (cxfirf_handle_t _cxfir,
                              complex_float * restrict  y,
                              const complex_float * restrict  x, int N))
#else

/* process block of samples */
void cxfirf_process( cxfirf_handle_t _cxfir, 
                         complex_float * restrict  y,
                   const complex_float * restrict  x, int N )
{
  int n, m, M;
  xtfloat x0, x1, x2, x3;
  xtfloat s0, s1, s2, s3;
  xtfloat acc0, acc1, acc2, acc3;
  const xtfloat* restrict pX = (const xtfloat*)x;
        xtfloat* restrict pD;
  const xtfloat* restrict pH;
        xtfloat*          pZ = (xtfloat*)y;
  cxfirf_t* state;
  NASSERT(_cxfir);
  state=(cxfirf_t*)_cxfir;
  NASSERT(state->h);
  NASSERT(state->d);
  NASSERT(state->p);
  NASSERT_ALIGN(state->h,8);
  NASSERT_ALIGN(state->d,8);
  NASSERT_ALIGN(state->p,8);
  NASSERT(N%4==0);
  NASSERT_ALIGN(x,8);
  NASSERT((state->M%4)==0);
  NASSERT(x);
  NASSERT(y);
  if(N<=0) return;
  M=state->M;
  pD = ( xtfloat*)state->p;
  pH = (const xtfloat*)state->h;
  NASSERT(N>0);
  NASSERT(M>0);
  WUR_AE_CBEGIN0((uintptr_t)(state->d));
  WUR_AE_CEND0((uintptr_t)(state->d + M));
  for (n = 0; n<(N>>1); n ++)
  {
    XT_LSIP(x0, pX, 4);
    XT_LSIP(x1, pX, 4);
    XT_LSIP(x2, pX, 4);
    XT_LSIP(x3, pX, 4);
    acc0 = XT_CONST_S(0);
    acc1 = XT_CONST_S(0);
    acc2 = XT_CONST_S(0);
    acc3 = XT_CONST_S(0);
    s0 = x0;
    s1 = x1;
    s2 = x2;
    s3 = x3;
    { xtfloat dummy;  XT_LSXC(dummy, pD, -4); }
    for (m = 0; m<M; m ++)
    {
      xtfloat h0, h1;
      h0 = pH[2*m + 0];
      h1 = pH[2*m + 1];
      XT_MADD_S(acc0, x0, h0); /*re*re*/
      XT_MADD_S(acc1, x0, h1); /*re*im*/

      XT_MADD_S(acc2, x2, h0);
      XT_MADD_S(acc3, x2, h1);

      XT_MSUB_S(acc0, x1, h1); /*-im*im*/
      XT_MADD_S(acc1, x1, h0); /*im*re*/

      XT_MSUB_S(acc2, x3, h1);
      XT_MADD_S(acc3, x3, h0);
      x3 = x1;
      x2 = x0;
      XT_LSXC(x1, pD, -4); 
      XT_LSXC(x0, pD, -4);

    } 

    { xtfloat dummy;  XT_LSXC(dummy, pD, 4); }
    XT_SSXC(s0, pD, 4);
    XT_SSXC(s1, pD, 4);
    XT_SSXC(s2, pD, 4);
    XT_SSXC(s3, pD, 4);

    XT_SSIP(acc0, pZ, 4);
    XT_SSIP(acc1, pZ, 4);
    XT_SSIP(acc2, pZ, 4);
    XT_SSIP(acc3, pZ, 4);

  }
  state->p = (complex_float*)pD;
} // cxfirf_process()
#endif
