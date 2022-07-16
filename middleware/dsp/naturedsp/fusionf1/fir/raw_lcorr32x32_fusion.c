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
#include "raw_lcorr32x32.h"

/*-----------------------------------------------------
    raw linear correlation:
    Input:
    x[N] padded with extra 3 zeroes
    y[M] padded with extra 3 zeroes
    Output:
    r[N+M-1]
    restriction:
    M should be >0
-----------------------------------------------------*/
void fir_lxcorr32x32(int32_t * r, const int32_t * restrict x, const int32_t * restrict y, int N, int M)
{
    const ae_f32x2 *          X;
    const ae_f32x2 *          S;
    const ae_f32x2 *          Y;
    ae_f32x2 * restrict R;

    int n, m;

    ae_f64     q0, q1, q2, q3;
    ae_int32x2 t0, t1, t2, t3;
    ae_f32x2   x0, x1, x2, x3;
    ae_f32x2   y0, y1;

    ae_valign  ar;

    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0);
    NASSERT(N > 0);

    /*
    * Compute first M-1 entries.
    */
    {
        ae_valign  ax, ay;
        const int32_t * pY = y + M - 4;

        ar = AE_ZALIGN64();
        R = (ae_f32x2 *)(r);

        for (n = 0; n < ((M - 1)&~3); n += 4, pY -= 4){
            X = (const ae_f32x2 *)(x);

            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);

            Y = (const ae_f32x2 *)(pY);
            ay = AE_LA64_PP(Y);
            AE_LA32X2_IP(t0, ay, Y); y0 = t0;
            AE_LA32X2_IP(t1, ay, Y); y1 = t1;

            q0 = AE_MULF32R_LH(y1, x0);

            q1 = AE_MULF32R_HH(x0, y1);
            AE_MULAF32R_LL(q1, x0, y1);

            q2 = AE_MULF32R_LH(y0, x0);
            AE_MULAF32R_LH(q2, x0, y1);
            AE_MULAF32R_LH(q2, y1, x1);

            q3 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q3, x0, y0);
            AE_MULAF32R_HH(q3, x1, y1);
            AE_MULAF32R_LL(q3, x1, y1);

            for (m = 0; m < (n >> 2); m++){
                AE_L32X2_IP(t2, X, 8);     x2 = (t2);
                AE_L32X2_IP(t3, X, 8);     x3 = (t3);

                AE_LA32X2_IP(t0, ay, Y);   y0 = (t0);
                AE_LA32X2_IP(t1, ay, Y);   y1 = (t1);

                AE_MULAF32R_LH(q0, x0, y0);
                AE_MULAF32R_LH(q0, y0, x1);
                AE_MULAF32R_LH(q0, x1, y1);
                AE_MULAF32R_LH(q0, y1, x2);

                AE_MULAF32R_HH(q1, x1, y0);
                AE_MULAF32R_LL(q1, x1, y0);
                AE_MULAF32R_HH(q1, x2, y1);
                AE_MULAF32R_LL(q1, x2, y1);

                AE_MULAF32R_LH(q2, x1, y0);
                AE_MULAF32R_LH(q2, y0, x2);
                AE_MULAF32R_LH(q2, x2, y1);
                AE_MULAF32R_LH(q2, y1, x3);

                AE_MULAF32R_HH(q3, x2, y0);
                AE_MULAF32R_LL(q3, x2, y0);
                AE_MULAF32R_HH(q3, x3, y1);
                AE_MULAF32R_LL(q3, x3, y1);
                x0 = x2;
                x1 = x3;
            }

            x0 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x0, ar, R);
            x0 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x0, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        for (n = ((M - 1)&~3); n < (M - 1); n++)
        {
            X = (const ae_f32x2 *)(x + 1);
            t0=AE_L32_I((const ae_int32*)X, -4); x0 = (t0);
            ax = AE_LA64_PP(X);

            Y = (const ae_f32x2 *)(y + (M - 2) - n);
            ay = AE_LA64_PP(Y);
            AE_LA32X2_IP(t0, ay, Y); y0 = (t0);

            q0 = AE_MULF32R_LL(x0, y0);

            for (m = 0; m < (n + 1) >> 1; m++){
                AE_LA32X2_IP(t0, ax, X); x0 = (t0);
                AE_LA32X2_IP(t0, ay, Y); y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);
            }
            x0 = AE_ROUND32X2F48SASYM(q0, q0);
            AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
        }
    }

    /*
    * Compute r[M]...r[MIN(N + M - 1,(N + 1)&~1] entries.
    */
    {
        int n_iter = XT_MIN(N, ((N - (M - 1)) + 1)&~1);
        int m_iter = ((M + 1) >> 1) - 1;

        ar = AE_ZALIGN64();

        X = (const ae_f32x2 *)x;
        R = (ae_f32x2 *)(r + M - 1);

        for (n = 0; n<(n_iter&~3); n += 4)
        {
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, 8); x2 = (t2);

            Y = (const ae_f32x2 *)y;

            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            q3 = AE_MULF32R_LH(x1, y0);
            AE_MULAF32R_LH(q3, y0, x2);

            //__Pragma("loop_count min=1")
            for (m = 0; m < m_iter; m++){

                x0 = x1; x1 = x2;
                AE_L32X2_IP(t2, S, 8); x2 = (t2);

                AE_L32X2_IP(t0, Y, 8);
                y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);

                AE_MULAF32R_LH(q1, x0, y0);
                AE_MULAF32R_LH(q1, y0, x1);

                AE_MULAF32R_HH(q2, x1, y0);
                AE_MULAF32R_LL(q2, x1, y0);

                AE_MULAF32R_LH(q3, x1, y0);
                AE_MULAF32R_LH(q3, y0, x2);
            }

            x0 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x0, ar, R);
            x1 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x1, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        n_iter &= 3;
        if (n_iter)
        {
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, 8); x2 = (t2);

            Y = (const ae_f32x2 *)y;

            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            //__Pragma("loop_count min=1")
            for (m = 0; m < m_iter; m++){

                x0 = x1; x1 = x2;
                AE_L32X2_IP(t2, S, 8); x2 = (t2);

                AE_L32X2_IP(t0, Y, 8);
                y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);

                AE_MULAF32R_LH(q1, x0, y0);
                AE_MULAF32R_LH(q1, y0, x1);

                AE_MULAF32R_HH(q2, x1, y0);
                AE_MULAF32R_LL(q2, x1, y0);
            }
            x0 = AE_ROUND32X2F48SASYM(q0, q1);
            AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
            if (n_iter>1) AE_S32_L_IP(x0, castxcc(ae_int32, R), 4);
            x0 = AE_ROUND32X2F48SASYM(q2, q2);
            if (n_iter>2) AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
        }
    }

    /*
    * Compute r[MIN(N + M - 1,(N + 1)&~1]....r[N+M-1] entries.
    */
    {
        int offset = 1 - (((N - (M - 1)) + 1) & 1);
        int n_iter = XT_MAX(0, (M - 1) - offset);
        int m_iter = ((M - offset) >> 1) - 2;

        X = (const ae_f32x2 *)(x + N - (M - 1) + offset);
        R = (ae_f32x2 *)(r + N + offset);

        ar = AE_ZALIGN64();

        for (n = 0; n < (n_iter&~3); n += 4, m_iter -= 2){
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);
            S = X;
            AE_L32X2_IP(t2, S, 8); x2 = (t2);

            Y = (const ae_f32x2 *)(y);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            q3 = AE_MULF32R_LH(x1, y0);
            AE_MULAF32R_LH(q3, y0, x2);

            for (m = 0; m < m_iter; m++){
                x0 = x1; x1 = x2;
                AE_L32X2_IP(t2, S, 8); x2 = (t2);

                AE_L32X2_IP(t0, Y, 8);  y0 = (t0);

                AE_MULAF32R_HH(q0, x0, y0);
                AE_MULAF32R_LL(q0, x0, y0);

                AE_MULAF32R_LH(q1, x0, y0);
                AE_MULAF32R_LH(q1, y0, x1);

                AE_MULAF32R_HH(q2, x1, y0);
                AE_MULAF32R_LL(q2, x1, y0);

                AE_MULAF32R_LH(q3, x1, y0);
                AE_MULAF32R_LH(q3, y0, x2);
            }

            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            AE_MULAF32R_HH(q0, x1, y0);
            AE_MULAF32R_LL(q0, x1, y0);

            AE_MULAF32R_LH(q1, x1, y0);

            x0 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x0, ar, R);
            x0 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x0, ar, R);
        }
        AE_SA64POS_FP(ar, R);

        n_iter &= 3;
        if (n_iter){
            AE_L32X2_IP(t0, X, 8); x0 = (t0);
            AE_L32X2_IP(t1, X, 8); x1 = (t1);

            Y = (const ae_f32x2 *)(y);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);

            q1 = AE_MULF32R_LH(x0, y0);
            AE_MULAF32R_LH(q1, y0, x1);

            q2 = AE_MULF32R_HH(x1, y0);
            AE_MULAF32R_LL(q2, x1, y0);

            q0 = AE_MULF32R_HH(x0, y0);
            AE_MULAF32R_LL(q0, x0, y0);
            AE_L32X2_IP(t0, Y, 8); y0 = (t0);
            AE_MULAF32R_HH(q0, x1, y0);

            x0 = AE_ROUND32X2F48SASYM(q0, q1);
            AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
            if (n_iter>1) AE_S32_L_IP(x0, castxcc(ae_int32, R), 4);
            x0 = AE_ROUND32X2F48SASYM(q2, q2);
            if (n_iter>2) AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
        }
    }
}
