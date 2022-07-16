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
#include "common.h"
#include "NatureDSP_Signal.h"
#include "baseop.h"
#include "fft_cplx_twiddles_24x24.h"

#define MAX(x,y) ((x)>(y)?(x):(y))                                    
#define MIN(x,y) ((x)<(y)?(x):(y))                                    

#if 0 //__MNSD__
static const int32_t MSC_ALIGNED ifft24x24_twd_r8[12] GCC_ALIGNED = {
    (int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x5a827900,(int32_t)0x5a827900,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0xa57d8600,(int32_t)0x5a827900};
#endif 

#define IFFT_BUTTERFLY_S0(_T,_step,_X,twd_step) \
      {                                         \
        vT2 = AE_L32X2F24_I(p_twd, 8);          \
        vB3 = AE_SEL32_LH(vB3, vB3);            \
        vA1 = AE_SUB32S(vB0, vB1);              \
        vT3 = AE_L32X2F24_I(p_twd, 16);         \
        vB0 = AE_ADD32S(vB0, vB1);              \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step); \
        vA2 = AE_ADDSUB32S(vB2, vB3);           \
        vA0 = AE_L32X2F24_##_T(p_x0, _step);    \
        vA3 = AE_SUBADD32S(vB2, vB3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);     \
        vB1 = AE_MULFC24RA(vF1, vT2);           \
        vA1 = AE_L32X2F24_##_T(p_x1, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);     \
        vB2 = AE_MULFC24RA(vF1, vT3);           \
        vA2 = AE_L32X2F24_##_T(p_x2, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);     \
        vB3 = AE_MULFC24RA(vF1, vT1);           \
        vA3 = AE_L32X2F24_##_T(p_x3, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB0);     \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step);  \
        vB0 = AE_ADD32S(vA0, vA2);              \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB2);     \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step);  \
        vB2 = AE_SUB32S(vA0, vA2);              \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB1);     \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step);  \
        vB1 = AE_ADD32S(vA1, vA3);              \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB3);     \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step);  \
        vB3 = AE_SUB32S(vA1, vA3);              \
      }

#define IFFT_BUTTERFLY_S0_LAST(_T,_step,_X,twd_step) \
      {                                         \
        vT2 = AE_L32X2F24_I(p_twd, 8);          \
        vB3 = AE_SEL32_LH(vB3, vB3);            \
        vA1 = AE_SUB32S(vB0, vB1);              \
        vT3 = AE_L32X2F24_I(p_twd, 16);         \
        vB0 = AE_ADD32S(vB0, vB1);              \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step); \
        vA2 = AE_ADDSUB32S(vB2, vB3);           \
        vA3 = AE_SUBADD32S(vB2, vB3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);     \
        vB1 = AE_MULFC24RA(vF1, vT2);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);     \
        vB2 = AE_MULFC24RA(vF1, vT3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);     \
        vB3 = AE_MULFC24RA(vF1, vT1);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB0);     \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step);  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB2);     \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step);  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB1);     \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step);  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB3);     \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step);  \
      }
#define IFFT_BUTTERFLY_S0_T0F(_T,_step) \
      {                                         \
        vB3 = AE_SEL32_LH(vB3, vB3);            \
        vA1 = AE_SUB32S(vB0, vB1);              \
        vB0 = AE_ADD32S(vB0, vB1);              \
        vA2 = AE_ADDSUB32S(vB2, vB3);           \
        vA0 = AE_L32X2F24_##_T(p_x0, _step);    \
        vA3 = AE_SUBADD32S(vB2, vB3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);     \
        vB1 = AE_MULFC24RA(vF1, vT2);           \
        vA1 = AE_L32X2F24_##_T(p_x1, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);     \
        vB2 = AE_MULFC24RA(vF1, vT3);           \
        vA2 = AE_L32X2F24_##_T(p_x2, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);     \
        vB3 = AE_MULFC24RA(vF1, vT1);           \
        vA3 = AE_L32X2F24_##_T(p_x3, _step);    \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB0);     \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step);  \
        vB0 = AE_ADD32S(vA0, vA2);              \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB2);     \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step);  \
        vB2 = AE_SUB32S(vA0, vA2);              \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB1);     \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step);  \
        vB1 = AE_ADD32S(vA1, vA3);              \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB3);     \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step);  \
        vB3 = AE_SUB32S(vA1, vA3);              \
      }
#define IFFT_BUTTERFLY_S0_T0F_LAST(_T,_step) \
      {                                         \
        vB3 = AE_SEL32_LH(vB3, vB3);            \
        vA1 = AE_SUB32S(vB0, vB1);              \
        vB0 = AE_ADD32S(vB0, vB1);              \
        vA2 = AE_ADDSUB32S(vB2, vB3);           \
        vA3 = AE_SUBADD32S(vB2, vB3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);     \
        vB1 = AE_MULFC24RA(vF1, vT2);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);     \
        vB2 = AE_MULFC24RA(vF1, vT3);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);     \
        vB3 = AE_MULFC24RA(vF1, vT1);           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB0);     \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step);  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB2);     \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step);  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vB1);     \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step);  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vB3);     \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step);  \
      }

#define IFFT_BUTTERFLY_S1(_T,_step,_X,twd_step)\
      {                                        \
        vT2 = AE_L32X2F24_I(p_twd, 8);         \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA0 = AE_L32X2F24_##_T(p_x0, _step);   \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vMX = AE_MAXABS32S(vMX, vC0);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vC2 = AE_MULFC24RA(vF1, vT3);          \
        vA2 = AE_L32X2F24_##_T(p_x2, _step);   \
        vA0 = AE_SRAS32(vA0);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vC1 = AE_MULFC24RA(vF1, vT2);          \
        vA1 = AE_L32X2F24_##_T(p_x1, _step);   \
        vA2 = AE_SRAS32(vA2);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vC3 = AE_MULFC24RA(vF1, vT1);          \
        vA3 = AE_L32X2F24_##_T(p_x3, _step);   \
        vA1 = AE_SRAS32(vA1);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vB0 = AE_ADD32S(vA0, vA2);             \
        vA3 = AE_SRAS32(vA3);                  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vB2 = AE_SUB32S(vA0, vA2);             \
        vMX = AE_MAXABS32S(vMX, vC2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vB1 = AE_ADD32S(vA1, vA3);             \
        vMX = AE_MAXABS32S(vMX, vC1);          \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vB3 = AE_SUB32S(vA1, vA3);             \
        vMX = AE_MAXABS32S(vMX, vC3);          \
      }

#define IFFT_BUTTERFLY_S1_LAST(_T,_step,_X,twd_step)\
      {                                        \
        vT2 = AE_L32X2F24_I(p_twd, 8);         \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vMX = AE_MAXABS32S(vMX, vC0);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vC2 = AE_MULFC24RA(vF1, vT3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vC1 = AE_MULFC24RA(vF1, vT2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vC3 = AE_MULFC24RA(vF1, vT1);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vMX = AE_MAXABS32S(vMX, vC2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vMX = AE_MAXABS32S(vMX, vC1);          \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vMX = AE_MAXABS32S(vMX, vC3);          \
      }

#define IFFT_BUTTERFLY_S1_T1F(_T,_step)\
      {                                        \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA0 = AE_L32X2F24_##_T(p_x0, _step);   \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vMX = AE_MAXABS32S(vMX, vC0);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vC2 = AE_MULFC24RA(vF1, vT3);          \
        vA2 = AE_L32X2F24_##_T(p_x2, _step);   \
        vA0 = AE_SRAS32(vA0);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vC1 = AE_MULFC24RA(vF1, vT2);          \
        vA1 = AE_L32X2F24_##_T(p_x1, _step);   \
        vA2 = AE_SRAS32(vA2);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vC3 = AE_MULFC24RA(vF1, vT1);          \
        vA3 = AE_L32X2F24_##_T(p_x3, _step);   \
        vA1 = AE_SRAS32(vA1);                  \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vB0 = AE_ADD32S(vA0, vA2);             \
        vA3 = AE_SRAS32(vA3);                  \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vB2 = AE_SUB32S(vA0, vA2);             \
        vMX = AE_MAXABS32S(vMX, vC2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vB1 = AE_ADD32S(vA1, vA3);             \
        vMX = AE_MAXABS32S(vMX, vC1);          \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vB3 = AE_SUB32S(vA1, vA3);             \
        vMX = AE_MAXABS32S(vMX, vC3);          \
      }
#define IFFT_BUTTERFLY_S1_T1F_LAST(_T,_step)\
      {                                        \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vMX = AE_MAXABS32S(vMX, vC0);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vC2 = AE_MULFC24RA(vF1, vT3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vC1 = AE_MULFC24RA(vF1, vT2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vC3 = AE_MULFC24RA(vF1, vT1);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vMX = AE_MAXABS32S(vMX, vC2);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vMX = AE_MAXABS32S(vMX, vC1);          \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vMX = AE_MAXABS32S(vMX, vC3);          \
      }

#define IFFT_BUTTERFLY_S3_0(_T,_step,_X,twd_step)\
      {                                        \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vF1 = AE_L32X2F24_##_T(p_x0, _step);   \
        vA0 = (vF1);                           \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x1, _step);   \
        vA1 = (vF2);                           \
        vA0 = AE_SRAI32(vA0, 3);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_L32X2F24_##_T(p_x2, _step);   \
        vA2 = (vF1);                           \
        vA1 = AE_SRAI32(vA1, 3);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x3, _step);   \
        vA3 = (vF2);                           \
        vA2 = AE_SRAI32(vA2, 3);               \
        vT2 = AE_L32X2F24_I(p_twd, 8);         \
        vB0 = AE_ADD32S(vA0, vA2);             \
        vA3 = AE_SRAI32(vA3, 3);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vB2 = AE_SUB32S(vA0, vA2);             \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vB1 = AE_ADD32S(vA1, vA3);             \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vB3 = AE_SUB32S(vA1, vA3);             \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vA1 = AE_SUB32S(vB0, vB1);             \
      }
#define IFFT_BUTTERFLY_S3_0_LAST(_T,_step,_X,twd_step)\
      {                                        \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
      }

#define IFFT_BUTTERFLY_S3(_T,_step,_X,twd_step)\
      {                                        \
        vT2 = AE_L32X2F24_I(p_twd, 8);         \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vF1 = AE_L32X2F24_##_T(p_x0, _step);   \
        vA0 = (vF1);                           \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x1, _step);   \
        vA1 = (vF2);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_L32X2F24_##_T(p_x2, _step);   \
        vA2 = (vF1);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x3, _step);   \
        vA3 = (vF2);                           \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vB0 = AE_ADD32S(vA0, vA2);             \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vB2 = AE_SUB32S(vA0, vA2);             \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vB1 = AE_ADD32S(vA1, vA3);             \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vB3 = AE_SUB32S(vA1, vA3);             \
      }
#define IFFT_BUTTERFLY_S3_LAST(_T,_step,_X,twd_step)\
      {                                        \
        vT2 = AE_L32X2F24_I(p_twd, 8);         \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vT3 = AE_L32X2F24_I(p_twd, 16);        \
        vC0 = AE_ADD32S(vB0, vB1);             \
        AE_L32X2F24_##_X(vT1, p_twd, twd_step);\
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
      }

#define IFFT_BUTTERFLY_S3_T3F(_T,_step)\
      {                                        \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vF1 = AE_L32X2F24_##_T(p_x0, _step);   \
        vA0 = (vF1);                           \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x1, _step);   \
        vA1 = (vF2);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_L32X2F24_##_T(p_x2, _step);   \
        vA2 = (vF1);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vF2 = AE_L32X2F24_##_T(p_x3, _step);   \
        vA3 = (vF2);                           \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vB0 = AE_ADD32S(vA0, vA2);             \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vB2 = AE_SUB32S(vA0, vA2);             \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vB1 = AE_ADD32S(vA1, vA3);             \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
        vB3 = AE_SUB32S(vA1, vA3);             \
      }
#define IFFT_BUTTERFLY_S3_T3F_LAST(_T,_step)\
      {                                        \
        vB3 = AE_SEL32_LH(vB3, vB3);           \
        vA1 = AE_SUB32S(vB0, vB1);             \
        vC0 = AE_ADD32S(vB0, vB1);             \
        vA2 = AE_ADDSUB32S(vB2, vB3);          \
        vA3 = AE_SUBADD32S(vB2, vB3);          \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA1);    \
        vF0 = AE_MULFC24RA(vF1, vT2);          \
        vC1 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA2);    \
        vF0 = AE_MULFC24RA(vF1, vT3);          \
        vC2 = (vF0);                           \
        vF1 = AE_MOVF24X2_FROMINT32X2(vA3);    \
        vF0 = AE_MULFC24RA(vF1, vT1);          \
        vC3 = (vF0);                           \
        vC0 = AE_SRAI32(vC0, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC0);    \
        AE_S32X2F24_##_T##P(vF1, p_x0, _step); \
        vC2 = AE_SRAI32(vC2, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC2);    \
        AE_S32X2F24_##_T##P(vF2, p_x3, _step); \
        vC1 = AE_SRAI32(vC1, 2);               \
        vF1 = AE_MOVF24X2_FROMINT32X2(vC1);    \
        AE_S32X2F24_##_T##P(vF1, p_x1, _step); \
        vC3 = AE_SRAI32(vC3, 2);               \
        vF2 = AE_MOVF24X2_FROMINT32X2(vC3);    \
        AE_S32X2F24_##_T##P(vF2, p_x2, _step); \
      }

#define IFFT_BUTTERFLY_R2(idx)         \
    {                                  \
    vA1 = AE_L32X2F24_I(p_x0, 8);      \
    vA2 = AE_L32X2F24_I(p_x0, 16);     \
    vA3 = AE_L32X2F24_I(p_x0, 24);     \
    AE_L32X2F24_IP(vF2, p_x0, 32);     \
    vA0 = (vF2);                       \
    vB0 = AE_ADD32S(vA0, vA1);         \
    vB2 = AE_SUB32S(vA0, vA1);         \
    vB1 = AE_ADD32S(vA2, vA3);         \
    vB3 = AE_SUB32S(vA2, vA3);         \
    vF1 = AE_MOVF24X2_FROMINT32X2(vB0);\
    AE_S32X2F24_X(vF1, p_y0, idx);     \
    vF2 = AE_MOVF24X2_FROMINT32X2(vB1);\
    AE_S32X2F24_X(vF2, p_y1, idx);     \
    vF1 = AE_MOVF24X2_FROMINT32X2(vB2);\
    AE_S32X2F24_X(vF1, p_y2, idx);     \
    vF2 = AE_MOVF24X2_FROMINT32X2(vB3);\
    AE_S32X2F24_X(vF2, p_y3, idx);     \
    }

#define IFFT_BUTTERFLY_R4(idx)         \
    {                                  \
    vA1 = AE_L32X2F24_I(p_x0, 8);      \
    vA2 = AE_L32X2F24_I(p_x0, 16);     \
    vA3 = AE_L32X2F24_I(p_x0, 24);     \
    AE_L32X2F24_IP(vF2, p_x0, 32);     \
    vA0 = (vF2);                       \
    vB0 = AE_ADD32S(vA0, vA2);         \
    vB2 = AE_SUB32S(vA0, vA2);         \
    vB1 = AE_ADD32S(vA1, vA3);         \
    vB3 = AE_SUB32S(vA1, vA3);         \
    vB3 = AE_SEL32_LH(vB3, vB3);       \
    vA0 = AE_ADD32S(vB0, vB1);         \
    vA2 = AE_SUB32S(vB0, vB1);         \
    vA1 = AE_SUBADD32S(vB2, vB3);      \
    vA3 = AE_ADDSUB32S(vB2, vB3);      \
    vF1 = AE_MOVF24X2_FROMINT32X2(vA0);\
    AE_S32X2F24_X(vF1, p_y0, idx);     \
    vF2 = AE_MOVF24X2_FROMINT32X2(vA1);\
    AE_S32X2F24_X(vF2, p_y1, idx);     \
    vF1 = AE_MOVF24X2_FROMINT32X2(vA2);\
    AE_S32X2F24_X(vF1, p_y2, idx);     \
    vF2 = AE_MOVF24X2_FROMINT32X2(vA3);\
    AE_S32X2F24_X(vF2, p_y3, idx);     \
    }

static void ifft_cplx_24x24_lbut2 (int32_t *y, int32_t *x, int N)
{
  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f24x2    vF1, vF2;

    int32_t i,i0,i1,i2,i3,ai;
    ae_f24x2 * restrict p_y0 = (ae_f24x2 *)(y);
    ae_f24x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_f24x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_f24x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_f24x2 * restrict p_x0 = (ae_f24x2 *)(x);

    i = AE_NSAZ32_L(N)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;
    for (i = 0; i < (N>>4); i++) 
    {
      i1 = AE_ADDBRBA32(i0, ai);
      i2 = AE_ADDBRBA32(i1, ai);
      i3 = AE_ADDBRBA32(i2, ai);
      //----------------------------------------------------------------
      // Read eight inputs, and perform radix4 decomposition            
      //----------------------------------------------------------------
      IFFT_BUTTERFLY_R2(i0);
      IFFT_BUTTERFLY_R2(i1);
      IFFT_BUTTERFLY_R2(i2);
      IFFT_BUTTERFLY_R2(i3);
      i0 = AE_ADDBRBA32(i3, ai);
    }

}

static void ifft_cplx_24x24_lbut4 (int32_t *y, int32_t *x, int N)
{
  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f24x2    vF1, vF2;

    int32_t i,i0,i1,i2,i3,ai;
    ae_f24x2 * restrict p_y0 = (ae_f24x2 *)(y);
    ae_f24x2 * restrict p_y1 = (p_y0 + (N >> 2));
    ae_f24x2 * restrict p_y2 = (p_y1 + (N >> 2));
    ae_f24x2 * restrict p_y3 = (p_y2 + (N >> 2));
    ae_f24x2 * restrict p_x0 = (ae_f24x2 *)(x);

    i = AE_NSAZ32_L(N)+1;
    ai=((int32_t)0x1)<<i;
    i0=0;
    for (i = 0; i < (N>>4); i++) 
    {
      i1 = AE_ADDBRBA32(i0, ai);
      i2 = AE_ADDBRBA32(i1, ai);
      i3 = AE_ADDBRBA32(i2, ai);
      //----------------------------------------------------------------
      // Read eight inputs, and perform radix4 decomposition            
      //----------------------------------------------------------------
      IFFT_BUTTERFLY_R4(i0);
      IFFT_BUTTERFLY_R4(i1);
      IFFT_BUTTERFLY_R4(i2);
      IFFT_BUTTERFLY_R4(i3);
      i0 = AE_ADDBRBA32(i3, ai);
    }
}

static int ifft_cplx_24x24_core_s0(int32_t *y, int32_t *x, const tFftDescr* pDescr)
{
  ae_f24x2 * restrict p_x0, * restrict p_x1, * restrict p_x2, * restrict p_x3;
  const ae_f24x2 * restrict p_twd;
  const ae_int16 * restrict p_inc;
  int step_circ;

  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f24x2    vF1, vF2, vT1, vT2, vT3;
  ae_f24x2    vT1_1, vT2_1, vT3_1;
  ae_f24x2    vT1_2, vT2_2, vT3_2;

  ae_int16x4  vS;

  int N;
  int stride,scale=0;

  {
    N=pDescr->N;
    p_twd = (const ae_f24x2 *)pDescr->twd;
    p_inc = (const ae_int16 *)pDescr->inc;
  }
  WUR_AE_CBEGIN0( (uintptr_t)p_twd );
  WUR_AE_CEND0  ( (uintptr_t)(p_twd)+6*N );
  step_circ=24;
  stride     =   N; // The stride is quartered with every iteration of the outer loop.

  if (stride > 4)
  { // first iteration for pre-scaling

    int lc = (N>>2)-1;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
    } while(--lc);
    IFFT_BUTTERFLY_S0_LAST(I, 2*4,XC,step_circ)

    stride  >>= 2;
    step_circ<<=2;
  }

  while (stride > 16)
  {
    int lc = (N>>4)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      int step;
      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0(X, step,XC,step_circ);

    } while(--lc);
    {
      int step;
      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S0_LAST(X, step,XC,step_circ);
    }

    stride  >>= 2;
    step_circ<<=2;
  }

  if (stride == 16)
  {
    int lc = (N>>4)-1;
    const int step_fwd = 26*4;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + 4;
    p_x2 = p_x1 + 4;
    p_x3 = p_x2 + 4;

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S0(X, step_fwd,XC,step_circ)
    } while(--lc);
    IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S0(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S0_LAST(X, step_fwd,XC,step_circ)

    stride  >>= 2;
  }

  if (stride == 8)
  {
    int lc = (N>>3)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + 2;
    p_x2 = p_x1 + 2;
    p_x3 = p_x2 + 2;

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    vT2_1 = AE_L32X2F24_I(p_twd, 8);          
    vT3_1 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_1, p_twd,step_circ); 
    vT2_2 = AE_L32X2F24_I(p_twd, 8);          
    vT3_2 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_2, p_twd,step_circ); 
    do
    {
      vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
      IFFT_BUTTERFLY_S0_T0F(I, 2*4 )
      vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
      IFFT_BUTTERFLY_S0_T0F(I, 14*4)
    } while(--lc);
    vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
    IFFT_BUTTERFLY_S0_T0F(I, 2*4 )
    vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
    IFFT_BUTTERFLY_S0_T0F_LAST(I, 14*4)

    stride  >>= 2;
  }

  switch (stride)
  {
    case 2:
      ifft_cplx_24x24_lbut2(y, x, N);
      break;
    case 4:
      ifft_cplx_24x24_lbut4(y, x, N);
      break;
  }

  return scale;
}

static int ifft_cplx_24x24_core_s1(int32_t *y, int32_t *x, const tFftDescr* pDescr, int exp)
{
  ae_f24x2 * restrict p_x0, * restrict p_x1, * restrict p_x2, * restrict p_x3;
  const ae_f24x2 * restrict p_twd;
  const ae_int16 * restrict p_inc;

  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3, vC0, vC1, vC2, vC3, vMX;
  ae_int64    vL1;
  ae_f24x2    vF1, vF2, vT1, vT2, vT3;
  ae_f24x2    vT1_1, vT2_1, vT3_1;
  ae_f24x2    vT1_2, vT2_2, vT3_2;
  ae_int16x4  vS;
  int N;
  int step_circ;
  int stride,scale=0;

  {
    N=pDescr->N;
    p_twd = (const ae_f24x2 *)pDescr->twd;
    p_inc = (const ae_int16 *)pDescr->inc;
  }
  WUR_AE_CBEGIN0( (uintptr_t)p_twd );
  WUR_AE_CEND0  ( (uintptr_t)(p_twd)+6*N );
  step_circ=24;
  stride     =   N; // The stride is quartered with every iteration of the outer loop.

  if (stride > 4)
  { // first iteration for pre-scaling
    int lc = (N>>2)-1;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vMX = AE_MOVI(0);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);
    scale += exp;

    WUR_AE_SAR(exp);

    vA0 = AE_SRAS32(vA0);
    vA1 = AE_SRAS32(vA1);
    vA2 = AE_SRAS32(vA2);
    vA3 = AE_SRAS32(vA3);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
    } while(--lc);
    IFFT_BUTTERFLY_S1_LAST(I, 2*4,XC,step_circ)

    stride  >>= 2;
    step_circ<<=2;

    vB1 = AE_SEL32_LH(vMX, vMX);
    vMX = AE_MAX32(vMX, vB1);
    vL1 = AE_MOVINT64_FROMINT32X2(vMX);
    exp = 11-AE_NSA64(vL1);
    exp=XT_MAX(exp,0);
  }

  while (stride > 16)
  {
    int lc = (N>>4)-1;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vMX = AE_MOVI(0);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    scale += exp;

    WUR_AE_SAR(exp);

    vA0 = AE_SRAS32(vA0);
    vA1 = AE_SRAS32(vA1);
    vA2 = AE_SRAS32(vA2);
    vA3 = AE_SRAS32(vA3);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      int step;

      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);

      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(X, step,XC,step_circ)
    } while(--lc);
    {
      int step;

      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);

      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1_LAST(X, step,XC,step_circ)
    } 

    stride  >>= 2;
    step_circ<<=2;

    vB1 = AE_SEL32_LH(vMX, vMX);
    vMX = AE_MAX32(vMX, vB1);
    vL1 = AE_MOVINT64_FROMINT32X2(vMX);
    exp = 11-AE_NSA64(vL1);
    exp=XT_MAX(exp,0);
  }

  if (stride==16)
  {
    int lc = (N>>4)-1;
    const int step_fwd = 26*4;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vMX = AE_MOVI(0);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    scale += exp;

    WUR_AE_SAR(exp);

    vA0 = AE_SRAS32(vA0);
    vA1 = AE_SRAS32(vA1);
    vA2 = AE_SRAS32(vA2);
    vA3 = AE_SRAS32(vA3);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S1(X, step_fwd,XC,step_circ)
    } while(--lc);
    AE_L16_IP(vS, p_inc, 2);
    vS = AE_SLAI16S(vS, 2);
    IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S1(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S1_LAST(X, step_fwd,XC,step_circ)

    stride  >>= 2;

    vB1 = AE_SEL32_LH(vMX, vMX);
    vMX = AE_MAX32(vMX, vB1);
    vL1 = AE_MOVINT64_FROMINT32X2(vMX);
    exp = 11-AE_NSA64(vL1);
    exp=XT_MAX(exp,0);
  }

  if (stride==8)
  {
    int lc = (N>>3)-1;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vMX = AE_MOVI(0);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    scale += exp;

    WUR_AE_SAR(exp);

    vA0 = AE_SRAS32(vA0);
    vA1 = AE_SRAS32(vA1);
    vA2 = AE_SRAS32(vA2);
    vA3 = AE_SRAS32(vA3);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    vT2_1 = AE_L32X2F24_I(p_twd, 8);          
    vT3_1 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_1, p_twd,step_circ); 
    vT2_2 = AE_L32X2F24_I(p_twd, 8);          
    vT3_2 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_2, p_twd,step_circ); 
    do
    {
      vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
      IFFT_BUTTERFLY_S1_T1F(I, 2*4)
      vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
      IFFT_BUTTERFLY_S1_T1F(I, 14*4)
    } while(--lc);
    vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
    IFFT_BUTTERFLY_S1_T1F(I, 2*4)
    vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
    IFFT_BUTTERFLY_S1_T1F_LAST(I, 14*4)

    stride  >>= 2;

    vB1 = AE_SEL32_LH(vMX, vMX);
    vMX = AE_MAX32(vMX, vB1);
    vL1 = AE_MOVINT64_FROMINT32X2(vMX);
    exp = 11-AE_NSA64(vL1);
    exp=XT_MAX(exp,0);
  }

  if (exp>0)
  {
    int k;

    WUR_AE_SAR(exp);

    p_x0 = (ae_f24x2 *)x;

    AE_L32X2F24_IP(vF1, p_x0, 8);
    AE_L32X2F24_IP(vF2, p_x0, 8);
    vA0 = (vF1);
    vA1 = (vF2);

    for (k=0; k<(N>>1)-1; ++k)
    {
      vB0 = AE_SRAS32(vA0);
      AE_L32X2F24_IP(vF1, p_x0, 8);
      vA0 = (vF1);
      
      vB1 = AE_SRAS32(vA1);
      AE_L32X2F24_IP(vF2, p_x0, 8);
      vA1 = (vF2);
      
      vF1 = AE_MOVF24X2_FROMINT32X2(vB0);
      AE_S32X2F24_I(vF1, p_x0, -32);

      vF2 = AE_MOVF24X2_FROMINT32X2(vB1);
      AE_S32X2F24_I(vF2, p_x0, -24);
    }
    vB0 = AE_SRAS32(vA0);
    vB1 = AE_SRAS32(vA1);
    vF1 = AE_MOVF24X2_FROMINT32X2(vB0);
    AE_S32X2F24_I(vF1, p_x0, -16);
    vF2 = AE_MOVF24X2_FROMINT32X2(vB1);
    AE_S32X2F24_I(vF2, p_x0, -8);
    scale += exp;
  }

  switch (stride)
  {
    case 2:
      ifft_cplx_24x24_lbut2(y, x, N);
      break;
    case 4:
      ifft_cplx_24x24_lbut4(y, x, N);
      break;
  }

  return scale;
}

static int ifft_cplx_24x24_core_s3 (int32_t *y, int32_t *x, const tFftDescr* pDescr)
{
  ae_f24x2 * restrict p_x0, * restrict p_x1, * restrict p_x2, * restrict p_x3;
  const ae_f24x2 * restrict p_twd;
  const ae_int16 * restrict p_inc;

  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3, vC0, vC1, vC2, vC3;
  ae_f24x2    vF1, vF2, vT1, vT2, vT3;
  ae_f24x2    vT1_1, vT2_1, vT3_1;
  ae_f24x2    vT1_2, vT2_2, vT3_2;
  ae_f32x2    vF0;
  ae_int16x4  vS;

  int N;
  int step_circ;
  int stride,scale=0;

  {
    N=pDescr->N;
    p_twd = (const ae_f24x2 *)pDescr->twd;
    p_inc = (const ae_int16 *)pDescr->inc;
  }
  WUR_AE_CBEGIN0( (uintptr_t)p_twd );
  WUR_AE_CEND0  ( (uintptr_t)(p_twd)+6*N );
  step_circ=24;
  stride     =   N; // The stride is quartered with every iteration of the outer loop.

  scale = 5;

  if (stride > 4)
  { // first iteration for pre-scaling

    int lc = (N>>2)-1;

    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vA0 = AE_SRAI32(vA0, 3);
    vA1 = AE_SRAI32(vA1, 3);
    vA2 = AE_SRAI32(vA2, 3);
    vA3 = AE_SRAI32(vA3, 3);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    vT2 = AE_L32X2F24_I(p_twd, 8);
    vA1 = AE_SUB32S(vB0, vB1);
    do
    {
      IFFT_BUTTERFLY_S3_0(I, 2*4,XC,step_circ)
    } while(--lc);
    IFFT_BUTTERFLY_S3_0_LAST(I, 2*4,XC,step_circ)

    stride  >>= 2;
    step_circ<<=2;
  }

  while (stride > 16)
  {
    int lc = (N>>4)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      int step;

      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);

      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3(X, step,XC,step_circ);

    } while(--lc);
    {
      int step;

      AE_L16_IP(vS, p_inc, 2);
      vS = AE_SLAI16S(vS, 2);
      step = AE_MOVAD16_0(vS);

      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ);
      IFFT_BUTTERFLY_S3_LAST(X, step,XC,step_circ);
    } 

    stride  >>= 2;
    step_circ<<=2;
    scale += 2;
  }

  if (stride == 16)
  {
    int lc = (N>>4)-1;
    const int step_fwd = 26*4;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    do 
    {
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
      IFFT_BUTTERFLY_S3(X, step_fwd,XC,step_circ)
    } while(--lc);
    IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S3(I, 2*4,XC,step_circ)
    IFFT_BUTTERFLY_S3_LAST(X, step_fwd,XC,step_circ)

    stride  >>= 2;

    scale += 2;
  }

  if (stride == 8)
  {
    int lc = (N>>3)-1;

    //-----------------------------------------------------------------
    // Set up pointers to access "N/4", "N/2", "3N/4" complex point or  
    // "N/2", "N", "3N/2" half word                                    
    //-----------------------------------------------------------------
    p_x0 = (ae_f24x2 *)x;
    p_x1 = p_x0 + (stride>>2);
    p_x2 = p_x1 + (stride>>2);
    p_x3 = p_x2 + (stride>>2);

    vA0 = AE_L32X2F24_I(p_x0, 0);
    vA1 = AE_L32X2F24_I(p_x1, 0);
    vA2 = AE_L32X2F24_I(p_x2, 0);
    vA3 = AE_L32X2F24_I(p_x3, 0);

    vB0 = AE_ADD32S(vA0, vA2);
    vB2 = AE_SUB32S(vA0, vA2);
    vB1 = AE_ADD32S(vA1, vA3);
    vB3 = AE_SUB32S(vA1, vA3);

    vT2_1 = AE_L32X2F24_I(p_twd, 8);          
    vT3_1 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_1, p_twd,step_circ); 
    vT2_2 = AE_L32X2F24_I(p_twd, 8);          
    vT3_2 = AE_L32X2F24_I(p_twd, 16);         
    AE_L32X2F24_XC(vT1_2, p_twd,step_circ); 
    do
    {
      vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
      IFFT_BUTTERFLY_S3_T3F(I, 2*4)
      vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
      IFFT_BUTTERFLY_S3_T3F(I, 14*4)
    } while(--lc);
    vT1 = vT1_1; vT2 = vT2_1; vT3 = vT3_1;
    IFFT_BUTTERFLY_S3_T3F(I, 2*4)
    vT1 = vT1_2; vT2 = vT2_2; vT3 = vT3_2;
    IFFT_BUTTERFLY_S3_T3F_LAST(I, 14*4)

    stride  >>= 2;

    scale += 2;
  }

  switch (stride)
  {
    case 2:
      ifft_cplx_24x24_lbut2(y, x, N);
      break;
    case 4:
      ifft_cplx_24x24_lbut4(y, x, N);
      break;
  }

  return scale;
}

/*-------------------------------------------------------------------------
  These functions make inverse FFT on complex data.
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
  x[2*N]     complex input spectrum. Real and imaginary data are interleaved 
             and real data goes first
  N          FFT size
  scalingOpt scaling option
  Output:
  y[2*N]     complex output signal. Real and imaginary data are interleaved 
             and real data goes first

  Returned value: total number of right shifts occurred during scaling 
                  procedure

  Restrictions:
  x,y should not overlap
  x,y - aligned on a 8-bytes boundary
  N   - 2^m: 16...4096

-------------------------------------------------------------------------*/
int ifft_cplx24x24( 
              f24* y,
              int32_t* x,
              fft_handle_t h,
              int scalingOpt)
{
  ae_int32x2 *  restrict  px_32;
  ae_f24x2 *    restrict  px_24;

  ae_int32x2    vA0, vA1, vB0, vB1;
  ae_f24x2      vF0;
  int scale=0;
  int exp = 0;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(scalingOpt>=0 && scalingOpt<=3);

  if (scalingOpt==1)
  {
    int k, a, b,N;
    ae_int64 vL;

    N=((const tFftDescr*)h)->N;
    px_24 = (ae_f24x2 *)x;
    exp = 31;
    
    vB0 = AE_MOVI(0);
    vB1 = AE_MOVI(0);

    for (k=0; k<N; ++k)
    {
      AE_L32X2F24_IP(vF0, px_24, 8);
      vA1 = (vF0);
      vB0 = AE_MIN32(vB0, vA1);
      vB1 = AE_MAX32(vB1, vA1);
    }
    vA0 = AE_SEL32_LH(vB0, vB0);
    vA1 = AE_SEL32_LH(vB1, vB1);
    vB0 = AE_MIN32(vA0, vB0);
    vB1 = AE_MAX32(vA1, vB1);
    vL = AE_MOVINT64_FROMINT32X2(vB0);
    a = AE_NSA64(vL);
    vL = AE_MOVINT64_FROMINT32X2(vB1);
    b = AE_NSA64(vL);
    exp = XT_MIN(exp, a);
    exp = XT_MIN(exp, b);
    exp = 11-exp;
  }
  if (scalingOpt==2)
  {
    int k, a, b,N;
    ae_int64 vL;

    N=((const tFftDescr*)h)->N;
    px_32 = (ae_int32x2 *)x;
    exp = 31;
    
    vB0 = AE_MOVI(0);
    vB1 = AE_MOVI(0);

    for (k=0; k<N; ++k)
    {
      AE_L32X2_IP(vA1, px_32, 8);
      vB0 = AE_MIN32(vB0, vA1);
      vB1 = AE_MAX32(vB1, vA1);
    }
    vA0 = AE_SEL32_LH(vB0, vB0);
    vA1 = AE_SEL32_LH(vB1, vB1);
    vB0 = AE_MIN32(vA0, vB0);
    vB1 = AE_MAX32(vA1, vB1);
    vL = AE_MOVINT64_FROMINT32X2(vB0);
    a = AE_NSA64(vL);
    vL = AE_MOVINT64_FROMINT32X2(vB1);
    b = AE_NSA64(vL);
    exp = XT_MIN(exp, a);
    exp = XT_MIN(exp, b);
    exp = 3-exp;

    if (exp<0)
    {
      scale = exp;

      px_32 = (ae_int32x2 *)x;

      for (k=0; k<N/2; k++)
      {
        AE_L32X2_IP(vA0, px_32, 8);
        AE_L32X2_IP(vA1, px_32, 8);
        vB0 = AE_SRAA32(vA0, exp);
        vB1 = AE_SRAA32(vA1, exp);
        AE_S32X2_I(vB0, px_32, -16);
        AE_S32X2_I(vB1, px_32, -8);
      }
      exp = 0;
    }
    scalingOpt=1; /* further use 24bit downscaling */
  }

  switch(scalingOpt)
  {
    case 1:
      scale+=ifft_cplx_24x24_core_s1(y, x, (const tFftDescr*)h, exp);
      break;
    case 3:
      scale+=ifft_cplx_24x24_core_s3(y, x, (const tFftDescr*)h);
      break;
    default:
      scale+=ifft_cplx_24x24_core_s0(y, x, (const tFftDescr*)h);
      break;
  }
  return scale;
}
