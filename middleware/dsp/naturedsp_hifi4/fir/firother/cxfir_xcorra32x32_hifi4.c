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
    Real data circular cross-correlation, complex 32x32, no requirements on vectors 
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
void cxfir_xcorra32x32(   void           * restrict s,
                      complex_fract32  * restrict r,
                const complex_fract32  * restrict x,
                const complex_fract32  * restrict y,
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
    const ae_int32x2 * restrict pX;
    const ae_int32x2 * restrict pY;
          ae_int32x2 * restrict pR;
    ae_int64 q0_re,q1_re,q2_re,q3_re;
    ae_int64 q0_im,q1_im,q2_im,q3_im;
    ae_int32x2 X0, X1, X2, X3, Y;
    int n, m;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    (void)s;
    NASSERT(N>0 && M>0);
    NASSERT(N>=M-1);

    pR=(ae_int32x2*)r;
    /* set circular buffer boundaries */
    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );

    for ( n=0; n<(N&~3); n+=4,x+=4)
    {
        pX=(const ae_int32x2 *)(x);
        pY=(const ae_int32x2 *)y;
        q0_re=q1_re=q2_re=q3_re=
        q0_im=q1_im=q2_im=q3_im= AE_ZERO64();
        /* preload data from x */
        AE_L32X2_XC(X0,pX,sizeof(X0));
        AE_L32X2_XC(X1,pX,sizeof(X1));
        AE_L32X2_XC(X2,pX,sizeof(X2));
        __Pragma("loop_count min=1")
        for ( m=0; m<M; m++ )
        {
            /* load data from x */
            AE_L32X2_XC(X3,pX,2*sizeof(int32_t));
            /* load data from y */
            AE_L32X2_IP(Y ,pY,2*sizeof(int32_t));
            /* compute correlation of 4 values */
            AE_MULAAFD32RA_HH_LL(q0_re, X0,Y);
            AE_MULASFD32RA_HL_LH(q0_im, X0,Y);
            AE_MULAAFD32RA_HH_LL(q1_re, X1,Y);
            AE_MULASFD32RA_HL_LH(q1_im, X1,Y);
            AE_MULAAFD32RA_HH_LL(q2_re, X2,Y);
            AE_MULASFD32RA_HL_LH(q2_im, X2,Y);
            AE_MULAAFD32RA_HH_LL(q3_re, X3,Y);
            AE_MULASFD32RA_HL_LH(q3_im, X3,Y);
            /* shift input line for the next iteration */
            X0=X1; X1=X2; X2=X3;
        }
        /* save computed samples */
        X0=AE_ROUND32X2F48SASYM(q0_re, q0_im);
        X1=AE_ROUND32X2F48SASYM(q1_re, q1_im);
        X2=AE_ROUND32X2F48SASYM(q2_re, q2_im);
        X3=AE_ROUND32X2F48SASYM(q3_re, q3_im);

        AE_S32X2_IP(X0,pR,2*sizeof(int32_t));
        AE_S32X2_IP(X1,pR,2*sizeof(int32_t));
        AE_S32X2_IP(X2,pR,2*sizeof(int32_t));
        AE_S32X2_IP(X3,pR,2*sizeof(int32_t));
    }
    /* Compute last (N%3) samples */
    for ( ; n<(N); n++,x++)
    {
        pX=(const ae_int32x2 *)(x);
        pY=(const ae_int32x2 *)y;
        q0_re=
        q0_im= AE_ZERO64();
        AE_L32X2_XC(X0,pX,sizeof(X0));
        for ( m=0; m<M; m++ )
        {
            AE_L32X2_IP(Y ,pY,sizeof(Y ));
            AE_MULAAFD32RA_HH_LL(q0_re, X0,Y);
            AE_MULASFD32RA_HL_LH(q0_im, X0,Y);
            AE_L32X2_XC(X0,pX,sizeof(X0));
        }
        X0=AE_ROUND32X2F48SASYM(q0_re, q0_im);
        AE_S32X2_IP(X0,pR,2*sizeof(int32_t));
    }
} // cxfir_xcorra32x32()
