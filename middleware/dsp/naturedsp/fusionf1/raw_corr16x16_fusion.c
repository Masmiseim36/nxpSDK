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
    helper for correlation/convolution
    C code optimized for Fusion
*/
/* Portable data types. */
#include "NatureDSP_types.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "raw_corr16x16.h"

/*-----------------------------------------------------
    raw correlation:
    Input:
    x[N+M-1] padded with extra 3 zeroes
    y[M]
    Output:
    r[N]
    restriction:
    M should be a multiple of 4 and >0
-----------------------------------------------------*/
void raw_corr16x16(int16_t * r, const int16_t * restrict x, const int16_t * restrict y, int N, int M)
#if !(XCHAL_HAVE_FUSION_16BIT_BASEBAND)
{
        /* code with no Quad MAC support */
    const ae_int16x4 * restrict pX0;
    const ae_int16x4 * restrict pX1;
    const ae_int16x4 * restrict pX2;
    const ae_int16x4 * restrict pX3;
    const ae_int16x4 * restrict pY;
          ae_int16x4 * restrict pR;
    ae_int16x4 X0123,X1234,X2345,X3456;
    ae_int16x4 XX;
    ae_int32x2 t0,t1;
    ae_int16x4 Y0123;
    ae_f64 a0,a1,a2,a3;
    ae_valign ax1,ax2,ax3,ar;
    int n, m;

    if (N<0) return;
    pR=(ae_int16x4*)r;
    ar=AE_ZALIGN64();
    for ( n=0; n<(N&~3); n+=4)
    {
        a0=a1=a2=a3=AE_ZERO();
        pX0=(const ae_int16x4*)(x+n);
        pX1=(const ae_int16x4*)(x+n+1);
        pX2=(const ae_int16x4*)(x+n+2);
        pX3=(const ae_int16x4*)(x+n+3);
        ax1=AE_LA64_PP(pX1);
        ax2=AE_LA64_PP(pX2);
        ax3=AE_LA64_PP(pX3);
        pY =(const ae_int16x4*)y;
        __Pragma("loop_count min=1")
        for ( m=0; m<(M>>2); m++)
        {
            AE_L16X4_IP (X0123,pX0,sizeof(ae_int16x4));
            AE_LA16X4_IP(X1234,ax1,pX1);
            AE_LA16X4_IP(X2345,ax2,pX2);
            AE_LA16X4_IP(X3456,ax3,pX3);
            AE_L16X4_IP(Y0123,pY,sizeof(ae_int16x4));
            t0 = AE_CVT32X2F16_32(Y0123);
            t1 = AE_CVT32X2F16_10(Y0123);

            AE_MULAAFD32X16_H3_L2(a0,t0,X0123);
            AE_MULAAFD32X16_H3_L2(a1,t0,X1234);
            AE_MULAAFD32X16_H3_L2(a2,t0,X2345);
            AE_MULAAFD32X16_H3_L2(a3,t0,X3456);

            AE_MULAAFD32X16_H1_L0(a0,t1,X0123);
            AE_MULAAFD32X16_H1_L0(a1,t1,X1234);
            AE_MULAAFD32X16_H1_L0(a2,t1,X2345);
            AE_MULAAFD32X16_H1_L0(a3,t1,X3456);
        }
        t0 = AE_TRUNCA32X2F64S(a0,a1,  16);
        t1 = AE_TRUNCA32X2F64S(a2,a3,  16);
        XX = AE_ROUND16X4F32SASYM(t0, t1);
        AE_SA16X4_IP(XX,ar,pR);
    }
    AE_SA64POS_FP(ar, pR);
    N&=3;
    if (N)  /* last 1..3 output samples */
    {
        a0=a1=a2=a3=AE_ZERO();
        pX0=(const ae_int16x4*)(x+n);
        pX1=(const ae_int16x4*)(x+n+1);
        pX2=(const ae_int16x4*)(x+n+2);
        pX3=(const ae_int16x4*)(x+n+3);
        ax1=AE_LA64_PP(pX1);
        ax2=AE_LA64_PP(pX2);
        ax3=AE_LA64_PP(pX3);
        pY =(const ae_int16x4*)y;
        __Pragma("loop_count min=1")
        for ( m=0; m<(M>>2); m++)
        {
            AE_L16X4_IP (X0123,pX0,sizeof(ae_int16x4));
            AE_LA16X4_IP(X1234,ax1,pX1);
            AE_LA16X4_IP(X2345,ax2,pX2);
            AE_LA16X4_IP(X3456,ax3,pX3);
            AE_L16X4_IP(Y0123,pY,sizeof(ae_int16x4));
            t0 = AE_CVT32X2F16_32(Y0123);
            t1 = AE_CVT32X2F16_10(Y0123);

            AE_MULAAFD32X16_H3_L2(a0,t0,X0123);
            AE_MULAAFD32X16_H3_L2(a1,t0,X1234);
            AE_MULAAFD32X16_H3_L2(a2,t0,X2345);
            AE_MULAAFD32X16_H3_L2(a3,t0,X3456);

            AE_MULAAFD32X16_H1_L0(a0,t1,X0123);
            AE_MULAAFD32X16_H1_L0(a1,t1,X1234);
            AE_MULAAFD32X16_H1_L0(a2,t1,X2345);
            AE_MULAAFD32X16_H1_L0(a3,t1,X3456);
        }
        t0 = AE_TRUNCA32X2F64S(a3,a2,  16);
        t1 = AE_TRUNCA32X2F64S(a1,a0,  16);
        XX = AE_ROUND16X4F32SASYM(t0, t1);
        AE_S16_0_IP(XX, castxcc(ae_int16, pR), +2);
        if (N > 1) { XX = AE_SEL16_4321(XX, XX); AE_S16_0_IP(XX, castxcc(ae_int16, pR), +2); }
        if (N > 2) { XX = AE_SEL16_4321(XX, XX); AE_S16_0_IP(XX, castxcc(ae_int16, pR), +2); }
    }
}
#else
{
        /* code with Quad MAC support */
    const ae_int16x4 * pX0;
    const ae_int16x4 * pX1;
    const ae_int16x4 * pX2;
    const ae_int16x4 * pX3;
    const ae_int16x4 * S0;
    const ae_int16x4 * S0_1;
    const ae_int16x4 * pY;

    ae_int16x4 * restrict pR;

    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 y0;
    ae_f32x2   t0, t1;

    ae_valign ar1, ar2, ar3, ar;

    int n, m;

    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0 && M % 2 == 0);

    /* compute N&~3 correlation results */
    ar = AE_ZALIGN64();

    pX0 = (const ae_int16x4 *)x;
    pR = (ae_int16x4 *)r;

    for (n = 0; n < (N&~7); n += 8){
        pX1 = (const ae_int16x4 *)((int16_t *) pX0 + 1);
        pX2 = (const ae_int16x4 *)((int16_t *) pX0 + 2);
        pX3 = (const ae_int16x4 *)((int16_t *) pX0 + 3);

        ar1 = AE_LA64_PP(pX1);
        ar2 = AE_LA64_PP(pX2);
        ar3 = AE_LA64_PP(pX3);

        AE_L16X4_IP(x0, pX0, +8);
        AE_LA16X4_IP(x1, ar1, pX1);
        AE_LA16X4_IP(x2, ar2, pX2);
        AE_LA16X4_IP(x3, ar3, pX3);

        S0 = pX0;

        pY = (const ae_int16x4 *)y;
        AE_L16X4_IP(y0, pY, 8);

        q0 = AE_MULZAAAAQ16(x0, y0);
        q1 = AE_MULZAAAAQ16(x1, y0);
        q2 = AE_MULZAAAAQ16(x2, y0);
        q3 = AE_MULZAAAAQ16(x3, y0);

        AE_L16X4_IP(x0, pX0, +8);
        S0_1 = pX0;
        AE_LA16X4_IP(x1, ar1, pX1);
        AE_LA16X4_IP(x2, ar2, pX2);
        AE_LA16X4_IP(x3, ar3, pX3);

        q4 = AE_MULZAAAAQ16(x0, y0);
        q5 = AE_MULZAAAAQ16(x1, y0);
        q6 = AE_MULZAAAAQ16(x2, y0);
        q7 = AE_MULZAAAAQ16(x3, y0);

        for (m = 0; m < (M >> 2) - 1; m++){

            AE_L16X4_IP(x0, S0, +8);
            AE_L16X4_IP(y0, pY, 8);

            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);

            AE_L16X4_IP(x0, S0_1, 8);
            AE_LA16X4_IP(x1, ar1, pX1);
            AE_LA16X4_IP(x2, ar2, pX2);
            AE_LA16X4_IP(x3, ar3, pX3);

            AE_MULAAAAQ16(q4, x0, y0);
            AE_MULAAAAQ16(q5, x1, y0);
            AE_MULAAAAQ16(q6, x2, y0);
            AE_MULAAAAQ16(q7, x3, y0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, pR);

        t0 = AE_TRUNCA32X2F64S(q4, q5, 33);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, pR);
    }
    /* process last 1...7 samples */
    N &= 7;
    if (N & 4){
        pX1 = (const ae_int16x4 *)((int16_t *)pX0 + 1);
        pX2 = (const ae_int16x4 *)((int16_t *)pX0 + 2);
        pX3 = (const ae_int16x4 *)((int16_t *)pX0 + 3);

        ar1 = AE_LA64_PP(pX1);
        ar2 = AE_LA64_PP(pX2);
        ar3 = AE_LA64_PP(pX3);

        AE_L16X4_IP(x0, pX0, +8);
        AE_LA16X4_IP(x1, ar1, pX1);
        AE_LA16X4_IP(x2, ar2, pX2);
        AE_LA16X4_IP(x3, ar3, pX3);

        S0 = pX0;

        pY = (const ae_int16x4 *)y;
        AE_L16X4_IP(y0, pY, 8);

        q0 = AE_MULZAAAAQ16(x0, y0);
        q1 = AE_MULZAAAAQ16(x1, y0);
        q2 = AE_MULZAAAAQ16(x2, y0);
        q3 = AE_MULZAAAAQ16(x3, y0);

        for (m = 0; m < (M >> 2) - 1; m++){
            AE_L16X4_IP(x0, S0, +8);
            AE_LA16X4_IP(x1, ar1, pX1);
            AE_LA16X4_IP(x2, ar2, pX2);
            AE_LA16X4_IP(x3, ar3, pX3);

            AE_L16X4_IP(y0, pY, 8);
            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), ar, pR);
    }
    AE_SA64POS_FP(ar, pR);

    N &= 3;
    if (N)
    {
        pX1 = (const ae_int16x4 *)((int16_t *)pX0 + 1);
        pX2 = (const ae_int16x4 *)((int16_t *)pX0 + 2);

        ar1 = AE_LA64_PP(pX1);
        ar2 = AE_LA64_PP(pX2);

        AE_L16X4_IP(x0, pX0, +8);
        AE_LA16X4_IP(x1, ar1, pX1);
        AE_LA16X4_IP(x2, ar2, pX2);

        pY = (const ae_int16x4 *)y;
        AE_L16X4_IP(y0, pY, 8);

        q0 = AE_MULZAAAAQ16(x0, y0);
        q1 = AE_MULZAAAAQ16(x1, y0);
        q2 = AE_MULZAAAAQ16(x2, y0);

        for (m = 0; m < (M >> 2) - 1; m++){
            AE_L16X4_IP(x0, pX0, +8);
            AE_LA16X4_IP(x1, ar1, pX1);
            AE_LA16X4_IP(x2, ar2, pX2);

            AE_L16X4_IP(y0, pY, 8);
            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
        }

        t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q2, 33);
        x0 = AE_ROUND16X4F32SASYM(t1, t0);

        AE_S16_0_IP(x0, castxcc(ae_int16, pR), +2);
        if (N > 1) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, pR), +2); }
        if (N > 2) { x0 = AE_SEL16_4321(x0, x0); AE_S16_0_IP(x0, castxcc(ae_int16, pR), +2); }
    }
} /* raw_corr16x16() */
#endif
