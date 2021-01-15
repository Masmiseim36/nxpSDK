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
    Real data circular auto-correlation, 32x32-bit with 72-bit accumulator
    for intermediate computations, no requirements on vectors
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
#include "raw_corr32x32ep.h"

/*-------------------------------------------------------------------------
  Circular Autocorrelation 
  Estimates the auto-correlation of vector x. Returns autocorrelation of 
  length N.
  These functions implement the circular autocorrelation algorithm with no 
  limitations on x vector length and alignment at the cost of increased 
  processing complexity. In addition, this implementation variant requires
  scratch memory area.

  Precision: 
  16x16    16-bit data, 16-bit outputs
  32x32    32-bit data, 32-bit outputs
  32x32ep  the same as above but using 72-bit accumulator for intermediate 
           computations
  f        floating point

  Input:
  s[]     scratch area of
            FIR_ACORRA16X16_SCRATCH_SIZE( N ) or
            FIR_ACORRA32X32_SCRATCH_SIZE( N ) or
            FIR_ACORRA32X32EP_SCRATCH_SIZE( N ) or
            FIR_ACORRAF_SCRATCH_SIZE( N ) bytes
              
  x[N]    input data Q15, Q31 or floating point
  N       length of x
  Output:
  r[N]    output data, Q15, Q31 or floating point

  Restrictions:
  x,r,s   should not overlap
  N       must be non-zero
  s       aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/
void fir_acorra32x32ep( void * restrict s,
                      int32_t  * restrict r,
                const int32_t  * restrict x,
                int N )
{
          ae_int32x2 * restrict S;
    const ae_int32x2 * restrict X;
    ae_valign x_align;
    ae_int32x2 t;
    //
    // Circular cross-correlation algorithm:
    //
    //   r[n] = sum( x[mod(n+m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //
    NASSERT( s && r && x && N > 0 && N > 0);
    NASSERT_ALIGN8(s);
    if ((N&1)==0)
    {   
        // for even N we may use more economic way to allocate in scratch memory
        int32_t * x_buf;
        int32_t * y_buf;
        int n;
        // Partition the scratch memory area.
        x_buf = (int32_t*)s;
        y_buf = x_buf + N;

        // Copy x[N] twice and append 4 zeroes, totally (2*N+4) int32_t elements
        S = (ae_int32x2 *) y_buf;
        X = (const ae_int32x2 *)(x);
        x_align = AE_LA64_PP(X);
        __Pragma("loop_count min=1")
        for ( n=0; n<(N>>1); n++ ) 
        {
          AE_LA32X2_IP(t,x_align,X);
          AE_S32X2_X (t,S,-4*N);
          AE_S32X2_IP(t,S,8);
        }
        t=AE_MOVDA32X2(0,0);  // append 4 zeroes
        AE_S32X2_IP(t,S,8);
        AE_S32X2_IP(t,S,8);
        // Compute the correlation.
        raw_corr32x32ep(r,x_buf,y_buf,N,N);
    }
    else 
    {
        fir_xcorra32x32ep(s,r,x,x,N,N );
    }
} // fir_acorra32x32ep()
