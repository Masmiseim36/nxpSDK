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
    Real data circular cross-correlation, 32x32-bit, no requirements on vectors 
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
#include "raw_corr32x32.h"

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
void fir_xcorra32x32( void * restrict s,
                      int32_t  * restrict r,
                const int32_t  * restrict x,
                const int32_t  * restrict y,
                int N, int M )
{
  //
  // Circular cross-correlation algorithm:
  //
  //   r[n] = sum( x[mod(n+m,N)]*y[m] )
  //        m=0..M-1
  //
  //   where n = 0..N-1
    int32_t  * x_buf;
    int32_t  * y_buf;
        ae_int32x2 * restrict S;
    const ae_int32x2 * restrict X;
    const ae_int32x2 * restrict Y;

    ae_int32x2 t;
    ae_valign ax,as,ay;
    int n, m;

    NASSERT( s && r && x && y && N > 0 && M > 0 && N >= M-1 );
    NASSERT_ALIGN8(s);

    // Partition the scratch memory area.
    x_buf = (int32_t*)s;
    y_buf = (int32_t*)(x_buf+(((N+M-1)+1)&(~1)));   // closest bigger aligned 

    //----------------------------------------------------------------------------
    // Copy x[N] data into the aligned buffer in a way that simplifies the
    // correlation calculation:
    //  x_buf[N+M-1+3] = { x[0]..x[N-1] x[0]..x[M-2] X X X }
    // Three X locations are reserved to allow for block-4 processing.
    S=(ae_int32x2*)x_buf;
    X=(const ae_int32x2*)x;
    ax = AE_LA64_PP( X );
    __Pragma("loop_count min=1")
    for ( n=0; n<((N+1)>>1); n++ )
    {
        AE_LA32X2_IP( t, ax, X );
        AE_S32X2_IP( t, S, +8 );
    }
    S=(ae_int32x2*)(x_buf+N);
    as=AE_ZALIGN64();
    X=(const ae_int32x2*)x;
    ax = AE_LA64_PP( X );
    for ( n=0; n<(M>>1); n++ )
    {
        AE_LA32X2_IP( t, ax, X );
        AE_SA32X2_IP( t, as, S );
    }
    AE_SA64POS_FP( as, S );
    S=(ae_int32x2*)(x_buf+N+M-1);
    AE_S32_L_IP(AE_ZERO32(),castxcc(ae_int32,S),4);

    //----------------------------------------------------------------------------
    // Copy y[M] data into the aligned buffer and append 3 zeros:
    //  y_buf[M+3] = { y[0]..y[M-1] 0 0 0 }
    S=(ae_int32x2*)(y_buf);
    Y=(const ae_int32x2*)y;
    ay = AE_LA64_PP( Y );
    __Pragma("loop_count min=1")
    for ( m=0; m<((M+1)>>1); m++) 
    {
    AE_LA32X2_IP( t, ay, Y );
    AE_S32X2_IP ( t, S , +8 );
    }
    S=(ae_int32x2*)(y_buf+M);
    t=AE_ZERO32();
    AE_S32_L_IP ( t, castxcc(ae_int32,S), 4 );
    AE_S32_L_IP ( t, castxcc(ae_int32,S), 4 );
    AE_S32_L_IP ( t, castxcc(ae_int32,S), 4 );

    // linear correlation
    raw_corr32x32(r, x_buf,y_buf, N,M);
} // fir_xcorra32x32()
