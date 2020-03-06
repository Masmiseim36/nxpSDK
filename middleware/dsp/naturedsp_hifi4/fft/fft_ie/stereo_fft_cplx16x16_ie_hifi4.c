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

/* Radix-4 DIF butterfly                 *
 * x0, x1, x2, x3 - input/output samples *
 * shift - scaling factor                */
#define DFT4XI2(x0, x1, x2, x3, shift)\
{\
    ae_int16x4 s0, s1, d0, d1;                      \
    xtbool4 mask = 0xA;                             \
    x0 = AE_SRAA16RS(x0, shift);                    \
    x1 = AE_SRAA16RS(x1, shift);                    \
    x2 = AE_SRAA16RS(x2, shift);                    \
    x3 = AE_SRAA16RS(x3, shift);                    \
    s0 = AE_ADD16S(x0, x2); d0 = AE_SUB16S(x0, x2); \
    s1 = AE_ADD16S(x1, x3); d1 = AE_SUB16S(x1, x3); \
    /*  d1 = AE_MULJ16S(d1); */                     \
    d1 = AE_SEL16_2301(d1, d1);                     \
    AE_MOVT16X4(d1, AE_NEG16S(d1), mask);           \
    x0 = AE_ADD16S(s0, s1); x2 = AE_SUB16S(s0, s1); \
    x3 = AE_ADD16S(d0, d1); x1 = AE_SUB16S(d0, d1); \
}


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
int stereo_fft_cplx16x16_ie(complex_fract16* y, complex_fract16* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    ae_int16x4 * restrict px;
    ae_int16x4 * restrict py;
    ae_int32   * restrict ptw;
    int bexp, shift = 0;
    int N4, N2, stridey = 1;
    int i;
    int tw_inc;
    ae_int32x2 t10, t20, t30;
    ae_int16x4 x0, x1, x2, x3, y0, y1;
    ae_int32x2 x10, x20, x30, x11, x21, x31;
    ae_int16x4 accbits;
    int32_t sumbits;
    complex_fract16 *pdest = y;

    NASSERT_ALIGN8(twd);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(x != y);
    NASSERT(scalingOpt == 2);
    NASSERT(N>=8 && (N&(N-1))==0);

    N2 = N >> 1;
    N4 = N >> 2;
    tw_inc = N4 * twdstep*sizeof(complex_fract16);

    /* Calculate the exponent to prescale the input data */
    {
        px = (ae_int16x4*)x;
        accbits = 0;

        __Pragma("loop_count min=2");
        for (i = 0; i < N; i++)
        {
            AE_L16X4_IP(x0, px, sizeof(ae_int16x4));
            x0 = AE_ABS16S(x0);
            accbits = AE_OR16(accbits, x0);
        }
        accbits = AE_OR16(accbits, AE_SEL16_5432(accbits, accbits));
        accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));

        sumbits = AE_MOVAD16_0(accbits);
        bexp = NSA(sumbits) - 16;
        XT_MOVEQZ(bexp, 0, i);
    }

    /*
     * First stage of FFT, radix-4
     */
    {
        const int min_shift = 3;
        shift = min_shift - bexp;
        accbits = 0;

        px = (ae_int16x4 *)x;
        py = (ae_int16x4 *)y;
        ptw = (ae_int32 *)twd;

        __Pragma("loop_count min=1");
        for (i = 0; i < N4; i++)
        {
            /* load twiddle factors */
            AE_L32_XP(t10, ptw, tw_inc);
            AE_L32_XP(t20, ptw, tw_inc);
            AE_L32_XP(t30, ptw, twdstep*sizeof(complex_fract16) - 2*tw_inc);
            t10 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t10));
            t20 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t20));
            t30 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t30));

            /* load input data */
            AE_L16X4_XP(x0, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x1, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x2, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x3, px, sizeof(ae_int16x4) - 3*N4*2*sizeof(complex_fract16));

            /* compute the butterfly */
            DFT4XI2(x0, x1, x2, x3, shift);
            x10 = AE_MULFC32X16RAS_H(t10, x1);
            x11 = AE_MULFC32X16RAS_L(t10, x1);
            x20 = AE_MULFC32X16RAS_H(t20, x2);
            x21 = AE_MULFC32X16RAS_L(t20, x2);
            x30 = AE_MULFC32X16RAS_H(t30, x3);
            x31 = AE_MULFC32X16RAS_L(t30, x3);
            x1 = AE_ROUND16X4F32SASYM(x10, x11);
            x2 = AE_ROUND16X4F32SASYM(x20, x21);
            x3 = AE_ROUND16X4F32SASYM(x30, x31);

            /* store data */
            AE_S16X4_IP(x0, py, sizeof(ae_int16x4));
            AE_S16X4_IP(x1, py, sizeof(ae_int16x4));
            AE_S16X4_IP(x2, py, sizeof(ae_int16x4));
            AE_S16X4_IP(x3, py, sizeof(ae_int16x4));

            /* accumulate bits of output data to calculate the scaling amount further */
            x0 = AE_ABS16S(x0);
            x1 = AE_ABS16S(x1);
            x2 = AE_ABS16S(x2);
            x3 = AE_ABS16S(x3);
            accbits = AE_OR16(accbits, x0);
            accbits = AE_OR16(accbits, x1);
            accbits = AE_OR16(accbits, x2);
            accbits = AE_OR16(accbits, x3);
        }

        accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
        accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
        sumbits = AE_MOVAD16_0(accbits);
        bexp = NSA(sumbits) - 16;
        XT_MOVEQZ(bexp,sumbits,sumbits);

        stridey *= 4;
        SWAP_PTR(x, y);
    }

    /*
     * Next FFT stages except the last, radix-4
     */
    while (stridey < N4)
    {
        int j, stage_shift;
        const int min_shift = 3;

        stage_shift = min_shift - bexp;
        XT_MOVLTZ(stage_shift, 0, stage_shift);
        NASSERT(stage_shift >= 0 && stage_shift <= 3);

        accbits = 0;
        px = (ae_int16x4 *)x;
        py = (ae_int16x4 *)y;
        ptw = (ae_int32 *)twd;
        twdstep *= 4;

        __Pragma("loop_count min=2");
        for (i = 0; i < N4; i+=stridey)
        {
            ae_int32x2 t10, t20, t30;
            py = (ae_int16x4*)(4*i*2*sizeof(complex_fract16)+(uintptr_t)y);

            /* load twiddle factors */
            AE_L32_XP(t10, ptw, tw_inc);
            AE_L32_XP(t20, ptw, tw_inc);
            AE_L32_XP(t30, ptw, twdstep*sizeof(complex_fract16) - 2*tw_inc);

            t10 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t10));
            t20 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t20));
            t30 = AE_CVT32X2F16_10(AE_MOVINT16X4_FROMINT32X2(t30));

            __Pragma("loop_count min=2");
            for (j = 0; j < stridey; j++)
            {
                /* load input data */
                AE_L16X4_XP(x0, px, N4*2*sizeof(complex_fract16));
                AE_L16X4_XP(x1, px, N4*2*sizeof(complex_fract16));
                AE_L16X4_XP(x2, px, N4*2*sizeof(complex_fract16));
                AE_L16X4_XP(x3, px, sizeof(ae_int16x4) - 3*N4*2*sizeof(complex_fract16));

                /* compute the butterfly */
                DFT4XI2(x0, x1, x2, x3, stage_shift);
                x10 = AE_MULFC32X16RAS_H(t10, x1);
                x11 = AE_MULFC32X16RAS_L(t10, x1);
                x20 = AE_MULFC32X16RAS_H(t20, x2);
                x21 = AE_MULFC32X16RAS_L(t20, x2);
                x30 = AE_MULFC32X16RAS_H(t30, x3);
                x31 = AE_MULFC32X16RAS_L(t30, x3);
                x1 = AE_ROUND16X4F32SASYM(x10, x11);
                x2 = AE_ROUND16X4F32SASYM(x20, x21);
                x3 = AE_ROUND16X4F32SASYM(x30, x31);

                /* store data */
                AE_S16X4_XP(x0, py, stridey*2*sizeof(complex_fract16));
                AE_S16X4_XP(x1, py, stridey*2*sizeof(complex_fract16));
                AE_S16X4_XP(x2, py, stridey*2*sizeof(complex_fract16));
                AE_S16X4_XP(x3, py, sizeof(ae_int16x4) - 3*stridey*2*sizeof(complex_fract16));

                /* accumulate bits of output data to calculate the scaling amount further */
                x0 = AE_ABS16S(x0);
                x1 = AE_ABS16S(x1);
                x2 = AE_ABS16S(x2);
                x3 = AE_ABS16S(x3);
                accbits = AE_OR16(accbits, x0);
                accbits = AE_OR16(accbits, x1);
                accbits = AE_OR16(accbits, x2);
                accbits = AE_OR16(accbits, x3);
            }
        }

        accbits = AE_OR16(accbits, AE_SEL16_2301(accbits, accbits));
        accbits = AE_OR16(accbits, AE_SHORTSWAP(accbits));
        sumbits = AE_MOVAD16_0(accbits);
        bexp = NSA(sumbits) - 16;
        XT_MOVEQZ(bexp,sumbits,sumbits);

        stridey *= 4;
        shift += stage_shift;
        SWAP_PTR(x, y);
    }

    /*
     * Last FFT stage, radix-2 or radix-4
     */
    if (y != pdest)
    {
        /* Execute the last stage inplace */
        y = x;
    }

    if (stridey == N2)/* radix-2 */
    {
        px = (ae_int16x4 *)x;
        py = (ae_int16x4 *)y;
        if (bexp == 0)/* butterfly with scaling */
        {
            shift++;
             __Pragma("loop_count min=4, factor=2");
            for (i = 0; i < N2; i++)
            {
                /* load input data */
                AE_L16X4_XP(x0, px, N2*2*sizeof(complex_fract16));
                AE_L16X4_XP(x1, px, sizeof(ae_int16x4) - N2*2*sizeof(complex_fract16));

                /* compute the butterfly */
                x0 = AE_SRAI16(x0, 1);
                x1 = AE_SRAI16(x1, 1);
                y0 = AE_ADD16S(x0, x1);
                y1 = AE_SUB16S(x0, x1);

                /* store data */
                AE_S16X4_XP(y0, py, N2*2*sizeof(complex_fract16));
                AE_S16X4_XP(y1, py, sizeof(ae_int16x4) - N2*2*sizeof(complex_fract16));
            }
        }
        else/* butterfly without scaling */
        {
            __Pragma("loop_count min=2");
            for (i = 0; i < N2; i++)
            {
                /* load input data */
                AE_L16X4_XP(x0, px, N2*2*sizeof(complex_fract16));
                AE_L16X4_XP(x1, px, sizeof(ae_int16x4) - N2*2*sizeof(complex_fract16));

                /* compute the butterfly */
                y0 = AE_ADD16S(x0, x1);
                y1 = AE_SUB16S(x0, x1);

                /* store data */
                AE_S16X4_XP(y0, py, N2*2*sizeof(complex_fract16));
                AE_S16X4_XP(y1, py, sizeof(ae_int16x4) - N2*2*sizeof(complex_fract16));
            }
        }
    }
    else/* radix-4 */
    {
        int stage_shift;
        const int min_shift = 2;
    
        stage_shift = min_shift - bexp;
        XT_MOVLTZ(stage_shift, 0, stage_shift);

        px = (ae_int16x4 *)x;
        py = (ae_int16x4 *)y;

        __Pragma("loop_count min=2");
        for (i = 0; i < N4; i++)
        {
            /* load input data */
            AE_L16X4_XP(x0, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x1, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x2, px, N4*2*sizeof(complex_fract16));
            AE_L16X4_XP(x3, px, sizeof(ae_int16x4) - 3*N4*2*sizeof(complex_fract16));

            /* compute the butterfly */
            DFT4XI2(x0, x1, x2, x3, stage_shift);

            /* store data */
            AE_S16X4_XP(x0, py, N4*2*sizeof(complex_fract16));
            AE_S16X4_XP(x1, py, N4*2*sizeof(complex_fract16));
            AE_S16X4_XP(x2, py, N4*2*sizeof(complex_fract16));
            AE_S16X4_XP(x3, py, sizeof(ae_int16x4) - 3*N4*2*sizeof(complex_fract16));
        }
        shift += stage_shift;
    }
    return shift;
} /* stereo_fft_cplx16x16_ie() */
