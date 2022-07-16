/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"

/*------------------------------------------------------------------------------
  Histogram of image 
  Functions compute the histogram of input image. First bin corresponds minimum
  intensity (0 for signed 16-bit and unsigned 8-bit data, -128 for signed 8-bit)
  and the last bin corresponds to the maximum intensity (32767 for signed 16-bit,
  255 for unsigned 8-bit data, 127  for signed 8-bit data)

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  inImg   input image
  sz      image size
  M       number of bins
  res->h  pointer to the histogram array
  Output:
  res[1]  histogram values, mean and variance

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
--------------------------------------------------------------------------------*/

void imghist_gs8  (imghist_t * restrict res, const void * restrict inImg, const imgsize_t* sz, int M)
{
    ae_int64 var64;
    ae_int16x4 x, x0, x_;
    ae_int32x2 z, t;
    ae_int32x2 d;
    ae_int32x2 d0, d1, d2, d3;
    int8_t p;
    int32_t* h = res->h;
    int32_t mean, N, rN, idx;
    int64_t var;
    int32_t rQ31 = 8421504;//(2^8-1)^(-1) Q31
    const signed char * restrict pIn;
    const int8_t * restrict in = (const int8_t *)inImg;
    int i, j, k;
    int s, sN;
    int w = (int)sz->width;
    int istride = sz->stride;
    int off = 0;
    NASSERT(h);
    res->M = M;
    res->mean = 0;
    res->var = 0;
    for (k = 0; k<M; k++) h[k] = 0;
    if (M<1) return;
    imgsize_validate(sz, 1, 0);
    NASSERT(inImg);
    NASSERT_ALIGN(inImg, 1);
    mean = 0;
    N = (int)sz->height*(int)sz->width;

    ae_int64 mean64 = 0;
    ae_int64 mean64_ = 0;
    for (i = 0; i < (int)sz->height; i++)
    {
        pIn = (const signed char *)(in + i*istride);
        off = (uintptr_t)pIn&(3);
        if (off > 0) off = 4 - off;
        pIn = (const signed char *)(in + i*istride + off);
        for (j = 0; j<off; j++)
        {
            p = in[i*sz->stride + j];
            mean += p;
        }
        for (j = 0; j < ((w-off) >> 3); j++)
        {
            AE_L8X4F_IP(x, pIn, 4 * sizeof(int8_t));
            x = AE_SRAI16(x, 8);
            AE_L8X4F_IP(x0, pIn, 4 * sizeof(int8_t));
            x0 = AE_SRAI16(x0, 8);
            //add 4 elements to each accumulator
            AE_MULAAAAQ16(mean64, x, 1);
            AE_MULAAAAQ16(mean64_, x0, 1);
        }
        for (j = off + (((w - off) >> 3) << 3); j<w; j++)
        {
            p = in[i*sz->stride + j];
            mean += p;
        }
    }
    mean64 += mean64_;
    mean64 = mean64 + (ae_int64)mean;

    // reciprocal of N
    {
        ae_int32x2 Z, E, Y;
        int sy;
        Y = AE_MOVDA32(N);
        sy = AE_NSAZ32_L(Y); Y = AE_SLAA32(Y, sy);
        /* first approximation */
        Z = AE_SUB32(AE_MOVDA32(0xBAEC0000), Y);
        /* 4 iterations to achieve 1 LSB for reciprocal */
        E = 0x40000000; AE_MULSFP32X2RAS(E, Y, Z); AE_MULAFP32X2RAS(Z, Z, AE_ADD32(E, E));
        E = 0x40000000; AE_MULSFP32X2RAS(E, Y, Z); AE_MULAFP32X2RAS(Z, Z, AE_ADD32(E, E));
        E = 0x40000000; AE_MULSFP32X2RAS(E, Y, Z); AE_MULAFP32X2RAS(Z, Z, AE_ADD32(E, E));
        E = 0x40000000; AE_MULSFP32X2RAS(E, Y, Z); AE_MULAFP32X2RAS(Z, Z, AE_ADD32(E, E));
        /* */
        rN = AE_MOVAD32_H(Z);
        sN = sy;
    }

    //Divide mean by N
    s = AE_NSA64(mean64);
    mean64 = AE_SLAA64(mean64, s);
    mean64 = AE_SRAA64(mean64, 32);
    mean = (int32_t)((int64_t)mean64);
    mean = AE_MOVAD32_H(AE_MULFP32X2RS(mean, rN));
    mean = AE_MOVAD32_H(AE_SLAA32(mean, sN - 14 - (s - 32)));
    res->mean = mean;

    var64 = 0;
    for (i = 0; i < (int)sz->height; i++)
    {
        pIn = (const signed char *)(in + i*istride);
        off = (uintptr_t)pIn&(3);
        if (off > 0) off = 4 - off;
        pIn = (const signed char *)(in + i*istride + off);

        for (j = 0; j < off; j++)
        {
            p = in[i*istride + j];
            x = AE_MOVDA16(p);
            x_ = AE_ADD16(x, 128);
            AE_MUL16X4(d0, d1, x_, (M - 1));
            d0 = AE_MULFP32X2RS(d0, rQ31);
            idx = AE_MOVAD32_H(d0) << 2;
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);
            d1 = AE_AND32(AE_MOVINT32X2_FROMINT16X4(x), 0xffff0000);
            d = AE_SUB32(mean, d1);    // Q16
            AE_MULA32_HH(var64, d, d); // Q32 
        }
        for (j = 0; j < ((w - off) >> 2); j++)
        {
            AE_L8X4F_IP(x, pIn, 4 * sizeof(int8_t));
            x = AE_SRAI16(x, 8);
            x_ = AE_ADD16(x, 128);
            AE_MUL16X4(d0, d1, x_, (M - 1));
            d0 = AE_MULFP32X2RS(d0, rQ31);
            d1 = AE_MULFP32X2RS(d1, rQ31);
            d0 = AE_SLAI32(d0, 2);
            d1 = AE_SLAI32(d1, 2);

            /*increment histogam's bins*/
            idx = AE_MOVAD32_H(d0);
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);

            idx = AE_MOVAD32_L(d0);
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);

            idx = AE_MOVAD32_H(d1);
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);

            idx = AE_MOVAD32_L(d1);
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);

            d2 = AE_AND32(AE_MOVINT32X2_FROMINT16X4(x), 0xffff0000);
            d3 = AE_SRLA32(AE_MOVINT32X2_FROMINT16X4(x), -16);

            d = AE_SUB32(mean, d2);    // Q16
            AE_MULAAD32_HH_LL(var64, d, d); // Q32 
            d = AE_SUB32(mean, d3);    // Q16
            AE_MULAAD32_HH_LL(var64, d, d); // Q32 
        }
        for (j = off + (((w - off) >> 2) << 2); j < w; j++)
        {
            p = in[i*istride + j];
            x = AE_MOVDA16(p);
            x_ = AE_ADD16(x, 128);
            AE_MUL16X4(d0, d1, x_, (M - 1));
            d0 = AE_MULFP32X2RS(d0, rQ31);
            idx = AE_MOVAD32_H(d0) << 2;
            t = AE_L32_X((const ae_int32*)h, idx);
            t = AE_ADD32(t, 1);
            AE_S32_L_X(t, (ae_int32*)h, idx);
            d1 = AE_AND32(AE_MOVINT32X2_FROMINT16X4(x), 0xffff0000);
            d = AE_SUB32(mean, d1);    // Q16
            AE_MULA32_HH(var64, d, d); // Q32 
        }
    }
    s = AE_NSA64(var64);
    var64 = AE_SLAA64(var64, s);
    var64 = AE_SRLI64(var64, 32);
    var = (int64_t)var64;
    z = AE_MULFP32X2RS((int32_t)var, rN);
    s = s - sN + 30;
    z = AE_SRAA32RS(z, s);
    res->var = AE_MOVAD32_H(z);
}
