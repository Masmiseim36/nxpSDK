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
    helper for correlation/convolution
    C code optimized for HiFi4
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
{
    const ae_int16x4 * pX0;
    const ae_int16x4 * pX1;
    const ae_int16x4 * pX2;
    const ae_int16x4 * pX3;
    const ae_int16x4 * S;
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
    NASSERT(M > 0 && M % 4 == 0);

    /* compute N&~3 correlation results */
    ar = AE_ZALIGN64();

    pX0 = (const ae_int16x4 *)x;
    pR = (ae_int16x4 *)r;

    for (n = 0; n < (N>>3); n++)
    {
        pY = (const ae_int16x4 *)y;
        pX1 = (const ae_int16x4 *)((int16_t *) pX0 + 1);
        pX2 = (const ae_int16x4 *)((int16_t *) pX0 + 2);
        pX3 = (const ae_int16x4 *)((int16_t *) pX0 + 3);

        ar1 = AE_LA64_PP(pX1);
        ar2 = AE_LA64_PP(pX2);
        ar3 = AE_LA64_PP(pX3);

        q0 = q1 = q2 = q3 = q4 = q5 = q6 = q7 = AE_ZERO64();

        AE_L16X4_IP(x0, pX0, +8);
        AE_LA16X4_IP(x1, ar1, pX1);
        AE_LA16X4_IP(x2, ar2, pX2);
        AE_LA16X4_IP(x3, ar3, pX3);

        S = pX0;
        pX0 = (const ae_int16x4 *)XT_ADDX8(1, (uintptr_t)pX0);

        __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 2); m++)
        {
            AE_L16X4_IP(y0, pY, 8);
                
            AE_MULAAAAQ16(q0, y0, x0);
            AE_MULAAAAQ16(q1, y0, x1);
            AE_MULAAAAQ16(q2, y0, x2);
            AE_MULAAAAQ16(q3, y0, x3);
                
            AE_L16X4_IP(x0, S, +8);
            AE_LA16X4_IP(x1, ar1, pX1);
            AE_LA16X4_IP(x2, ar2, pX2);
            AE_LA16X4_IP(x3, ar3, pX3);
                
            AE_MULAAAAQ16(q4, y0, x0);
            AE_MULAAAAQ16(q5, y0, x1);
            AE_MULAAAAQ16(q6, y0, x2);
            AE_MULAAAAQ16(q7, y0, x3);
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
    if (N & 4)
    {
        pX1 = (const ae_int16x4 *)((int16_t *)pX0 + 1);
        pX2 = (const ae_int16x4 *)((int16_t *)pX0 + 2);
        pX3 = (const ae_int16x4 *)((int16_t *)pX0 + 3);

        ar1 = AE_LA64_PP(pX1);
        ar2 = AE_LA64_PP(pX2);
        ar3 = AE_LA64_PP(pX3);

        pY = (const ae_int16x4 *)y;
        S = pX0;

        q0 = q1 = q2 = q3 = AE_ZERO64();

        __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 2); m++)
        {
            AE_L16X4_IP(x0, S, +8);
            AE_LA16X4_IP(x1, ar1, pX1);
            AE_LA16X4_IP(x2, ar2, pX2);
            AE_LA16X4_IP(x3, ar3, pX3);

            AE_L16X4_IP(y0, pY, 8);

            AE_MULAAAAQ16(q0, x0, y0);
            AE_MULAAAAQ16(q1, x1, y0);
            AE_MULAAAAQ16(q2, x2, y0);
            AE_MULAAAAQ16(q3, x3, y0);
        }
        pX0 = (const ae_int16x4 *)XT_ADDX8(1, (uintptr_t)pX0);

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

        pY = (const ae_int16x4 *)y;

        q0 = q1 = q2 = AE_ZERO64();

        __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 2); m++)
        {
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
