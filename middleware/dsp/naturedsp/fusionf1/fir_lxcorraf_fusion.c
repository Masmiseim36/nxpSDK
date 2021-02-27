/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (ÿCadence    */
/* Librariesÿ) are the copyrighted works of Cadence Design Systems Inc.	    */
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
    Real data linear cross-correlation, floating point, no requirements on vectors 
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Linear Correlation
  Functions estimate the linear cross-correlation between vectors x (of 
  length N) and y (of length M) resulting in vector r of length N+M-1. It 
  is similar to convolution but y is read in opposite direction.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_LXCORRA16X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X16_SCRATCH_SIZE(N, M) 
                FIR_LXCORRA32X32_SCRATCH_SIZE(N, M) 
                FIR_LXCORRAF_SCRATCH_SIZE(N, M)     
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N+M-1]      output data, Q31, Q15  or floating point

  Restrictions:
  x,y,r,s should not overlap
  s should be aligned on 8-byte boundary
  N>0, M>0
  N>=M-1
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_lxcorraf, (void    * restrict s,
  float32_t * restrict r,
  const float32_t * restrict x,
  const float32_t * restrict y,
  int N, int M))
#else

#if 0
/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))


void fir_lxcorraf( void    * restrict s,
                      float32_t * restrict r,
                const float32_t * restrict x,
                const float32_t * restrict y,
                int N, int M )
{

 // void    * s_ptr;
  //int16_t * x_buf;
  float32_t   acc;

  int j,k,m, j0,j1;

  NASSERT(s);
  NASSERT(r);
  NASSERT(x);
  NASSERT(y);
  NASSERT_ALIGN(s,8);
  NASSERT(N>0 && M>0);
  NASSERT(N>=M-1);

  //
  // Compute the correlation.
  //

  for (k=0; k<N+M-1; k++)
  {
    j0=MAX(0,k-M+1);
    j1=MIN(N,k+1);
    acc=0;
    for (j=j0; j<j1; j++)
    {
      m=M-1-(k-j);
      NASSERT(j>=0 && j<N);
      NASSERT(m>=0 && m<M);
      acc+=( x[j] * y[m] );
    }
    r[k] = acc;
  }
} // fir_lxcorraf()
#else

#include "raw_lcorrf.h"

void fir_lxcorraf(   void    * restrict s,
                   float32_t * restrict r,
             const float32_t * restrict x,
             const float32_t * restrict y,
                   int N, int M)
{
    void *  s_ptr;
          ae_int32x2 * restrict S;
    const ae_int32x2 * restrict Y;
    ae_valign ay;
    ae_int32x2 t;

    float32_t * y_buf;

    int m;
    NASSERT(r && x && y && N > 0 && M > 0 && N >= M - 1);
    NASSERT_ALIGN(s,8);
    /*
    * Partition the scratch memory area.
    */
    s_ptr = s;
    y_buf = (float32_t*)s_ptr;
    y_buf +=4;
    s_ptr = y_buf + M + 3;

    ASSERT((int8_t*)s_ptr - (int8_t*)s <=
        (int)FIR_LXCORRAF_SCRATCH_SIZE(N, M));

    S = (ae_int32x2*)(y_buf);
    Y = (const ae_int32x2*)(y);
    ay = AE_LA64_PP(Y);
    __Pragma("loop_count min=1")
    for (m = 0; m<((M + 1) >> 1); m++)
    {
        AE_LA32X2_IP(t, ay, Y);
        AE_S32X2_IP(t, castxcc(ae_int32x2, S), +8);
    }
    /* add 4 zeros from the left and 3 zeros from the right */
    y_buf[-4]=
    y_buf[-3]=
    y_buf[-2]=
    y_buf[-1]=
    y_buf[M+0]=
    y_buf[M+1]=
    y_buf[M+2]=0.f;
    raw_lcorrf(r, x, y, N, M);
} // fir_lxcorraf()
#endif

#endif
