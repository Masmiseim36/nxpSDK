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
    Complex-valued FFT stages with butterflies radix-2, radix-3, radix-5
    with static data scaling: 16-bit data, 16-bit twiddle factors
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/
#include "NatureDSP_types.h"
#include "common.h"
#include "fft_16x16_stages.h"

/*
DFT3 algorithm:
x - input complex vector
y - output complex vector
y = fft(x)
y = [ x(1) + x(2)  + x(3);
x(1) + (x(2) + x(3))*cos(2*pi/3) - 1j*(x(2) - x(3))*sin(2*pi/3);
x(1) + (x(2) + x(3))*cos(2*pi/3) + 1j*(x(2) - x(3))*sin(2*pi/3) ]

*/
#define DFT3X2(x0, x1, x2)\
{\
    ae_int16x4 s0, s1, d0, c;\
    ae_int32x2 c32;          \
    c32 = AE_MOVDA32(0x6EDA9126);      \
    c = AE_MOVINT16X4_FROMINT32X2(c32);\
    s0 = AE_ADD16S(x1, x2);            \
    d0 = AE_SUB16S(x1, x2);            \
    s1 = AE_ADD16S(x0, s0);            \
    s0 = AE_SRAI16(s0, 1);             \
    s0 = AE_SUB16S(x0, s0);            \
    d0 = AE_MULFP16X4RAS(d0, c);       \
    d0 = AE_SEL16_2301(d0, d0);        \
    x0 = s1;                           \
    x1 = AE_SUB16S(s0, d0);            \
    x2 = AE_ADD16S(s0, d0);            \
}

/*
DFT5 algorithm:
x - input complex vector
y - output complex vector
y = fft(x)
w1 =  exp(-1j*2*pi/5);
w2 =  exp(-1j*2*pi*2/5);

y = zeros(5,1);
s1 = (x1+x4);
s2 = (x2+x3);
d1 = (x1-x4);
d2 = (x2-x3);

y(1) = x0 + s1 + s2;
y(2) = x0 + (s1*real(w1) + s2*real(w2)) + 1j*(d1*imag(w1) + d2*imag(w2));
y(5) = x0 + (s1*real(w1) + s2*real(w2)) - 1j*(d1*imag(w1) + d2*imag(w2));
y(3) = x0 + (s1*real(w2) + s2*real(w1)) + 1j*(d1*imag(w2) - d2*imag(w1));
y(4) = x0 + (s1*real(w2) + s2*real(w1)) - 1j*(d1*imag(w2) - d2*imag(w1));

*/
ALIGN(8) static const int16_t __dft5_tw[] =
{
  (int16_t)0x278E, (int16_t)0x278E, (int16_t)0x278E, (int16_t)0x278E,
  (int16_t)0x8644, (int16_t)0x79BC, (int16_t)0x8644, (int16_t)0x79BC,
  (int16_t)0x9872, (int16_t)0x9872, (int16_t)0x9872, (int16_t)0x9872,
  (int16_t)0xB4C3, (int16_t)0x4B3D, (int16_t)0xB4C3, (int16_t)0x4B3D
};

/* twiddles should be loaded from the table above */
#define DFT5X2(x0, x1, x2, x3, x4, w1, w2, w3, w4)\
{ \
  ae_int16x4 s1, s2, d1, d2;             \
  ae_int16x4 t0, t1, t2, t3;             \
  ae_int16x4 y0, y1, y2, y3;             \
  s1 = AE_ADD16S(x1, x4);                \
  s2 = AE_ADD16S(x2, x3);                \
  d1 = AE_SUB16S(x1, x4);                \
  d2 = AE_SUB16S(x2, x3);                \
  \
  t0 = AE_MULFP16X4RAS(s1, w1);         \
  t1 = AE_MULFP16X4RAS(s2, w3);         \
  t2 = AE_MULFP16X4RAS(s1, w3);         \
  t3 = AE_MULFP16X4RAS(s2, w1);         \
  y0 = AE_ADD16S(x0, AE_ADD16S(t0, t1));\
  y1 = AE_ADD16S(x0, AE_ADD16S(t2, t3));\
  \
  t0 = AE_MULFP16X4RAS(d1, w2);\
  t1 = AE_MULFP16X4RAS(d2, w4);\
  t2 = AE_MULFP16X4RAS(d2, w2);\
  t3 = AE_MULFP16X4RAS(d1, w4);\
  y2 = AE_ADD16S(t0, t1);      \
  y3 = AE_SUB16S(t3, t2);      \
  y2 = AE_SEL16_2301(y2, y2);  \
  y3 = AE_SEL16_2301(y3, y3);  \
  \
  x0 = AE_ADD16S(x0, AE_ADD16S(s1, s2));\
  x1 = AE_ADD16S(y0, y2);               \
  x2 = AE_ADD16S(y1, y3);               \
  x3 = AE_SUB16S(y1, y3);               \
  x4 = AE_SUB16S(y0, y2);               \
}

/*
 *  First stage of FFT 16x16, radix-3, static scaling
 */
int fft_16x16_stage_first_scl3_DFT3(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 3; // stage radix
  const int stride = N / R;
  ae_int16x4 x0, x1, x2;
  ae_int32x2 x00, x01, x02, x10, x11, x12;
  ae_int16x4 tw0, tw1, c1;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT((stride&1)==0);

  px0 = (ae_int16x4 *)x;
  px1 = px0 + stride/2;
  px2 = px1 + stride/2;
  py0 = (ae_int16x4 *)y;
  ptwd = (const ae_int16x4 *)tw;

  c1 = 1;

  __Pragma("loop_count min=1");
  for (i = 0; i < (stride>>1); i++)
  {
    AE_L16X4_IP(x0, px0, sizeof(ae_int16x4));
    AE_L16X4_IP(x1, px1, sizeof(ae_int16x4));
    AE_L16X4_IP(x2, px2, sizeof(ae_int16x4));

    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    DFT3X2(x0, x1, x2);
    x00 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    AE_MUL16X4(x01, x11, x1, c1);
    AE_MUL16X4(x02, x12, x2, c1);

    AE_L16X4_IP(tw0, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw1, ptwd, sizeof(ae_int16x4));

    x01 = AE_MULFC32X16RAS_H(x01, tw0);
    x02 = AE_MULFC32X16RAS_L(x02, tw0);
    x11 = AE_MULFC32X16RAS_H(x11, tw1);
    x12 = AE_MULFC32X16RAS_L(x12, tw1);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x10);
    x2 = AE_SAT16X4(x11,x12);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
  }

  *v = *v * R;
  return shift;
  #undef shift
} /* fft_16x16_stage_first_scl3_DFT3() */

/*
 *  First stage of FFT 16x16, radix-5, static scaling
 */
int fft_16x16_stage_first_scl3_DFT5(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const int R = 5; // stage radix
  const int stride = N / R;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
  const ae_int16x4 * restrict px2;
  const ae_int16x4 * restrict px3;
  const ae_int16x4 * restrict px4;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const ae_int16x4 * restrict ptwd_dft;
  ae_int16x4 x0, x1, x2, x3, x4;
  ae_int32x2 x00, x01, x02, x03, x04;
  ae_int32x2 x10, x11, x12, x13, x14;
  ae_int16x4 tw012, tw034, tw112, tw134;
  ae_int16x4 w1, w2, w3, w4;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT((stride&1)==0);

  px0 = (ae_int16x4 *)x;
  px1 = px0 + stride/2;
  px2 = px1 + stride/2;
  px3 = px2 + stride/2;
  px4 = px3 + stride/2;
  py0 = (ae_int16x4 *)y;
  ptwd     = (const ae_int16x4 *)tw;
  ptwd_dft = (const ae_int16x4 *)__dft5_tw;
  AE_L16X4_IP(w1, ptwd_dft, sizeof(ae_int16x4));
  AE_L16X4_IP(w2, ptwd_dft, sizeof(ae_int16x4));

  __Pragma("loop_count min=2");
  for (i = 0; i <(stride>>1); i++)
  {
    AE_L16X4_IP(x0, px0, sizeof(ae_int16x4));
    AE_L16X4_IP(x1, px1, sizeof(ae_int16x4));
    AE_L16X4_IP(x2, px2, sizeof(ae_int16x4));
    AE_L16X4_IP(x3, px3, sizeof(ae_int16x4));
    AE_L16X4_IP(x4, px4, sizeof(ae_int16x4));
    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    x3 = AE_SRAI16R(x3, shift);
    x4 = AE_SRAI16R(x4, shift);
    AE_L16X4_IP(w3, ptwd_dft, sizeof(ae_int16x4));
    AE_L16X4_XP(w4, ptwd_dft, -(int)sizeof(ae_int16x4));
    DFT5X2(x0, x1, x2, x3, x4, w1, w2, w3, w4);
    x00 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    x01 = AE_SEXT32X2D16_32(x1);
    x11 = AE_SEXT32X2D16_10(x1);
    x02 = AE_SEXT32X2D16_32(x2);
    x12 = AE_SEXT32X2D16_10(x2);
    x03 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x04 = AE_SEXT32X2D16_32(x4);
    x14 = AE_SEXT32X2D16_10(x4);

    AE_L16X4_IP(tw012, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw034, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw112, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw134, ptwd, sizeof(ae_int16x4));
    x01 = AE_MULFC32X16RAS_H(x01, tw012);
    x02 = AE_MULFC32X16RAS_L(x02, tw012);
    x03 = AE_MULFC32X16RAS_H(x03, tw034);
    x04 = AE_MULFC32X16RAS_L(x04, tw034);
    x11 = AE_MULFC32X16RAS_H(x11, tw112);
    x12 = AE_MULFC32X16RAS_L(x12, tw112);
    x13 = AE_MULFC32X16RAS_H(x13, tw134);
    x14 = AE_MULFC32X16RAS_L(x14, tw134);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x04,x10);
    x3 = AE_SAT16X4(x11,x12);
    x4 = AE_SAT16X4(x13,x14);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x4, py0, sizeof(ae_int16x4));
  }
  *v *= R;
  return shift;
  #undef shift
} /* fft_16x16_stage_first_scl3_DFT5() */

/*
 *  Intermediate stage of FFT/IFFT 16x16, radix-2, static scaling
 */
int fft_16x16_stage_inner_scl3_DFT2(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{ 
  #define shift 1
  int i, j, _v;
  const int R = 2; // stage radix
  const int stride = N / R;
  const ae_int16x4 * restrict px0;
  const ae_int16x4 * restrict px1;
        ae_int16x4 * restrict py0;
        ae_int16x4 * restrict py1;
  const ae_int16x4 * restrict ptwd;
  ae_int32x2 x00, x01, y01, y11, tw0, tw1;
  ae_int16x4 x0, x1, y0, y1;
  ae_int16x4 tw01, c1;
  ae_valign al_x0, al_x1, al_y1;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);
  NASSERT(tw_step == 1);

  c1 = 1;
  _v = *v;
  al_y1 = AE_ZALIGN64();
  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    ptwd = (const ae_int16x4 *)tw;
    px0 = (const ae_int16x4 *)((complex_fract16 *)x + 2*j);
    px1 = (const ae_int16x4 *)((complex_fract16 *)px0 + stride);
    py0 = (      ae_int16x4 *)((complex_fract16 *)y + 2*j);
    py1 = (      ae_int16x4 *)((complex_fract16 *)py0 + _v);
    __Pragma("loop_count min=1");
    for (i = 0; i < ((stride/_v)>>1); i++)
    {
      AE_L16X4_IP(tw01, ptwd, 2*sizeof(complex_fract16));
      tw0 = AE_SEXT32X2D16_32(tw01);
      tw1 = AE_SEXT32X2D16_10(tw01);

      /* butterfly 0 */
      AE_L16X4_XP(x0, px0, _v*sizeof(complex_fract16));
      AE_L16X4_XP(x1, px1, _v*sizeof(complex_fract16));

      x0 = AE_SRAI16R(x0, shift);
      x1 = AE_SRAI16R(x1, shift);
      y0 = AE_ADD16S(x0, x1);
      y1 = AE_SUB16S(x0, x1);
      y01 = AE_MULFC32X16RAS_H(tw0, y1);
      y11 = AE_MULFC32X16RAS_L(tw0, y1);
      y1 = AE_SAT16X4(y01,y11);

      py1 = (ae_int16x4 *)((complex_fract16 *)py0 + _v);
      AE_S16X4_XP(y0, py0, 2*_v*sizeof(complex_fract16));
      AE_SA16X4_IP(y1, al_y1, py1);
      AE_SA64POS_FP(al_y1, py1);

      /* butterfly 1 */
      al_x0 = AE_LA64_PP(px0);
      al_x1 = AE_LA64_PP(px1);
      AE_LA16X4_IP(x0, al_x0, px0);
      AE_LA16X4_IP(x1, al_x1, px1);
      px0 = (const ae_int16x4 *)((complex_fract16*)px0 + _v - 2);
      px1 = (const ae_int16x4 *)((complex_fract16*)px1 + _v - 2);

      x0 = AE_SRAI16R(x0, shift);
      x1 = AE_SRAI16R(x1, shift);
      y0 = AE_ADD16S(x0, x1);
      y1 = AE_SUB16S(x0, x1);
      y01 = AE_MULFC32X16RAS_H(tw1, y1);
      y11 = AE_MULFC32X16RAS_L(tw1, y1);
      y1 = AE_SAT16X4(y01,y11);

      py1 = (ae_int16x4 *)((complex_fract16 *)py0 + _v);
      AE_S16X4_XP(y0, py0, 2*_v*sizeof(complex_fract16));
      AE_SA16X4_IP(y1, al_y1, py1);
      AE_SA64POS_FP(al_y1, py1);
    }
  }
  if (_v&1)
  {
    ptwd = (const ae_int16x4 *)tw;
    px0 = (const ae_int16x4 *)((complex_fract16 *)x + _v-1);
    px1 = (const ae_int16x4 *)((complex_fract16 *)px0 + stride);
    py0 = (ae_int16x4 *)((complex_fract16 *)y + _v-1);
    __Pragma("loop_count min=1");
    for (i = 0; i < (stride/_v); i++)
    {
      AE_L32_XP(x00, castxcc(ae_int32,px0), _v*sizeof(complex_fract16));
      AE_L32_XP(x01, castxcc(ae_int32,px1), _v*sizeof(complex_fract16));
      x0 = AE_MOVINT16X4_FROMINT32X2(x00);
      x1 = AE_MOVINT16X4_FROMINT32X2(x01);

      x0 = AE_SRAI16R(x0, shift);
      x1 = AE_SRAI16R(x1, shift);
      y0 = AE_ADD16S(x0, x1);
      y1 = AE_SUB16S(x1, x0);

      AE_L32_IP(tw1, castxcc(ae_int32,ptwd), sizeof(complex_fract16));
      tw1 = AE_SEXT32X2D16_32(AE_MOVINT16X4_FROMINT32X2(tw1));
      tw1 = AE_MUL32JS(tw1);
      y11 = AE_MULFC32X16RAS_H(tw1, y1);

      y01 = AE_MOVINT32X2_FROMINT16X4(y0);
      y11 = AE_MOVINT32X2_FROMINT16X4(AE_SAT16X4(y11,y11));
      AE_S32_L_XP(y01, castxcc(ae_int32,py0), _v*sizeof(complex_fract16));
      AE_S32_L_XP(y11, castxcc(ae_int32,py0), _v*sizeof(complex_fract16));
    }
  }
  *v *= R;
  return shift;
  #undef shift
} /* fft_16x16_stage_inner_scl3_DFT2() */

/*
 *  Intermediate stage of FFT/IFFT 16x16, radix-3, static scaling
 */
int fft_16x16_stage_inner_scl3_DFT3(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{ 
  #define shift 2
  int i, j, _v;
  const int R = 3; // stage radix
  const int stride = N / R;
  const ae_int16x4 * restrict px0;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  ae_int16x4 x0, x1, x2;
  ae_int32x2 x01, x02;
  ae_int32x2 x11, x12;
  ae_int16x4 tw12, c1;

  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  _v = *v;
  c1 = 1;
  NASSERT((_v&1)==0);
  __Pragma("loop_count min=1");
  for (j = 0; j < (_v>>1); j++)
  {
    ptwd = (const ae_int16x4 *)tw;
    px0 = (const ae_int16x4 *)x + j;
    py0 = (      ae_int16x4 *)y + j;
    __Pragma("loop_count min=1");
    for (i = 0; i < (stride/ _v); i++)
    {
      x1 = AE_L16X4_X(px0, 1*stride*sizeof(complex_fract16));
      x2 = AE_L16X4_X(px0, 2*stride*sizeof(complex_fract16));
      AE_L16X4_XP(x0, px0, _v*sizeof(complex_fract16));
      x0 = AE_SRAI16R(x0, shift);
      x1 = AE_SRAI16R(x1, shift);
      x2 = AE_SRAI16R(x2, shift);

      DFT3X2(x0, x1, x2);
      AE_MUL16X4(x01, x11, x1, c1);
      AE_MUL16X4(x02, x12, x2, c1);

      AE_L16X4_XP(tw12, ptwd, tw_step*sizeof(ae_int16x4));
      x01 = AE_MULFC32X16RAS_H(x01, tw12);
      x11 = AE_MULFC32X16RAS_H(x11, tw12);
      x02 = AE_MULFC32X16RAS_L(x02, tw12);
      x12 = AE_MULFC32X16RAS_L(x12, tw12);

      x1 = AE_SAT16X4(x01,x11);
      x2 = AE_SAT16X4(x02,x12);
      AE_S16X4_XP(x0, py0, _v * sizeof(complex_fract16));
      AE_S16X4_XP(x1, py0, _v * sizeof(complex_fract16));
      AE_S16X4_XP(x2, py0, _v * sizeof(complex_fract16));
    }
  }
  *v *= R;
  return shift;
  #undef shift
} /* fft_16x16_stage_inner_scl3_DFT3() */

/*
 *  First stage of IFFT 16x16, radix-3, static scaling
 */
int ifft_16x16_stage_first_scl3_DFT3(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const ae_int16x4 * restrict px0;
  const ae_int32   * restrict px0s;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const int R = 3; // stage radix
  const int stride = N / R;
  ae_int16x4 x0, x1, x2;
  ae_int32x2 x00, x01, x02;
  ae_int32x2 x10, x11, x12;
  ae_int32x2 x20, x21, x22;
  ae_int16x4 tw012, tw112, c1;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  ptwd = (const ae_int16x4 *)tw;
  py0 = (ae_int16x4 *)y;
  c1 = 1;
  /*
  ifft(x) = fft( [ x(1), x(end:-1:2)] )
  */
  px0s = (ae_int32 *)x + N - 2*stride;
  {
    /* First butterfly radix 3 */
    AE_L32_XP(x02, px0s, stride*sizeof(ae_int32));
    x01 = AE_L32_I(px0s, 0);
    x00 = AE_L32_I((ae_int32*)x, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x00));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x01));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x02));

    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    DFT3X2(x0, x1, x2);
    x00 = AE_SEXT32X2D16_32(x0);
    x01 = AE_SEXT32X2D16_32(x1);
    x02 = AE_SEXT32X2D16_32(x2);

    AE_L16X4_IP(tw012, ptwd, sizeof(ae_int16x4));
    x01 = AE_MULFC32X16RAS_H(x01, tw012);
    x02 = AE_MULFC32X16RAS_L(x02, tw012);
  }
  px0 = (ae_int16x4 *)((complex_fract16*)x + N-2 - 2*stride);
  __Pragma("loop_count min=1");
  for (i = 1; i < (stride>>1); i++)
  {
    AE_L16X4_XP(x2, px0, stride * sizeof(complex_fract16));
    AE_L16X4_XP(x1, px0, stride * sizeof(complex_fract16));
    AE_L16X4_XP(x0, px0, (-2*stride - 2) * sizeof(complex_fract16));

    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    DFT3X2(x0, x1, x2);
    x20 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    AE_MUL16X4(x21, x11, x1, c1);
    AE_MUL16X4(x22, x12, x2, c1);

    AE_L16X4_IP(tw112, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw012, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw112);
    x12 = AE_MULFC32X16RAS_L(x12, tw112);
    x21 = AE_MULFC32X16RAS_H(x21, tw012);
    x22 = AE_MULFC32X16RAS_L(x22, tw012);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x10);
    x2 = AE_SAT16X4(x11,x12);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));

    x00 = x20;
    x01 = x21;
    x02 = x22;
  }
  /* Last butterfly radix 3 */
  px0s = (ae_int32 *)x + 1;
  {
    AE_L32_XP(x12, px0s, stride * sizeof(complex_fract16));
    AE_L32_XP(x11, px0s, stride * sizeof(complex_fract16));
    x10 = AE_L32_I(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x10));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x11));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x12));

    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    DFT3X2(x0, x1, x2);
    x10 = AE_SEXT32X2D16_10(x0);
    x11 = AE_SEXT32X2D16_10(x1);
    x12 = AE_SEXT32X2D16_10(x2);

    AE_L16X4_IP(tw112, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw112);
    x12 = AE_MULFC32X16RAS_L(x12, tw112);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x10);
    x2 = AE_SAT16X4(x11,x12);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
  }

  *v *= R;
  return shift;
  #undef shift
} /* ifft_16x16_stage_first_scl3_DFT3() */

/*
 *  First stage of IFFT 16x16, radix-5, static scaling
 */
int ifft_16x16_stage_first_scl3_DFT5(const int16_t *tw, const int16_t *x, int16_t *y, int N, int *v, int tw_step, int *bexp)
{
  #define shift 3
  int i;
  const int R = 5; // stage radix
  const int stride = N / R;
  const ae_int16x4 * restrict px0;
  const ae_int32   * restrict px0s;
        ae_int16x4 * restrict py0;
  const ae_int16x4 * restrict ptwd;
  const ae_int16x4 * restrict ptwd_dft;
  ae_int16x4 x0, x1, x2, x3, x4;
  ae_int32x2 x00, x01, x02, x03, x04;
  ae_int32x2 x10, x11, x12, x13, x14;
  ae_int32x2 x20, x21, x22, x23, x24;
  ae_int16x4 tw012, tw034, tw112, tw134, c1;
  ae_int16x4 w1, w2, w3, w4;
  NASSERT_ALIGN8(x);
  NASSERT_ALIGN8(y);

  ptwd     = (const ae_int16x4 *)tw;
  ptwd_dft = (const ae_int16x4 *)__dft5_tw;
  py0 = (ae_int16x4 *)y;
  AE_L16X4_IP(w1, ptwd_dft, sizeof(ae_int16x4));
  AE_L16X4_IP(w2, ptwd_dft, sizeof(ae_int16x4));
  AE_L16X4_IP(w3, ptwd_dft, sizeof(ae_int16x4));
  AE_L16X4_XP(w4, ptwd_dft, -3*(int)sizeof(ae_int16x4));
  c1 = 1;
  /*
  ifft(x) = fft( [ x(1), x(end:-1:2)] )
  */
  px0s = (ae_int32 *)x + N - 4*stride;
  {
    /* First butterfly radix 4 */
    AE_L32_XP(x04, px0s, stride*sizeof(ae_int32));
    AE_L32_XP(x03, px0s, stride*sizeof(ae_int32));
    AE_L32_XP(x02, px0s, stride*sizeof(ae_int32));
    x01 = AE_L32_I(px0s, 0);
    x00 = AE_L32_I((ae_int32*)x, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x00));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x01));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x02));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x03));
    x4 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x04));
    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    x3 = AE_SRAI16R(x3, shift);
    x4 = AE_SRAI16R(x4, shift);
    DFT5X2(x0, x1, x2, x3, x4,  w1, w2, w3, w4);
    x00 = AE_SEXT32X2D16_32(x0);
    x01 = AE_SEXT32X2D16_32(x1);
    x02 = AE_SEXT32X2D16_32(x2);
    x03 = AE_SEXT32X2D16_32(x3);
    x04 = AE_SEXT32X2D16_32(x4);

    AE_L16X4_IP(tw012, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw034, ptwd, sizeof(ae_int16x4));
    x01 = AE_MULFC32X16RAS_H(x01, tw012);
    x02 = AE_MULFC32X16RAS_L(x02, tw012);
    x03 = AE_MULFC32X16RAS_H(x03, tw034);
    x04 = AE_MULFC32X16RAS_L(x04, tw034);
  }
  px0 = (ae_int16x4 *)((complex_fract16 *)x + N - 2 - 4*stride);
  __Pragma("loop_count min=2");
  for (i = 1; i < (stride>>1); i++)
  {
    x0 = AE_L16X4_X(px0, 4*stride*sizeof(complex_fract16));
    x1 = AE_L16X4_X(px0, 3*stride*sizeof(complex_fract16));
    x2 = AE_L16X4_X(px0, 2*stride*sizeof(complex_fract16));
    x3 = AE_L16X4_X(px0, 1*stride*sizeof(complex_fract16));
    AE_L16X4_XP(x4, px0, -2*(int)sizeof(complex_fract16));

    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    x3 = AE_SRAI16R(x3, shift);
    x4 = AE_SRAI16R(x4, shift);

    DFT5X2(x0, x1, x2, x3, x4,  w1, w2, w3, w4);
    x20 = AE_SEXT32X2D16_32(x0);
    x10 = AE_SEXT32X2D16_10(x0);
    x21 = AE_SEXT32X2D16_32(x1);
    x11 = AE_SEXT32X2D16_10(x1);
    x22 = AE_SEXT32X2D16_32(x2);
    x12 = AE_SEXT32X2D16_10(x2);
    x23 = AE_SEXT32X2D16_32(x3);
    x13 = AE_SEXT32X2D16_10(x3);
    x24 = AE_SEXT32X2D16_32(x4);
    x14 = AE_SEXT32X2D16_10(x4);

    AE_L16X4_IP(tw112, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw134, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw012, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw034, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw112);
    x12 = AE_MULFC32X16RAS_L(x12, tw112);
    x13 = AE_MULFC32X16RAS_H(x13, tw134);
    x14 = AE_MULFC32X16RAS_L(x14, tw134);
    x21 = AE_MULFC32X16RAS_H(x21, tw012);
    x22 = AE_MULFC32X16RAS_L(x22, tw012);
    x23 = AE_MULFC32X16RAS_H(x23, tw034);
    x24 = AE_MULFC32X16RAS_L(x24, tw034);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x04,x10);
    x3 = AE_SAT16X4(x11,x12);
    x4 = AE_SAT16X4(x13,x14);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x4, py0, sizeof(ae_int16x4));

    x00 = x20;
    x01 = x21;
    x02 = x22;
    x03 = x23;
    x04 = x24;
  }
  px0s = (ae_int32 *)x + 1;
  {
    AE_L32_XP(x14, px0s, stride*sizeof(ae_int32));
    AE_L32_XP(x13, px0s, stride*sizeof(ae_int32));
    AE_L32_XP(x12, px0s, stride*sizeof(ae_int32));
    AE_L32_XP(x11, px0s, stride*sizeof(ae_int32));
    x10 = AE_L32_I(px0s, 0);
    x0 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x10));
    x1 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x11));
    x2 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x12));
    x3 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x13));
    x4 = AE_SHORTSWAP(AE_MOVINT16X4_FROMINT32X2(x14));
    x0 = AE_SRAI16R(x0, shift);
    x1 = AE_SRAI16R(x1, shift);
    x2 = AE_SRAI16R(x2, shift);
    x3 = AE_SRAI16R(x3, shift);
    x4 = AE_SRAI16R(x4, shift);
    DFT5X2(x0, x1, x2, x3, x4,  w1, w2, w3, w4);
    x10 = AE_SEXT32X2D16_10(x0);
    x11 = AE_SEXT32X2D16_10(x1);
    x12 = AE_SEXT32X2D16_10(x2);
    x13 = AE_SEXT32X2D16_10(x3);
    x14 = AE_SEXT32X2D16_10(x4);

    AE_L16X4_IP(tw112, ptwd, sizeof(ae_int16x4));
    AE_L16X4_IP(tw134, ptwd, sizeof(ae_int16x4));
    x11 = AE_MULFC32X16RAS_H(x11, tw112);
    x12 = AE_MULFC32X16RAS_L(x12, tw112);
    x13 = AE_MULFC32X16RAS_H(x13, tw134);
    x14 = AE_MULFC32X16RAS_L(x14, tw134);

    x0 = AE_SAT16X4(x00,x01);
    x1 = AE_SAT16X4(x02,x03);
    x2 = AE_SAT16X4(x04,x10);
    x3 = AE_SAT16X4(x11,x12);
    x4 = AE_SAT16X4(x13,x14);
    AE_S16X4_IP(x0, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x1, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x2, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x3, py0, sizeof(ae_int16x4));
    AE_S16X4_IP(x4, py0, sizeof(ae_int16x4));
  }
  *v *= R;
  return shift;
  #undef shift
} /* ifft_16x16_stage_first_scl3_DFT5() */
