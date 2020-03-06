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

#define SWAP_PTR(_x, _y) {complex_fract16 *tmp = _x; _x = _y ; _y = tmp; }

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

static int stage_first_DFT4_16x16_ie(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
    int i, shift;
    const int R = 4; // stage radix
    const int stride = N / R;
    const int _v = v[0];
    const int tw_inc0 = N / _v / 4 * tw_step* sizeof(complex_fract16);
    ae_int16x4 * restrict _py;
    ae_int16x4 * restrict _px;
    ae_int32   * restrict ptw;
    ae_int32x2 t10, t20, t30, t11, t21, t31;
    ae_int32x2 x10, x20, x30, x11, x21, x31;
    ae_int16x4 acc16 = AE_MOVINT16X4_FROMF32X2(AE_MOVI(0));

    shift = 3 - *bexp;

    _py = (ae_int16x4 *)y;
    _px = (ae_int16x4 *)x;
    ptw = (ae_int32 *)((uintptr_t)tw);

    WUR_AE_SAR(5); // Set scaling = 3
    __Pragma("loop_count min=1");
    for (i = 0; i < N / 2 / R; i++)
    {
        ae_int16x4 _x0, _x1, _x2, _x3;

        AE_L32_XP(t10, ptw, tw_inc0);
        AE_L32_XP(t20, ptw, tw_inc0);
        AE_L32_XP(t30, ptw, tw_step* sizeof(complex_fract16) - 2 * tw_inc0);
        AE_L32_XP(t11, ptw, tw_inc0);
        AE_L32_XP(t21, ptw, tw_inc0);
        AE_L32_XP(t31, ptw, tw_step* sizeof(complex_fract16) - 2 * tw_inc0);

        t10 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t10), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));
        t20 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t20), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));
        t30 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t30), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));
        t11 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t11), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));
        t21 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t21), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));
        t31 = AE_MOVINT32X2_FROMF16X4(AE_SEL16_5140(AE_MOVINT16X4_FROMF32X2(t31), AE_MOVINT16X4_FROMF32X2(AE_MOVI(0))));

        AE_L16X4_XP(_x0, _px, stride*sizeof(complex_fract16));
        AE_L16X4_XP(_x1, _px, stride*sizeof(complex_fract16));
        AE_L16X4_XP(_x2, _px, stride*sizeof(complex_fract16));
        AE_L16X4_XP(_x3, _px, sizeof(*_px) - 3 * stride*sizeof(complex_fract16));

        DFT4XI2(_x0, _x1, _x2, _x3, shift);

        x10 = AE_MULFC32X16RAS_H(t10, _x1);
        x11 = AE_MULFC32X16RAS_L(t11, _x1);
        x20 = AE_MULFC32X16RAS_H(t20, _x2);
        x21 = AE_MULFC32X16RAS_L(t21, _x2);
        x30 = AE_MULFC32X16RAS_H(t30, _x3);
        x31 = AE_MULFC32X16RAS_L(t31, _x3);

        _x1 = AE_ROUND16X4F32SASYM(x10, x11);
        _x2 = AE_ROUND16X4F32SASYM(x20, x30);
        _x3 = AE_ROUND16X4F32SASYM(x21, x31);

        AE_S16X4_IP(AE_SEL16_7632(_x0, _x1), _py, sizeof(*_py));
        AE_S16X4_IP(_x2                    , _py, sizeof(*_py));
        AE_S16X4_IP(AE_SEL16_5410(_x0, _x1), _py, sizeof(*_py));
        AE_S16X4_IP(_x3                    , _py, sizeof(*_py));

        _x0 = AE_ABS16S(_x0);
        _x1 = AE_ABS16S(_x1);
        _x2 = AE_ABS16S(_x2);
        _x3 = AE_ABS16S(_x3);

        acc16 = AE_OR16(acc16, _x0);
        acc16 = AE_OR16(acc16, _x1);
        acc16 = AE_OR16(acc16, _x2);
        acc16 = AE_OR16(acc16, _x3);
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
} //stage_first_DFT4_16x16_ie

static int stage_last_DFT4_16x16_ie(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
    int i, shift;
    const int R = 4; // stage radix
    const int min_shift = 2;
    const int stride = N / R;
    const int _v = v[0];
    ae_int16x4 * restrict _py;
    ae_int16x4 * restrict _px;

    shift = min_shift - *bexp;
    XT_MOVLTZ(shift, 0, shift);

    if (_v == stride)
    {
        _py = (ae_int16x4 *)y;
        _px = (ae_int16x4 *)x;
        // Last phase, without twiddles
        __Pragma("loop_count min=2");
        for (i = 0; i < N / 8; i++)
        {
            ae_int16x4 _x0, _x1, _x2, _x3;

            AE_L16X4_XP(_x0, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x1, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x2, _px, stride*sizeof(complex_fract16));
            AE_L16X4_XP(_x3, _px, sizeof(*_px) - 3 * stride*sizeof(complex_fract16));

            DFT4XI2(_x0, _x1, _x2, _x3, shift);

            AE_S16X4_XP(_x0, _py, stride*sizeof(complex_fract16));
            AE_S16X4_XP(_x1, _py, stride*sizeof(complex_fract16));
            AE_S16X4_XP(_x2, _py, stride*sizeof(complex_fract16));
            AE_S16X4_XP(_x3, _py, sizeof(*_py) - 3 * stride*sizeof(complex_fract16));
        }
    }
    return shift;
} //stage_last_DFT4_16x16_ie

extern int stage_inner_DFT4_16x16_ie(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp);


/*-------------------------------------------------------------------------
  FFT on Complex Data with Optimized Memory Usage
  These functions make FFT on complex data with optimized memory usage.
  Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      |  fft_cplx16x16_ie |  2 - 16-bit dynamic scaling            | 
      |  fft_cplx32x16_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      |  fft_cplx32x32_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call.
  3. FFT of size N may be supplied with constant data
     (twiddle factors) of a larger-sized FFT = N*twdstep.
  4. Stereo FFTs accept inputs/form outputs in the interleaved order:
     left complex sample, right complex sample

  Precision: 
  16x16_ie      16-bit input/outputs, 16-bit twiddles
  32x16_ie      32-bit input/outputs, 16-bit twiddles
  32x32_ie      32-bit input/outputs, 32-bit twiddles
  f_ie          floating point
 
  Input:
  S                     1 for ordinary (single channel) FFT, 2 - for stereo
                        input/outputs
  x[N*S]                complex input signal. Real and imaginary data 
                        are interleaved and real data goes first
  twd[N*twdstep*3/4]    twiddle factor table of a complex-valued FFT of 
                        size N*twdstep
  N                     FFT size
  twdstep               twiddle step 
  scalingOpt            scaling option (see table above), not applicable
                        to the floating point function 
  Output:
  y[N*S]                output spectrum. Real and imaginary data are 
                        interleaved and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure. Floating point function always return 0.

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/

int fft_cplx16x16_ie(complex_fract16* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    int bexp, shift = 0;
    int v = 1;
    complex_fract16 *pdest = y;
    int log2N = 30 - NSA(N);
    ae_int16x4 * restrict px;
    ae_int16x4 * restrict py;

    NASSERT_ALIGN8(twd);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(x != y);
    NASSERT(scalingOpt == 2);
    NASSERT(N == 128 || N == 256 || N == 512 || N == 1024);
    
    {
        int i;
        ae_int16x4 acc = AE_MOVINT16X4_FROMINT32X2(AE_MOVI(0)), tmp;
        px = (ae_int16x4*)x;

        __Pragma("loop_count min=2");
        for (i = 0; i < (N >> 1); i++)
        {
            AE_L16X4_IP(tmp, px, sizeof(*px));
            tmp = AE_ABS16S(tmp);
            acc = AE_OR16(acc, tmp);
        }
        acc = AE_OR16(acc, AE_SEL16_5432(acc, acc));
        acc = AE_OR16(acc, AE_SHORTSWAP(acc));

        i = AE_MOVAD16_0(acc);
        bexp = NSA(i) - 16;
        XT_MOVEQZ(bexp, 0, i);
    }

    shift += stage_first_DFT4_16x16_ie((const int16_t*)twd, (int16_t*)x, (int16_t*)y, N, &v, twdstep, &bexp);
    SWAP_PTR(x, y);
    log2N -= 2;
    twdstep *= 4;

    while (log2N > 2)
    {
        shift += stage_inner_DFT4_16x16_ie((const int16_t*)twd, (int16_t*)x, (int16_t*)y, N, &v, twdstep, &bexp);
        SWAP_PTR(x, y);
        log2N -= 2;
        twdstep *= 4;
    }

    if (y != pdest)
    {
        /* Execute the last stage inplace */
        y = x;
    }

    /* Last stage */
    if (log2N & 1)
    {
     //   shift += fft_stageS2_DFT2_16x16(NULL, (int16_t*)x, (int16_t*)y, N, &v, 0, &bexp);
        const int stride = N / 2;
        int i;
        px = (ae_int16x4 *)x;
        py = (ae_int16x4 *)y;
        if (bexp == 0)
        {
            shift++;
             __Pragma("loop_count min=4, factor=2");
            for (i = 0; i < N / 4; i++)
            {
                ae_int16x4 x0, x1, tmp0;

                AE_L16X4_XP(x0, px, stride*sizeof(complex_fract16));
                AE_L16X4_XP(x1, px, sizeof(*px) - stride*sizeof(complex_fract16));

                x0 = AE_SRAI16(x0, 1);
                x1 = AE_SRAI16(x1, 1);

                tmp0 = AE_ADD16S(x0, x1);
                x1 = AE_SUB16S(x0, x1);

                AE_S16X4_XP(tmp0, py, stride*sizeof(complex_fract16));
                AE_S16X4_XP(x1, py, sizeof(*px) - stride*sizeof(complex_fract16));
            }
        }
        else
        {
            __Pragma("loop_count min=2");
            for (i = 0; i < N / 4; i++)
            {
                ae_int16x4 x0, x1, tmp0;

                AE_L16X4_XP(x0, px, stride*sizeof(complex_fract16));
                AE_L16X4_XP(x1, px, sizeof(*px) - stride*sizeof(complex_fract16));

                tmp0 = AE_ADD16S(x0, x1);
                x1 = AE_SUB16S(x0, x1);

                AE_S16X4_XP(tmp0, py, stride*sizeof(complex_fract16));
                AE_S16X4_XP(x1, py, sizeof(*px) - stride*sizeof(complex_fract16));
            }
        }
    }
    else
    {
        shift += stage_last_DFT4_16x16_ie(NULL, (int16_t*)x, (int16_t*)y, N, &v, 0, &bexp);
    }
    return shift;
} /* fft_cplx16x16_ie() */
