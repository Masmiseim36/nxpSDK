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
#include "raw_corr32x32.h"

/*-----------------------------------------------------
    raw correlation:
    Input:
    x[N+M-1] padded with extra 3 zeroes
    y[M]
    Output:
    r[N]
    restriction:
    M should be a multiple of 2 and >0
-----------------------------------------------------*/
void raw_corr32x32(int32_t * r, const int32_t * restrict x, const int32_t * restrict y, int N, int M){
    const ae_f32x2 *          X;
    const ae_f32x2 *          S;
    const ae_f32x2 *          Y;
    ae_f32x2 * restrict R;

    ae_f64     q0, q1, q2, q3;
    ae_int32x2 t0, t1, t2;
    ae_f32x2 x0, x1, x2;
    ae_f32x2 y0;
    ae_valign ar;

    int n, m;

    NASSERT_ALIGN(x, 8);
    NASSERT_ALIGN(y, 8);
    NASSERT(M > 0 && M % 2 == 0);

    /* compute N&~3 correlation results */
    ar = AE_ZALIGN64();

    X = (const ae_f32x2 *)x;
    R = (ae_f32x2 *)r;

    for (n = 0; n<(N&~3); n += 4)
    {
        AE_L32X2_IP(t0, X, 8); x0 = (t0);
        AE_L32X2_IP(t1, X, 8); x1 = (t1);
        S = X;
        AE_L32X2_IP(t2, S, 8); x2 = (t2);

        Y = (const ae_f32x2 *)y;

        AE_L32X2_IP(t0, Y, 8); y0 = (t0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q0 = AE_MULF32R_HH(x0, y0);
        AE_MULAF32R_LL(q0, x0, y0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q1 = AE_MULF32R_LH(x0, y0);
        AE_MULAF32R_LH(q1, y0, x1);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q2 = AE_MULF32R_HH(x1, y0);
        AE_MULAF32R_LL(q2, x1, y0);

        // Q16.47 <- Q16.47 + [ Q31*Q31 - 15 ] w/ symmetric rounding
        q3 = AE_MULF32R_LH(x1, y0);
        AE_MULAF32R_LH(q3, y0, x2);

       // __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 1) - 1; m++){

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

        // Convert and save 4 outputs.
        // 2xQ31 <- 2xQ16.47 - 16 w/ rounding and saturation.
        x0 = AE_ROUND32X2F48SASYM(q0, q1); AE_SA32X2_IP(x0, ar, R);
        x1 = AE_ROUND32X2F48SASYM(q2, q3); AE_SA32X2_IP(x1, ar, R);
    }
    AE_SA64POS_FP(ar, R);
    /* process last 1...3 samples */
    N &= 3;
    if (N)
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

       // __Pragma("loop_count min=1")
        for (m = 0; m < (M >> 1) - 1; m++){

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
        if (N>1) AE_S32_L_IP(x0, castxcc(ae_int32, R), 4);
        x0 = AE_ROUND32X2F48SASYM(q2, q2);
        if (N>2) AE_S32_L_IP(AE_INTSWAP(x0), castxcc(ae_int32, R), 4);
    }
}
