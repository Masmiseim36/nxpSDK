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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FFT part
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"

#define DFT4XI2(x0, x1, x2, x3, shift)/* output x0, x3, x1, x2*/\
{\
    ae_int16x4 t0, t1, t2, t3;                                        \
    xtbool4 mask = 0x5;                                               \
    x0 = AE_SRAA16RS(x0, shift);                                      \
    x1 = AE_SRAA16RS(x1, shift);                                      \
    x2 = AE_SRAA16RS(x2, shift);                                      \
    x3 = AE_SRAA16RS(x3, shift);                                      \
    t0 = AE_ADD16S(x0, x2); t2 = AE_SUB16S(x0, x2);                   \
    t1 = AE_ADD16S(x1, x3); t3 = AE_SUB16S(x1, x3);                   \
    x0 = t0; x1 = t1; x2 = t2; x3 = t3;                               \
    AE_MOVT16X4(x3, AE_NEG16S(x3), mask); /*  x3 = AE_CONJ16S(x3); */ \
    x3 = AE_SEL16_2301(x3, x3);                                       \
    t0 = AE_ADD16S(x0, x1); t1 = AE_SUB16S(x0, x1);                   \
    t2 = AE_ADD16S(x2, x3); t3 = AE_SUB16S(x2, x3);                   \
    x0 = t0; x1 = t1; x2 = t2; x3 = t3;                               \
    t1 = x3;                                                          \
    t2 = x1;                                                          \
    t3 = x2;                                                          \
    x1 = t1;                                                          \
    x2 = t2;                                                          \
    x3 = t3;                                                          \
}

/*
    Internal stages of fft_cplx16x16_ie, ifft_cplx16x16_ie.
*/
int stage_inner_DFT4_16x16_ie(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
    int i, j, shift;
    const int R = 4; // stage radix
    const int min_shift = 3;
    const int stride = N / R;
    const int _v = v[0];
    const int tw_inc0 = N / _v  / 4 * tw_step* sizeof(complex_fract16);
    ae_int16x4 * restrict _py;
    ae_int16x4 * restrict _px;
    ae_p16x2s  * restrict ptw16x2;
    ae_int16x4 acc16 = AE_MOVINT16X4_FROMF32X2(AE_MOVI(0));

    shift = min_shift - *bexp;
    XT_MOVLTZ(shift, 0, shift);
    ASSERT(shift >= 0 && shift <= 3);

    _py = (ae_int16x4 *)y;
    _px = (ae_int16x4 *)x;
    ptw16x2 = (ae_p16x2s *)((uintptr_t)tw);

    __Pragma("loop_count min=2");
    for (i = 0; i < N / _v / R; i++)
    {
        ae_int32x2 t10, t20, t30;
        _py = (ae_int16x4*)(4 * _v * i  * sizeof(complex_fract16)+(uintptr_t)y);

        t10 = AE_L16X2M_X(ptw16x2, 0);
        t20 = AE_L16X2M_X(ptw16x2, tw_inc0);
        t30 = AE_L16X2M_X(ptw16x2, 2 * tw_inc0);
        ptw16x2 += tw_step;

        t10 = AE_SLAI32(t10, 8);
        t20 = AE_SLAI32(t20, 8);
        t30 = AE_SLAI32(t30, 8);

        t10 = AE_SEL32_LH(t10, t10);
        t20 = AE_SEL32_LH(t20, t20);
        t30 = AE_SEL32_LH(t30, t30);

        ASSERT(_v >= 2);
        __Pragma("loop_count min=1");
        for (j = 0; j < _v; j += 2)
        {
            ae_int16x4 _x0, _x1, _x2, _x3;
            ae_int32x2  x10, x20, x30, x11, x21, x31;

            AE_L16X4_XP(_x0, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x1, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x2, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x3, _px, sizeof(*_px) - 3 * stride*sizeof(complex_fract16));

            DFT4XI2(_x0, _x1, _x2, _x3, shift);

            x10 = AE_MULFC32X16RAS_H(t10, _x1);
            x11 = AE_MULFC32X16RAS_L(t10, _x1);
            x20 = AE_MULFC32X16RAS_H(t20, _x2);
            x21 = AE_MULFC32X16RAS_L(t20, _x2);
            x30 = AE_MULFC32X16RAS_H(t30, _x3);
            x31 = AE_MULFC32X16RAS_L(t30, _x3);

            _x1 = AE_ROUND16X4F32SASYM(x10, x11);
            _x2 = AE_ROUND16X4F32SASYM(x20, x21);
            _x3 = AE_ROUND16X4F32SASYM(x30, x31);

            AE_S16X4_XP(_x0, _py, _v*sizeof(complex_fract16));
            AE_S16X4_XP(_x1, _py, _v*sizeof(complex_fract16));
            AE_S16X4_XP(_x2, _py, _v*sizeof(complex_fract16));
            AE_S16X4_XP(_x3, _py, sizeof(*_py) - 3 * _v*sizeof(complex_fract16));

            _x0 = AE_ABS16S(_x0);
            _x1 = AE_ABS16S(_x1);
            _x2 = AE_ABS16S(_x2);
            _x3 = AE_ABS16S(_x3);

            acc16 = AE_OR16(acc16, _x0);
            acc16 = AE_OR16(acc16, _x1);
            acc16 = AE_OR16(acc16, _x2);
            acc16 = AE_OR16(acc16, _x3);
        }
    }

    acc16 = AE_OR16(acc16, AE_SEL16_5432(acc16, acc16)); //AE_INTSWAP
    acc16 = AE_OR16(acc16, AE_SHORTSWAP(acc16));
    {
        int sar = (AE_MOVAD16_0(acc16) >> (15 - 3)) << 3;
        WUR_AE_SAR(sar);
        /* Bits 5, 4, and 3 respectively of AE_SAR are set if bits 14, 13 and 12
        of any quarter of d is different than their respective sign bit.  */
        *bexp = NSA((sar << (30 - 5)) | 1);
    }

    *v *= R;
    return shift;
} //stage_inner_DFT4_16x16_ie
