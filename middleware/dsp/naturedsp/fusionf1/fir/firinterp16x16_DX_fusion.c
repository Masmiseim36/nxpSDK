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

/*-------------------------------------------------------------------------
Interpolating Block Real/Complex FIR Filter
Computes a real FIR filter (direct-form) with interpolation using IR stored
in vector h. The real data input is stored in vector x. The filter output
result is stored in vector y. The filter calculates N*D output samples
using M*D coefficients and requires last N+M*D-1 samples on the delay line.
NOTE:
user application is not responsible for management of delay lines

Precision:
16x16     16-bit real data, 16-bit coefficients, 16-bit real outputs
16x16     16-bit complex data, 16-bit coefficients, 16-bit complex outputs
24x24     24-bit real data, 24-bit coefficients, 24-bit real outputs
32x16     32-bit real data, 16-bit coefficients, 32-bit real outputs
32x32     32-bit real data, 32-bit coefficients, 32-bit real outputs
f         floating point

Input:
h[M*D]        filter coefficients; h[0] is to be multiplied with the
newest sample,Q31, Q15, floating point
D             interpolation ratio
N             length of input sample block
M             length of subfilter. Total length of filter is M*D
x[N]          input samples,Q15, Q31 or floating point
Output:
y[N*D]        output samples, Q15, Q31 or floating point

Restrictions:
x,h,y should not overlap
x,h - aligned on an 8-bytes boundary
N   - multiple of 8
M   - multiple of 4
D should be >1

PERFORMANCE NOTE:
for optimum performance follow rules:
D   - 2, 3 or 4

-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
#include "firinterp16x16_common.h"


/* Generic data processing function for a decimating FIR filter. */
int firinterp16x16_DX_proc( int16_t * restrict y,
                            int16_t * delayLine,
                            int       delayLen,
                      const int16_t * restrict x,
                      const int16_t * restrict h,
                            int wrIx, int D, int N, int M)
{
          ae_int16x4 * restrict D_wr;
          ae_int16x4 *          D_tmp;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd0_1;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16x4 *          X;
          ae_int16   * restrict Y;
    const ae_int16x4 *          C;

    ae_valign  D_va1, D_va2, D_va3;
    ae_int16x4 d01, d23, d12, d34;
    ae_int16x4 c0;
    ae_f32x2   t0, t1;
    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int64   z;

    ae_int32x2   g_frac;

    int m, n, d;

    NASSERT(y && delayLine && x && h
        &&
        D > 0 && N > 0 && M > 0);

    NASSERT(!(N & 7) && !(M & 3));

    NASSERT(IS_ALIGN(delayLine) &&
        IS_ALIGN(x) &&
        IS_ALIGN(h));

    //
    // Setup pointers and circular delay line buffer.
    //

    D_wr = (ae_int16x4 *)(delayLine + wrIx);
    X    = (const ae_int16x4 *)x;
    Y    = (ae_int16     *)y;

    WUR_AE_CBEGIN0((uintptr_t)(delayLine));
    WUR_AE_CEND0((uintptr_t)(delayLine + delayLen));
    z = AE_ZERO64();
    

    {
        int q_frac_int = D << (15);
        g_frac = AE_MOVDA32(q_frac_int);
    }

    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 3); n++)
    {
        // Load 8 input samples.
        AE_L16X4_IP(d01, X, +8);
        AE_L16X4_IP(d12, X, +8);
        // Store 8 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);
        AE_S16X4_XC(d12, D_wr, +8);

        Y = (ae_int16 *)(y + 8 * n*D);

        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)h;

        for (d = 0; d < D; d++){
            D_tmp = D_wr;
            D_rd0 = D_wr;

            AE_L16_XC(d01, castxcc(ae_int16, D_tmp), +8);
            D_rd0_1 = D_tmp;
            D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
            D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
            D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

            AE_LA16X4POS_PC(D_va1, D_rd1);
            AE_LA16X4POS_PC(D_va2, D_rd2);
            AE_LA16X4POS_PC(D_va3, D_rd3);

            // Zero the accumulators.
            q0 = z; q1 = z; q2 = z; q3 = z;
            q4 = z; q5 = z; q6 = z; q7 = z;

            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);

            for (m = 0; m < (M >> 2) + 1; m++)
            {
                AE_L16X4_IP(c0, C, +8);
                AE_L16X4_XC(d01, D_rd0, 8);

                AE_MULAAAAQ16(q0, d01, c0);
                AE_MULAAAAQ16(q1, d12, c0);
                AE_MULAAAAQ16(q2, d23, c0);
                AE_MULAAAAQ16(q3, d34, c0);

                AE_L16X4_XC(d01, D_rd0_1, 8);
                AE_LA16X4_IC(d12, D_va1, D_rd1);
                AE_LA16X4_IC(d23, D_va2, D_rd2);
                AE_LA16X4_IC(d34, D_va3, D_rd3);

                AE_MULAAAAQ16(q4, d01, c0);
                AE_MULAAAAQ16(q5, d12, c0);
                AE_MULAAAAQ16(q6, d23, c0);
                AE_MULAAAAQ16(q7, d34, c0);
            }

            t0 = AE_TRUNCA32X2F64S(q1, q0, 33);
            t1 = AE_TRUNCA32X2F64S(q3, q2, 33);
            d01 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0  = AE_MULFP32X16X2RAS_H(g_frac, d01);
            t1  = AE_MULFP32X16X2RAS_L(g_frac, d01);
            d01 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(d01, Y, +2*D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, +2 * D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, +2 * D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, +2 * D);

            t0 = AE_TRUNCA32X2F64S(q5, q4, 33);
            t1 = AE_TRUNCA32X2F64S(q7, q6, 33);
            d01 = AE_ROUND16X4F32SASYM(t1, t0);

            // Scale output samples
            t0 = AE_MULFP32X16X2RAS_H(g_frac, d01);
            t1 = AE_MULFP32X16X2RAS_L(g_frac, d01);
            d01 = AE_SAT16X4(t0, t1);

            AE_S16_0_XP(d01, Y, +2 * D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, +2 * D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, +2 * D);
            d01 = AE_SEL16_4321(d01, d01); AE_S16_0_XP(d01, Y, -7 * 2 * D + 2);
        }
    }
    return (int)((int16_t *)D_wr - delayLine);
}
#endif
