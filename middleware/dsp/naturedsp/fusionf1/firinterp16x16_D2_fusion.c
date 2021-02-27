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

/* Data processing function for a factor 2 interpolating FIR filter. */
int firinterp16x16_D2_proc( int16_t * restrict y,
                            int16_t * delayLine,
                            int       delayLen,
                      const int16_t * restrict x,
                      const int16_t * restrict h,
                            int wrIx, int D, int N, int M )
{
          ae_int16x4 * restrict D_wr;
    const ae_int16x4 *          D_rd0;
    const ae_int16x4 *          D_rd1;
    const ae_int16x4 *          D_rd2;
    const ae_int16x4 *          D_rd3;
    const ae_int16x4 *          X;
          ae_int16   * restrict Y;
    const ae_int16x4 *          C;

    ae_valign  D_va1, D_va2, D_va3;
    ae_valign  y_align;
    ae_int16x4 d01, d23, d12, d34;
    ae_int16x4 c0, c2;
    ae_f32x2   t0, t1;
    ae_int64   q0, q1, q2, q3;
    ae_int64   q4, q5, q6, q7;
    ae_int64   z;

    int m, n;

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
    y_align = AE_ZALIGN64();

    Y = (ae_int16 *)(y);

    //
    // Break the input signal into 4-samples blocks. For each block, store 4
    // samples to the delay line and compute the filter response.
    //
    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 2); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C  = (const ae_int16x4*)h;

        // Load 4 input samples.
        AE_L16X4_IP(d01, X, +8);
        // Store 4 samples to the delay line buffer with circular address update.
        AE_S16X4_XC(d01, D_wr, +8);
        
        // Start reading the delay line from the oldest sample, M+8 samples back
        // from the newest one.
        D_rd0 = D_wr;
        D_rd1 = (ae_int16x4*)((int16_t*)D_rd0 + 1);
        D_rd2 = (ae_int16x4*)((int16_t*)D_rd0 + 2);
        D_rd3 = (ae_int16x4*)((int16_t*)D_rd0 + 3);

        AE_LA16X4POS_PC(D_va1, D_rd1);
        AE_LA16X4POS_PC(D_va2, D_rd2);
        AE_LA16X4POS_PC(D_va3, D_rd3);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;
        q4 = z; q5 = z; q6 = z; q7 = z;

        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_L16X4_XC(d01, D_rd0, +8);
            AE_LA16X4_IC(d12, D_va1, D_rd1);
            AE_LA16X4_IC(d23, D_va2, D_rd2);
            AE_LA16X4_IC(d34, D_va3, D_rd3);
            
            AE_L16X4_IP(c0, C, +8);
            c2 = AE_L16X4_X(C, +2 * (M + 4) - 8);

            AE_MULAAAAQ16(q0, d01, c0);
            AE_MULAAAAQ16(q1, d01, c2);

            AE_MULAAAAQ16(q2, d12, c0);
            AE_MULAAAAQ16(q3, d12, c2);

            AE_MULAAAAQ16(q4, d23, c0);
            AE_MULAAAAQ16(q5, d23, c2);

            AE_MULAAAAQ16(q6, d34, c0);
            AE_MULAAAAQ16(q7, d34, c2);
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 34);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 34);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), y_align, castxcc(ae_int16x4, Y));

        t0 = AE_TRUNCA32X2F64S(q4, q5, 34);
        t1 = AE_TRUNCA32X2F64S(q6, q7, 34);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), y_align, castxcc(ae_int16x4, Y));
    }
    AE_SA64POS_FP(y_align, Y);
    return (int)((int16_t *)D_wr - delayLine);
}
#endif
