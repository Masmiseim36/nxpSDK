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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
  NatureDSP Signal Processing Library. FFT part
    Complex-valued FFT stages with butterflies radix-4, radix-8
    with dynamic data scaling: 32-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "fft_32x16_stages.h"
#include "fft_x16_common.h"

/* Table of 32x16 FFT stages with dynamic scaling used in processing */
const fft_cplx32x16_stage_t fft32x16_stages_scl2_tbl[NUM_FFT_STAGE_TYPES] =
{
    fft_32x16_stage_first_scl2_DFT2,
    fft_32x16_stage_first_scl2_DFT3,
    fft_32x16_stage_first_scl2_DFT4,
    fft_32x16_stage_first_scl2_DFT5,
    ifft_32x16_stage_first_scl2_DFT2,
    ifft_32x16_stage_first_scl2_DFT3,
    ifft_32x16_stage_first_scl2_DFT4,
    ifft_32x16_stage_first_scl2_DFT5,
    fft_32x16_stage_inner_scl2_DFT2,
    fft_32x16_stage_inner_scl2_DFT3,
    fft_32x16_stage_inner_scl2_DFT4,
    fft_32x16_stage_inner_scl2_DFT4x2,
    NULL,
    fft_32x16_stage_inner_scl2_DFT5,
    fft_32x16_stage_penultimate_scl2_DFT4x4,
    fft_32x16_stage_last_scl2_DFT2,
    fft_32x16_stage_last_scl2_DFT3,
    fft_32x16_stage_last_scl2_DFT4,
    fft_32x16_stage_last_scl2_DFT5,
    fft_32x16_stage_last_scl2_DFT8
};


#define DFT4X1(x0, x1, x2, x3)\
{   \
\
    ae_int32x2 s0, s1, d0, d1;       \
    AE_ADDANDSUB32S(s0, d0, x0, x2); \
    AE_ADDANDSUB32S(s1, d1, x1, x3); \
    d1 = AE_MUL32JS(d1);             \
    AE_ADDANDSUB32S(x0, x2, s0, s1); \
    AE_ADDANDSUB32S(x3, x1, d0, d1); \
}
/* radix-4 butterfly with normalization */
#define DFT4X1RNG(x0, x1, x2, x3)\
{   \
    ae_int32x2 s0, s1, d0, d1;         \
    AE_ADDANDSUBRNG32(s0, d0, x0, x2); \
    AE_ADDANDSUBRNG32(s1, d1, x1, x3); \
    d1 = AE_MUL32JS(d1);               \
    AE_ADDANDSUB32S(x0, x2, s0, s1);   \
    AE_ADDANDSUB32S(x3, x1, d0, d1);   \
}

/*
 *  32x16 FFT first stage Radix 4, scalingOption=2
 */
int fft_32x16_stage_first_scl2_DFT4(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
  int i;
  int shift, shiftl, shiftr;
  const ae_int32x2 * restrict px0;
  const ae_int32x2 * restrict px1;
  const ae_int32x2 * restrict px2;
  const ae_int32x2 * restrict px3;
        ae_int32x2 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  ae_int32x2 scl;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  const int min_shift = 3;
  shift = min_shift - *bexp;
  shiftl = XT_MAX(0, -shift);
  shiftr = XT_MAX(0,  shift);

  NASSERT(shift>-32 && shift<4);
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);
  NASSERT(0==(N&7)); /* N must be a multiple of 8 */

  scl = 1 << shiftl;
  WUR_AE_SAR(shiftr);
  px0 = (ae_int32x2 *)x;
  px1 = px0 + stride;
  px2 = px1 + stride;
  px3 = px2 + stride;
  py0 = (ae_int32x2 *)y;
  ptwd = (const ae_int16x4 *)tw;
  __Pragma("loop_count min=2");
  for (i = 0; i < (stride>>1); i++)
  {
    ae_int32x2 x0, x1, x2, x3;
    ae_int16x4 tw0102, tw0311, tw1213;

    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    AE_L32X2_XP(x0, px0, sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px1, sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px2, sizeof(ae_int32x2));
    AE_L32X2_XP(x3, px3, sizeof(ae_int32x2));

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_H(x1, tw0102);
    x2 = AE_MULFC32X16RAS_L(x2, tw0102);
    x3 = AE_MULFC32X16RAS_H(x3, tw0311);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));

    AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
    AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
    AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
    AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_L(x1, tw0311);
    x2 = AE_MULFC32X16RAS_H(x2, tw1213);
    x3 = AE_MULFC32X16RAS_L(x3, tw1213);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));
  }

  AE_CALCRNG3();
  *bexp = 3 - RUR_AE_SAR();
  *v *= R;
  return shift;

} /* fft_32x16_stage_first_scl2_DFT4() */

/*
 *  32x16 FFT/IFFT last stage Radix 4, scalingOption=2
 */
int fft_32x16_stage_last_scl2_DFT4(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
  const ae_int32x2 * restrict px0;
  const ae_int32x2 * restrict px1;
  const ae_int32x2 * restrict px2;
  const ae_int32x2 * restrict px3;
        ae_int32x2 * restrict py0;
        ae_int32x2 * restrict py1;
        ae_int32x2 * restrict py2;
        ae_int32x2 * restrict py3;
  const int min_shift = 3;
  const int stride = (N >> 2);
  int shift;
  int j;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(*bexp >= 0);

  shift = (min_shift-*bexp);
  WUR_AE_SAR(shift);
  px0 = (const ae_int32x2 *)x;
  px1 = px0 + stride;
  px2 = px1 + stride;
  px3 = px2 + stride;
  py0 = (ae_int32x2 *)y;
  py1 = py0 + stride;
  py2 = py1 + stride;
  py3 = py2 + stride;

  NASSERT((stride&1)==0);
  __Pragma("loop_count min=2"); 
  for (j = 0; j < (stride>>1); j++)
  {
    ae_int32x2 x0, x1, x2, x3;
    ae_int32x2 x4, x5, x6, x7;

    AE_L32X2_IP(x0, px0, sizeof(ae_int32x2));
    AE_L32X2_IP(x1, px1, sizeof(ae_int32x2));
    AE_L32X2_IP(x2, px2, sizeof(ae_int32x2));
    AE_L32X2_IP(x3, px3, sizeof(ae_int32x2));
    AE_L32X2_IP(x4, px0, sizeof(ae_int32x2));
    AE_L32X2_IP(x5, px1, sizeof(ae_int32x2));
    AE_L32X2_IP(x6, px2, sizeof(ae_int32x2));
    AE_L32X2_IP(x7, px3, sizeof(ae_int32x2));

    DFT4X1RNG(x0, x1, x2, x3);
    DFT4X1RNG(x4, x5, x6, x7);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py1, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py2, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py3, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x4, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x5, py1, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x6, py2, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x7, py3, sizeof(ae_int32x2));
  }

  return shift;
} /* fft_32x16_stage_last_scl2_DFT4() */

/* Table of twiddle factors for radix-8 butterfly */
ALIGN(8) static const int16_t __fft8_tw1[] =
{
  (int16_t)0x4000, (int16_t)0x4000,/* first 2 numbers are used for scaling */
  (int16_t)0x2D41, (int16_t)0xD2BF,
  (int16_t)0x0000, (int16_t)0xC000,
  (int16_t)0xD2BF, (int16_t)0xD2BF,
};

/*
 *  32x16 FFT/IFFT last stage Radix 8, scalingOption=2
 */
int fft_32x16_stage_last_scl2_DFT8(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
    int shift;
    int j;
    const ae_int32x2 * restrict px0;
    const ae_int32x2 * restrict px1;
          ae_int32x2 * restrict py0;
          ae_int32x2 * restrict py1;
    const ae_int16x4 * restrict ptwd;
    ae_int16x4 tw1, tw2;

    const int stride = (N >> 3);
    const int min_shift = 3;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(*bexp >= 0);
    NASSERT(v[0] == stride);

    shift = XT_MAX(0, min_shift - *bexp);
    WUR_AE_SAR(shift);
    px0 = (ae_int32x2 *)x;
    px1 = px0 + 4*stride;
    py0 = (ae_int32x2 *)y;
    py1 = py0 + 4*stride;
    ptwd = (const ae_int16x4 *)__fft8_tw1;
    tw1 = AE_L16X4_I(ptwd, 0*sizeof(ae_int16x4));
    tw2 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));

    __Pragma("loop_count min=3");
    for (j = 0; j < stride; j++)
    {
        ae_int32x2 x0, x1, x2, x3;
        ae_int32x2 x4, x5, x6, x7;

        AE_L32X2_XP(x0, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x3, px0, (1-3*stride)*sizeof(ae_int32x2));
        AE_L32X2_XP(x4, px1, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x5, px1, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x6, px1, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x7, px1, (1-3*stride)*sizeof(ae_int32x2));

        DFT4X1RNG(x0, x2, x4, x6);
        DFT4X1RNG(x1, x3, x5, x7);
        x3 = AE_MULFC32X16RAS_L(x3, tw1);
        x5 = AE_MULFC32X16RAS_H(x5, tw2);
        x7 = AE_MULFC32X16RAS_L(x7, tw2);

        {
            ae_int32x2 s0, s1, s2, s3;
            ae_int32x2 d0, d1, d2, d3;

            x0 = AE_SRAI32(x0, 1);
            x1 = AE_SRAI32(x1, 1);
            x2 = AE_MULFP32X16X2S_H(x2, tw1);
            x4 = AE_MULFP32X16X2S_H(x4, tw1);
            x6 = AE_MULFP32X16X2S_H(x6, tw1);

            AE_ADDANDSUB32S(s0, d0, x0, x1);
            AE_ADDANDSUB32S(s1, d1, x2, x3);
            AE_ADDANDSUB32S(s2, d2, x4, x5);
            AE_ADDANDSUB32S(s3, d3, x6, x7);

            x0 = s0;        x4 = d0;
            x1 = s1;        x5 = d1;
            x2 = s2;        x6 = d2;
            x3 = s3;        x7 = d3;
        }

        AE_S32X2_XP(x0, py0, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x1, py0, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x2, py0, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x3, py0, (1-3*stride)*sizeof(ae_int32x2));
        AE_S32X2_XP(x4, py1, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x5, py1, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x6, py1, stride*sizeof(ae_int32x2));
        AE_S32X2_XP(x7, py1, (1-3*stride)*sizeof(ae_int32x2));
    }

    return shift+1;
} /* fft_32x16_stage_last_scl2_DFT8() */

/*
 *  32x16 FFT/IFFT intermediate stage Radix 4, scalingOption=2
 */
int fft_32x16_stage_inner_scl2_DFT4(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{ 
  const ae_int32x2 * restrict px0;
        ae_int32x2 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  int shift;
  const int min_shift = 3;
  const int R = 4; /* stage radix */
  const int stride = (N >> 2);

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step==1);
  shift = (min_shift-*bexp);
  WUR_AE_SAR(shift);
  {
    int i, j, _v;
    _v = *v;
    for (j=0; j<_v; j++)
    {
      ae_int32x2 x0, x1, x2, x3;
      ae_int16x4 tw0102, tw0311, tw1213;
      ptwd = (const ae_int16x4 *)tw;
      px0 = (ae_int32x2 *)x + j;
      py0 = (ae_int32x2 *)y + j;

      __Pragma("loop_count min=1");
      for (i = 0; i < ((stride/_v)>>1); i++)
      {
        AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

        AE_L32X2_XP(x0, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x3, px0, (_v-3*stride)*sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_H(x1, tw0102);
        x2 = AE_MULFC32X16RAS_L(x2, tw0102);
        x3 = AE_MULFC32X16RAS_H(x3, tw0311);
        AE_S32X2RNG_XP(x0, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py0, _v * sizeof(ae_int32x2));

        AE_L32X2_XP(x0, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
        AE_L32X2_XP(x3, px0, (_v-3*stride)*sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_L(x1, tw0311);
        x2 = AE_MULFC32X16RAS_H(x2, tw1213);
        x3 = AE_MULFC32X16RAS_L(x3, tw1213);
        AE_S32X2RNG_XP(x0, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py0, _v * sizeof(ae_int32x2));
      }
    }
    AE_CALCRNG3();
    *bexp = 3 - RUR_AE_SAR();
    *v *= R;
    return shift;
  }
} /* fft_32x16_stage_inner_scl2_DFT4() */

/*
 *  32x16 FFT/IFFT intermediate stage Radix 4 unrolled 2 times, scalingOption=2
 */
int fft_32x16_stage_inner_scl2_DFT4x2(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
  const ae_int32x2 * restrict px0; 
  const ae_int32x2 * restrict px1; 
        ae_int32x2 * restrict py0;
        ae_int32x2 * restrict py1;
  const ae_int16x4 * restrict ptwd;
  int shift;
  const int min_shift = 3;
  const int R = 4; /* stage radix */
  const int stride = (N >> 2);

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  shift = (min_shift-*bexp);
  WUR_AE_SAR(shift);
  {
    int i, j, _v, log_v;
    _v = *v;
    NASSERT(_v>=2 && (_v&(_v-1))==0);
    log_v = 30 - NSA(_v);
    for (j = 0; j < (_v>>1); j++)
    {
      ae_int32x2 x0, x1, x2, x3;
      ae_int16x4 tw0102, tw0311, tw1213;
      ptwd = (const ae_int16x4 *)tw;
      px0 = (ae_int32x2 *)x + j*2;
      py0 = (ae_int32x2 *)y + j*2;
      px1 = (ae_int32x2 *)x + j*2 + 1;
      py1 = (ae_int32x2 *)y + j*2 + 1;

      __Pragma("loop_count min=1");
      for (i = 0; i < ((stride>>log_v)>>1); i++)
      {
        AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));


        x1 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 1);
        x2 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 2);
        x3 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 3);
        AE_L32X2_XP(x0, px0, _v * sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_H(x1, tw0102);
        x2 = AE_MULFC32X16RAS_L(x2, tw0102);
        x3 = AE_MULFC32X16RAS_H(x3, tw0311);
        AE_S32X2RNG_XP(x0, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py0, _v * sizeof(ae_int32x2));

        x1 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 1);
        x2 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 2);
        x3 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 3);
        AE_L32X2_XP(x0, px1, _v * sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_H(x1, tw0102);
        x2 = AE_MULFC32X16RAS_L(x2, tw0102);
        x3 = AE_MULFC32X16RAS_H(x3, tw0311);
        AE_S32X2RNG_XP(x0, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py1, _v * sizeof(ae_int32x2));


        x1 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 1);
        x2 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 2);
        x3 = AE_L32X2_X(px0, sizeof(ae_int32x2) * stride * 3);
        AE_L32X2_XP(x0, px0, _v * sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_L(x1, tw0311);
        x2 = AE_MULFC32X16RAS_H(x2, tw1213);
        x3 = AE_MULFC32X16RAS_L(x3, tw1213);
        AE_S32X2RNG_XP(x0, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py0, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py0, _v * sizeof(ae_int32x2));

        x1 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 1);
        x2 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 2);
        x3 = AE_L32X2_X(px1, sizeof(ae_int32x2) * stride * 3);
        AE_L32X2_XP(x0, px1, _v * sizeof(ae_int32x2));
        DFT4X1RNG(x0, x1, x2, x3);
        x1 = AE_MULFC32X16RAS_L(x1, tw0311);
        x2 = AE_MULFC32X16RAS_H(x2, tw1213);
        x3 = AE_MULFC32X16RAS_L(x3, tw1213);
        AE_S32X2RNG_XP(x0, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x1, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x2, py1, _v * sizeof(ae_int32x2));
        AE_S32X2RNG_XP(x3, py1, _v * sizeof(ae_int32x2));
      }
    }

    AE_CALCRNG3();
    *bexp = 3 - RUR_AE_SAR();
    *v *= R;
    return shift;
  }
} /* fft_32x16_stage_inner_scl2_DFT4x2() */

/*
 *  32x16 FFT/IFFT penultimate stage Radix 4 unrolled 4 times, scalingOption=2
 *  Restriction: last stage must be radix 4
 */
int fft_32x16_stage_penultimate_scl2_DFT4x4(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
  int shift;
  const ae_int32x2 * restrict px0;
  const ae_int32x2 * restrict px1;
  const ae_int32x2 * restrict px2;
  const ae_int32x2 * restrict px3;
        ae_int32x2 * restrict py0;
        ae_int32x2 * restrict py1;
        ae_int32x2 * restrict py2;
        ae_int32x2 * restrict py3;
  const ae_int16x4 * restrict ptwd;
  int j, _v;
  ae_int32x2 x0, x1, x2, x3;
  ae_int16x4 tw0311, tw1213;
  ae_int16x4 tw2122, tw2331;
  ae_int16x4 tw3233;
  const int R = 4; // stage radix
  const int stride = (N >> 2);

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step==1);

  shift = 3 - *bexp;
  WUR_AE_SAR( shift );
  _v = *v;
  NASSERT(stride/_v == 4);

  px0 = (ae_int32x2 *)x;
  px1 = px0 + stride;
  px2 = px1 + stride;
  px3 = px2 + stride;
  py0 = (ae_int32x2 *)y;
  py1 = py0 + _v;
  py2 = py1 + _v;
  py3 = py2 + _v;

  ptwd = (const ae_int16x4 *)tw;
  tw0311 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));
  tw1213 = AE_L16X4_I(ptwd, 2*sizeof(ae_int16x4));
  tw2122 = AE_L16X4_I(ptwd, 3*sizeof(ae_int16x4));
  tw2331 = AE_L16X4_I(ptwd, 4*sizeof(ae_int16x4));
  tw3233 = AE_L16X4_I(ptwd, 5*sizeof(ae_int16x4));

  __Pragma("loop_count min=4, factor=4");
  for (j = 0; j < _v; j++)
  {
    AE_L32X2_XP(x0, px0, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px1, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px2, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x3, px3, _v*sizeof(ae_int32x2));
    DFT4X1RNG(x0, x1, x2, x3);
    AE_S32X2RNG_XP(x0, py0, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x1, py1, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x2, py2, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x3, py3, 4*_v*sizeof(ae_int32x2));

    AE_L32X2_XP(x0, px0, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px1, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px2, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x3, px3, _v*sizeof(ae_int32x2));
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_L(x1, tw0311);
    x2 = AE_MULFC32X16RAS_H(x2, tw1213);
    x3 = AE_MULFC32X16RAS_L(x3, tw1213);
    AE_S32X2RNG_XP(x0, py0, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x1, py1, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x2, py2, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x3, py3, 4*_v*sizeof(ae_int32x2));

    AE_L32X2_XP(x0, px0, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px1, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px2, _v*sizeof(ae_int32x2));
    AE_L32X2_XP(x3, px3, _v*sizeof(ae_int32x2));
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_H(x1, tw2122);
    x2 = AE_MULFC32X16RAS_L(x2, tw2122);
    x3 = AE_MULFC32X16RAS_H(x3, tw2331);
    AE_S32X2RNG_XP(x0, py0, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x1, py1, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x2, py2, 4*_v*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x3, py3, 4*_v*sizeof(ae_int32x2));

    AE_L32X2_XP(x0, px0, (1-3*_v)*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px1, (1-3*_v)*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px2, (1-3*_v)*sizeof(ae_int32x2));
    AE_L32X2_XP(x3, px3, (1-3*_v)*sizeof(ae_int32x2));
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_L(x1, tw2331);
    x2 = AE_MULFC32X16RAS_H(x2, tw3233);
    x3 = AE_MULFC32X16RAS_L(x3, tw3233);
    AE_S32X2RNG_XP(x0, py0, (1-3*4*_v)*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x1, py1, (1-3*4*_v)*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x2, py2, (1-3*4*_v)*sizeof(ae_int32x2));
    AE_S32X2RNG_XP(x3, py3, (1-3*4*_v)*sizeof(ae_int32x2));

  }

  AE_CALCRNG3();
  *bexp = 3 - RUR_AE_SAR();
  *v *= R;
  return shift;
} /* fft_32x16_stage_penultimate_scl2_DFT4x4() */

/*
 *  32x16 IFFT first stage Radix 4, scalingOption=2
 */
int ifft_32x16_stage_first_scl2_DFT4(const int16_t *tw, const int32_t *x, int32_t *y, int N, int *v, int tw_step, int *bexp)
{
  int i;
  int shift, shiftl, shiftr;
  const ae_int32x2 * restrict px0;
        ae_int32x2 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  const int min_shift = 3;
  ae_int16x4 tw0102, tw0311, tw1213;
  ae_int32x2 x0, x1, x2, x3;
  ae_int32x2 scl;
  shift = min_shift - *bexp;
  shiftl = XT_MAX(0, -shift);
  shiftr = XT_MAX(0,  shift);

  NASSERT(shift>-32 && shift<4);
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  scl = 1 << shiftl;
  WUR_AE_SAR(shiftr);
  py0 = (ae_int32x2 *)y;
  px0 = (ae_int32x2 *)x + N-3*stride;
  ptwd = (const ae_int16x4 *)tw;

  /*
  ifft(x) = fft( [ x(1), x(end:-1:2)] )
  */
  {
    /* First 2 butterflies radix 4 */
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    px0 = (ae_int32x2 *)x + N - 3*stride;
    AE_L32X2_XP(x3, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
    x0 = AE_L32X2_X((ae_int32x2*)x, 0);

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_H(x1, tw0102);
    x2 = AE_MULFC32X16RAS_L(x2, tw0102);
    x3 = AE_MULFC32X16RAS_H(x3, tw0311);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));

    px0 = (ae_int32x2*)x + N - 3*stride - 1;
    AE_L32X2_XP(x3, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x0, px0, (-3*stride-1)*sizeof(ae_int32x2));

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_L(x1, tw0311);
    x2 = AE_MULFC32X16RAS_H(x2, tw1213);
    x3 = AE_MULFC32X16RAS_L(x3, tw1213);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));
  }

  __Pragma("loop_count min=1");
  for (i = 1; i < (stride>>1); i++)
  {
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    AE_L32X2_XP(x3, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x0, px0, (-3*stride-1)*sizeof(ae_int32x2));

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_H(x1, tw0102);
    x2 = AE_MULFC32X16RAS_L(x2, tw0102);
    x3 = AE_MULFC32X16RAS_H(x3, tw0311);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));

    AE_L32X2_XP(x3, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x2, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x1, px0, stride*sizeof(ae_int32x2));
    AE_L32X2_XP(x0, px0, (-3*stride-1)*sizeof(ae_int32x2));

    x0 = AE_MULP32X2(x0, scl);
    x1 = AE_MULP32X2(x1, scl);
    x2 = AE_MULP32X2(x2, scl);
    x3 = AE_MULP32X2(x3, scl);
    DFT4X1RNG(x0, x1, x2, x3);
    x1 = AE_MULFC32X16RAS_L(x1, tw0311);
    x2 = AE_MULFC32X16RAS_H(x2, tw1213);
    x3 = AE_MULFC32X16RAS_L(x3, tw1213);

    AE_S32X2RNG_IP(x0, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x1, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x2, py0, sizeof(ae_int32x2));
    AE_S32X2RNG_IP(x3, py0, sizeof(ae_int32x2));
  }

  AE_CALCRNG3();
  *bexp = 3 - RUR_AE_SAR();
  *v *= R;
  return shift;
} /* ifft_32x16_stage_first_scl2_DFT4() */
