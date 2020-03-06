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

#define SWAP_PTR(_x, _y) {complex_fract32 *tmp = _x; _x = _y ; _y = tmp; } 

/* Radix-4 butterfly with normalization   *
 * x0, x1, x2, x3 - input/output samples  *
 * AE_SAR - contains normalization factor */
#define DFT4X1RNG(x0, x1, x2, x3)\
{   \
    ae_int32x2 s0, s1, d0, d1;         \
    AE_ADDANDSUBRNG32(s0, d0, x0, x2); \
    AE_ADDANDSUBRNG32(s1, d1, x1, x3); \
    d1 = AE_MUL32JS(d1);               \
    AE_ADDANDSUB32S(x0, x2, s0, s1);   \
    AE_ADDANDSUB32S(x3, x1, d0, d1);   \
}

/*-------------------------------------------------------------------------
  Internal stages of functions stereo_fft_cplx32x32_ie(),
  stereo_ifft_cplx32x32_ie()

  Performs all stages of FFT except the first one. It is assumed that
  the first stage performs radix-4 DIF butterflies with normal input/output
  order.
  Static scaling is applied.
 
  Input/Output:
  x[N]                intermediate signal after computing the first
                      stage of FFT/IFFT.

  Input:
  twd[N*tw_step*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  tw_step             twiddle step 
  N                   FFT size

  Temporary:
  y[N]                temporary buffer

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x, y - should not overlap and must be aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int stereo_fft_cplx32x32_innerstage_scl3(complex_fract32 *x, complex_fract32 *y, const complex_fract32 *tw, int tw_step, int N);

/*-------------------------------------------------------------------------
  Internal stages of functions stereo_fft_cplx32x32_ie(),
  stereo_ifft_cplx32x32_ie()

  Performs all stages of FFT except the first one. It is assumed that
  the first stage performs radix-4 DIF butterflies with normal input/output
  order.
  Autoscaling is applied.
 
  Input/Output:
  x[N]                intermediate signal after computing the first
                      stage of FFT/IFFT.

  Input:
  twd[N*tw_step*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  tw_step             twiddle step 
  N                   FFT size
  bexp                common block exponent

  Temporary:
  y[N]                temporary buffer

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x, y - should not overlap and must be aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int stereo_fft_cplx32x32_innerstage_scl2(complex_fract32 *x, complex_fract32 *y, const complex_fract32 *tw, int tw_step, int N, int bexp);

/*-------------------------------------------------------------------------
  Inverse FFT on Complex Data with Optimized Memory Usage
  These functions make inverse FFT on complex data with optimized 
  memory usage.
  Scaling: 
      +-------------------+----------------------------------------+
      |      Function     |           Scaling options              |
      +-------------------+----------------------------------------+
      | ifft_cplx16x16_ie |  2 - 16-bit dynamic scaling            | 
      | ifft_cplx32x16_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      | ifft_cplx32x32_ie |  2 - 32-bit dynamic scaling            | 
      |                   |  3 - fixed scaling before each stage   | 
      +-------------------+----------------------------------------+
  NOTES:
  1. Bit-reversing reordering is done here.
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call
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
  S                   1 for ordinary (single channel) FFT, 2 - for stereo
                      input/outputs
  x[N*S]              complex input signal. Real and imaginary data 
                      are interleaved and real data goes first

  twd[N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of 
                      size N*twdstep
  N                   FFT size
  twdstep             twiddle step 
  scalingOpt          scaling option (see table above)

  Output:
  y[N*S]              output spectrum. Real and imaginary data are 
                      interleaved and real data goes first

  Returned value:     total number of right shifts occurred during 
                      scaling procedure

  Restrictions:
  x,y   should not overlap
  x,y   aligned on 8-bytes boundary
-------------------------------------------------------------------------*/
int stereo_ifft_cplx32x32_ie(complex_fract32* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt)
{
    int i;
    int shift = 0;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
    const ae_int32x2 * restrict px2;
    const ae_int32x2 * restrict px3;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
    const ae_int32x2 * restrict ptwd;
    const int stride = (N >> 2);
    ae_int32x2 x0, x1, x2, x3;
    ae_int32x2 tw1, tw2, tw3;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(twd);
    NASSERT(N>=8 && (N&(N-1))==0);
    NASSERT(x != y);
    NASSERT(scalingOpt == 2 || scalingOpt == 3);

    /*
     * The order of input data is reverted to make inverse FFT
     */

    /* dynamic scaling */
    if (scalingOpt==2)
    {
        const int min_shift = 3;
        int bexp, shiftl, shiftr;
        int32_t t;
        ae_int32x2 scl, acc, tmp;
        /* Calculate the exponent to prescale the input data */
        px0 = (const ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=2, factor=2");
        for (i = 0; i < N*2; i++)
        {
            AE_L32X2_IP(tmp, px0, sizeof(ae_int32x2));
            tmp = AE_ABS32S(tmp);
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        t = AE_MOVAD32_L(acc);
        bexp = NSA(t);
        XT_MOVEQZ(bexp, 0, t);

        /*
         * First stage
         */

        shift = min_shift - bexp;
        shiftl = XT_MAX(0, -shift);
        shiftr = XT_MAX(0,  shift);
        NASSERT(shift>-32 && shift<4);

        scl = 1 << shiftl;
        WUR_AE_SAR(shiftr);
        px0 = (const ae_int32x2 *)x;
        px3 = px0 + stride*2;
        px2 = px3 + stride*2;
        px1 = px2 + stride*2;
        py0 = (ae_int32x2 *)y;
        py1 = py0 + 1;
        ptwd = (const ae_int32x2 *)twd;
        /* first butterfly */
        {
            AE_L32X2_IP(tw1, ptwd, sizeof(ae_int32x2));
            AE_L32X2_IP(tw2, ptwd, sizeof(ae_int32x2));
            AE_L32X2_XP(tw3, ptwd, (3 * twdstep - 2) * sizeof(ae_int32x2));

            /* left channel */
            AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
            AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
            AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
            AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

            x0 = AE_MULP32X2(x0, scl);
            x1 = AE_MULP32X2(x1, scl);
            x2 = AE_MULP32X2(x2, scl);
            x3 = AE_MULP32X2(x3, scl);
            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2RNG_IP(x0, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x1, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x2, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x3, py0, 2*sizeof(ae_int32x2));

            /* right channel */
            AE_L32X2_XP(x0, px0, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x1, px1, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x2, px2, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x3, px3, -3*(int)sizeof(ae_int32x2));

            x0 = AE_MULP32X2(x0, scl);
            x1 = AE_MULP32X2(x1, scl);
            x2 = AE_MULP32X2(x2, scl);
            x3 = AE_MULP32X2(x3, scl);
            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2RNG_IP(x0, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x1, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x2, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x3, py1, 2*sizeof(ae_int32x2));
        }
        px0 = px1 + stride*2;
        __Pragma("loop_count min=3");
        for (i = 0; i < stride-1; i++)
        {
            AE_L32X2_IP(tw1, ptwd, sizeof(ae_int32x2));
            AE_L32X2_IP(tw2, ptwd, sizeof(ae_int32x2));
            AE_L32X2_XP(tw3, ptwd, (3 * twdstep - 2) * sizeof(ae_int32x2));

            /* left channel */
            AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
            AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
            AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
            AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

            x0 = AE_MULP32X2(x0, scl);
            x1 = AE_MULP32X2(x1, scl);
            x2 = AE_MULP32X2(x2, scl);
            x3 = AE_MULP32X2(x3, scl);
            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2RNG_IP(x0, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x1, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x2, py0, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x3, py0, 2*sizeof(ae_int32x2));

            /* right channel */
            AE_L32X2_XP(x0, px0, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x1, px1, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x2, px2, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x3, px3, -3*(int)sizeof(ae_int32x2));

            x0 = AE_MULP32X2(x0, scl);
            x1 = AE_MULP32X2(x1, scl);
            x2 = AE_MULP32X2(x2, scl);
            x3 = AE_MULP32X2(x3, scl);
            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2RNG_IP(x0, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x1, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x2, py1, 2*sizeof(ae_int32x2));
            AE_S32X2RNG_IP(x3, py1, 2*sizeof(ae_int32x2));
        }

        AE_CALCRNG3();
        bexp = min_shift - RUR_AE_SAR();
        SWAP_PTR(x, y);
        twdstep *= 4;

        /*
         * Next stages
         */

        shift += stereo_fft_cplx32x32_innerstage_scl2(x, y, twd, twdstep, N, bexp);
    }
    /* static scaling */
    else
    {
        /*
         * First stage
         */

        px0 = (const ae_int32x2 *)x;
        px3 = px0 + stride*2;
        px2 = px3 + stride*2;
        px1 = px2 + stride*2;
        py0 = (      ae_int32x2 *)y;
        py1 = py0 + 1;

        ptwd = (const ae_int32x2 *)twd;

        shift = 3;
        WUR_AE_SAR(3);
        /* first butterfly */
        {
            AE_L32X2_IP(tw1, ptwd, sizeof(ae_int32x2));
            AE_L32X2_IP(tw2, ptwd, sizeof(ae_int32x2));
            AE_L32X2_XP(tw3, ptwd, (3*twdstep - 2) * sizeof(ae_int32x2));

            /* left channel */
            AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
            AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
            AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
            AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2_IP(x0, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x1, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x2, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x3, py0, 2*sizeof(ae_int32x2));

            /* right channel */
            AE_L32X2_XP(x0, px0, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x1, px1, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x2, px2, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x3, px3, -3*(int)sizeof(ae_int32x2));

            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2_IP(x0, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x1, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x2, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x3, py1, 2*sizeof(ae_int32x2));
        }
        px0 = px1 + stride*2;
        __Pragma("loop_count min=3");
        for (i = 0; i < stride-1; i++)
        {
            AE_L32X2_IP(tw1, ptwd, sizeof(ae_int32x2));
            AE_L32X2_IP(tw2, ptwd, sizeof(ae_int32x2));
            AE_L32X2_XP(tw3, ptwd, (3*twdstep - 2) * sizeof(ae_int32x2));

            /* left channel */
            AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
            AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
            AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
            AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2_IP(x0, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x1, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x2, py0, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x3, py0, 2*sizeof(ae_int32x2));

            /* right channel */
            AE_L32X2_XP(x0, px0, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x1, px1, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x2, px2, -3*(int)sizeof(ae_int32x2));
            AE_L32X2_XP(x3, px3, -3*(int)sizeof(ae_int32x2));

            DFT4X1RNG(x0, x1, x2, x3);
            x1 = AE_MULFC32RAS(x1, tw1);
            x2 = AE_MULFC32RAS(x2, tw2);
            x3 = AE_MULFC32RAS(x3, tw3);

            AE_S32X2_IP(x0, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x1, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x2, py1, 2*sizeof(ae_int32x2));
            AE_S32X2_IP(x3, py1, 2*sizeof(ae_int32x2));
        }
        SWAP_PTR(x, y);
        twdstep *= 4;

        /*
         * Next stages
         */

        shift += stereo_fft_cplx32x32_innerstage_scl3(x, y, twd, twdstep, N);
    }

    return shift;
} /* stereo_ifft_cplx32x32_ie() */
