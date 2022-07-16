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
#include "img_getCoef_dn.h"
#include "imgresizer_bilinear_common.h"

/*    image resizer, downsample 1...2x in vertical direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT(in->width==out->width &&  hin<2*hout && hin>hout && in->stride==out->stride);
    return img_getCoef_dn_alloc(hin,hout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT(in->width==out->width &&  hin<2*hout && hin>hout && in->stride==out->stride);
    img_getCoef_dn_init(coef,hin,hout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    /* 8 columns */
    return out->height*sizeof(int16_t)*8;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    int16_t * restrict t=(int16_t *)pScr;
    const int16_t * restrict w= ((img_coefdn_t*)pCoef)->coef;
    const int16_t * restrict up=((img_coefdn_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pT;
    const ae_int16   * restrict pW;
    ae_int16x4 x00, x10, x20, x30;
    ae_int16x4 x01, x11, x21, x31;
    ae_int16x4 w0, w1, w2, w3;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 y0, y1, t0, t1;
    int m, n, k;
    int wout = out->width,
        hout = out->height,
        stride = out->stride;

    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->width == out->width && in->height<2*out->height && in->height>out->height && in->stride == out->stride);
    NASSERT(hout >= 1);

    stride = stride * sizeof(int16_t);
    w = ((img_coefdn_t*)pCoef)->coef;

    /* Process image by 8 columns */
    for (m = 0; m < ((wout+3) & ~7); m+=8)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        pW   = (const ae_int16   *)(w);

        /* Process by 1x8 samples per iteration */
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            /* Load 4x8 elems from the 8 columns */
            k = up[n];
            pT = (const ae_int16x4 *)((uintptr_t)pIn + k*stride);
            x01 = AE_L16X4_I(pT, sizeof(ae_int16x4));
            AE_L16X4_XP(x00, pT, stride);
            x11 = AE_L16X4_I(pT, sizeof(ae_int16x4));
            AE_L16X4_XP(x10, pT, stride);
            x21 = AE_L16X4_I(pT, sizeof(ae_int16x4));
            AE_L16X4_XP(x20, pT, stride);
            x31 = AE_L16X4_I(pT, sizeof(ae_int16x4));
            AE_L16X4_XP(x30, pT, stride);
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y0, Y1, x00, w0);
            AE_MULAF16X4SS(Y0, Y1, x10, w1);
            AE_MULAF16X4SS(Y0, Y1, x20, w2);
            AE_MULAF16X4SS(Y0, Y1, x30, w3);
            AE_MULF16X4SS (Y2, Y3, x01, w0);
            AE_MULAF16X4SS(Y2, Y3, x11, w1);
            AE_MULAF16X4SS(Y2, Y3, x21, w2);
            AE_MULAF16X4SS(Y2, Y3, x31, w3);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            AE_S16X4_IP(y1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }
        /* Move computed samples from the scratch to the result image */
        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            AE_L16X4_IP(t1, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
            AE_S16X4_I (t1, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(t0, pOut, stride);
        }
    }
    /* Process last 4 columns */
    if (((wout-1) & 4) == 0)
    {
        m = wout & ~7;
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        pW   = (const ae_int16   *)(w);

        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            /* Load 4x4 elems from the 4 columns */
            k = up[n];
            pT = (const ae_int16x4 *)((uintptr_t)pIn + k*stride);
            AE_L16X4_XP(x00, pT, stride);
            AE_L16X4_XP(x10, pT, stride);
            AE_L16X4_XP(x20, pT, stride);
            AE_L16X4_XP(x30, pT, stride);
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y0, Y1, x00, w0);
            AE_MULAF16X4SS(Y0, Y1, x10, w1);
            AE_MULAF16X4SS(Y0, Y1, x20, w2);
            AE_MULAF16X4SS(Y0, Y1, x30, w3);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }
        /* Move computed samples from the scratch to the result image */
        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
            AE_S16X4_XP(t0, pOut, stride);
        }
    }
}

const imgresizer_api_t imgresizer_api_dnxv={NULL,getCoefSz,getCoef,getScratchSize,process};
