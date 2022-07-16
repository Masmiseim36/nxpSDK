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
    Real data circular convolution, 16x16-bit
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

void fir_convol16x16(int16_t * restrict r,
    const int16_t * restrict x,
    const int16_t * restrict y,
    int N, int M)
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
    /* code with no Quad MAC option */
    int n,m;
    const ae_int16x4 *pX0,*pX1,*pX2,*pX3,*pY;
    ae_int16x4 * restrict pR;
    ae_int16x4 X6543,X5432,X4321,X3210,Y0123,XX;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax1,ax2,ax3;
    ae_int32x2 t0,t1;
    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r, 8);
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0 && M % 4 == 0);
    NASSERT(N > 0 && N % 4 == 0);
    WUR_AE_CBEGIN0( (uintptr_t)( x     ) );
    WUR_AE_CEND0  ( (uintptr_t)( x + N ) );
    pR=(ae_int16x4*)r;
    for ( n=0; n<N; n+=4 )
    {
        a0=a1=a2=a3=AE_ZERO64();
        pX1=(const ae_int16x4*)(x+n+2);
        pX2=(const ae_int16x4*)(x+n+1);
        pX3=(const ae_int16x4*)(x+n+0);
        pX0=(const ae_int16x4*)(x+n+0);
        AE_LA16X4NEG_PC(ax1, pX1);
        AE_LA16X4NEG_PC(ax2, pX2);
        AE_LA16X4NEG_PC(ax3, pX3);
        pY =(const ae_int16x4*)y;
        __Pragma("loop_count min=1")
        for (m=0; m<(M>>2); m++)
        {
            AE_L16X4_RIC(X6543, pX0);
            AE_LA16X4_RIC(X5432, ax1, pX1);
            AE_LA16X4_RIC(X4321, ax2, pX2);
            AE_LA16X4_RIC(X3210, ax3, pX3);
            AE_L16X4_IP(Y0123,pY,sizeof(ae_int16x4));
            t0 = AE_CVT32X2F16_32(Y0123);
            t1 = AE_CVT32X2F16_10(Y0123);

            AE_MULAAFD32X16_H3_L2(a0,t0,X3210);
            AE_MULAAFD32X16_H3_L2(a1,t0,X4321);
            AE_MULAAFD32X16_H3_L2(a2,t0,X5432);
            AE_MULAAFD32X16_H3_L2(a3,t0,X6543);

            AE_MULAAFD32X16_H1_L0(a0,t1,X3210);
            AE_MULAAFD32X16_H1_L0(a1,t1,X4321);
            AE_MULAAFD32X16_H1_L0(a2,t1,X5432);
            AE_MULAAFD32X16_H1_L0(a3,t1,X6543);
        }
        t0 = AE_TRUNCA32X2F64S(a0,a1,  16);
        t1 = AE_TRUNCA32X2F64S(a2,a3,  16);
        XX = AE_ROUND16X4F32SASYM(t0, t1);
        AE_S16X4_IP(XX,pR,sizeof(ae_int16x4));
    }
}
#else
{
    /* code with Quad MAC option */
    //
    // Circular convolution algorithm:
    //
    //   r[n] = sum( x[mod(n-m,N)]*y[m] )
    //        m=0..M-1
    //
    //   where n = 0..N-1
    //
    const int16_t    * xn;
    const ae_int16x4 * pX0;
    const ae_int16x4 * pX1;
    const ae_int16x4 * pX2;
    const ae_int16x4 * pX3;
    const ae_int16x4 * pX3_1;
    const ae_int16x4 * pY;

    ae_int16x4 * restrict pR;

    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 y0;
    ae_f32x2   t0, t1;

    ae_valign ar0, ar1, ar2;

    int n, m;

    NASSERT(r);
    NASSERT(x);
    NASSERT(y);
    NASSERT_ALIGN(r, 8);
    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0 && M % 4 == 0);
    NASSERT(N > 0 && N % 4 == 0);

    //
    // Setup pointers and circular addressing for array x[N].
    //
    xn = x;
    pR = (ae_int16x4 *)r;

    WUR_AE_CBEGIN0((uintptr_t)(x + 0));
    WUR_AE_CEND0((uintptr_t)(x + N));

    for (n = 0; n < (N&~7); n += 8){
        pX0 = (const ae_int16x4 *)(xn + 4);
        pX1 = (const ae_int16x4 *)(xn + 5);
        pX2 = (const ae_int16x4 *)(xn + 6);
        pX3 = (const ae_int16x4 *)(xn + 4);

        xn += 8;

        AE_LA16X4NEG_PC(ar0, pX0);
        AE_LA16X4NEG_PC(ar1, pX1);
        AE_LA16X4NEG_PC(ar2, pX2);
        
        AE_LA16X4_RIC(x0, ar0, pX0);
        AE_LA16X4_RIC(x1, ar1, pX1);
        AE_LA16X4_RIC(x2, ar2, pX2);
        AE_L16X4_RIC(x3, pX3);

        pY = (const ae_int16x4 *)y;
        AE_L16X4_IP(y0, pY, 8);

        q4 = AE_MULZAAAAQ16(x0, y0);
        q5 = AE_MULZAAAAQ16(x1, y0);
        q6 = AE_MULZAAAAQ16(x2, y0);
        q7 = AE_MULZAAAAQ16(x3, y0);

        pX3_1 = pX3;

        AE_LA16X4_RIC(x0, ar0, pX0);
        AE_LA16X4_RIC(x1, ar1, pX1);
        AE_LA16X4_RIC(x2, ar2, pX2);
        AE_L16X4_RIC(x3, pX3_1);

        q0 = AE_MULZAAAAQ16(x0, y0);
        q1 = AE_MULZAAAAQ16(x1, y0);
        q2 = AE_MULZAAAAQ16(x2, y0);
        q3 = AE_MULZAAAAQ16(x3, y0);

        for (m = 0; m < (M >> 2) - 1; m++){
            AE_L16X4_RIC(x3, pX3);
            AE_L16X4_IP(y0, pY, 8);

            AE_MULAAAAQ16(q4, x0, y0);
            AE_MULAAAAQ16(q5, x1, y0);
            AE_MULAAAAQ16(q6, x2, y0);
            AE_MULAAAAQ16(q7, x3, y0);

            AE_L16X4_RIC(x3, pX3_1);
            AE_LA16X4_RIC(x0, ar0, pX0);
            AE_LA16X4_RIC(x1, ar1, pX1);
            AE_LA16X4_RIC(x2, ar2, pX2);

            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_S16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), pR, 8);

        t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
        AE_S16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), pR, 8);
    }
    
    if (N & 4){
        pX0 = (const ae_int16x4 *)(xn + 0);
        pX1 = (const ae_int16x4 *)(xn + 1);
        pX2 = (const ae_int16x4 *)(xn + 2);
        pX3 = (const ae_int16x4 *)(xn);

        AE_LA16X4NEG_PC(ar0, pX0);
        AE_LA16X4NEG_PC(ar1, pX1);
        AE_LA16X4NEG_PC(ar2, pX2);

        AE_L16X4_RIC(x3, pX3);
        AE_LA16X4_RIC(x0, ar0, pX0);
        AE_LA16X4_RIC(x1, ar1, pX1);
        AE_LA16X4_RIC(x2, ar2, pX2);

        pY = (const ae_int16x4 *)y;
        AE_L16X4_IP(y0, pY, 8);

        q0 = AE_MULZAAAAQ16(x0, y0);
        q1 = AE_MULZAAAAQ16(x1, y0);
        q2 = AE_MULZAAAAQ16(x2, y0);
        q3 = AE_MULZAAAAQ16(x3, y0);

        for (m = 0; m < (M >> 2) - 1; m++){
            AE_L16X4_RIC(x3, pX3);
            AE_LA16X4_RIC(x0, ar0, pX0);
            AE_LA16X4_RIC(x1, ar1, pX1);
            AE_LA16X4_RIC(x2, ar2, pX2);

            AE_L16X4_IP(y0, pY, 8);
            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_S16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), pR, 8);
    }
} /* fir_convol16x16() */
#endif

