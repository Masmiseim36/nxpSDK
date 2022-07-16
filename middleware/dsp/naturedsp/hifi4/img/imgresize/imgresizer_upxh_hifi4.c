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
#include "img_getCoef_up.h"
#include "imgresizer_bilinear_common.h"

/*    image resizer, upsample 1...2x in horizontal direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return img_getCoef_up_alloc(win,wout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    img_getCoef_up_init(coef,win,wout);
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
    const int16_t * restrict w =    ((img_coefup_t*)pCoef)->coef;
    const int16_t * restrict left = ((img_coefup_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16   * restrict pW;
    ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7;
    ae_int16x4 w0, w1, w2, w3, w4, w5, w6, w7;
    ae_int32x2 Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7;
    ae_int16x4 y0, y1, y2, y3;
    int m, n, k0, k1, k2, k3;
    int hout = out->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;

    (void)pCoef;
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
            AE_L16X4_XP(x0, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x1, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x2, pIn, stride*sizeof(int16_t));
            AE_L16X4_XP(x3, pIn, -3*stride*sizeof(int16_t) + sizeof(ae_int16x4));
            y0 = AE_SEL16_7531(x0, x1);
            y1 = AE_SEL16_6420(x0, x1);
            y2 = AE_SEL16_7531(x2, x3);
            y3 = AE_SEL16_6420(x2, x3);
            x0 = AE_SEL16_7531(y0, y2);
            x2 = AE_SEL16_6420(y0, y2);
            x1 = AE_SEL16_7531(y1, y3);
            x3 = AE_SEL16_6420(y1, y3);
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x2, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(x3, pOut, sizeof(ae_int16x4));
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
            /* Load 2x4 elems for each of 4 rows */
            x0 = AE_L16X4_X(pIn, k0*sizeof(ae_int16x4));
            x1 = AE_L16X4_X(pIn, k0*sizeof(ae_int16x4) + sizeof(ae_int16x4));
            x2 = AE_L16X4_X(pIn, k1*sizeof(ae_int16x4));
            x3 = AE_L16X4_X(pIn, k1*sizeof(ae_int16x4) + sizeof(ae_int16x4));
            x4 = AE_L16X4_X(pIn, k2*sizeof(ae_int16x4));
            x5 = AE_L16X4_X(pIn, k2*sizeof(ae_int16x4) + sizeof(ae_int16x4));
            x6 = AE_L16X4_X(pIn, k3*sizeof(ae_int16x4));
            x7 = AE_L16X4_X(pIn, k3*sizeof(ae_int16x4) + sizeof(ae_int16x4));
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));
            AE_L16_IP(w4, pW, sizeof(int16_t));
            AE_L16_IP(w5, pW, sizeof(int16_t));
            AE_L16_IP(w6, pW, sizeof(int16_t));
            AE_L16_IP(w7, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y0, Y1, x0, w0);
            AE_MULAF16X4SS(Y0, Y1, x1, w1);
            AE_MULF16X4SS (Y2, Y3, x2, w2);
            AE_MULAF16X4SS(Y2, Y3, x3, w3);
            AE_MULF16X4SS (Y4, Y5, x4, w4);
            AE_MULAF16X4SS(Y4, Y5, x5, w5);
            AE_MULF16X4SS (Y6, Y7, x6, w6);
            AE_MULAF16X4SS(Y6, Y7, x7, w7);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            y2 = AE_ROUND16X4F32SASYM(Y4, Y5);
            y3 = AE_ROUND16X4F32SASYM(Y6, Y7);

            x0 = AE_SEL16_7531(y0, y1);
            x1 = AE_SEL16_6420(y0, y1);
            x2 = AE_SEL16_7531(y2, y3);
            x3 = AE_SEL16_6420(y2, y3);
            y0 = AE_SEL16_7531(x0, x2);
            y2 = AE_SEL16_6420(x0, x2);
            y1 = AE_SEL16_7531(x1, x3);
            y3 = AE_SEL16_6420(x1, x3);
            AE_S16X4_XP(y0, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(y1, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(y2, pOut, stride*sizeof(int16_t));
            AE_S16X4_XP(y3, pOut, -3*stride*sizeof(int16_t) + sizeof(ae_int16x4));
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
            AE_L16X4_IP(x0, pIn, sizeof(ae_int16x4));
            AE_S16X4_IP(x0, pOut, sizeof(ae_int16x4));
        }

        pIn = (const ae_int16x4 *)t;
        pOut = (ae_int16x4 *)((int16_t *)img + n*stride);
        pW = (const ae_int16 *)w;
        /* Process by 1 output sample per iteration */
        for (m = 0; m < wout; m++)
        {
            k0 = left[m];
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            x0 = AE_L16_X((const ae_int16 *)pIn, k0*sizeof(int16_t));
            x1 = AE_L16_X((const ae_int16 *)pIn, k0*sizeof(int16_t) + sizeof(int16_t));
            Y0 = AE_MULF16SS_00(x0, w0);
            AE_MULAF16SS_00(Y0, x1, w1);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
            AE_S16_0_IP(y0, castxcc(ae_int16,pOut), sizeof(int16_t));
        }
    }
}

const imgresizer_api_t imgresizer_api_upxh={"imgresizer_api_upxh",getCoefSz,getCoef,getScratchSize,process};
