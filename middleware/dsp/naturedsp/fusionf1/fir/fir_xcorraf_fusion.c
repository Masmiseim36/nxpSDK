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
    Real data circular cross-correlation, floating point, no requirements on vectors 
    length and alignment.
*/

/*-------------------------------------------------------------------------
  Circular Correlation
  Estimates the circular cross-correlation between vectors x (of length N) 
  and y (of length M)  resulting in vector r of length N. It is a similar 
  to correlation but x is read in opposite direction.
  These functions implement the circular correlation algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16     16x16-bit data, 16-bit outputs
  24x24     24x24-bit data, 24-bit outputs
  32x16     32x16-bit data, 32-bit outputs
  32x32     32x32-bit data, 32-bit outputs
  f         floating point (both real and complex data)

  Input:
  s[]           Scratch memory, 
                FIR_XCORRA16X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA24X24_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X16_SCRATCH_SIZE( N, M ) 
                FIR_XCORRA32X32_SCRATCH_SIZE( N, M ) 
                FIR_XCORRAF_SCRATCH_SIZE( N, M )     
                CXFIR_XCORRAF_SCRATCH_SIZE( N, M )   
                bytes
  x[N]          input data Q31, Q15  or floating point
  y[M]          input data Q31, Q15 or floating point
  N             length of x
  M             length of y
  Output:
  r[N]          output data, Q31, Q15  or floating point

  Restrictions:
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
#include "raw_corrf.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_xcorraf, (void       * restrict s,
  float32_t  * restrict r,
  const float32_t  * restrict x,
  const float32_t  * restrict y,
  int N, int M))
#else

void fir_xcorraf( void       * restrict s,
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

