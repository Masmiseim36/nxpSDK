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
    Complex data circular convolution, 32x16-bit
    C code optimized for Fusion
*/

/*-------------------------------------------------------------------------
  Circular Convolution
  Performs circular convolution between vectors x (of length N) and y (of 
  length M)  resulting in vector r of length N.

  Precision: 
  16x16  16x16-bit data, 16-bit outputs
  24x24  24x24-bit data, 24-bit outputs
  32x16  32x16-bit data, 32-bit outputs (both real and complex)
  32x32  32x32-bit data, 32-bit outputs 
  f      floating point 

  Input:
  x[N]          input data (Q31,Q15 or floating point)
  y[M]          input data (Q31,Q15 or floating point)
  N             length of x
  M             length of y
  Output:
  r[N]          output data,Q31,Q15 or floating point

  Restriction:
  x,y,r should not overlap
  x,y,r - aligned on an 8-bytes boundary
  N,M   - multiples of 4 and >0
-------------------------------------------------------------------------*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"

#include "baseop.h"
void cxfir_convol32x16(complex_fract32 * restrict r,
  const complex_fract32 * restrict x,
  const complex_fract16 * restrict y,
  int N,
  int M)
{
    ae_f64 ar,ai;
    const ae_int32x2 *pX;
    const ae_int16x4 *pY;
    const ae_f32x2 *pR;
    ae_int32x2 X0,X1;
    ae_int16x4 Y;
    int m,n;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r,8);
    NASSERT_ALIGN(x,8);
    NASSERT_ALIGN(y,8);
    NASSERT(M%4==0 && N%4==0);

    WUR_AE_CBEGIN0( (uintptr_t)( x + 0 ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    pR=(ae_f32x2*)r;
    for ( n=0; n<N; n++ )
    {
        ar=ai=AE_ZERO();
        pX=(const ae_int32x2*)(x+n);
        pY=(const ae_int16x4 *)y;
        __Pragma("loop_count min=2")
        for (m = 0; m<M; m+=2)
        {
            AE_L32X2_XC(X0,pX,-8);
            AE_L32X2_XC(X1,pX,-8);
            AE_L16X4_IP(Y,pY,8);
            AE_MULASFD32X16_H3_L2(ar, X0, Y);
            AE_MULAAFD32X16_H2_L3(ai, X0, Y);
            AE_MULASFD32X16_H1_L0(ar, X1, Y);
            AE_MULAAFD32X16_H0_L1(ai, X1, Y);
        }
        AE_S32X2RA64S_IP(ar, ai, castxcc(ae_f32x2, pR));
    }
} // cxfir_convol32x16()
