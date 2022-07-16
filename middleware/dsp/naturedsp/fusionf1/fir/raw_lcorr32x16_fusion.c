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
#include "raw_lcorr32x16.h"

/*-----------------------------------------------------
raw correlation:
Input:
x[N] padded with extra 3 zeroes
y[M]
Output:
r[N]
restriction:
M should be a multiple of 2 and >0
-----------------------------------------------------*/
void raw_lcorr32x16(int32_t* r, const int32_t* restrict x, const int16_t* restrict y, int N, int M)
{
    const ae_f32x2 *          X0;
    const ae_f32x2 *          X1;
    const ae_f32x2 *          S;
    const ae_f16x4 *          Y;
    ae_f32x2 * restrict R;

    int n, m;

    ae_f64     q0, q1, q2, q3;
    ae_int32x2 x01, x12, x23, x34, x45;
    ae_int16x4 y0;

    ae_valign  ar, ax;

    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0);
    NASSERT(N > 0);

    /*
    * Compute first M-1 entries.
    */
    {
        const int16_t * pY = y + M - 4;
        ae_valign ay;

        ar = AE_ZALIGN64();
        R = (ae_f32x2 *)(r);

        for (n = 0; n < ((M - 1)&~3); n += 4, pY -= 4){
            q1 = q3 = AE_ZERO();

            X0 = (const ae_f32x2 *)(x + 1);
            X1 = (const ae_f32x2 *)(x + 0);

            ax = AE_LA64_PP(X0);

            AE_L32X2_IP(x01, X1, 8);
            AE_L32X2_IP(x23, X1, 8);

            AE_LA32X2_IP(x12, ax, X0);

            Y = (const ae_f16x4 *)(pY);
            ay = AE_LA64_PP(Y);
            AE_LA16X4_IP(y0, ay, Y);

            q0 = AE_MULF32X16_H0(x01, y0);

            AE_MULAAFD32X16_H1_L0(q1, x01, y0);

            q2 = AE_MULF32X16_H2(x01, y0);
            AE_MULAAFD32X16_H1_L0(q2, x12, y0);

            AE_MULAAFD32X16_H3_L2(q3, x01, y0);
            AE_MULAAFD32X16_H1_L0(q3, x23, y0);

            for (m = 0; m < (n >> 2); m++){
                AE_LA16X4_IP(y0, ay, Y);

                AE_LA32X2_IP(x34, ax, X0);
                AE_L32X2_IP(x45, X1, 8);

                AE_MULAAFD32X16_H3_L2(q0, x12, y0);
                AE_MULAAFD32X16_H1_L0(q0, x34, y0);

                AE_MULAAFD32X16_H3_L2(q1, x23, y0);
                AE_MULAAFD32X16_H1_L0(q1, x45, y0);

                x12 = x34; x23 = x45;

                AE_LA32X2_IP(x34, ax, X0);
                AE_L32X2_IP(x45, X1, 8);

                AE_MULAAFD32X16_H3_L2(q2, x12, y0);
                AE_MULAAFD32X16_H1_L0(q2, x34, y0);

                AE_MULAAFD32X16_H3_L2(q3, x23, y0);
                AE_MULAAFD32X16_H1_L0(q3, x45, y0);

                x12 = x34; x23 = x45;
            }

            x01 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x01, ar, R);
            x01 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x01, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        if ((M - 1) & 3)
        {
            q1 = AE_ZERO();

            X0 = (const ae_f32x2 *)(x + 1);
            X1 = (const ae_f32x2 *)(x + 0);

            ax = AE_LA64_PP(X0);

            AE_L32X2_IP(x01, X1, 8);
            AE_L32X2_IP(x23, X1, 8);

            AE_LA32X2_IP(x12, ax, X0);

            Y = (const ae_f16x4 *)(pY);
            ay = AE_LA64_PP(Y);
            AE_LA16X4_IP(y0, ay, Y);

            q0 = AE_MULF32X16_H0(x01, y0);

            AE_MULAAFD32X16_H1_L0(q1, x01, y0);

            q2 = AE_MULF32X16_H2(x01, y0);
            AE_MULAAFD32X16_H1_L0(q2, x12, y0);

            for (m = 0; m < (n >> 2); m++){
                AE_LA16X4_IP(y0, ay, Y);

                AE_LA32X2_IP(x34, ax, X0);
                AE_L32X2_IP(x45, X1, 8);

                AE_MULAAFD32X16_H3_L2(q0, x12, y0);
                AE_MULAAFD32X16_H1_L0(q0, x34, y0);

                AE_MULAAFD32X16_H3_L2(q1, x23, y0);
                AE_MULAAFD32X16_H1_L0(q1, x45, y0);

                x12 = x34; x23 = x45;

                AE_LA32X2_IP(x34, ax, X0);
                AE_L32X2_IP(x45, X1, 8);

                AE_MULAAFD32X16_H3_L2(q2, x12, y0);
                AE_MULAAFD32X16_H1_L0(q2, x34, y0);

                x12 = x34; x23 = x45;
            }

            AE_S32RA64S_IP(q0, castxcc(ae_f32, R), sizeof(int32_t));
            if (((M - 1) & 3)>1) AE_S32RA64S_IP(q1, castxcc(ae_f32, R), sizeof(int32_t));
            if (((M - 1) & 3)>2) AE_S32RA64S_IP(q2, castxcc(ae_f32, R), sizeof(int32_t));
        }
    }

    /*
    * Compute r[M]...r[MIN(N + M - 1,(N + 1)&~1] entries.
    */
    {
        int n_iter = XT_MIN(N, ((N - (M - 1)) + 1)&~1);
        int m_iter = ((M + 1) >> 1);

        R = (ae_f32x2 *)(r + M - 1);
        X0 = (const ae_f32x2 *)(x)-1;

        ar = AE_ZALIGN64();

        for (n = 0; n<(n_iter&~3); n += 4)
        {
            q0 = q1 = q2 = q3 = AE_ZERO();

            X0 = X0 + 1;
            X1 = (const ae_f32x2 *)(((int32_t *)X0) + 1);

            AE_L32X2_IP(x01, X0, 8);
            S = X0;
            ax = AE_LA64_PP(X1);
            AE_LA32X2_IP(x12, ax, X1);

            Y = (const ae_f16x4 *)y;

            __Pragma("loop_count min=1")
            for (m = 0; m < m_iter; m++){
                ae_int32x2 t;
                AE_L32_IP(t, castxcc(ae_int32, Y), sizeof(int32_t)); y0 = AE_MOVF16X4_FROMINT32X2(t);

                AE_MULAAFD32X16_H2_L3(q0, x01, y0);
                AE_MULAAFD32X16_H2_L3(q1, x12, y0);
                AE_L32X2_IP(x01, S, 8);
                AE_LA32X2_IP(x12, ax, X1);
                AE_MULAAFD32X16_H2_L3(q2, x01, y0);
                AE_MULAAFD32X16_H2_L3(q3, x12, y0);
            }

            x01 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x01, ar, R);
            x01 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x01, ar, R);
        }
        AE_SA64POS_FP(ar, R);
        n_iter &= 3;
        if (n_iter)
        {
            q0 = q1 = q2 = AE_ZERO();

            X0 = X0 + 1;
            X1 = (const ae_f32x2 *)(((int32_t *)X0) + 1);

            AE_L32X2_IP(x01, X0, 8);
            S = X0;
            ax = AE_LA64_PP(X1);
            AE_LA32X2_IP(x12, ax, X1);

            Y = (const ae_f16x4 *)y;

            __Pragma("loop_count min=1")
            for (m = 0; m < m_iter; m++){
                ae_int32x2 t;
                AE_L32_IP(t, castxcc(ae_int32, Y), sizeof(int32_t)); y0 = AE_MOVF16X4_FROMINT32X2(t);

                AE_MULAAFD32X16_H2_L3(q0, x01, y0);
                AE_MULAAFD32X16_H2_L3(q1, x12, y0);
                AE_L32X2_IP(x01, S, 8);
                AE_LA32X2_IP(x12, ax, X1);
                AE_MULAAFD32X16_H2_L3(q2, x01, y0);
            }

            AE_S32RA64S_IP(q0, castxcc(ae_f32, R), sizeof(int32_t));
            if (n_iter>1) AE_S32RA64S_IP(q1, castxcc(ae_f32, R), sizeof(int32_t));
            if (n_iter>2) AE_S32RA64S_IP(q2, castxcc(ae_f32, R), sizeof(int32_t));
        }
    }

    /*
    * Compute r[MIN(N + M - 1,(N + 1)&~1]....r[N+M-1] entries.
    */
    {
        int offset = 1 - (((N - (M - 1)) + 1) & 1);
        int n_iter = XT_MAX(0, (M - 1) - offset);
        int m_iter = ((M - offset) >> 1) - 2;

        ae_int16x4 y1;
        ae_int32x2 t;

        X0 = (const ae_f32x2 *)(x + N - (M - 1) + offset) - 1;
        R = (ae_f32x2 *)(r + N + offset);

        ar = AE_ZALIGN64();

        for (n = 0; n < (n_iter&~3); n += 4, m_iter -= 2){
            q0 = q1 = q2 = q3 = AE_ZERO();

            X0 = X0 + 1;
            X1 = (const ae_f32x2 *)(((int32_t *)X0) + 1);

            AE_L32X2_IP(x01, X0, 8);
            S = X0;
            ax = AE_LA64_PP(X1);
            AE_LA32X2_IP(x12, ax, X1);

            Y = (const ae_f16x4 *)y;

            for (m = 0; m < m_iter; m++){
                AE_L32_IP(t, castxcc(ae_int32, Y), sizeof(int32_t)); y0 = AE_MOVF16X4_FROMINT32X2(t);

                AE_MULAAFD32X16_H2_L3(q0, x01, y0);
                AE_MULAAFD32X16_H2_L3(q1, x12, y0);
                AE_L32X2_IP(x01, S, 8);
                AE_LA32X2_IP(x12, ax, X1);
                AE_MULAAFD32X16_H2_L3(q2, x01, y0);
                AE_MULAAFD32X16_H2_L3(q3, x12, y0);
            }

            AE_L32_IP(t, castxcc(ae_int32, Y), sizeof(int32_t)); y0 = AE_MOVF16X4_FROMINT32X2(t);
            AE_L32_IP(t, castxcc(ae_int32, Y), sizeof(int32_t)); y1 = AE_MOVF16X4_FROMINT32X2(t);

            AE_MULAAFD32X16_H2_L3(q0, x01, y0);
            AE_MULAAFD32X16_H2_L3(q1, x12, y0);

            AE_L32X2_IP(x01, S, 8);

            AE_MULAAFD32X16_H2_L3(q2, x01, y0);
            AE_MULAF32X16_L2(q3, x01, y0);

            AE_MULAAFD32X16_H2_L3(q0, x01, y1);
            AE_MULAF32X16_L2(q1, x01, y1);

            x01 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x01, ar, R);
            x01 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x01, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        n_iter &= 3;
        if (n_iter){
            q0 = q1 = AE_ZERO();

            X0 = X0 + 1;
            X1 = (const ae_f32x2 *)(((int32_t *)X0) + 1);
            ax = AE_LA64_PP(X1);

            AE_L32X2_IP(x01, X0, 8);
            AE_LA32X2_IP(x12, ax, X1);

            Y = (const ae_f16x4 *)y;
            AE_L16X4_IP(y0, Y, 8);

            AE_MULAAFD32X16_H3_L2(q0, x01, y0);
            AE_MULAF32X16_L1(q0, x12, y0);

            AE_MULAAFD32X16_H3_L2(q1, x12, y0);

            q2 = AE_MULF32X16_L3(x12, y0);

            AE_S32RA64S_IP(q0, castxcc(ae_f32, R), sizeof(int32_t));
            if (n_iter>1) AE_S32RA64S_IP(q1, castxcc(ae_f32, R), sizeof(int32_t));
            if (n_iter>2) AE_S32RA64S_IP(q2, castxcc(ae_f32, R), sizeof(int32_t));
        }
    }
}
