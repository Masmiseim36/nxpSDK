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
Interpolating block real FIR filter, 32x32-bit
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
#include "cxfirinterp16x16_common.h"


/* Generic data processing function for a decimating FIR filter. */
int cxfirinterp16x16_DX_proc(int16_t * restrict y,
                             int16_t * delayLine,
                             int       delayLen,
                       const int16_t * restrict x,
                       const int16_t * restrict h,
                             int wrIx, int D, int N, int M)
{
    const ae_int16x4 *          C;
    ae_int16x4 *          D_tmp;
    ae_int16x4 * restrict D_wr_r;
    ae_int16x4 * restrict D_wr_i;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd0_1;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16x4 *          X;
    ae_int16   * restrict Y;

    ae_valign D_va1, D_va2, D_va3;

    ae_int16x4 d01, d12, d23, d34;
    ae_int16x4 dr0, di0, dr1, di1;
    ae_int64   q0r, q1r, q2r, q3r;
    ae_int64   q4r, q5r, q6r, q7r;
    ae_int64   q0i, q1i, q2i, q3i;
    ae_int64   q4i, q5i, q6i, q7i;
    ae_int64   z;
    ae_int16x4 c0;
    ae_f32x2   t0, t1;

    ae_int32x2 g_frac;

    int m, n, d;

    NASSERT(y && x);
    NASSERT_ALIGN8(x);

    NASSERT(!(M & 3) && !(N & 7));
    NASSERT_ALIGN8(delayLine);
    NASSERT_ALIGN8(h);

    {
        int q_frac_int = D << (15);
        g_frac = AE_MOVDA32(q_frac_int);
    }

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr_r = (ae_int16x4*)(delayLine + wrIx);
    D_wr_i = (ae_int16x4*)(delayLine + delayLen + wrIx);
    X = (const ae_int16x4 *)x;

    z = AE_ZERO64();

    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 3); n++)
    {
        // Load 4 complex input samples.
        AE_L16X4_IP(d01, X, +8);
        AE_L16X4_IP(d12, X, +8);

        di0 = AE_SEL16_6420(d01, d12);
        d12 = AE_SEL16_4321(d12, d12);
        dr0 = AE_SEL16_7520(d01, d12);

        // Load 4 complex input samples.
        AE_L16X4_IP(d01, X, +8);
        AE_L16X4_IP(d12, X, +8);

        di1 = AE_SEL16_6420(d01, d12);
        d12 = AE_SEL16_4321(d12, d12);
        dr1 = AE_SEL16_7520(d01, d12);

        // Store 4 samples to the delay line buffer with circular address update.
        WUR_AE_CBEGIN0((uintptr_t)(delayLine + delayLen));
        WUR_AE_CEND0((uintptr_t)(delayLine + 2 * delayLen));

        AE_S16X4_XC(di0, D_wr_i, +8);
        AE_S16X4_XC(di1, D_wr_i, +8);

        // Store 4 samples to the delay line buffer with circular address update.
        WUR_AE_CBEGIN0((uintptr_t)(delayLine));
        WUR_AE_CEND0((uintptr_t)(delayLine + delayLen));

        AE_S16X4_XC(dr0, D_wr_r, +8);
        AE_S16X4_XC(dr1, D_wr_r, +8);        

        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)h;

        Y = (ae_int16 *)(y + 8 * 2 * n*D);

        __Pragma("loop_count min=5")
        for (d = 0; d < D; d++){
            D_tmp = D_wr_r;
            D_rd0 = D_wr_r;

            AE_L16_XC(d01, castxcc(ae_int16, D_tmp), +8);
            D_rd0_1 = D_tmp;
            D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
            D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
            D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

            AE_LA16X4POS_PC(D_va1, D_rd1);
            AE_LA16X4POS_PC(D_va2, D_rd2);
            AE_LA16X4POS_PC(D_va3, D_rd3);

            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);

            // Zero the accumulators.
            q0r = z; q1r = z; q2r = z; q3r = z;
            q4r = z; q5r = z; q6r = z; q7r = z;

            for (m = 0; m < (M >> 2) + 1; m++)
            {
                AE_L16X4_IP(c0, C, +8);
                AE_L16X4_XC(d01, D_rd0, 8);

                AE_MULAAAAQ16(q0r, d01, c0);
                AE_MULAAAAQ16(q1r, d12, c0);
                AE_MULAAAAQ16(q2r, d23, c0);
                AE_MULAAAAQ16(q3r, d34, c0);

                AE_L16X4_XC(d01, D_rd0_1, 8);
                AE_LA16X4_IC(d12, D_va1, D_rd1);
                AE_LA16X4_IC(d23, D_va2, D_rd2);
                AE_LA16X4_IC(d34, D_va3, D_rd3);

                AE_MULAAAAQ16(q4r, d01, c0);
                AE_MULAAAAQ16(q5r, d12, c0);
                AE_MULAAAAQ16(q6r, d23, c0);
                AE_MULAAAAQ16(q7r, d34, c0);
            }

            t0 = AE_TRUNCA32X2F64S(q1r, q0r, 33);
            t1 = AE_TRUNCA32X2F64S(q3r, q2r, 33);
            dr0 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0 = AE_MULFP32X16X2RAS_H(g_frac, dr0);
            t1 = AE_MULFP32X16X2RAS_L(g_frac, dr0);
            dr0 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, +2 * 2 * D);

            t0 = AE_TRUNCA32X2F64S(q5r, q4r, 33);
            t1 = AE_TRUNCA32X2F64S(q7r, q6r, 33);
            dr0 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0 = AE_MULFP32X16X2RAS_H(g_frac, dr0);
            t1 = AE_MULFP32X16X2RAS_L(g_frac, dr0);
            dr0 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, +2 * 2 * D);
            dr0 = AE_SEL16_4321(dr0, dr0);  AE_S16_0_XP(dr0, Y, -7 * 2 * 2 * D + 4);
        }

        Y = (ae_int16 *)(y + 8 * 2 * n*D + 1);
        //--------------------------------------------------------
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)h;

        WUR_AE_CBEGIN0((uintptr_t)(delayLine + delayLen));
        WUR_AE_CEND0((uintptr_t)(delayLine + 2 * delayLen));

        __Pragma("loop_count min=5")
        for (d = 0; d < D; d++)
        {
            D_tmp = D_wr_i;
            D_rd0 = D_wr_i;

            AE_L16_XC(d01, castxcc(ae_int16, D_tmp), +8);
            D_rd0_1 = D_tmp;
            D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
            D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
            D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

            AE_LA16X4POS_PC(D_va1, D_rd1);
            AE_LA16X4POS_PC(D_va2, D_rd2);
            AE_LA16X4POS_PC(D_va3, D_rd3);

            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);

            q0i = z; q1i = z; q2i = z; q3i = z;
            q4i = z; q5i = z; q6i = z; q7i = z;

            for (m = 0; m < (M >> 2) + 1; m++)
            {
                AE_L16X4_IP(c0, C, +8);
                AE_L16X4_XC(d01, D_rd0, 8);

                AE_MULAAAAQ16(q0i, d01, c0);
                AE_MULAAAAQ16(q1i, d12, c0);
                AE_MULAAAAQ16(q2i, d23, c0);
                AE_MULAAAAQ16(q3i, d34, c0);

                AE_L16X4_XC(d01, D_rd0_1, 8);
                AE_LA16X4_IC(d12, D_va1, D_rd1);
                AE_LA16X4_IC(d23, D_va2, D_rd2);
                AE_LA16X4_IC(d34, D_va3, D_rd3);

                AE_MULAAAAQ16(q4i, d01, c0);
                AE_MULAAAAQ16(q5i, d12, c0);
                AE_MULAAAAQ16(q6i, d23, c0);
                AE_MULAAAAQ16(q7i, d34, c0);
            }

            t0 = AE_TRUNCA32X2F64S(q1i, q0i, 33);
            t1 = AE_TRUNCA32X2F64S(q3i, q2i, 33);
            di0 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0 = AE_MULFP32X16X2RAS_H(g_frac, di0);
            t1 = AE_MULFP32X16X2RAS_L(g_frac, di0);
            di0 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, +2 * 2 * D);

            t0 = AE_TRUNCA32X2F64S(q5i, q4i, 33);
            t1 = AE_TRUNCA32X2F64S(q7i, q6i, 33);
            di0 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0 = AE_MULFP32X16X2RAS_H(g_frac, di0);
            t1 = AE_MULFP32X16X2RAS_L(g_frac, di0);
            di0 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, +2 * 2 * D);
            di0 = AE_SEL16_4321(di0, di0);  AE_S16_0_XP(di0, Y, -7 * 2 * 2 * D + 4);
        }
    }
    return ((int)((int16_t *)D_wr_r - delayLine));
}
#endif
