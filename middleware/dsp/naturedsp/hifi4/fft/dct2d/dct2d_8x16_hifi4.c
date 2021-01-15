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
    2D-DCT, 8-bit input & 16-bit output with no scaling
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
  2-D Discrete Cosine Transform.
  These functions apply DCT (Type II) to the series of L input blocks 
  of NxN pixels. Algorithm uses ITU-T T.81 (JPEG compression) DCT-II 
  definition with bias 128 and left-to-right, top-to-bottom orientation.

  Scaling:
      +-----------------------+--------------------------------------+
      |      Function         |           Scaling options            |
      +-----------------------+--------------------------------------+
      |       dct2d_8x16      |           0 - no scaling             |
      +-----------------------+--------------------------------------+
  Notes:
  N - DCT size (depends on selected DCT handle)

  Precision: 
  8x16  8-bit unsigned input, 16-bit signed output

  Input:
  x[N*N*L]    input pixels: L NxN blocks
  h           DCT handle
  L           number of input blocks
  scalingOpt  scaling option (see table above), should be 0

  Output:
  y[N*N*L]    output of transform: L NxN blocks
  
  Returned value: 0
  Restriction:
  x,y         should not overlap
  x,y         aligned on 8-bytes boundary

-------------------------------------------------------------------------*/

/* Twiddles table */
static const int32_t ALIGN(8) coef_tbl[] =
{
    /* C(k)=cos(k*pi/16), S(k)=sin(k*pi/16) */
    (int32_t)54491,/*  cos(3*pi/16),         Q16 */
    (int32_t)36410,/*  cos(5*pi/16),         Q16 */
   -(int32_t)12785,/* -cos(7*pi/16),         Q16 */
    (int32_t)64277,/*  cos(1*pi/16),         Q16 */
    (int32_t)35468,/*  cos(6*pi/16)*sqrt(2), Q16 */
    (int32_t)85627,/*  cos(2*pi/16)*sqrt(2), Q16 */
    (int32_t)92682 /*  sqrt(2),              Q16 */
};
static const tdct2_twd twd={0,8,NULL,(void *)coef_tbl};
const dct_handle_t dct2d_16_8=(const dct_handle_t*)&twd;

#ifndef AE_SUBADD32_HL_LH
#define AE_SUBADD32_HL_LH(A,B) AE_SUBADD32(A,AE_SEL32_LH(B,B))
#endif

int dct2d_8x16(int16_t* y, uint8_t * x, dct_handle_t h, int L, int scalingOpt)
{
    int16_t ALIGN(8) rows[8*8];
    const tdct2_twd *ptwd=(const tdct2_twd *)h;
    const int8_t     * restrict pX;
    const ae_int16x4 * restrict pRrd;
          ae_int16   * restrict pRwr0;
          ae_int16   * restrict pRwr1;
    const ae_int32x2 * restrict pC;
          ae_int16   * restrict pY0;
          ae_int16   * restrict pY1;

    ae_int16x4 z0, z1, z2, z3, z4, z5, z6, z7;
    ae_int16x4 t0123, t4567, t0v, t1v, u23, mask;
    ae_int32x2 T01, T23, T45, T67, U01, U23, U45, U67;
    ae_int32x2 CF0, CF1, CF2, CR2;
    int l, n;

    NASSERT_ALIGN8(x);
    NASSERT_ALIGN8(y);
    NASSERT(scalingOpt == 0);

    pC  =(const ae_int32x2 *)(ptwd->fft_twd);
    pX  =(const int8_t     *)(x);

    mask = AE_MOVDA16(0xFF);
    AE_L32X2_IP(CF0, pC, sizeof(ae_int32x2));
    AE_L32X2_IP(CF1, pC, sizeof(ae_int32x2));
    AE_L32X2_IP(CF2, pC, sizeof(ae_int32x2));
    CR2 = AE_L32_I((const ae_int32 *)pC, 0);

    for (l=0;l<L;l++)
    {
        /* Process rows */
        pRwr0=(ae_int16 *)(rows);
        pRwr1=(ae_int16 *)(rows+8);
        for (n=0;n<8;n++)
        {
            /* Stage 1 */
            AE_L8X4F_IP(t0v, pX, 4*sizeof(uint8_t));
            AE_L8X4F_IP(t1v, pX, 4*sizeof(uint8_t));
            t0v = AE_SRAI16(t0v, 8);
            t1v = AE_SRAI16(t1v, 8);
            t0123 = AE_AND16(t0v, mask);
            t4567 = AE_AND16(t1v, mask);

            t0v = AE_SHORTSWAP(t0123);
            t1v = AE_SHORTSWAP(t4567);
            t0123 = AE_SEL16_7632(t0123, t0v);
            t4567 = AE_SEL16_7632(t1v, t4567);
            t0v = AE_ADD16(t0123, t4567);
            t1v = AE_SUB16(t0123, t4567);
            /* Stage 2 */
            T01 = AE_SEXT32X2D16_32(t0v);
            T23 = AE_SEXT32X2D16_10(t0v);
            U01 = AE_ADD32(T01, T23);
            U23 = AE_SUB32(T01, T23);
            t1v = AE_SEL16_7520(t1v, t1v);
            U45 = AE_MULC32X16_H(CF0, t1v);
            U67 = AE_MULC32X16_L(CF1, t1v);
            /* Stage 3 */
            T01 = AE_SUBADD32_HL_LH(U01, U01);
            u23 = AE_SAT16X4(U23, U23);
            T23 = AE_MULC32X16_L(CF2, u23);
            T45 = AE_SUBADD32(U45, U67);
            T67 = AE_ADDSUB32(U45, U67);
            /* Stage 4 */
            T45 = AE_SUBADD32_HL_LH(T45, T45);
            t0v = AE_ROUND16X4F32SASYM(T67, T45);
            T67 = AE_MULP32X16X2_H(CR2, t0v);
            t1v = AE_ROUND16X4F32SASYM(T23, T67);
            z0 = AE_MOVINT16X4_FROMINT32X2(T01);
            z1 = t0v;
            z2 = AE_SEL16_5432(t1v, t1v);
            z3 = AE_SEL16_4321(t1v, t1v);
            z4 = AE_MOVINT16X4_FROMINT32X2(AE_SEL32_LH(T01, T01));
            z5 = t1v;
            z6 = AE_SEL16_6543(t1v, t1v);
            z7 = AE_SEL16_4321(t0v, t0v);
            AE_S16_0_XP(z0, pRwr0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z1, pRwr1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z2, pRwr0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z3, pRwr1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z4, pRwr0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z5, pRwr1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z6, pRwr0, sizeof(int16_t)-6*8*(int)sizeof(int16_t));
            AE_S16_0_XP(z7, pRwr1, sizeof(int16_t)-6*8*(int)sizeof(int16_t));
        }
        __Pragma("no_reorder");
        /* Process columns */
        pRrd=(const ae_int16x4 *)(rows);
        pY0 =(      ae_int16 *)(y);
        pY1 =(      ae_int16 *)(y+8);
        for (n=0;n<8;n++)
        {
            /* Stage 1 */
            AE_L16X4_IP(t0123, pRrd, sizeof(ae_int16x4));
            AE_L16X4_IP(t4567, pRrd, sizeof(ae_int16x4));
            t0v = AE_SHORTSWAP(t0123);
            t1v = AE_SHORTSWAP(t4567);
            t0123 = AE_SEL16_7632(t0123, t0v);
            t4567 = AE_SEL16_7632(t1v, t4567);
            t0v = AE_ADD16(t0123, t4567);
            t1v = AE_SUB16(t0123, t4567);
            /* Stage 2 */
            T01 = AE_SEXT32X2D16_32(t0v);
            T23 = AE_SEXT32X2D16_10(t0v);
            U01 = AE_ADD32(T01, T23);
            U23 = AE_SUB32(T01, T23);
            t1v = AE_SEL16_7520(t1v, t1v);
            U45 = AE_MULC32X16_H(CF0, t1v);
            U67 = AE_MULC32X16_L(CF1, t1v);
            /* Stage 3 */
            T01 = AE_SUBADD32_HL_LH(U01, U01);
            u23 = AE_SAT16X4(U23, U23);
            T23 = AE_MULC32X16_L(CF2, u23);
            T45 = AE_SUBADD32(U45, U67);
            T67 = AE_ADDSUB32(U45, U67);
            /* Stage 4 */
            T45 = AE_SUBADD32_HL_LH(T45, T45);
            t0v = AE_ROUND16X4F32SASYM(T67, T45);
            T67 = AE_MULP32X16X2_H(CR2, t0v);
            t1v = AE_ROUND16X4F32SASYM(T23, T67);
            T01 = AE_SRAI32R(T01, 3);
            t0v = AE_SRAI16R(t0v, 3);
            t1v = AE_SRAI16R(t1v, 3);
            z0 = AE_MOVINT16X4_FROMINT32X2(T01);
            z1 = t0v;
            z2 = AE_SEL16_5432(t1v, t1v);
            z3 = AE_SEL16_4321(t1v, t1v);
            z4 = AE_MOVINT16X4_FROMINT32X2(AE_SEL32_LH(T01, T01));
            z5 = t1v;
            z6 = AE_SEL16_6543(t1v, t1v);
            z7 = AE_SEL16_4321(t0v, t0v);
            AE_S16_0_XP(z0, pY0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z1, pY1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z2, pY0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z3, pY1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z4, pY0, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z5, pY1, 2*8*sizeof(int16_t));
            AE_S16_0_XP(z6, pY0, sizeof(int16_t)-6*8*(int)sizeof(int16_t));
            AE_S16_0_XP(z7, pY1, sizeof(int16_t)-6*8*(int)sizeof(int16_t));
        }
        y[0] = y[0]-8*128;
        /* Go to the next block */
        y += 8*8;
    }

    return 0;
} /* dct2d_8x16() */
