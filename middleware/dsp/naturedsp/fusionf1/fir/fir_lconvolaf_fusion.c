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
    Real data linear convolution, floating point
*/

/*-------------------------------------------------------------------------
  Linear Convolution
  Functions perform linear convolution between vectors x (of length N) and
  y (of length M) resulting in vector r of length N+M-1.

  Precision: 
  16x16   16x16-bit data, 16-bit outputs
  32x16   32x16-bit data, 32-bit outputs 
  32x32   32x32-bit data, 32-bit outputs 
  f       floating point 

  Input:
  s[]           Scratch memory,
                FIR_LCONVOLA16X16_SCRATCH_SIZE( N, M )
                FIR_LCONVOLA32X16_SCRATCH_SIZE( N, M )
                FIR_LCONVOLA32X32_SCRATCH_SIZE( N, M )
                FIR_LCONVOLAF_SCRATCH_SIZE( N, M )    
                bytes
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N+M-1]      output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  s        - must be aligned on an 8-bytes boundary
  N,M      - must be >0
  N >= M-1 - minimum allowed length of vector x is the length of y minus one
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_lconvolaf, (void* s,
  float32_t * restrict r,
  const float32_t * restrict x,
  const float32_t * restrict y,
  int N, int M))
#else

#if 0
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))


/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"

void fir_lconvolaf( void* s,
                      float32_t * restrict r,
                const float32_t * restrict x,
                const float32_t * restrict y,
                int N, int M )
{
  float32_t acc;

  int j,k,j0,j1, m;

  NASSERT(s);
  NASSERT(r);
  NASSERT(x);
  NASSERT(y);
  NASSERT_ALIGN(s,8);
  NASSERT(N>0 && M>0);
  NASSERT(N>=M-1);

  for (k=0; k<N+M-1; k++)
  {
    acc=0;
    j0=MAX(0,k-M+1);
    j1=MIN(N,k+1);
    for (j=j0; j<j1; j++)
    {
      m=k-j;
      NASSERT(j>=0 && j<N);
      NASSERT(m>=0 && m<M);
      acc += (x[j]*y[m]);
    }

    r[k] = acc;
  }

} // fir_lconvolaf()
#else
#include "raw_lcorrf.h"

/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_lconvolaf(      void * s,
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

    NASSERT(s && r && x && y && N > 0 && M > 0 && N >= M - 1);
    NASSERT_ALIGN8(s);

    /*
    * Partition the scratch memory area.
    */
    s_ptr = s;
    y_buf = (float32_t*)ALIGNED_ADDR(s_ptr, 8);
    y_buf +=4;
    s_ptr = y_buf + M + 3;

    ASSERT((int8_t*)s_ptr - (int8_t*)s <=
        (int)FIR_LCONVOLAF_SCRATCH_SIZE(N, M));

    S = (ae_int32x2*)(y_buf);
    Y = (const ae_int32x2*)(y + M - 1);
    ay = AE_LA64_PP(Y);
    __Pragma("loop_count min=1")
    for (m = 0; m<((M + 1) >> 1); m++)
    {
        AE_LA32X2_RIP(t, ay, Y);
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
    raw_lcorrf(r, x, y_buf, N, M);
} // fir_lconvolaf()

#endif
#endif
