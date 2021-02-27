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
Decimating block real FIR filter, 32x32-bit
*/

/*-------------------------------------------------------------------------
Decimating Block Real FIR Filter
Computes a real FIR filter (direct-form) with decimation using IR stored
in vector h. The real data input is stored in vector x. The filter output
result is stored in vector r. The filter calculates N output samples using
M coefficients and requires last D*N+M-1 samples on the delay line.
NOTE:
- To avoid aliasing IR should be synthesized in such a way to be narrower
than input sample rate divided to 2D.
- user application is not responsible for management of delay lines

Precision:
24x24     24-bit data, 24-bit coefficients, 24-bit outputs
32x16     32-bit data, 16-bit coefficients, 32-bit outputs
f         floating point

Input:
h[M]          filter coefficients; h[0] is to be multiplied with the newest
sample, Q31, Q15, floating point
D             decimation factor
N             length of output sample block
M             length of filter
x[D*N]        input samples , Q15, Q31 or floating point
Output:
y[N]          output samples, Q15, Q31 or floating point

Restriction:
x,h,r should not overlap
x, h - aligned on an 8-bytes boundary
N - multiple of 8
D should exceed 1

PERFORMANCE NOTE:
for optimum performance follow rules:
D - 2, 3 or 4
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal.h"
/* Common utility and macros declarations. */
#include "common.h"
#if (XCHAL_HAVE_FUSION_16BIT_BASEBAND)
#include "firdec16x16_common.h"

int fir16x16_D2_proc( int16_t * restrict y,
                      int16_t * delayLine,
                      int       delayLen,
                const int16_t * restrict x,
                const int16_t * restrict h,
                      int wrIx, int D, int N, int M )
{
          ae_int16x4  * restrict D_wr;
    const ae_int16x4  *          D_rd0;
    const ae_int16x4  *          D_rd1;
    const ae_int16x4  *          X;
          ae_int16    * restrict Y;
    const ae_int16x4  *          C;

    ae_valign D_va1;
    ae_valign y_align;

    ae_int16x4 d0, d1, d2, d3;    
    ae_int16x4 c0;
    ae_int64   q0, q1, q2, q3;
    ae_f32x2   t0, t1;
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
    Y    = (ae_int16    *)y;

    WUR_AE_CBEGIN0((uintptr_t)(delayLine));
    WUR_AE_CEND0((uintptr_t)(delayLine + delayLen));
    z  = AE_ZERO64();
    y_align = AE_ZALIGN64();

    //
    // Break the input signal into 4*D-samples blocks. For each block, store
    // 4*D samples to the delay line buffer, and compute 4 samples of decimated
    // response signal.
    //
    __Pragma("loop_count min=1")
    for (n = 0; n < (N >> 2); n++)
    {
        // Reset the coefficients pointer. Now it looks at the tap corresponding
        // to the oldest sample in the delay line.
        C = (const ae_int16x4*)h;

        AE_L16X4_IP(d0, X, +8);
        AE_L16X4_IP(d1, X, +8);
        AE_S16X4_XC(d0, D_wr, +8);
        AE_S16X4_XC(d1, D_wr, +8);

        //
        // Setup 4-way delay line reading pointers, one per an accumulator.
        //
        D_rd0 = (const ae_int16x4 *)D_wr;
        D_rd1 = (const ae_int16x4 *)((int16_t *)D_wr + 2);

        AE_LA16X4POS_PC(D_va1, D_rd1);

        // Zero the accumulators.
        q0 = z; q1 = z; q2 = z; q3 = z;

        AE_L16X4_XC(d0, D_rd0, +8);
        AE_LA16X4_IC(d1, D_va1, D_rd1);

        __Pragma("loop_count min=2")
        for (m = 0; m < (M >> 2) + 1; m++)
        {
            AE_L16X4_XC(d2, D_rd0, +8);
            AE_LA16X4_IC(d3, D_va1, D_rd1);

            AE_L16X4_IP(c0, C, +8);

            AE_MULAAAAQ16(q0, d0, c0);
            AE_MULAAAAQ16(q1, d1, c0);
            AE_MULAAAAQ16(q2, d2, c0);
            AE_MULAAAAQ16(q3, d3, c0);

            d0 = d2;
            d1 = d3;
        }

        t0 = AE_TRUNCA32X2F64S(q0, q1, 33);
        t1 = AE_TRUNCA32X2F64S(q2, q3, 33);
        AE_SA16X4_IP(AE_ROUND16X4F32SASYM(t0, t1), y_align, castxcc(ae_int16x4,Y));
    }
    AE_SA64POS_FP(y_align, Y);

    return (int)((int16_t *)D_wr - delayLine);
}
#endif
