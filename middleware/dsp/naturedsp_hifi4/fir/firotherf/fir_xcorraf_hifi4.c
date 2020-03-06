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
  NatureDSP Signal Processing Library. FIR part
    Real data circular cross-correlation, floating point, no requirements on vectors 
    length and alignment.
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/


/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corrf.h"
#include "common_fpu.h"

#if (HAVE_FPU==0 && HAVE_VFPU==0)
DISCARD_FUN(void,fir_xcorraf,(     void       * restrict s,
                      float32_t  * restrict r,
                const float32_t  * restrict x,
                const float32_t  * restrict y,
                int N, int M ))
#elif (HAVE_VFPU)

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.
  These functions implement the circular correlation algorithm with no 
  limitations on x and y vectors length and alignment at the cost of 
  increased processing complexity. In addition, this implementation variant
  requires scratch memory area.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  32x32ep   the same as above but using 72-bit accumulator for intermediate 
            computations
  f         floating point

  Input:
  s[]       scratch area, 
              FIR_XCORRA16X16_SCRATCH_SIZE( N, M )
              FIR_XCORRA32X16_SCRATCH_SIZE( N, M ) or
              FIR_XCORRA32X32_SCRATCH_SIZE( N, M ) or
              FIR_XCORRA32X32EP_SCRATCH_SIZE( N, M ) or
              FIR_XCORRAF_SCRATCH_SIZE( N, M ) or
              CXFIR_XCORRA32X32_SCRATCH_SIZE( N, M ) or
              CXFIR_XCORRAF_SCRATCH_SIZE( N, M ) bytes

  x[N]      input data Q15, Q31 or floating point
  y[M]      input data Q15, Q31 or floating point
  N         length of x
  M         length of y
  Output:
  r[N]      output data, Q15, Q31 or floating point

  Restrictions:
  x,y,r,s   should not overlap
  s         must be aligned on an 8-bytes boundary
  N,M       must be >0
  N >= M-1  minimum allowed length of vector x is the length of y minus one

-------------------------------------------------------------------------*/
void fir_xcorraf(     void       * restrict s,
                      float32_t  * restrict r,
                const float32_t  * restrict x,
                const float32_t  * restrict y,
                int N, int M )
{
    //
    // Circular cross-correlation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //
    const xtfloatx2 * restrict x_inp;
    const xtfloatx2 * restrict y_inp;
          xtfloatx2 * restrict buf;
    xtfloatx2 regx2;
    xtfloat reg;
    ae_valign al_inp, al_buf;
    int xbuf_len, ybuf_len;
    int n, m, T;

    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(s,8);
    NASSERT(N>0 && M>0);
    NASSERT(N>=M-1);

    //
    // Partition the scratch memory area.
    //
    buf = (xtfloatx2 *)(s);
    // compute length of the buffer for x
    xbuf_len = (N+M) & ~1;
    // compute length of the buffer for y
    ybuf_len = (M+3) & ~3;

    ASSERT( (xbuf_len+ybuf_len)*(int)sizeof(float32_t) <= 
            (int)FIR_XCORRAF_SCRATCH_SIZE( N, M ) );

    //
    // Copy x[N] data into the scratch memory in a way that simplifies the
    // correlation calculation:
    // x[0]..x[N-1] x[0]..x[M-2]
    //
    T = N>>1;
    for (m = 0; m < 2; m++)
    {
        x_inp = (const xtfloatx2 *)x;
        al_inp = XT_LASX2PP(x_inp);
        al_buf = AE_ZALIGN64();
        for ( n=0; n<T; n++ )
        {
            XT_LASX2IP(regx2, al_inp, x_inp);
            XT_SASX2IP(regx2, al_buf, buf);
        }
        XT_SASX2POSFP(al_buf, buf);
        if (N&1)
        {
            XT_LSIP(reg, castxcc(xtfloat,x_inp), sizeof(float32_t));
            XT_SSIP(reg, castxcc(xtfloat,buf  ), sizeof(float32_t));
        }
        T = (xbuf_len-N)>>1;
    }
    //
    // Copy y[M] data into the scratch memory after x[N].
    // Round M to a next multiple of 4 and set excess values to zero.
    //
    y_inp = (const xtfloatx2 *)y;
    al_inp = XT_LASX2PP(y_inp);
    for ( m=0; m<(M>>1); m++ )
    {
        XT_LASX2IP(regx2, al_inp, y_inp);
        XT_SSX2IP(regx2, buf, 2*sizeof(float32_t));
    }
    if (M&1)
    {
        reg = XT_LSI((xtfloat *)y_inp, 0);
        XT_SSIP(reg, castxcc(xtfloat,buf), sizeof(float32_t));
    }
    reg = 0.0f;
    for (m = 0; m < ybuf_len-M+1; m++)
    {
        XT_SSIP(reg, castxcc(xtfloat,buf), sizeof(float32_t));
    }
    __Pragma("no_reorder")
    raw_corrf(r,(const float32_t *)s,(const float32_t *)s+xbuf_len,N,M);
} // fir_xcorraf()
#else
// for scalar FPU
void fir_xcorraf(     void       * restrict s,
                      float32_t  * restrict r,
                const float32_t  * restrict x,
                const float32_t  * restrict y,
                int N, int M )
{
    /*
    * Circular cross-correlation algorithm:
    *
    *   r[n] = sum( x[mod(n+m,N)]*y[m] )
    *        m=0..M-1
    *
    *   where n = 0..N-1
    */
    void       *  s_ptr;
    float32_t  * restrict x_buf;

    int n, m;
    NASSERT(s);
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(s,8);
    NASSERT(N>0 && M>0);
    NASSERT(N>=M-1);
    /*
    * Partition the scratch memory area.
    */

    s_ptr = s;
    x_buf = (float32_t*)s_ptr ;
    s_ptr = x_buf + N + M-1;

    ASSERT( (int8_t*)s_ptr - (int8_t*)s <= 
            (int)FIR_XCORRAF_SCRATCH_SIZE( N, M ) );

    /*
    * Copy x[N] data into the scratch memory in a way that simplifies the
    * correlation calculation:
    * x[0]..x[N-1] x[0]..x[M-2]
    */
    for ( n=0; n<N; n++ )
    {
        x_buf[n] = x[n];
    }

    for ( m=0; m<M-1; m++, n++ )
    {
        x_buf[n] = x[m];
    }
  __Pragma("no_reorder")
   raw_corrf(r,x_buf,y,N,M);

} /* fir_xcorraf() */

#endif
