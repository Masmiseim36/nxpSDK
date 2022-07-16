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
    Real data linear convolution, 32x16-bit
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

#if 0
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))


/* NON OPTIMIZED REFERENCE CODE: to be use it for educational purposes only */
#include "baseop.h"

void fir_lconvola32x16( void* s,
                      int32_t * restrict r,
                const int32_t * restrict x,
                const int16_t * restrict y,
                int N, int M )
{
  int64_t acc;

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
      acc += mpy_Q31Q15(x[j],y[m]);
    }

    r[k] = satQ31( ( acc + (1LL<<15) )>>16);
  }

} // fir_lconvol32x16()
#else

#include "raw_lcorr32x16.h"
/* Align address on a specified boundary. */
#define ALIGNED_ADDR( addr, align ) \
    (void*)(((uintptr_t)(addr)+((align)-1)) & ~((align)-1))

void fir_lconvola32x16( void          * s,
                        int32_t       * restrict r,
                        const int32_t * restrict x,
                        const int16_t * restrict y,
                        int N, int M)
{
    void    * s_ptr;
    int32_t * x_buf;
    int16_t * y_buf;

    const ae_int32x2 *          S;
          ae_int32x2 * restrict D;
          ae_int32   * restrict D_s;
    const ae_int16x4 *          S16;
          ae_int16x4 * restrict D16;
          ae_int16   * restrict D16_s;
    ae_valign S_va, D_va;
    ae_int32x2 t;
    ae_int16x4 p;

    int n, m;

    ASSERT(s && r && x && y && N > 0 && M > 0 && N >= M - 1);

    ASSERT(IS_ALIGN(s));

    //----------------------------------------------------------------------------
    // Partition the scratch memory area.

    s_ptr = s;
    x_buf = (int32_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = x_buf + N + 3;
    y_buf = (int16_t*)ALIGNED_ADDR(s_ptr, 8);
    s_ptr = y_buf + M + 3;

    ASSERT((int8_t *)s_ptr - (int8_t *)s <=
        (int)FIR_LCONVOLA32X16_SCRATCH_SIZE(N, M));

    //----------------------------------------------------------------------------
    S = (const ae_int32x2 *)x;
    D = (ae_int32x2 *)x_buf;
    S_va = AE_LA64_PP(S);
    D_va = AE_ZALIGN64();

    for (n = 0; n < ((N + 1) >> 1); n++)
    {
        AE_LA32X2_IP(t, S_va, S);
        AE_S32X2_IP(t, D, +8);
    }

    D_s = (ae_int32 *)((int32_t *)D - (N & 1));
    t = 0;
    for (n = 0; n < 3; n++)
    {
        AE_S32_L_IP(t, D_s, +4);
    }

    //----------------------------------------------------------------------------
    S16 = (const ae_int16x4 *)(y + M - 1);
    D16 = (ae_int16x4 *)y_buf;
    S_va = AE_LA64_PP(S16);
    for (m = 0; m<((M + 3) >> 2); m++)
    {
        AE_LA16X4_RIP(p, S_va, S16);
        AE_S16X4_IP(p, D16, +8);
    }
    // Append three zeros to allow for block-4 processing.
    D16_s = (ae_int16 *)((int16_t *)D16 - (-M & 3));
    p = AE_ZERO16();
    for (m = 0; m<3; m++)
    {
        AE_S16_0_IP(p, D16_s, +2);
    }
    raw_lcorr32x16(r, x_buf, y_buf, N, M);
} // fir_lconvola32x16()
#endif
