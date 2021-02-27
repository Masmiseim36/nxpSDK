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
*/
/*===========================================================================
Fast Fourier Transforms:
fft_cplx             FFT on Complex Data
fft_real             FFT on Real Data
ifft_cplx            IFFT on Complex Data
ifft_real            Inverse FFT Forming Real Data
fft_cplx<prec>_ie    FFT on Complex Data with optimized memory usage
fft_real<prec>_ie    FFT on Real Data with optimized memory usage
ifft_cplx<prec>_ie   IFFT on Complex Data with optimized memory usage
ifft_real<prec>_ie   Inverse FFT Forming Real Data with optimized memory usage
dct                  Discrete Cosine Transform

There are limited combinations of precision and scaling options available:
----------------+---------------------------------------------------------------
FFT/IFFT        | Scaling options                        | Restrictions on the
                |                                        | input dynamic range
----------------+---------------------------------------------------------------
cplx24x24       | 0 ÿ no scaling                         | input signal < 2^23/(2*N),
                |                                        | N-fft-size
real24x24       | 1 ÿ 24-bit scaling                     |        none
                | 2 ÿ 32-bit scaling on the first stage  |        none
                | and 24-bit scaling later               |        none
                | 3 ÿ fixed scaling before each stage    |        none
------------------------------------------------------------------------------------
cplx32x16       | 3 ÿ fixed scaling before each stage    |        none
cplx16x16       | 3 ÿ fixed scaling before each stage    |        none
cplx32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
cplx24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16       | 3 ÿ fixed scaling before each stage    |        none
real16x16       | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie    | 3 ÿ fixed scaling before each stage    |        none
real24x24_ie    | 3 ÿ fixed scaling before each stage    |        none
real32x16_ie_24p| 3 ÿ fixed scaling before each stage    |        none
----------------+---------------------------------------------------------------
real24x24_ie_24p| 1 ÿ 24-bit scaling                     |        none
----------------+---------------------------------------------------------------
DCT:            |
----------------+---------------------------------------------------------------
24x24	          | 0 ÿ no scaling                         |        none
32x16	          | 0 ÿ no scaling                         |        none
----------------+---------------------------------------------------------------

===========================================================================*/
#include "NatureDSP_Signal.h"
#include "common.h"
#include "fft_real_twiddles.h"
#include "fft_cplx_twiddles.h"
#include "baseop.h"

/*
    in-place inverse split part of FFT:
    x[N+2]  input (N+2 samples)/output(N samples)
    N       size of FFT
    Scaling factor of isplitPart_x2 is 3.
*/

static void isplitPart_x2(int32_t *X,    int N, const int16_t *tw, int step)
{
  int i;

  const int step_back = -8;

  ae_int32x2 * restrict p_x0, * restrict p_x1;
  const ae_p16x2s * restrict p_twd;

  ae_int32x2  vA0, vA1, vB0, vB1, vC0, vC1, vR;
  ae_int16x4  vT;
  ae_f32x2    vF0, vF1;
  ae_f16x4    vF2;

  NASSERT_ALIGN8(X);

  step *= sizeof(int16_t)*2;

  p_twd = (const ae_p16x2s *)tw;
  p_x0 = (ae_int32x2 *)X;
  p_x1 = (ae_int32x2 *)(X+N);

  vR = AE_MOVI(1);

  // first point
  vA0 = AE_L32X2_I(p_x0, 0);
  vA1 = AE_L32X2_I(p_x1, 0);

  vA0 = AE_SRAI32R(vA0, 2); 
  vA1 = AE_SRAI32R(vA1, 2); 

  vB0 = AE_ADD32S(vA0, vA1);
  vB1 = AE_SUB32S(vA0, vA1);
  vB0 = AE_SEL32_HH(vB0, vB1);
  vB1 = AE_MOVI(0);
  vB0 = AE_ADD32(vB0, vR);
  vB0 = AE_SRAI32(vB0, 1);
  AE_S32X2_IP(vB0, p_x0, 8);
  AE_S32X2_XP(vB1, p_x1, step_back);

  vA0 = AE_L32X2_I(p_x0, 0);
  vA1 = AE_L32X2_I(p_x1, 0);

  vA0 = AE_SRAI32R(vA0, 2); 
  vA1 = AE_SRAI32R(vA1, 2); 

  for(i = 1; i < (N>>2); i++) 
  {
    // load twiddle
    AE_L16X2M_XU(vB1, p_twd, step);
    vB1 = AE_SRAI32(vB1, 8);
    // conj (tw)
    vB1 = AE_SUBADD32S(0, vB1); 

    vT = AE_SAT16X4(vB1, vB1);

    // ADD/SUBB
    vB0 = AE_ADD32S(vA0, vA1);
    vB1 = AE_SUB32S(vA0, vA1);

    vA0 = AE_SEL32_LH(vB0, vB1);
    vB1 = AE_SEL32_HL(vB0, vB1);

    // do rotation
    vF1 = (vA0);
    vF2 = (vT);
    vF0 = AE_MULFC32X16RAS_H(vF1, vF2);
    vB0 = (vF0);
    vA1 = AE_NEG32S(vB0);
    vB0 = AE_SEL32_LH(vA1, vA1);

    // load next data
    vA0 = AE_L32X2_I(p_x0, 8);
    vA1 = AE_L32X2_I(p_x1, -8);

    vA0 = AE_SRAI32R(vA0, 2); 
    vA1 = AE_SRAI32R(vA1, 2); 

    vB0 = AE_ADD32S(vB0, vR);
    vB1 = AE_ADD32S(vB1, vR);
    vB0 = AE_SRAI32(vB0, 1);
    vB1 = AE_SRAI32(vB1, 1);

    // SUM/DIFF
    vC0 = AE_ADD32S(vB1, vB0);
    vC1 = AE_SUB32S(vB1, vB0);
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
  These functions make inverse FFT on real data with optimized memory usage.
  Scaling  : 
      24x24_ie        - Fixed data scaling at each stage
      32x16_ie        - Fixed data scaling at each stage
      32x16_ie_24p    - Fixed data scaling at each stage
      24x24_ie_24p    - 24-bit scaling
  NOTES:
  1. Bit-reversing reordering is done here.
  2. INPUT DATA MAY APPEAR DAMAGED after the call
  3. FFT functions may use input and output buffers for temporal
  storage of intermediate 32-bit data, so FFT functions
  with 24-bit packed I/O (Nx3-byte data) require that the
  buffers are large enough to keep Nx4-byte data
  4. Forward/inverse FFT of size N may be supplied with constant data
  (twiddle factors) of a larger-sized FFT = N*twdstep.

  Precision:
  24x24_ie      24-bit input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie      32-bit input/outputs, 32-bit data, 16-bit twiddles
  24x24_ie_24p  24-bit packed input/outputs, 24-bit data, 24-bit twiddles
  32x16_ie_24p  24-bit packed input/outputs, 32-bit data, 16-bit twiddles
  f_ie          floating point

  Input:
  x - complex input signal. Real and imaginary data are interleaved
  and real data goes first:
  --------------+----------+-----------------+----------------
  Function      |   Size   |  Allocated Size |       type    |
  --------------+----------+-----------------+----------------
  24x24_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  32x16_ie      |   N/2+1  |      N/2+1      |complex_fract32|
  24x24_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  32x16_ie_24p  |   3*(N+2)|      4*N+8      |       uint8_t |
  f_ie          |   N/2+1  |      N/2+1      | complex_float |
  --------------+----------+-----------------+----------------

  twd[2*N*twdstep*3/4]  twiddle factor table of a complex-valued FFT of size N*twdstep
  N                     FFT size
  twdstep               twiddle step
  scalingOpt            24x24_ie        - 3 (Fixed scaling)
                        32x16_ie        - 3 (Fixed scaling)
                        32x16_ie_24p    - 3 (Fixed scaling)
                        24x24_ie_24p    - 1 (24-bit scaling)
  Output:
  y - output spectrum. Real and imaginary data are interleaved and
  real data goes first:
  --------------+----------+-----------------+-----------
  Function      |   Size   |  Allocated Size |  type    |
  --------------+----------+-----------------+-----------
  24x24_ie      |      N   |      N          |   f24    |
  32x16_ie      |      N   |      N          |  int32_t |
  24x24_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  32x16_ie_24p  |    3*N   |      4*N+8      |  uint8_t |
  f_ie          |      N   |      N          | float32_t|
  --------------+----------+-----------------+-----------

  Returned value: total number of right shifts occurred during scaling
  procedure

  Restrictions:
  x,y - should not overlap
  x,y - aligned on 8-bytes boundary
  N   - 256, 512, 1024 for fixed point functions, power of two and >=8 for 
        floating point

-------------------------------------------------------------------------*/


int ifft_real32x16_ie    (  int32_t* y,complex_fract32* x, const complex_fract16* twd, int twdstep, int N, int scalingOpt)
{
    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt==3);
    NASSERT(N==256||N==512||N==1024);

    int scale  = 3; //Scaling factor of isplitPart_x2
#if 1
    isplitPart_x2((int32_t*)x, N, (const int16_t*)twd, twdstep);
    scale +=  ifft_cplx32x16_ie((complex_fract32*)y, x, twd, twdstep*2, N/2, scalingOpt);
#else
    // Call old FFT
    int i; 
    int shift = 1;

    if(N==256)
    {
        shift += ifft_real32x16(y, x, rifft16_256, 3);
    }
    else if(N==512)
    {
        shift += ifft_real32x16(y, x, rifft16_512, 3);
    }
    else if(N==1024)
    {
        shift += ifft_real32x16(y, x, rifft16_1024, 3);
    }

    scale = shift; 
#endif
    return scale;
}
