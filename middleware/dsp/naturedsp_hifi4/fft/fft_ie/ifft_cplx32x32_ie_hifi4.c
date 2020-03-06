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
#include "fft_twiddles32x32.h"

#define SWAP_PTR(_x, _y) {complex_fract32 *tmp = _x; _x = _y ; _y = tmp; } 

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
int ifft_cplx32x32_ie(complex_fract32* y, complex_fract32* x, const complex_fract32* twd, int twdstep, int N, int scalingOpt)
{
    int bexp = 0;
    int v = 1;
    int shift = 0;
    const ae_int32x2 * restrict px;

    complex_fract32 *pdest = y;
    int log2N = 30 - NSA(N);

    const fft_cplx32x32_stage_t first_stg_fn = (scalingOpt == 2) ? ifft_stageS2_DFT4_first_32x32 : ifft_stageS3_DFT4_first_32x32;
    const fft_cplx32x32_stage_t stg_fn = (scalingOpt == 2) ? fft_stageS2_DFT4x4_32x32 : fft_stageS3_DFT4x4_32x32;
    const fft_cplx32x32_stage_t last_stg_fn = (log2N & 1) ?
                                              ((scalingOpt == 2) ? fft_stageS2_DFT2_last_32x32 : fft_stageS3_DFT2_last_32x32) :
                                              ((scalingOpt == 2) ? fft_stageS2_DFT4_last_32x32 : fft_stageS3_DFT4_last_32x32);
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT_ALIGN8(twd);
    NASSERT((N&(N - 1)) == 0);
    NASSERT(x != y);
    NASSERT(scalingOpt == 2 || scalingOpt == 3);

    if (scalingOpt == 2)
    {
        int n;
        int32_t t;
        ae_int32x2 acc, tmp;
        px = (const ae_int32x2 *)x;
        acc = AE_ZERO32();
        __Pragma("loop_count min=2, factor=2");
        for (n = 0; n < N; n++)   
        {
            AE_L32X2_IP(tmp, px, sizeof(*px));
            tmp = AE_ABS32S(tmp); 
            acc = AE_OR32(acc, tmp);
        }
        acc = AE_OR32(acc, AE_SEL32_LH(acc, acc));
        t = AE_MOVAD32_L(acc);
        bexp = NSA(t);
        XT_MOVEQZ(bexp, 0, t);
    }

    shift += first_stg_fn((const int32_t*)twd, (int32_t*)x, (int32_t*)y, N, &v, twdstep, &bexp);
    SWAP_PTR(x, y);
    log2N -= 2;
    twdstep *= 4;

    while (log2N > 2)
    {
        shift += stg_fn((const int32_t*)twd, (int32_t*)x, (int32_t*)y, N, &v, twdstep, &bexp);
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
    shift += last_stg_fn(NULL, (int32_t*)x, (int32_t*)y, N, &v, 0, &bexp);
    return shift;
} /* ifft_cplx32x32_ie() */
