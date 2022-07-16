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
    Real data circular convolution, floating point, no requirements on vectors 
    length and alignment.
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M) resulting in vector r of length N.
  These functions implement the circular convolution algorithm described in
  the previous chapter with no limitations on x and y vectors length and
  alignment at the cost of increased processing complexity. In addition, this
  implementation variant requires scratch memory area.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  s[]           Scratch memory,
                FIR_CONVOLA16X16_SCRATCH_SIZE( N, M )  
                FIR_CONVOLA24X24_SCRATCH_SIZE( N, M )   
                FIR_CONVOLA32X16_SCRATCH_SIZE( N, M )  
                CXFIR_CONVOLA32X16_SCRATCH_SIZE( N, M )
                FIR_CONVOLA32X32_SCRATCH_SIZE( N, M )  
                FIR_CONVOLAF_SCRATCH_SIZE( N, M )      
                bytes
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

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
#include "raw_corrf.h"

#if !(XCHAL_HAVE_FUSION_FP)
DISCARD_FUN(void, fir_convolaf, (void       * restrict s,
  float32_t  * restrict r,
  const float32_t  * restrict x,
  const float32_t  * restrict y,
  int N, int M))
#else
void fir_convolaf(     void       * restrict s,
                       float32_t  * restrict r,
                 const float32_t  * restrict x,
                 const float32_t  * restrict y,
                 int N, int M )
{
          ae_int32x2* restrict S;
    const ae_int32x2* restrict X;
    const ae_int32x2 * restrict Y;
    ae_valign as, ax, ay;
    ae_int32x2 t;

    /*
    * Circular convolution algorithm:
    *
    *   r[n] = sum( x[mod(n-m,N)]*y[m] )
    *        m=0..M-1
    *
    *   where n = 0..N-1
    */
    float32_t * x_buf;
    float32_t * y_buf;

    int n,m;

    NASSERT( s && r && x && y && N > 0 && M > 0 && N >= M-1 );
    NASSERT_ALIGN8( s);

    /* Partition the scratch memory area. */
    x_buf = (float32_t*)s;
    y_buf = (float32_t*)(x_buf+(((N+M-1)+1)&(~1)));   /* closest bigger aligned */ 

    /*
    * Copy x[N] data into the scratch memory in a way that simplifies the
    * convolution calculation:
    *  x[N-(M-1)]..x[N-1] x[0] x[1]..x[N-1]
    */
    S=(ae_int32x2*)x_buf;
    X=(const ae_int32x2*)(x+N-(M-1));
    ax = AE_LA64_PP( X );
    for ( n=0; n<(M>>1); n++ )
    {
        AE_LA32X2_IP( t, ax, X );
        AE_S32X2_IP( t, S, +8 );
    }
    S=(ae_int32x2*)(x_buf+(M-1));
    as=AE_ZALIGN64();
    X=(const ae_int32x2*)x;
    ax = AE_LA64_PP( X );
    __Pragma("loop_count min=1")
    for ( n=0; n<((N+1)>>1); n++ )
    {
        AE_LA32X2_IP( t, ax, X );
        AE_SA32X2_IP( t, as, S );
    }
      AE_SA64POS_FP( as, S );

    S=(ae_int32x2*)(y_buf);
    Y=(const ae_int32x2*)(y+M-1);
    ay = AE_LA64_PP( Y );
    __Pragma("loop_count min=1")
    for ( m=0; m<((M+1)>>1); m++) 
    {
        AE_LA32X2_RIP( t, ay, Y );
        AE_S32X2_IP ( t, castxcc(ae_int32x2,S), +8 );
    }

  raw_corrf(r,x_buf,y_buf,N,M);
} /* fir_convolaf() */

#endif
