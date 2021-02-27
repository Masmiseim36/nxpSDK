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
FFT/IFFT    | Scaling options                        | Restrictions on the
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
#include "fft_cplx_twiddles.h"
#include "common.h"
#include "baseop.h"


/*-------------------------------------------------------------------------
  FFT on complex data
  NOTES:
  1. bit-reversing reordering is done here. 
  2. FFT does not make scaling of  input data and it should be done externally 
     to avoid possible overflows.
  3. FFT runs in-place algorithm so INPUT DATA WILL BE DAMAGED after call

  Input:
  x[N*2]    - input signal, Q15. Real and imaginary data are interleaved and 
              real data goes first
  N         - FFT size
  Output:
  y[N*2]    - output spectrum, Q15

  Restrictions:
  arrays should not be overlapping
  x,y - should be aligned by 4 bytes
  N   - may be 16,32,64,128,256 or 512.

  PERFORMANCE NOTES:
  none
-------------------------------------------------------------------------*/

#define FIRST_STAGE_SCALE 3

#define FFT_BUTTERFLY_S3(_T,_step,_X,twd_step)\
      {                                    \
      vT1 = AE_L16X4_I(p_twd, 8);          \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      vT2 = AE_L16X4_I(p_twd, 16);         \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      AE_L16X4_##_X(vTL, p_twd, twd_step); \
      vT0 = (vTL);                         \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vA0 = AE_L32X2_I(p_x0, 8);           \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_I(p_x1, 8);           \
      vC1 = AE_MULFC32X16RAS_L(vC1, vT0);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_I(p_x2, 8);           \
      vC2 = AE_MULFC32X16RAS_H(vC2, vT1);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_I(p_x3, 8);           \
      vC3 = AE_MULFC32X16RAS_H(vC3, vT0);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_IP(vC0, p_x0, 8);           \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_IP(vC1, p_x1, 8);           \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_IP(vC2, p_x3, 8);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      AE_S32X2_IP(vC3, p_x2, 8);           \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vA0 = AE_L32X2_##_T(p_x0, _step);    \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_##_T(p_x1, _step);    \
      vC1 = AE_MULFC32X16RAS_H(vC1, vT2);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_##_T(p_x2, _step);    \
      vC2 = AE_MULFC32X16RAS_L(vC2, vT2);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_##_T(p_x3, _step);    \
      vC3 = AE_MULFC32X16RAS_L(vC3, vT1);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_##_T##P(vC0, p_x0, _step);  \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_##_T##P(vC1, p_x1, _step);  \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_##_T##P(vC2, p_x3, _step);  \
      AE_S32X2_##_T##P(vC3, p_x2, _step);  \
      }

#define FFT_BUTTERFLY_S3_LAST(_T,_step,_X,twd_step)\
      {                                    \
      vT1 = AE_L16X4_I(p_twd, 8);          \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      vT2 = AE_L16X4_I(p_twd, 16);         \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      AE_L16X4_##_X(vTL, p_twd, twd_step); \
      vT0 = (vTL);                         \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vA0 = AE_L32X2_I(p_x0, 8);           \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_I(p_x1, 8);           \
      vC1 = AE_MULFC32X16RAS_L(vC1, vT0);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_I(p_x2, 8);           \
      vC2 = AE_MULFC32X16RAS_H(vC2, vT1);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_I(p_x3, 8);           \
      vC3 = AE_MULFC32X16RAS_H(vC3, vT0);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_IP(vC0, p_x0, 8);           \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_IP(vC1, p_x1, 8);           \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_IP(vC2, p_x3, 8);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      AE_S32X2_IP(vC3, p_x2, 8);           \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vC1 = AE_MULFC32X16RAS_H(vC1, vT2);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vC2 = AE_MULFC32X16RAS_L(vC2, vT2);  \
      vC0 = AE_SRAS32(vC0);                \
      vC3 = AE_MULFC32X16RAS_L(vC3, vT1);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_##_T##P(vC0, p_x0, _step);  \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_##_T##P(vC1, p_x1, _step);  \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_##_T##P(vC2, p_x3, _step);  \
      AE_S32X2_##_T##P(vC3, p_x2, _step);  \
      }

#define FFT_BUTTERFLY_S3_T3F(_T, _step)    \
      {                                    \
      vA0 = AE_L32X2_I(p_x0, 8);           \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_I(p_x1, 8);           \
      vC1 = AE_MULFC32X16RAS_L(vC1, vT0);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_I(p_x2, 8);           \
      vC2 = AE_MULFC32X16RAS_H(vC2, vT1);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_I(p_x3, 8);           \
      vC3 = AE_MULFC32X16RAS_H(vC3, vT0);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_IP(vC0, p_x0, 8);           \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_IP(vC1, p_x1, 8);           \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_IP(vC2, p_x3, 8);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      AE_S32X2_IP(vC3, p_x2, 8);           \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vA0 = AE_L32X2_##_T(p_x0, _step);    \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_##_T(p_x1, _step);    \
      vC1 = AE_MULFC32X16RAS_H(vC1, vT2);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_##_T(p_x2, _step);    \
      vC2 = AE_MULFC32X16RAS_L(vC2, vT2);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_##_T(p_x3, _step);    \
      vC3 = AE_MULFC32X16RAS_L(vC3, vT1);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_##_T##P(vC0, p_x0, _step);  \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_##_T##P(vC1, p_x1, _step);  \
      vB3 = AE_SUB32S(vA1, vA3);           \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_##_T##P(vC2, p_x3, _step);  \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      AE_S32X2_##_T##P(vC3, p_x2, _step);  \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vC1 = AE_SUB32S(vB0, vB1);           \
      }

#define FFT_BUTTERFLY_S3_T3F_LAST(_T, _step)    \
      {                                    \
      vA0 = AE_L32X2_I(p_x0, 8);           \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vA1 = AE_L32X2_I(p_x1, 8);           \
      vC1 = AE_MULFC32X16RAS_L(vC1, vT0);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vA2 = AE_L32X2_I(p_x2, 8);           \
      vC2 = AE_MULFC32X16RAS_H(vC2, vT1);  \
      vC0 = AE_SRAS32(vC0);                \
      vA3 = AE_L32X2_I(p_x3, 8);           \
      vC3 = AE_MULFC32X16RAS_H(vC3, vT0);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_IP(vC0, p_x0, 8);           \
      vB0 = AE_ADD32S(vA0, vA2);           \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_IP(vC1, p_x1, 8);           \
      vB2 = AE_SUB32S(vA0, vA2);           \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_IP(vC2, p_x3, 8);           \
      vB1 = AE_ADD32S(vA1, vA3);           \
      vB3 = AE_SUB32S(vA1, vA3);           \
      AE_S32X2_IP(vC3, p_x2, 8);           \
      vC1 = AE_SUB32S(vB0, vB1);           \
      vB3 = AE_SEL32_LH(vB3, vB3);         \
      vC0 = AE_ADD32S(vB0, vB1);           \
      vC2 = AE_SUBADD32S(vB2, vB3);        \
      vC1 = AE_MULFC32X16RAS_H(vC1, vT2);  \
      vC3 = AE_ADDSUB32S(vB2, vB3);        \
      vC2 = AE_MULFC32X16RAS_L(vC2, vT2);  \
      vC0 = AE_SRAS32(vC0);                \
      vC3 = AE_MULFC32X16RAS_L(vC3, vT1);  \
      vC1 = AE_SRAS32(vC1);                \
      AE_S32X2_##_T##P(vC0, p_x0, _step);  \
      vC2 = AE_SRAS32(vC2);                \
      AE_S32X2_##_T##P(vC1, p_x1, _step);  \
      vC3 = AE_SRAS32(vC3);                \
      AE_S32X2_##_T##P(vC2, p_x3, _step);  \
      AE_S32X2_##_T##P(vC3, p_x2, _step);  \
      }


#define FFT_BUTTERFLY_R2(idx)       \
    {                               \
      vA1 = AE_L32X2_I(p_x0, 8);    \
      vA2 = AE_L32X2_I(p_x0, 16);   \
      vA3 = AE_L32X2_I(p_x0, 24);   \
      AE_L32X2_XP(vA0, p_x0, 32);   \
      vB0 = AE_ADD32S(vA0, vA1);    \
      vB2 = AE_SUB32S(vA0, vA1);    \
      vB1 = AE_ADD32S(vA2, vA3);    \
      vB3 = AE_SUB32S(vA2, vA3);    \
      AE_S32X2_X(vB0, p_y0, idx);   \
      AE_S32X2_X(vB1, p_y1, idx);   \
      AE_S32X2_X(vB2, p_y2, idx);   \
      AE_S32X2_X(vB3, p_y3, idx);   \
    }

#define FFT_BUTTERFLY_R4(idx)       \
    {                               \
      vA1 = AE_L32X2_I(p_x0, 8);    \
      vA2 = AE_L32X2_I(p_x0, 16);   \
      vA3 = AE_L32X2_I(p_x0, 24);   \
      AE_L32X2_XP(vA0, p_x0, 32);   \
      vB0 = AE_ADD32S(vA0, vA2);    \
      vB2 = AE_SUB32S(vA0, vA2);    \
      vB1 = AE_ADD32S(vA1, vA3);    \
      vB3 = AE_SUB32S(vA1, vA3);    \
      vB3 = AE_SEL32_LH(vB3, vB3);  \
      vA0 = AE_ADD32S(vB0, vB1);    \
      vA2 = AE_SUB32S(vB0, vB1);    \
      vA1 = AE_ADDSUB32S(vB2, vB3); \
      vA3 = AE_SUBADD32S(vB2, vB3); \
      AE_S32X2_X(vA0, p_y0, idx);   \
      AE_S32X2_X(vA1, p_y1, idx);   \
      AE_S32X2_X(vA2, p_y2, idx);   \
      AE_S32X2_X(vA3, p_y3, idx);   \
    }

#ifdef COMPILER_MSVC
#define MSC_ALIGNED ALIGN(8)
#define GCC_ALIGNED
#else
#define MSC_ALIGNED
#define GCC_ALIGNED ALIGN(8)
#endif
static const int16_t MSC_ALIGNED fft_twd_r8[12] GCC_ALIGNED = {
    (int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
    (int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x5a82,(int16_t)0xA57E,
    (int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xA57E
};

static int fft_core_s3(int32_t *y, int32_t *x, const tFftDescr* pDescr)
{
  ae_int32x2 * restrict p_x0, * restrict p_x1,
             * restrict p_x2, * restrict p_x3;
  const ae_int16x4 * restrict p_twd;
  int step_circ;

  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3, vC0, vC1, vC2, vC3;
  ae_int16x4  vTL;
  ae_f16x4    vT0, vT1, vT2;
  const ae_int16 * restrict p_inc;
  int stride, scale = 0;
  int rsa;
  int N;
  const cint32_ptr * restrict seq;

  {
    N=pDescr->N;
    p_inc = (const ae_int16 *)(pDescr->inc);
    seq=(const cint32_ptr * )pDescr->twd;
    p_twd = (const ae_int16x4 *)*seq++;
  }

  stride     =   N; // The stride is quartered with every iteration of the outer loop.
  rsa = 2;

  if (stride > 4)
  {
    int lc = (N>>3)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_int32x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2_I(p_x0, 0);
    vA1 = AE_L32X2_I(p_x1, 0);
    vA2 = AE_L32X2_I(p_x2, 0);
    vA3 = AE_L32X2_I(p_x3, 0);

    vA0 = AE_SRAI32(vA0, FIRST_STAGE_SCALE);
    vA1 = AE_SRAI32(vA1, FIRST_STAGE_SCALE);
    vA2 = AE_SRAI32(vA2, FIRST_STAGE_SCALE);
    vA3 = AE_SRAI32(vA3, FIRST_STAGE_SCALE);

    if (stride==8) rsa >>= 1;

    WUR_AE_SAR(rsa);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    vT1 = AE_L16X4_I(p_twd, 8);
    vB3 = AE_SEL32_LH(vB3, vB3);
    vC0 = AE_ADD32S(vB0, vB1);

    do
    {
      AE_L16X4_IP(vTL, p_twd, 24);
      vT0 = (vTL);
      vC1 = AE_SUB32S(vB0, vB1);
      vC2 = AE_SUBADD32S(vB2, vB3);
      vA0 = AE_L32X2_I(p_x0, 8);
      vC3 = AE_ADDSUB32S(vB2, vB3);
      vC0 = AE_SRAS32(vC0);
      vA2 = AE_L32X2_I(p_x2, 8);
      vC1 = AE_MULFC32X16RAS_L(vC1, vT0);
      vA0 = AE_SRAI32(vA0, FIRST_STAGE_SCALE);
      vA1 = AE_L32X2_I(p_x1, 8);
      vC2 = AE_MULFC32X16RAS_H(vC2, vT1);
      vA2 = AE_SRAI32(vA2, FIRST_STAGE_SCALE);
      vA3 = AE_L32X2_I(p_x3, 8);
      vC3 = AE_MULFC32X16RAS_H(vC3, vT0);
      vA1 = AE_SRAI32(vA1, FIRST_STAGE_SCALE);
      vT2 = AE_L16X4_I(p_twd, -8);
      vB0 = AE_ADD32S(vA0, vA2);
      vA3 = AE_SRAI32(vA3, FIRST_STAGE_SCALE);
      AE_S32X2_IP(vC0, p_x0, 8);
      vB2 = AE_SUB32S(vA0, vA2);
      vC1 = AE_SRAS32(vC1);
      AE_S32X2_IP(vC1, p_x1, 8);
      vB1 = AE_ADD32S(vA1, vA3);
      vC2 = AE_SRAS32(vC2);
      AE_S32X2_IP(vC2, p_x3, 8);
      vB3 = AE_SUB32S(vA1, vA3);
      vC3 = AE_SRAS32(vC3);
      AE_S32X2_IP(vC3, p_x2, 8);
      vB3 = AE_SEL32_LH(vB3, vB3);
      vC0 = AE_ADD32S(vB0, vB1);
      vA0 = AE_L32X2_I(p_x0, 8);
      vC1 = AE_SUB32S(vB0, vB1);
      vC0 = AE_SRAS32(vC0);
      vA1 = AE_L32X2_I(p_x1, 8);
      vC1 = AE_MULFC32X16RAS_H(vC1, vT2);
      vC2 = AE_SUBADD32S(vB2, vB3);
      vA0 = AE_SRAI32(vA0, FIRST_STAGE_SCALE);
      vA2 = AE_L32X2_I(p_x2, 8);
      vC2 = AE_MULFC32X16RAS_L(vC2, vT2);
      vC3 = AE_ADDSUB32S(vB2, vB3);
      vA1 = AE_SRAI32(vA1, FIRST_STAGE_SCALE);
      vA3 = AE_L32X2_I(p_x3, 8);
      vC3 = AE_MULFC32X16RAS_L(vC3, vT1);
      vA2 = AE_SRAI32(vA2, FIRST_STAGE_SCALE);
      AE_S32X2_IP(vC0, p_x0, 8);
      vB0 = AE_ADD32S(vA0, vA2);
      vC1 = AE_SRAS32(vC1);
      AE_S32X2_IP(vC1, p_x1, 8);
      vB2 = AE_SUB32S(vA0, vA2);
      vA3 = AE_SRAI32(vA3, FIRST_STAGE_SCALE);
      vT1 = AE_L16X4_I(p_twd, 8);
      vB3 = AE_SUB32S(vA1, vA3);
      vC2 = AE_SRAS32(vC2);
      AE_S32X2_IP(vC2, p_x3, 8);
      vB1 = AE_ADD32S(vA1, vA3);
      vC3 = AE_SRAS32(vC3);
      AE_S32X2_IP(vC3, p_x2, 8);
      vB3 = AE_SEL32_LH(vB3, vB3);
      vC0 = AE_ADD32S(vB0, vB1);
    } while(--lc);
    AE_L16X4_IP(vTL, p_twd, 24);
    vT0 = (vTL);
    vC1 = AE_SUB32S(vB0, vB1);
    vC2 = AE_SUBADD32S(vB2, vB3);
    vA0 = AE_L32X2_I(p_x0, 8);
    vC3 = AE_ADDSUB32S(vB2, vB3);
    vC0 = AE_SRAS32(vC0);
    vA2 = AE_L32X2_I(p_x2, 8);
    vC1 = AE_MULFC32X16RAS_L(vC1, vT0);
    vA0 = AE_SRAI32(vA0, FIRST_STAGE_SCALE);
    vA1 = AE_L32X2_I(p_x1, 8);
    vC2 = AE_MULFC32X16RAS_H(vC2, vT1);
    vA2 = AE_SRAI32(vA2, FIRST_STAGE_SCALE);
    vA3 = AE_L32X2_I(p_x3, 8);
    vC3 = AE_MULFC32X16RAS_H(vC3, vT0);
    vA1 = AE_SRAI32(vA1, FIRST_STAGE_SCALE);
    vT2 = AE_L16X4_I(p_twd, -8);
    vB0 = AE_ADD32S(vA0, vA2);
    vA3 = AE_SRAI32(vA3, FIRST_STAGE_SCALE);
    AE_S32X2_IP(vC0, p_x0, 8);
    vB2 = AE_SUB32S(vA0, vA2);
    vC1 = AE_SRAS32(vC1);
    AE_S32X2_IP(vC1, p_x1, 8);
    vB1 = AE_ADD32S(vA1, vA3);
    vC2 = AE_SRAS32(vC2);
    AE_S32X2_IP(vC2, p_x3, 8);
    vB3 = AE_SUB32S(vA1, vA3);
    vC3 = AE_SRAS32(vC3);
    AE_S32X2_IP(vC3, p_x2, 8);
    vB3 = AE_SEL32_LH(vB3, vB3);
    vC0 = AE_ADD32S(vB0, vB1);
    vC1 = AE_SUB32S(vB0, vB1);
    vC0 = AE_SRAS32(vC0);
    vC1 = AE_MULFC32X16RAS_H(vC1, vT2);
    vC2 = AE_SUBADD32S(vB2, vB3);
    vA0 = AE_SRAI32(vA0, FIRST_STAGE_SCALE);
    vC2 = AE_MULFC32X16RAS_L(vC2, vT2);
    vC3 = AE_ADDSUB32S(vB2, vB3);
    vC3 = AE_MULFC32X16RAS_L(vC3, vT1);
    AE_S32X2_IP(vC0, p_x0, 8);
    vC1 = AE_SRAS32(vC1);
    AE_S32X2_IP(vC1, p_x1, 8);
    vC2 = AE_SRAS32(vC2);
    AE_S32X2_IP(vC2, p_x3, 8);
    vC3 = AE_SRAS32(vC3);
    AE_S32X2_IP(vC3, p_x2, 8);
    scale+= (stride!=8) ? 2:1;
    stride  >>= 2;
  }

  while (stride > 16)
  {
    int lc = (N>>4)-1;
    int step;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_int32x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2_I(p_x0, 0);
    vA1 = AE_L32X2_I(p_x1, 0);
    vA2 = AE_L32X2_I(p_x2, 0);
    vA3 = AE_L32X2_I(p_x3, 0);

    p_twd=(const ae_int16x4 *)(*seq++);
    WUR_AE_CBEGIN0( (uintptr_t)p_twd );
    WUR_AE_CEND0  ( (uintptr_t)(p_twd)+stride*3 );
    step_circ=24;

    do
    {
      AE_L16_IP(vTL, p_inc, 2);
      vTL = AE_SLAI16S(vTL, 2);
      step = AE_MOVAD16_0(vTL);

      FFT_BUTTERFLY_S3(I, 8,XC,step_circ);
      FFT_BUTTERFLY_S3(X, step,XC,step_circ)
    } while(--lc);
    AE_L16_IP(vTL, p_inc, 2);
    vTL = AE_SLAI16S(vTL, 2);
    step = AE_MOVAD16_0(vTL);

    FFT_BUTTERFLY_S3(I, 8,XC,step_circ);
    FFT_BUTTERFLY_S3_LAST(X, step,XC,step_circ)

    scale+= (stride!=8) ? 2:1;
    stride  >>= 2;
  }

  if (stride==16)
  {
    int lc = (N>>4)-1;
    const int step_fwd = 26*4;
    int step_circ;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_int32x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2_I(p_x0, 0);
    vA1 = AE_L32X2_I(p_x1, 0);
    vA2 = AE_L32X2_I(p_x2, 0);
    vA3 = AE_L32X2_I(p_x3, 0);
    p_twd=(const ae_int16x4 *)(*seq++);
    WUR_AE_CBEGIN0( (uintptr_t)p_twd );
    WUR_AE_CEND0  ( (uintptr_t)(p_twd)+4*12 );
    step_circ=24;

    do
    {
      FFT_BUTTERFLY_S3(X, 8,XC,step_circ)
      FFT_BUTTERFLY_S3(X, step_fwd,XC,step_circ)
    } while(--lc);
    FFT_BUTTERFLY_S3(X, 8,XC,step_circ)
    FFT_BUTTERFLY_S3_LAST(X, step_fwd,XC,step_circ)

    scale+= 2;
    stride  >>= 2;
  }


  if (stride==8) 
  {
    int lc = (N>>3)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_int32x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2_I(p_x0, 0);
    vA1 = AE_L32X2_I(p_x1, 0);
    vA2 = AE_L32X2_I(p_x2, 0);
    vA3 = AE_L32X2_I(p_x3, 0);

    rsa >>= 1;
    WUR_AE_SAR(rsa);
    vT0 = AE_L16X4_I(((const ae_int16x4*)fft_twd_r8), 0);
    vT1 = AE_L16X4_I(((const ae_int16x4*)fft_twd_r8), 8);
    vT2 = AE_L16X4_I(((const ae_int16x4*)fft_twd_r8), 16);

    vB0 = AE_ADD32S(vA0, vA2);
    vB3 = AE_SUB32S(vA1, vA3);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SEL32_LH(vB3, vB3);
    vC1 = AE_SUB32S(vB0, vB1);

    do
    {
      FFT_BUTTERFLY_S3_T3F(I, 56)
    } while(--lc);
    FFT_BUTTERFLY_S3_T3F_LAST(I, 56)

    scale += 1;
    stride >>= 2;
  }

  {
    int32_t i,i0,i1,i2,i3,ai;
    ae_int32x2 *p_y0 = (ae_int32x2 *)(y);
    ae_int32x2 *p_y1 = (p_y0 + (N >> 2));
    ae_int32x2 *p_y2 = (p_y1 + (N >> 2));
    ae_int32x2 *p_y3 = (p_y2 + (N >> 2));
    p_x0 = (ae_int32x2 *)(x);

    i = AE_NSAZ32_L(N)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;
    if (stride==2) 
    {
        //--------------------------------------------------------------------------
        // last stage is RADIX2 !!!
        //--------------------------------------------------------------------------
        for (i = 0; i < (N>>4); i++) 
        {
          i1 = AE_ADDBRBA32(i0, ai);
          i2 = AE_ADDBRBA32(i1, ai);
          i3 = AE_ADDBRBA32(i2, ai);
          FFT_BUTTERFLY_R2(i0);
          FFT_BUTTERFLY_R2(i1);
          FFT_BUTTERFLY_R2(i2);
          FFT_BUTTERFLY_R2(i3);
          i0 = AE_ADDBRBA32(i3, ai);
        }
    } 
    else 
    {
        //--------------------------------------------------------------------------
        // last stage is RADIX4 !!!
        //--------------------------------------------------------------------------
        for (i = 0; i < (N>>4); i++) 
        {
          i1 = AE_ADDBRBA32(i0, ai);
          i2 = AE_ADDBRBA32(i1, ai);
          i3 = AE_ADDBRBA32(i2, ai);
          //----------------------------------------------------------------
          // Read eight inputs, and perform radix4 decomposition            
          //----------------------------------------------------------------
          FFT_BUTTERFLY_R4(i0);
          FFT_BUTTERFLY_R4(i1);
          FFT_BUTTERFLY_R4(i2);
          FFT_BUTTERFLY_R4(i3);
          i0 = AE_ADDBRBA32(i3, ai);
        }
    }
  }

  return scale;
}

/*-------------------------------------------------------------------------
  These functions make FFT on complex data.
  NOTES:
  1. Bit-reversing permutation is done here. 
  2. FFT runs in-place algorithm so INPUT DATA WILL APPEAR DAMAGED after 
     the call

  Precision: 
  32x32  32-bit input/outputs, 32-bit twiddles
  24x24  24-bit input/outputs, 24-bit twiddles
  32x16  32-bit input/outputs, 16-bit twiddles
  16x16  16-bit input/outputs, 16-bit twiddles
 
  Input:
  x[2*N]     complex input signal. Real and imaginary data are interleaved 
             and real data goes first
  N          FFT size
  scalingOpt scaling option
  Output:
  y[2*N]     output spectrum. Real and imaginary data are interleaved and 
             real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m, 16...4096

-------------------------------------------------------------------------*/
int fft_cplx32x16( int32_t* y,int32_t* x,fft_handle_t h,int scalingOption)
{
  int scale=0;
  int sar;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(scalingOption==3);

  sar = RUR_AE_SAR();
  scale=FIRST_STAGE_SCALE;
  scale+=fft_core_s3 (y, x, (const tFftDescr*)h);
  WUR_AE_SAR(sar);

  return scale;
}
