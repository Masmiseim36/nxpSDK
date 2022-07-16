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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "img_getCoef_up_cubic.h"
#include "imgresizer_bicubic_common.h"

/*    image resizer, upsample 1...2x in horizontal direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return img_getCoef_up_cubic_alloc(win,wout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    img_getCoef_up_cubic_init(coef,win,wout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return ((in->width*sizeof(int16_t)+ALIGNMENT-1)&~(ALIGNMENT-1))*4;  // 4 input rows
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    int16_t * restrict t = (int16_t *)pScr;
    const int16_t * restrict w =    ((img_coefup_cubic_t*)pCoef)->coef;
    const int16_t * restrict left = ((img_coefup_cubic_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16   * restrict pW;
    ae_int16x4 x00, x01, x02, x03;
    ae_int16x4 x10, x11, x12, x13;
    ae_int16x4 x20, x21, x22, x23;
    ae_int16x4 x30, x31, x32, x33;
    ae_int16x4 w0, w1, w2, w3, w4, w5, w6, w7;
    ae_int32x2 Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7;
    ae_int16x4 y0, y1, y2, y3;
    ae_int64 A0, A1, A2, A3;
    ae_valign alIn;
    int m, n, k0, k1, k2, k3;
    int hout = out->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;

    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height==out->height &&  wout<2*win && wout>win && in->stride==out->stride);

    /* Process the image by 4 output rows per iteration */
    for (n = 0; n < (hout & ~3); n+=4)
    {
        /* Interleave input samples from 4 rows and save them to the scratch */
        pIn = (const ae_int16x4 *)((int16_t *)img + n*stride);
        pOut = (ae_int16x4 *)t;
        for (m = 0; m < win; m+=4)
        {
            AE_L16X4_XP(x00, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x10, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x20, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x30, pIn, -3*stride*sizeof(int16_t) + sizeof(ae_int16x4));
            y0 = AE_SEL16_7531(x00, x10);
            y1 = AE_SEL16_6420(x00, x10);
            y2 = AE_SEL16_7531(x20, x30);
            y3 = AE_SEL16_6420(x20, x30);
            x00 = AE_SEL16_7531(y0, y2);
            x02 = AE_SEL16_6420(y0, y2);
            x01 = AE_SEL16_7531(y1, y3);
            x03 = AE_SEL16_6420(y1, y3);
            AE_S16X4_IP(x00, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x01, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x02, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x03, pOut, sizeof(ae_int16x4));
        }
        pIn = (const ae_int16x4 *)t; /* load input samples from scratch */
        pOut = (ae_int16x4 *)((int16_t *)img + n*stride);
        pW = (const ae_int16 *)w;
        /* Process by 4x4 output samples per iteration */
        for (m = 0; m < wout; m+=4)
        {
            /* Load indexes */
            k0 = left[m+0];
            k1 = left[m+1];
            k2 = left[m+2];
            k3 = left[m+3];
            /* Load 4x4 elems for each of 4 rows */
            pIn = (const ae_int16x4 *)((uintptr_t)t+k0*sizeof(ae_int16x4));
            AE_L16X4_IP(x00, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x01, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x02, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x03, pIn, sizeof(ae_int16x4));
            pIn = (const ae_int16x4 *)((uintptr_t)t+k1*sizeof(ae_int16x4));
            AE_L16X4_IP(x10, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x11, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x12, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x13, pIn, sizeof(ae_int16x4));
            pIn = (const ae_int16x4 *)((uintptr_t)t+k2*sizeof(ae_int16x4));
            AE_L16X4_IP(x20, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x21, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x22, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x23, pIn, sizeof(ae_int16x4));
            pIn = (const ae_int16x4 *)((uintptr_t)t+k3*sizeof(ae_int16x4));
            AE_L16X4_IP(x30, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x31, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x32, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(x33, pIn, sizeof(ae_int16x4));
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));
            AE_L16_IP(w4, pW, sizeof(int16_t));
            AE_L16_IP(w5, pW, sizeof(int16_t));
            AE_L16_IP(w6, pW, sizeof(int16_t));
            AE_L16_IP(w7, pW, sizeof(int16_t));

            AE_MUL16X4 (Y0, Y1, x00, w0);
            AE_MULA16X4(Y0, Y1, x01, w1);
            AE_MULA16X4(Y0, Y1, x02, w2);
            AE_MULA16X4(Y0, Y1, x03, w3);
            AE_MUL16X4 (Y2, Y3, x10, w4);
            AE_MULA16X4(Y2, Y3, x11, w5);
            AE_MULA16X4(Y2, Y3, x12, w6);
            AE_MULA16X4(Y2, Y3, x13, w7);
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));
            AE_L16_IP(w4, pW, sizeof(int16_t));
            AE_L16_IP(w5, pW, sizeof(int16_t));
            AE_L16_IP(w6, pW, sizeof(int16_t));
            AE_L16_IP(w7, pW, sizeof(int16_t));

            AE_MUL16X4 (Y4, Y5, x20, w0);
            AE_MULA16X4(Y4, Y5, x21, w1);
            AE_MULA16X4(Y4, Y5, x22, w2);
            AE_MULA16X4(Y4, Y5, x23, w3);
            AE_MUL16X4 (Y6, Y7, x30, w4);
            AE_MULA16X4(Y6, Y7, x31, w5);
            AE_MULA16X4(Y6, Y7, x32, w6);
            AE_MULA16X4(Y6, Y7, x33, w7);

            Y0 = AE_SRAI32R(Y0, 14);
            Y1 = AE_SRAI32R(Y1, 14);
            Y2 = AE_SRAI32R(Y2, 14);
            Y3 = AE_SRAI32R(Y3, 14);
            Y4 = AE_SRAI32R(Y4, 14);
            Y5 = AE_SRAI32R(Y5, 14);
            Y6 = AE_SRAI32R(Y6, 14);
            Y7 = AE_SRAI32R(Y7, 14);
            y0 = AE_SAT16X4(Y0, Y2);
            y1 = AE_SAT16X4(Y1, Y3);
            y2 = AE_SAT16X4(Y4, Y6);
            y3 = AE_SAT16X4(Y5, Y7);

            x00 = AE_SEL16_7531(y0, y2);
            x10 = AE_SEL16_6420(y0, y2);
            x20 = AE_SEL16_7531(y1, y3);
            x30 = AE_SEL16_6420(y1, y3);
            AE_S16X4_XP(x00, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(x10, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(x20, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(x30, pOut, -3*stride*sizeof(int16_t) + sizeof(ae_int16x4));
        }
    }
    /* Process last 0...3 rows */
    for (n = (hout & ~3); n < hout; n++)
    {
        /* Move input samples to the scratch */
        pIn = (const ae_int16x4 *)((int16_t *)img + n*stride);
        pOut = (ae_int16x4 *)t;
        for (m = 0; m < win; m+=4)
        {
            AE_L16X4_IP(x00, pIn, sizeof(ae_int16x4));
            AE_S16X4_IP(x00, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)t; /* load input samples from scratch */
        pOut = (ae_int16x4 *)((int16_t *)img + n*stride);
        pW = (const ae_int16 *)w;
        /* Process by 1x4 output samples per iteration */
        for (m = 0; m < wout; m+=4)
        {
            k0 = left[m+0];
            k1 = left[m+1];
            k2 = left[m+2];
            k3 = left[m+3];
            /* Load by 1x4 input samples for each of 4 output samples */
            pIn = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)t);
            alIn = AE_LA64_PP(pIn);  AE_LA16X4_IP(x00, alIn, pIn);
            pIn = (ae_int16x4 *)XT_ADDX2(k1, (uintptr_t)t);
            alIn = AE_LA64_PP(pIn);  AE_LA16X4_IP(x10, alIn, pIn);
            pIn = (ae_int16x4 *)XT_ADDX2(k2, (uintptr_t)t);
            alIn = AE_LA64_PP(pIn);  AE_LA16X4_IP(x20, alIn, pIn);
            pIn = (ae_int16x4 *)XT_ADDX2(k3, (uintptr_t)t);
            alIn = AE_LA64_PP(pIn);  AE_LA16X4_IP(x30, alIn, pIn);
            /* Load window coefficients */
            AE_L16X4_IP(w0, castxcc(ae_int16x4,pW), sizeof(ae_int16x4));
            AE_L16X4_IP(w1, castxcc(ae_int16x4,pW), sizeof(ae_int16x4));
            AE_L16X4_IP(w2, castxcc(ae_int16x4,pW), sizeof(ae_int16x4));
            AE_L16X4_IP(w3, castxcc(ae_int16x4,pW), sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x00, w0);
            A1 = AE_MULZAAAAQ16(x10, w1);
            A2 = AE_MULZAAAAQ16(x20, w2);
            A3 = AE_MULZAAAAQ16(x30, w3);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 34);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 34);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }
    }
}

const imgresizer_api_t imgresizer_api_upxh_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
