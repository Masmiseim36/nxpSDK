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
    2D-IDCT, 16-bit input & 8-bit output with no scaling
    C code optimized for HiFi4
    Integrit, 2006-2017
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Twiddle factor tables for DCTs. */
#include "dct2_twd.h"

/*-------------------------------------------------------------------------
  2-D Inverse Discrete Cosine Transform.
  These functions apply inverse DCT (Type II) to the series of L input 
  blocks of NxN pixels. Algorithm uses ITU-T T.81 (JPEG compression) DCT-II 
  definition with bias 128 and left-to-right, top-to-bottom orientation.

  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       idct2d_16x8     |           0 - no scaling             |
      +-----------------------+--------------------------------------+
  Notes:
  N - IDCT size (depends on selected IDCT handle)

  Precision: 
  16x8  16-bit signed input, 8-bit unsigned output

  Input:
  x[N*N*L]    input data: L NxN blocks
  h           DCT handle
  L           number of input blocks
  scalingOpt  scaling option (see table above), should be 0

  Output:
  y[N*N*L]    output pixels: L NxN blocks

  Returned value: 0
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary

-------------------------------------------------------------------------*/

/* DCT twiddles */
static const int32_t ALIGN(8) twd_tbl[] =
{
    (int32_t)12785,/* cos(7*pi/16), Q16 */
    (int32_t)64277,/* cos(1*pi/16), Q16 */
    (int32_t)54491,/* cos(3*pi/16), Q16 */
    (int32_t)36410,/* cos(5*pi/16), Q16 */
    (int32_t)17734,/* cos(2*pi/16)-cos(6*pi/16), Q15 */
    (int32_t)42813,/* cos(2*pi/16)+cos(6*pi/16), Q15 */
    (int32_t)46341 /* sqrt(2), Q15 */
};
static const tdct2_twd twd={0,8,NULL,(void *)twd_tbl};
const dct_handle_t idct2d_16_8=(const dct_handle_t*)&twd;
#define sz_i16 ((int)sizeof(int16_t))

#ifndef AE_ADDSUB32_HL_LH
#define AE_ADDSUB32_HL_LH(A,B) AE_ADDSUB32(A,AE_SEL32_LH(B,B))
#endif
#ifndef AE_SUBADD32_HL_LH
#define AE_SUBADD32_HL_LH(A,B) AE_SUBADD32(A,AE_SEL32_LH(B,B))
#endif

int idct2d_16x8(uint8_t * y, int16_t * x, dct_handle_t h, int L, int scalingOpt)
{
    int16_t ALIGN(8) rows[8*8];
    const tdct2_twd *ptwd=(const tdct2_twd *)h;
    const ae_int16   * restrict pX0;
    const ae_int16   * restrict pX1;
          ae_int16x4 * restrict pY;
    const ae_int32x2 * restrict pC;
    const ae_int16   * restrict pRld0;
    const ae_int16   * restrict pRld1;
          ae_int16x4 * restrict pRwr;

    ae_int16x4 x0, x1, v0, v1, v2, v3, v4, v5, v6, v7;
    ae_int32x2 A01, A23, A47, A56, A02, A13, A64, A75;
    ae_int32x2 B01, B23, B47, B56;
    ae_int32x2 Y0, Y1, Y2, Y3, T0, T1;
    ae_int32x2 CF0, CF1, CF2, CR2;
    int l, n;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 0);

    pC  = (const ae_int32x2 *)(ptwd->fft_twd);
    pX0 = (const ae_int16   *)(x+6*8);
    pX1 = (const ae_int16   *)(x+7*8);
    pY  = (      ae_int16x4 *)(y);

    AE_L32X2_IP(CF0, pC, sizeof(ae_int32x2));
    AE_L32X2_IP(CF1, pC, sizeof(ae_int32x2));
    AE_L32X2_IP(CF2, pC, sizeof(ae_int32x2));
    CR2 = AE_L32_I((const ae_int32 *)pC, 0);

    for (l=0;l<L;l++)
    {
        /* Process columns */
        pRld0 = (const ae_int16   *)(rows+6*8);
        pRld1 = (const ae_int16   *)(rows+7*8);
        pRwr  = (      ae_int16x4 *)(rows);
        {
            /* Stage 1 */
            v0 = AE_L16_X(pX0, -6*8*sz_i16);
            v1 = AE_L16_X(pX1, -6*8*sz_i16);
            v2 = AE_L16_X(pX0, -4*8*sz_i16);
            v3 = AE_L16_X(pX1, -4*8*sz_i16);
            v4 = AE_L16_X(pX0, -2*8*sz_i16);
            v5 = AE_L16_X(pX1, -2*8*sz_i16);
            AE_L16_IP(v6, pX0, sz_i16);
            AE_L16_IP(v7, pX1, sz_i16);
            v0 = AE_SEL16_7362(v0, v4);
            v2 = AE_SEL16_7362(v2, v6);
            x0 = AE_SEL16_7632(v0, v2);
            v5 = AE_SEL16_7362(v5, v3);
            v1 = AE_SEL16_7362(v1, v7);
            x1 = AE_SEL16_7632(v5, v1);
            A47 = AE_MULC32X16_L(CF0, x1);
            A56 = AE_MULC32X16_H(CF1, x1);
            /* Stage 2 */
            A01 = AE_SEXT32X2D16_32(x0);
            B01 = AE_SUBADD32_HL_LH(A01, A01);
            B01 = AE_ADD32(B01, AE_MOVDA32(8 * 128));/* remove bias of DC component */
            B23 = AE_MULFC32X16RAS_L(CF2, x0);
            B47 = AE_ADD32(A47, A56);
            B56 = AE_SUB32(A47, A56);
            /* Stage 3 */
            A01 = AE_ADD32(B01, B23);
            A23 = AE_SUB32(B01, B23);
            A56 = AE_ADDSUB32_HL_LH(B56, B56);
            A56 = AE_SRAI32R(A56, 16);
            v0 = AE_ROUND16X4F32SASYM(B47, B47);
            A47 = AE_MULFP32X16X2RAS_L(CR2, v0);
            /* Output */
            A01 = AE_SEL32_LH(A01, A01);
            T1 = AE_SEL32_LH(A47, A56);
            T0 = AE_SEL32_LH(A56, A47);
            Y0 = AE_ADD32(A01, T1);
            Y1 = AE_ADD32(A23, T0);
            Y2 = AE_SUB32(A01, T1);
            Y3 = AE_SUB32(A23, T0);
            x0 = AE_SAT16X4(Y0, Y1);
            x1 = AE_SAT16X4(Y3, Y2);
            T0 = AE_MOVINT32X2_FROMINT16X4(x1);
            AE_S16X4_IP(x0, pRwr, sizeof(ae_int16x4));
            AE_S32X2_IP(T0, castxcc(ae_int32x2,pRwr), sizeof(ae_int16x4));
        }
        for (n=1;n<8;n++)
        {
            /* Stage 1 */
            v0 = AE_L16_X(pX0, -6*8*sz_i16);
            v1 = AE_L16_X(pX1, -6*8*sz_i16);
            v2 = AE_L16_X(pX0, -4*8*sz_i16);
            v3 = AE_L16_X(pX1, -4*8*sz_i16);
            v4 = AE_L16_X(pX0, -2*8*sz_i16);
            v5 = AE_L16_X(pX1, -2*8*sz_i16);
            AE_L16_IP(v6, pX0, sz_i16);
            AE_L16_IP(v7, pX1, sz_i16);
            v0 = AE_SEL16_7362(v0, v4);
            v2 = AE_SEL16_7362(v2, v6);
            x0 = AE_SEL16_7632(v0, v2);
            v5 = AE_SEL16_7362(v5, v3);
            v1 = AE_SEL16_7362(v1, v7);
            x1 = AE_SEL16_7632(v5, v1);
            A47 = AE_MULC32X16_L(CF0, x1);
            A56 = AE_MULC32X16_H(CF1, x1);
            /* Stage 2 */
            A01 = AE_SEXT32X2D16_32(x0);
            B01 = AE_SUBADD32_HL_LH(A01, A01);
            B23 = AE_MULFC32X16RAS_L(CF2, x0);
            B47 = AE_ADD32(A47, A56);
            B56 = AE_SUB32(A47, A56);
            /* Stage 3 */
            A01 = AE_ADD32(B01, B23);
            A23 = AE_SUB32(B01, B23);
            A56 = AE_ADDSUB32_HL_LH(B56, B56);
            A56 = AE_SRAI32R(A56, 16);
            v0 = AE_ROUND16X4F32SASYM(B47, B47);
            A47 = AE_MULFP32X16X2RAS_L(CR2, v0);
            /* Output */
            A01 = AE_SEL32_LH(A01, A01);
            T1 = AE_SEL32_LH(A47, A56);
            T0 = AE_SEL32_LH(A56, A47);
            Y0 = AE_ADD32(A01, T1);
            Y1 = AE_ADD32(A23, T0);
            Y2 = AE_SUB32(A01, T1);
            Y3 = AE_SUB32(A23, T0);
            x0 = AE_SAT16X4(Y0, Y1);
            x1 = AE_SAT16X4(Y3, Y2);
            T0 = AE_MOVINT32X2_FROMINT16X4(x1);
            AE_S16X4_IP(x0, pRwr, sizeof(ae_int16x4));
            AE_S32X2_IP(T0, castxcc(ae_int32x2,pRwr), sizeof(ae_int16x4));
        }
        __Pragma("no_reorder");
        /* Process rows */
        for (n=0;n<8;n++)
        {
            /* Stage 1 */
            v0 = AE_L16_X(pRld0, -6*8*sz_i16);
            v1 = AE_L16_X(pRld1, -6*8*sz_i16);
            v2 = AE_L16_X(pRld0, -4*8*sz_i16);
            v3 = AE_L16_X(pRld1, -4*8*sz_i16);
            v4 = AE_L16_X(pRld0, -2*8*sz_i16);
            v5 = AE_L16_X(pRld1, -2*8*sz_i16);
            AE_L16_IP(v6, pRld0, sz_i16);
            AE_L16_IP(v7, pRld1, sz_i16);
            v0 = AE_SEL16_7362(v0, v4);
            v2 = AE_SEL16_7362(v2, v6);
            x0 = AE_SEL16_7632(v0, v2);
            v5 = AE_SEL16_7362(v5, v3);
            v1 = AE_SEL16_7362(v1, v7);
            x1 = AE_SEL16_7632(v5, v1);
            A47 = AE_MULC32X16_L(CF0, x1);
            A56 = AE_MULC32X16_H(CF1, x1);
            /* Stage 2 */
            A01 = AE_SEXT32X2D16_32(x0);
            B01 = AE_SUBADD32_HL_LH(A01, A01);
            B23 = AE_MULFC32X16RAS_L(CF2, x0);
            B47 = AE_ADD32(A47, A56);
            B56 = AE_SUB32(A47, A56);
            /* Stage 3 */
            A02 = AE_SUBADD32(B01, B23);
            A13 = AE_ADDSUB32(B01, B23);
            B56 = AE_SRAI32R(B56, 16);
            A56 = AE_ADDSUB32_HL_LH(B56, B56);
            v0 = AE_ROUND16X4F32SASYM(B47, B47);
            A47 = AE_MULFP32X16X2RAS_L(CR2, v0);
            A75 = AE_SEL32_LL(A56, A47);
            A64 = AE_SEL32_HH(A56, A47);
            /* Output */
            Y0 = AE_ADD32(A02, A75);
            Y1 = AE_ADD32(A13, A64);
            Y2 = AE_SUB32(A02, A75);
            Y3 = AE_SUB32(A13, A64);
            Y0 = AE_MIN32(Y0, AE_MOVDA32(255<<3));
            Y0 = AE_MAX32(Y0, AE_MOVDA32(0));
            Y1 = AE_MIN32(Y1, AE_MOVDA32(255<<3));
            Y1 = AE_MAX32(Y1, AE_MOVDA32(0));
            Y2 = AE_MIN32(Y2, AE_MOVDA32(255<<3));
            Y2 = AE_MAX32(Y2, AE_MOVDA32(0));
            Y3 = AE_MIN32(Y3, AE_MOVDA32(255<<3));
            Y3 = AE_MAX32(Y3, AE_MOVDA32(0));
            v0 = AE_SAT16X4(Y3, Y0);
            v1 = AE_SAT16X4(Y1, Y2);
            v0 = AE_SRAI16R(v0, 3);
            v1 = AE_SRAI16R(v1, 3);
            v0 = AE_SHORTSWAP(v0);
            v1 = AE_MOVINT16X4_FROMINT64(AE_SLAI64(AE_MOVINT64_FROMINT16X4(v1),8));
            x0 = AE_OR16(v0, v1);
            AE_S16X4_IP(x0, pY, 8*sizeof(uint8_t));
        }
        /* Go to the next block */
        pX0 += 7*8;
        pX1 += 7*8;
    }

    return 0;
} /* idct2d_16x8() */
