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
    with static data scaling: 16-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "fft_16x16_stages.h"

/* Table of 16x16 FFT stages with static scaling used in processing */
const fft_cplx16x16_stage_t fft16x16_stages_scl3_tbl[NUM_FFT_STAGE_TYPES] =
{
    NULL,
    fft_16x16_stage_first_scl3_DFT3,
    fft_16x16_stage_first_scl3_DFT4,
    fft_16x16_stage_first_scl3_DFT5,
    NULL,
    ifft_16x16_stage_first_scl3_DFT3,
    ifft_16x16_stage_first_scl3_DFT4,
    ifft_16x16_stage_first_scl3_DFT5,
    fft_16x16_stage_inner_scl3_DFT2,
    fft_16x16_stage_inner_scl3_DFT3,
    NULL,
    fft_16x16_stage_inner_scl3_DFT4x2,
    NULL,
    NULL,
    fft_16x16_stage_penultimate_scl3_DFT4x4,
    NULL,
    NULL,
    fft_16x16_stage_last_scl3_DFT4,
    NULL,
    fft_16x16_stage_last_scl3_DFT8
};

/* 16-bit radix-4 butterfly with normalization */
#define DFT4XI2(x0, x1, x2, x3, shift)\
{\
    ae_int16x4 s0, s1, d0, d1;                      \
    xtbool4 mask = 0x5;                             \
    x0 = AE_SRAI16R(x0, shift);                     \
    x1 = AE_SRAI16R(x1, shift);                     \
    x2 = AE_SRAI16R(x2, shift);                     \
    x3 = AE_SRAI16R(x3, shift);                     \
    s0 = AE_ADD16S(x0, x2); d0 = AE_SUB16S(x0, x2); \
    s1 = AE_ADD16S(x1, x3); d1 = AE_SUB16S(x1, x3); \
    /*  d1 = AE_MULJ16S(d1); */                     \
    AE_MOVT16X4(d1, AE_NEG16S(d1), mask);           \
    d1 = AE_SEL16_2301(d1, d1);                     \
    x0 = AE_ADD16S(s0, s1); x2 = AE_SUB16S(s0, s1); \
    x3 = AE_ADD16S(d0, d1); x1 = AE_SUB16S(d0, d1); \
}

/* 16-bit radix-4 butterfly with normalization, w/o rounding */
#define DFT4XI2_noRnd(x0, x1, x2, x3, shift)\
{\
    ae_int16x4 s0, s1, d0, d1;                      \
    xtbool4 mask = 0x5;                             \
    x0 = AE_SRAI16(x0, shift);                      \
    x1 = AE_SRAI16(x1, shift);                      \
    x2 = AE_SRAI16(x2, shift);                      \
    x3 = AE_SRAI16(x3, shift);                      \
    s0 = AE_ADD16S(x0, x2); d0 = AE_SUB16S(x0, x2); \
    s1 = AE_ADD16S(x1, x3); d1 = AE_SUB16S(x1, x3); \
    /*  d1 = AE_MULJ16S(d1); */                     \
    AE_MOVT16X4(d1, AE_NEG16S(d1), mask);           \
    d1 = AE_SEL16_2301(d1, d1);                     \
    x0 = AE_ADD16S(s0, s1); x2 = AE_SUB16S(s0, s1); \
    x3 = AE_ADD16S(d0, d1); x1 = AE_SUB16S(d0, d1); \
}

/*
 *  First stage of FFT 16x16, radix-4, static scaling
 */
int fft_16x16_stage_first_scl3_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  ae_int16x4 x0, x1, x2, x3, c1;
  ae_int32x2 x00, x01, x02, x03;
  ae_int32x2 x10, x11, x12, x13;
  ae_int16x4 tw0102, tw0311, tw1213;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);

  px0 = (const ae_int16x4 *)x;
  px1 = (const ae_int16x4 *)((complex_fract16*)px0 + stride);
  px2 = (const ae_int16x4 *)((complex_fract16*)px1 + stride);
  px3 = (const ae_int16x4 *)((complex_fract16*)px2 + stride);
  py0 = (      ae_int16x4 *)y;
  ptwd = (const ae_int16x4 *)tw;
  c1 = 1;
  __Pragma("loop_count min=1");
  for (i = 0; i < (stride>>1); i++)
  {
    AE_L16X4_IP(x0, px0, sizeof(ae_int16x4));
    AE_L16X4_IP(x1, px1, sizeof(ae_int16x4));
    AE_L16X4_IP(x2, px2, sizeof(ae_int16x4));
    AE_L16X4_IP(x3, px3, sizeof(ae_int16x4));

    DFT4XI2(x0, x1, x2, x3, shift);
    x00 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);
    AE_MUL16X4(x03, x13, x3, c1);

    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    x01 = AE_MULFC32X16RAS_H(x01, tw0102);
    x02 = AE_MULFC32X16RAS_L(x02, tw0102);
    x03 = AE_MULFC32X16RAS_H(x03, tw0311);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x10,x11);
    x3 = AE_SAT16X4(x12,x13);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
  }

  *v = v[0] * R;
  return shift;
  #undef shift
} /* fft_16x16_stage_first_scl3_DFT4() */

/*
 *  Last stage of FFT/IFFT 16x16, radix-4, static scaling
 */
int fft_16x16_stage_last_scl3_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 2
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const int stride = (N >> 2);
  int j;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  
  px0 = (const ae_int16x4 *)x;
  px1 = (const ae_int16x4 *)((complex_fract16*)px0 + stride);
  px2 = (const ae_int16x4 *)((complex_fract16*)px1 + stride);
  px3 = (const ae_int16x4 *)((complex_fract16*)px2 + stride);
  py0 = (      ae_int16x4 *)y;
  py1 = (      ae_int16x4 *)((complex_fract16*)py0 + stride);
  py2 = (      ae_int16x4 *)((complex_fract16*)py1 + stride);
  py3 = (      ae_int16x4 *)((complex_fract16*)py2 + stride);
  __Pragma("loop_count min=1");
  for (j = 0; j < (stride>>1); j++)
  {
    ae_int16x4 x0, x1, x2, x3;

    AE_L16X4_IP(x0, px0, 4*sizeof(int16_t));
    AE_L16X4_IP(x1, px1, 4*sizeof(int16_t));
    AE_L16X4_IP(x2, px2, 4*sizeof(int16_t));
    AE_L16X4_IP(x3, px3, 4*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);

    AE_S16X4_IP(x0, py0, 4*sizeof(int16_t));
    AE_S16X4_IP(x1, py1, 4*sizeof(int16_t));
    AE_S16X4_IP(x2, py2, 4*sizeof(int16_t));
    AE_S16X4_IP(x3, py3, 4*sizeof(int16_t));
  }

  return shift;
  #undef shift
} /* fft_16x16_stage_last_scl3_DFT4() */

/*
 *  Intermediate stage of FFT/IFFT 16x16, radix-4, static scaling
 */
int fft_16x16_stage_inner_scl3_DFT4x2(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 2
  const ae_int16x4 * restrict px0; 
  const ae_int16x4 * restrict px1; 
  const ae_int16x4 * restrict px2; 
  const ae_int16x4 * restrict px3; 
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 4; /* stage radix */
  const int stride = (N >> 2);
  ae_int16x4 x0, x1, x2, x3, c1;
  ae_int32x2 x01, x02, x03;
  ae_int32x2 x11, x12, x13;
  ae_int16x4 tw0102, tw0311, tw1213;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  c1 = 1;
  {
    int i, j, _v;
    _v = v[0];
    NASSERT(_v>=2 && (_v&1)==0);
    for (j = 0; j < (_v>>1); j++)
    {
      ptwd = (const ae_int16x4 *)tw;
      px0 = (ae_int16x4 *)x + j;
      px1 = px0 + stride/2;
      px2 = px1 + stride/2;
      px3 = px2 + stride/2;
      py0 = (ae_int16x4 *)y + j;

      __Pragma( "loop_count min=1" );
      for (i = 0; i < ((stride/_v)>>1); i++)
      {
        AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
        AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
        AE_L16X4_XP(tw1213, ptwd, (3*tw_step-2)*sizeof(ae_int16x4));

        AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

        DFT4XI2(x0, x1, x2, x3, shift);
        AE_MUL16X4(x01, x11, x1, c1);
        AE_MUL16X4(x02, x12, x2, c1);
        AE_MUL16X4(x03, x13, x3, c1);

        x01 = AE_MULFC32X16RAS_H(x01, tw0102);
        x11 = AE_MULFC32X16RAS_H(x11, tw0102);
        x02 = AE_MULFC32X16RAS_L(x02, tw0102);
        x12 = AE_MULFC32X16RAS_L(x12, tw0102);
        x03 = AE_MULFC32X16RAS_H(x03, tw0311);
        x13 = AE_MULFC32X16RAS_H(x13, tw0311);

        x1 = AE_SAT16X4(x01,x11);
        x2 = AE_SAT16X4(x02,x12);
        x3 = AE_SAT16X4(x03,x13);
        AE_S16X4_XP(x0, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x1, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x2, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x3, py0, _v*2*sizeof(int16_t));


        AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
        AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

        DFT4XI2(x0, x1, x2, x3, shift);
        AE_MUL16X4(x01, x11, x1, c1);
        AE_MUL16X4(x02, x12, x2, c1);
        AE_MUL16X4(x03, x13, x3, c1);

        x01 = AE_MULFC32X16RAS_L(x01, tw0311);
        x11 = AE_MULFC32X16RAS_L(x11, tw0311);
        x02 = AE_MULFC32X16RAS_H(x02, tw1213);
        x12 = AE_MULFC32X16RAS_H(x12, tw1213);
        x03 = AE_MULFC32X16RAS_L(x03, tw1213);
        x13 = AE_MULFC32X16RAS_L(x13, tw1213);

        x1 = AE_SAT16X4(x01,x11);
        x2 = AE_SAT16X4(x02,x12);
        x3 = AE_SAT16X4(x03,x13);
        AE_S16X4_XP(x0, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x1, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x2, py0, _v*2*sizeof(int16_t));
        AE_S16X4_XP(x3, py0, _v*2*sizeof(int16_t));
      }
    }
    *v *= R;
    return shift;
  }
  #undef shift
} /* fft_16x16_stage_inner_scl3_DFT4x2() */

/*
 *  Penultimate stage of FFT/IFFT 16x16, radix-4, static scaling
 *  Restriction: last stage must be radix 4
 */
int fft_16x16_stage_penultimate_scl3_DFT4x4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 2
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const ae_int16x4 * restrict ptwd;
  int j, _v;
  ae_int16x4 x0, x1, x2, x3, c1;
  ae_int32x2 x01, x02, x03;
  ae_int32x2 x11, x12, x13;
  ae_int16x4 tw0311, tw1213;
  ae_int16x4 tw2122, tw2331;
  ae_int16x4 tw3233;
  const int R = 4; // stage radix
  const int stride = (N >> 2);

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step==1);

  _v = *v;
  NASSERT(stride/_v == 4);
  NASSERT((_v>1) && ((_v&1)==0));/* _v must be a multiple of 2 */
  c1 = 1;

  px0 = (ae_int16x4 *)x;
  px1 = px0 + stride/2;
  px2 = px1 + stride/2;
  px3 = px2 + stride/2;
  py0 = (ae_int16x4 *)y;
  py1 = py0 + _v/2;
  py2 = py1 + _v/2;
  py3 = py2 + _v/2;

  /* preload twiddles */
  ptwd = (const ae_int16x4 *)tw;
  tw0311 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));
  tw1213 = AE_L16X4_I(ptwd, 2*sizeof(ae_int16x4));
  tw2122 = AE_L16X4_I(ptwd, 3*sizeof(ae_int16x4));
  tw2331 = AE_L16X4_I(ptwd, 4*sizeof(ae_int16x4));
  tw3233 = AE_L16X4_I(ptwd, 5*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);

    AE_S16X4_XP(x0, py0, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, 4*_v*2*sizeof(int16_t));

    AE_L16X4_XP(x0, px0, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, (2-_v)*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);
    AE_MUL16X4(x03, x13, x3, c1);

    x01 = AE_MULFC32X16RAS_L(x01, tw0311);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x02 = AE_MULFC32X16RAS_H(x02, tw1213);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x03 = AE_MULFC32X16RAS_L(x03, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_XP(x0, py0, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, (2-4*_v)*2*sizeof(int16_t));
  }

  px0 = (ae_int16x4 *)x + _v;
  px1 = px0 + stride/2;
  px2 = px1 + stride/2;
  px3 = px2 + stride/2;
  py0 = (ae_int16x4 *)y + 4*_v;
  py1 = py0 + _v/2;
  py2 = py1 + _v/2;
  py3 = py2 + _v/2;

  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    AE_L16X4_XP(x0, px0, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, _v*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, _v*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);
    AE_MUL16X4(x03, x13, x3, c1);
    x01 = AE_MULFC32X16RAS_H(x01, tw2122);
    x11 = AE_MULFC32X16RAS_H(x11, tw2122);
    x02 = AE_MULFC32X16RAS_L(x02, tw2122);
    x12 = AE_MULFC32X16RAS_L(x12, tw2122);
    x03 = AE_MULFC32X16RAS_H(x03, tw2331);
    x13 = AE_MULFC32X16RAS_H(x13, tw2331);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_XP(x0, py0, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, 4*_v*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, 4*_v*2*sizeof(int16_t));

    AE_L16X4_XP(x0, px0, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px1, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px2, (2-_v)*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px3, (2-_v)*2*sizeof(int16_t));

    DFT4XI2(x0, x1, x2, x3, shift);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);
    AE_MUL16X4(x03, x13, x3, c1);
    x01 = AE_MULFC32X16RAS_L(x01, tw2331);
    x11 = AE_MULFC32X16RAS_L(x11, tw2331);
    x02 = AE_MULFC32X16RAS_H(x02, tw3233);
    x12 = AE_MULFC32X16RAS_H(x12, tw3233);
    x03 = AE_MULFC32X16RAS_L(x03, tw3233);
    x13 = AE_MULFC32X16RAS_L(x13, tw3233);

    x1 = AE_SAT16X4(x01,x11);
    x2 = AE_SAT16X4(x02,x12);
    x3 = AE_SAT16X4(x03,x13);
    AE_S16X4_XP(x0, py0, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py1, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py2, (2-4*_v)*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py3, (2-4*_v)*2*sizeof(int16_t));
  }

  *v *= R;
  return shift;
  #undef shift
} /* fft_16x16_stage_penultimate_scl3_DFT4x4() */

/*
 *  First stage of IFFT 16x16, radix-4, static scaling
 */
int ifft_16x16_stage_first_scl3_DFT4(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const ae_int32   * restrict px0s;
  const ae_int16x4 * restrict px0;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  ae_int16x4 tw0102, tw0311, tw1213;
  ae_int16x4 x0, x1, x2, x3;
  ae_int32x2 x00, x01, x02, x03;
  ae_int32x2 x10, x11, x12, x13;
  const int R = 4; // stage radix
  const int stride = (N >> 2);
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);
  NASSERT(0==(N&7)); /* N must be a multiple of 8 */

  py0 = (ae_int16x4 *)y;
  ptwd = (const ae_int16x4 *)tw;

  /*
  ifft(x) = fft( [ x(1), x(end:-1:2)] )
  */
  {
    /* First butterfly radix 4 */
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));

    px0s = (ae_int32 *)x;
    x03 = AE_L32_X(px0s, (N-3*stride)*2*sizeof(int16_t));
    x02 = AE_L32_X(px0s, (N-2*stride)*2*sizeof(int16_t));
    x01 = AE_L32_X(px0s, (N-1*stride)*2*sizeof(int16_t));
    x00 = AE_L32_X(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x00));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x01));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x02));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x03));
    DFT4XI2_noRnd(x0, x1, x2, x3, shift);
    x00 = AE_CVT32X2F16_10(x0);
    x01 = AE_CVT32X2F16_10(x1);
    x02 = AE_CVT32X2F16_10(x2);
    x03 = AE_CVT32X2F16_10(x3);
    x01 = AE_MULFC32X16RAS_H(x01, tw0102);
    x02 = AE_MULFC32X16RAS_L(x02, tw0102);
    x03 = AE_MULFC32X16RAS_H(x03, tw0311);
    x0 = AE_TRUNC16X4F32(x00,x01);
    x1 = AE_TRUNC16X4F32(x02,x03);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
  }

  px0 = (ae_int16x4 *)((complex_fract16 *)x + N - 3*stride - 2);
  __Pragma("loop_count min=1");
  for (i = 1; i < (stride>>1); i++)
  {
    AE_L16X4_XP(x3, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x0, px0, (-3*stride-2)*2*sizeof(int16_t));

    DFT4XI2_noRnd(x0, x1, x2, x3, shift);
    x10 = AE_CVT32X2F16_32(x0);
    x00 = AE_CVT32X2F16_10(x0);
    x11 = AE_CVT32X2F16_32(x1);
    x01 = AE_CVT32X2F16_10(x1);
    x12 = AE_CVT32X2F16_32(x2);
    x02 = AE_CVT32X2F16_10(x2);
    x13 = AE_CVT32X2F16_32(x3);
    x03 = AE_CVT32X2F16_10(x3);

    x01 = AE_MULFC32X16RAS_L(x01, tw0311);
    x02 = AE_MULFC32X16RAS_H(x02, tw1213);
    x03 = AE_MULFC32X16RAS_L(x03, tw1213);
    AE_L16X4_IP(tw0102, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw0311, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1213, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw0102);
    x12 = AE_MULFC32X16RAS_L(x12, tw0102);
    x13 = AE_MULFC32X16RAS_H(x13, tw0311);

    x0 = AE_TRUNC16X4F32(x00,x01);
    x1 = AE_TRUNC16X4F32(x02,x03);
    x2 = AE_TRUNC16X4F32(x10,x11);
    x3 = AE_TRUNC16X4F32(x12,x13);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
  }
  {
    /* Last butterfly radix 4 */
    px0s = (ae_int32 *)x + 1;
    AE_L32_XP(x13, px0s, stride*2*sizeof(int16_t));
    AE_L32_XP(x12, px0s, stride*2*sizeof(int16_t));
    AE_L32_XP(x11, px0s, stride*2*sizeof(int16_t));
    x10 = AE_L32_X(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x10));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x11));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x12));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x13));
    DFT4XI2_noRnd(x0, x1, x2, x3, shift);
    x10 = AE_CVT32X2F16_10(x0);
    x11 = AE_CVT32X2F16_10(x1);
    x12 = AE_CVT32X2F16_10(x2);
    x13 = AE_CVT32X2F16_10(x3);
    x11 = AE_MULFC32X16RAS_L(x11, tw0311);
    x12 = AE_MULFC32X16RAS_H(x12, tw1213);
    x13 = AE_MULFC32X16RAS_L(x13, tw1213);
    x2 = AE_TRUNC16X4F32(x10,x11);
    x3 = AE_TRUNC16X4F32(x12,x13);
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
  }

  *v *= R;
  return shift;
  #undef shift
} /* ifft_16x16_stage_first_scl3_DFT4() */

/* Table of twiddle factors for radix-8 butterfly */
ALIGN(8) static const int16_t __fft8_tw1[] =
{
  (int16_t)0x4000, (int16_t)0x4000,
  (int16_t)0x2D41, (int16_t)0xD2BF,
  (int16_t)0x0000, (int16_t)0xC000,
  (int16_t)0xD2BF, (int16_t)0xD2BF,
};

/*
 *  Last stage of FFT/IFFT 16x16, radix-8, static scaling
 */
int fft_16x16_stage_last_scl3_DFT8(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 2
  int j;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
        ae_int16x4 * restrict py2;
        ae_int16x4 * restrict py3;
  const ae_int16x4 * restrict ptwd;
  ae_int16x4 tw1, tw2;
  const int R = 8; // stage radix
  const int stride = N / R;
  ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7;
  ae_int32x2 x03, x05, x07;
  ae_int32x2 x13, x15, x17;
  ae_int16x4 s0, s1, s2, s3;
  ae_int16x4 d0, d1, d2, d3;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(v[0] == stride);

  px0 = (ae_int16x4 *)x;
  px1 = px0 + stride;
  px2 = px1 + stride;
  px3 = px2 + stride;
  py0 = (ae_int16x4 *)y;
  py1 = py0 + stride;
  py2 = py1 + stride;
  py3 = py2 + stride;
  ptwd = (const ae_int16x4 *)__fft8_tw1;
  tw1 = AE_L16X4_I(ptwd, 0*sizeof(ae_int16x4));
  tw2 = AE_L16X4_I(ptwd, 1*sizeof(ae_int16x4));

  __Pragma("loop_count min=1");
  for (j = 0; j < (stride>>1); j++)
  {
    AE_L16X4_XP(x0, px0, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x1, px0, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x2, px1, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x3, px1, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x4, px2, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x5, px2, (2-stride)*2*sizeof(int16_t));
    AE_L16X4_XP(x6, px3, stride*2*sizeof(int16_t));
    AE_L16X4_XP(x7, px3, (2-stride)*2*sizeof(int16_t));


    DFT4XI2(x0, x2, x4, x6, shift);
    DFT4XI2(x1, x3, x5, x7, shift);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x05 = AE_SEXT32X2D16_32(x5);
    x15 = AE_SEXT32X2D16_10(x5);
    x07 = AE_SEXT32X2D16_32(x7);
    x17 = AE_SEXT32X2D16_10(x7);

    x03 = AE_MULFC32X16RAS_L(x03, tw1);
    x13 = AE_MULFC32X16RAS_L(x13, tw1);
    x05 = AE_MULFC32X16RAS_H(x05, tw2);
    x15 = AE_MULFC32X16RAS_H(x15, tw2);
    x07 = AE_MULFC32X16RAS_L(x07, tw2);
    x17 = AE_MULFC32X16RAS_L(x17, tw2);

    x0 = AE_SRAI16(x0,1);
    x1 = AE_SRAI16(x1,1);
    x2 = AE_SRAI16(x2,1);
    x3 = AE_SAT16X4(x03,x13);
    x4 = AE_SRAI16(x4,1);
    x5 = AE_SAT16X4(x05,x15);
    x6 = AE_SRAI16(x6,1);
    x7 = AE_SAT16X4(x07,x17);
    s0 = AE_ADD16S(x0, x1);
    d0 = AE_SUB16S(x0, x1);
    s1 = AE_ADD16S(x2, x3);
    d1 = AE_SUB16S(x2, x3);
    s2 = AE_ADD16S(x4, x5);
    d2 = AE_SUB16S(x4, x5);
    s3 = AE_ADD16S(x6, x7);
    d3 = AE_SUB16S(x6, x7);
    x0 = s0;
    x1 = s1;
    x2 = s2;
    x3 = s3;
    x4 = d0;
    x5 = d1;
    x6 = d2;
    x7 = d3;

    AE_S16X4_XP(x0, py0, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x1, py0, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x2, py1, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x3, py1, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x4, py2, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x5, py2, (2-stride)*2*sizeof(int16_t));
    AE_S16X4_XP(x6, py3, stride*2*sizeof(int16_t));
    AE_S16X4_XP(x7, py3, (2-stride)*2*sizeof(int16_t));
  }

  *v *= R;
  return (shift + 1);
  #undef shift
} /* fft_16x16_stage_last_scl3_DFT8 */
