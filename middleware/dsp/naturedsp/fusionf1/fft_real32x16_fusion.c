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
	NatureDSP Signal Processing Library. FFT part
    C code optimized for Fusion
    Real/halfcomplex FFT 32x16 with scaling option 3
*/

#include "NatureDSP_Signal.h"
#include "fft_real_twiddles.h"
#include "fft_cplx_twiddles.h"
#include "baseop.h"

/*
	in-place split part of FFT:
	x[N+2]	input/output
	N		size of FFT
*/

static void splitPart_x2(int32_t * x, int N)
{
  int i, step;

  const int step_back = -8;

  ae_int32x2 * restrict p_x0, * restrict p_x1;
  const ae_p16x2s * restrict p_twd;

  ae_int32x2  vA0, vA1, vB0, vB1, vC0, vC1, vR;
  ae_int16x4  vT;
  ae_f32x2    vF0, vF1;
  ae_f16x4    vF2;

  NASSERT_ALIGN8(x);

  // split part: remainder
  {
    ae_q56s tmp = AE_CVTQ48A32S(N);
    step = 1<<(AE_NSAQ56S(tmp)-(38-MAX_RFFT_PWR));
    step *= sizeof(int32_t);
  }

  p_twd = (const ae_p16x2s *)twiddleSplit;
  p_x0 = (ae_int32x2 *)x;
  p_x1 = (ae_int32x2 *)(x+N);

  // load data and prepare pointers for pre-increment
  // first and last samples
  vA0 = AE_L32X2_I(p_x0, 0);
  vA1 = AE_SEL32_LH(vA0, vA0);
  vB0 = AE_ADD32S(vA0, vA1);
  vB1 = AE_SUB32S(vA0, vA1);
  vA1 = AE_MOVI(0);
  vB0 = AE_SEL32_HH(vB0, vA1);
  vB1 = AE_SEL32_HH(vB1, vA1);
  AE_S32X2_IP(vB0, p_x0, 8);
  AE_S32X2_XP(vB1, p_x1, step_back);

  vA0 = AE_L32X2_I(p_x0, 0);
  vA1 = AE_L32X2_I(p_x1, 0);

  vR = AE_MOVI(1);

  for (i = 1; i < N>>2; i++) 
  {
    // load twiddle
    AE_L16X2M_XU(vB1, p_twd, step);
    vB1 = AE_SRAI32(vB1, 8);
    //vB1 = AE_SEL32_LH(vB1, vB1);
    vT = AE_CVT16X4(vB1, vB1);

    // ADD/SUBB
    vB0 = AE_ADD32S(vA0, vA1);
    vB1 = AE_SUB32S(vA0, vA1);

    vA0 = AE_SEL32_HL(vB1, vB0);
    vB1 = AE_SEL32_HL(vB0, vB1);

    // do rotation
    vF1 = (vA0);
    vF2 = (vT);
    vF0 = AE_MULFC32X16RAS_H(vF1, vF2);
    vB0 = (vF0);

    // load next data
    vA0 = AE_L32X2_I(p_x0, 8);
    vA1 = AE_L32X2_I(p_x1, -8);

    vB0 = AE_ADD32S(vB0, vR);
    vB1 = AE_ADD32S(vB1, vR);
    vB0 = AE_SRAI32(vB0, 1);
    vB1 = AE_SRAI32(vB1, 1);

    // SUM/DIFF
    vC0 = AE_SUB32S(vB1, vB0);
    vC1 = AE_ADD32S(vB1, vB0);
    vB1 = AE_NEG32S(vC1);
    vC1 = AE_SEL32_HL(vC1, vB1);

    AE_S32X2_IP(vC0, p_x0, 8);
    AE_S32X2_XP(vC1, p_x1, step_back);
  }

  // middle sample
  vB0 = AE_NEG32S(vA0);
  vC0 = AE_SEL32_HL(vA0, vB0);
  AE_S32X2_I(vC0, p_x0, 0);
}

/*-------------------------------------------------------------------------
  FFT on real data forming half of spectrum

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles

  NOTES:
  1. Bit-reversal reordering is done here. 
  2. FFT runs in-place so INPUT DATA WILL APPEAR DAMAGED after the call.
  3. Real data FFT function calls fft_cplx() to apply complex FFT of size
     N/2 to input data and then transforms the resulting spectrum.

  Input:
  x[N]         - input signal
  N            - FFT size
  scalingOpt   - scaling option:
                0 - no scaling
                1 - 24-bit scaling
                2 - 32-bit scaling on the first stage and 24-bit scaling later
                3 - fixed scaling
  Output:
  y[(N/2+1)*2] - output spectrum (positive side)

  Restrictions:
  Arrays should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 32...8192

-------------------------------------------------------------------------*/
int fft_real32x16( 
              f24* y,
              int32_t* x,
              fft_handle_t h,
              int scalingOpt)
{
  int scale=0;
  int N;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(scalingOpt==3);

  N=(((const tFftDescr*)h)->N)<<1;
  scale=fft_cplx32x16(y, x, h, scalingOpt);    /* fft of half-size              */

  splitPart_x2(y, N);	    

  return scale;
}
