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
    Discrete Cosine Transform, Type II
    C code optimized for HiFi4
   Integrit, 2006-2017
*/

/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct2_twd.h"

#define DFT4X1(x0, x1, x2, x3)\
{   \
    ae_int32x2 s0, s1, d0, d1;         \
    AE_ADDANDSUBRNG32(s0, d0, x0, x2); \
    AE_ADDANDSUBRNG32(s1, d1, x1, x3); \
    d1 = AE_MUL32JS(d1);               \
    AE_ADDANDSUB32S(x0, x2, s0, s1);   \
    AE_ADDANDSUB32S(x3, x1, d0, d1);   \
}

static const int16_t ALIGN(8) fft_twd_r8[8] = {
    (int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x5a82,(int16_t)0xa57e,
    (int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e
};

/*
   scaled fft with reordering
   N=16 - size of FFT
   NOTE: y is input and output, x - temporary
*/
void fft16_32x16(int32_t *y, int32_t *x, const int16_t *ptwd)
{
  ae_int32x2  * p_x0, * p_x1, * restrict p_y0;
  const ae_int16x4  * restrict p_twd;
  ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
  ae_f16x4    vT1, vT2;
  ae_int16x4  vTT;

  const int step_right   = (int)sizeof(ae_int32x2);
  const int step_left    = -step_right;
  const int step_down    = 4*step_right;
  const int step_nextc   = step_right - 3*step_down;
  const int step_hupleft = 7*step_left;
  const int step_upleft  = 15*step_left;

  p_x0  = (ae_int32x2 *)(x);
  p_x1  = (ae_int32x2 *)(x+2*15);
  p_y0  = (ae_int32x2 *)(y);
  p_twd = (const ae_int16x4 *)(ptwd)+1;

  /*** reordering ***/
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_right);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  AE_L32X2_XP(vA0, p_y0, step_right);
  AE_L32X2_XP(vA1, p_y0, step_right);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_L32X2_XP(vA2, p_y0, step_right);
  AE_L32X2_XP(vA3, p_y0, step_upleft);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_right);
  AE_S32X2_XP(vB2, p_x1, step_left);

  vB0 = AE_SEL32_HH(vA0, vA1);
  vB3 = AE_SEL32_LL(vA1, vA0);
  vB1 = AE_SEL32_HH(vA2, vA3);
  vB2 = AE_SEL32_LL(vA3, vA2);
  AE_S32X2_XP(vB0, p_x0, step_right);
  AE_S32X2_XP(vB3, p_x1, step_left);
  AE_S32X2_XP(vB1, p_x0, step_hupleft);
  AE_S32X2_XP(vB2, p_x1, 0);

  /*** first radix4 stage ***/
  WUR_AE_SAR(3);
  /* Elements 0, 4, 8, 12 */
  vA0 = AE_L32X2_I(p_x1, (0-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (4-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (8-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (12-8)*(int)sizeof(ae_int32x2));

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);
  AE_ADDANDSUB32S(vA2, vA3, vB2, vB3);

  AE_S32X2_I(vA0, p_x1, (0-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA3, p_x1, (4-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA1, p_x1, (8-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA2, p_x1, (12-8)*(int)sizeof(ae_int32x2));

  /* Elements 1, 5, 9, 13 */
  vA0 = AE_L32X2_I(p_x1, (1-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (5-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (9-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (13-8)*(int)sizeof(ae_int32x2));

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);
  AE_ADDANDSUB32S(vA2, vA3, vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);
  AE_L16X4_IP(vTT, p_twd, 8);
  vT2 = (vTT);

  vA3 = AE_MULFC32X16RAS_L(vA3, vT1);
  vA1 = AE_MULFC32X16RAS_H(vA1, vT2);
  vA2 = AE_MULFC32X16RAS_L(vA2, vT2);

  AE_S32X2_I(vA0, p_x1, (1-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA3, p_x1, (5-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA1, p_x1, (9-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA2, p_x1, (13-8)*(int)sizeof(ae_int32x2));

  /* Elements 2, 6, 10, 14 */
  vA0 = AE_L32X2_I(p_x1, (2-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (6-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (10-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (14-8)*(int)sizeof(ae_int32x2));

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);
  AE_ADDANDSUB32S(vA2, vA3, vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);
  AE_L16X4_IP(vTT, p_twd, 8);
  vT2 = (vTT);

  vA3 = AE_MULFC32X16RAS_H(vA3, vT1);
  vA1 = AE_MULFC32X16RAS_L(vA1, vT1);
  vA2 = AE_MULFC32X16RAS_H(vA2, vT2);

  AE_S32X2_I(vA0, p_x1, (2-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA3, p_x1, (6-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA1, p_x1, (10-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA2, p_x1, (14-8)*(int)sizeof(ae_int32x2));

  /* Elements 3, 7, 11, 15 */
  vA0 = AE_L32X2_I(p_x1, (3-8)*(int)sizeof(ae_int32x2));
  vA1 = AE_L32X2_I(p_x1, (7-8)*(int)sizeof(ae_int32x2));
  vA2 = AE_L32X2_I(p_x1, (11-8)*(int)sizeof(ae_int32x2));
  vA3 = AE_L32X2_I(p_x1, (15-8)*(int)sizeof(ae_int32x2));

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA1, vB0, vB1);
  AE_ADDANDSUB32S(vA2, vA3, vB2, vB3);

  AE_L16X4_IP(vTT, p_twd, 8);
  vT1 = (vTT);

  vA3 = AE_MULFC32X16RAS_L(vA3, vT2);
  vA1 = AE_MULFC32X16RAS_H(vA1, vT1);
  vA2 = AE_MULFC32X16RAS_L(vA2, vT1);

  AE_S32X2_I(vA0, p_x1, (3-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA3, p_x1, (7-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA1, p_x1, (11-8)*(int)sizeof(ae_int32x2));
  AE_S32X2_I(vA2, p_x1, (15-8)*(int)sizeof(ae_int32x2));

  /*** last radix4 stage ***/
  WUR_AE_SAR(2);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
  AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
  AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
  AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_XP(vA3, p_y0, step_nextc);

  AE_L32X2_XP(vA0, p_x0, step_right);
  AE_L32X2_XP(vA1, p_x0, step_right);
  AE_L32X2_XP(vA2, p_x0, step_right);
  AE_L32X2_XP(vA3, p_x0, step_right);

  AE_ADDANDSUBRNG32(vB0, vB2, vA0, vA2);
  AE_ADDANDSUBRNG32(vB1, vB3, vA1, vA3);
  vB3 = AE_MUL32JS(vB3);
  AE_ADDANDSUB32S(vA0, vA2, vB0, vB1);
  AE_ADDANDSUB32S(vA3, vA1, vB2, vB3);

  AE_S32X2_XP(vA0, p_y0, step_down);
  AE_S32X2_XP(vA1, p_y0, step_down);
  AE_S32X2_XP(vA2, p_y0, step_down);
  AE_S32X2_I(vA3, p_y0, 0);

}

/*
   scaled fft with reordering
   N=32 - size of FFT
   NOTE: y is input and output, x - temporary
*/
void fft32_32x16(int32_t *y, int32_t *x, const int16_t *ptwd)
{
    #define N 32
    #define stride 8
    int k;
    ae_int32x2  * restrict p_x0,
                * restrict p_x1,
                * restrict p_x2,
                * restrict p_x3,
                * restrict p_y0;
    const ae_int16x4 * restrict p_twd;
    ae_int32x2  vA0, vA1, vA2, vA3, vB0, vB1, vB2, vB3;
    {
        p_x0  = (ae_int32x2 *)(x);
        p_x1  = (ae_int32x2 *)(x+2*N-2);
        p_y0  = (ae_int32x2 *)(y);

        /* reordering */
        __Pragma("loop_count min=2");
    	for (k=0; k<(N>>2); k++) 
        { 
            AE_L32X2_IP(vA0, p_y0, sizeof(ae_int32x2));
            AE_L32X2_IP(vA1, p_y0, sizeof(ae_int32x2));
            AE_L32X2_IP(vA2, p_y0, sizeof(ae_int32x2));
            AE_L32X2_IP(vA3, p_y0, sizeof(ae_int32x2));
            vB0 = AE_SEL32_HH(vA0, vA1);
            vB3 = AE_SEL32_LL(vA1, vA0);
            vB1 = AE_SEL32_HH(vA2, vA3);
            vB2 = AE_SEL32_LL(vA3, vA2);
            AE_S32X2_IP(vB0, p_x0,       sizeof(ae_int32x2));
            AE_S32X2_XP(vB3, p_x1, -(int)sizeof(ae_int32x2));
            AE_S32X2_IP(vB1, p_x0,       sizeof(ae_int32x2));
            AE_S32X2_XP(vB2, p_x1, -(int)sizeof(ae_int32x2));
        }
    }
    __Pragma("no_reorder");
    p_x0 = (ae_int32x2 *)(x + 0*stride*2);
    p_x1 = (ae_int32x2 *)(x + 1*stride*2);
    p_x2 = (ae_int32x2 *)(x + 2*stride*2);
    p_x3 = (ae_int32x2 *)(x + 3*stride*2);
    p_y0 = (ae_int32x2 *)(y);

    p_twd = (const ae_int16x4 *)ptwd;
    WUR_AE_SAR(3);
    __Pragma("loop_count min=3");
    /* hifi3z: 10 cycles per stage, unroll=1*/
    for (k = 0; k < (stride>>1); k++)
    {
      ae_int32x2 x0, x1, x2, x3;
      ae_int16x4 tw1, tw2, tw3;

      AE_L16X4_IP(tw1, p_twd, sizeof(ae_int16x4));
      AE_L16X4_IP(tw2, p_twd, sizeof(ae_int16x4));
      AE_L16X4_IP(tw3, p_twd, sizeof(ae_int16x4));

      AE_L32X2_IP(x0, p_x0, sizeof(ae_int32x2));
      AE_L32X2_IP(x1, p_x1, sizeof(ae_int32x2));
      AE_L32X2_IP(x2, p_x2, sizeof(ae_int32x2));
      AE_L32X2_IP(x3, p_x3, sizeof(ae_int32x2));

      DFT4X1(x0, x1, x2, x3);

      x1 = AE_MULFC32X16RAS_H(x1, tw1);
      x2 = AE_MULFC32X16RAS_L(x2, tw1);
      x3 = AE_MULFC32X16RAS_H(x3, tw2);

      AE_S32X2_IP(x0, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x1, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x2, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x3, p_y0, sizeof(ae_int32x2));

      AE_L32X2_IP(x0, p_x0, sizeof(ae_int32x2));
      AE_L32X2_IP(x1, p_x1, sizeof(ae_int32x2));
      AE_L32X2_IP(x2, p_x2, sizeof(ae_int32x2));
      AE_L32X2_IP(x3, p_x3, sizeof(ae_int32x2));

      DFT4X1(x0, x1, x2, x3);

      x1 = AE_MULFC32X16RAS_L(x1, tw2);
      x2 = AE_MULFC32X16RAS_H(x2, tw3);
      x3 = AE_MULFC32X16RAS_L(x3, tw3);

      AE_S32X2_IP(x0, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x1, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x2, p_y0, sizeof(ae_int32x2));
      AE_S32X2_IP(x3, p_y0, sizeof(ae_int32x2));
    }

    /* Last stage */
    __Pragma("no_reorder");
    {
      ae_int16x4 tw1, tw2;
      int j, stride_;
      stride_ = stride >> 1;
      p_x0 = (ae_int32x2 *)y;
      p_y0 = (ae_int32x2 *)y;
      p_twd = (const ae_int16x4 *)fft_twd_r8;

      tw1 = AE_L16X4_I(p_twd, 0 * sizeof(ae_int16x4));
      tw2 = AE_L16X4_I(p_twd, 1 * sizeof(ae_int16x4));

      WUR_AE_SAR(2);
      __Pragma("loop_count min=3");
      for (j = 0; j < stride_; j++)
      {
        ae_int32x2 x0, x1, x2, x3;
        ae_int32x2 x4, x5, x6, x7;

        AE_L32X2_XP(x0, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x1, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x2, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x3, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x4, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x5, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x6, p_x0, stride_*sizeof(ae_int32x2));
        AE_L32X2_XP(x7, p_x0, (-7*stride_ + 1)* sizeof(ae_int32x2));

        DFT4X1(x0, x2, x4, x6); 
        DFT4X1(x1, x3, x5, x7);

        x3 = AE_MULFC32X16RAS_L(x3, tw1);
        x5 = AE_MULFC32X16RAS_H(x5, tw2);
        x7 = AE_MULFC32X16RAS_L(x7, tw2);

        {
          ae_int32x2 s0, s1, s2, s3;
          ae_int32x2 d0, d1, d2, d3;

          x0 = AE_SRAI32R(x0, 1);
          x1 = AE_SRAI32R(x1, 1);
          x2 = AE_SRAI32R(x2, 1);
          x3 = AE_SRAI32R(x3, 1);
          x4 = AE_SRAI32R(x4, 1);
          x5 = AE_SRAI32R(x5, 1);
          x6 = AE_SRAI32R(x6, 1);
          x7 = AE_SRAI32R(x7, 1);

          AE_ADDANDSUB32S(s0, d0, x0, x1);
          AE_ADDANDSUB32S(s1, d1, x2, x3);
          AE_ADDANDSUB32S(s2, d2, x4, x5);
          AE_ADDANDSUB32S(s3, d3, x6, x7);

          x0 = s0;        x4 = d0;
          x1 = s1;        x5 = d1;
          x2 = s2;        x6 = d2;
          x3 = s3;        x7 = d3;
        }

        AE_S32X2_XP(x0, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x1, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x2, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x3, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x4, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x5, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x6, p_y0, stride_*sizeof(ae_int32x2));
        AE_S32X2_XP(x7, p_y0, (-7*stride_ + 1)* sizeof(ae_int32x2));
      }
    }
    #undef stride
    #undef N
}
