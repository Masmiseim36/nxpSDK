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

/*    image resizer, upsample 1...2x in vertical direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    NASSERT(hout<2*hin && hout>hin);
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return img_getCoef_up_cubic_alloc(hin,hout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    NASSERT(hout<2*hin && hout>hin);
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    img_getCoef_up_cubic_init(coef,hin,hout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    /* 8 columns */
    return in->height*sizeof(int16_t)*8;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    int16_t * restrict t=(int16_t *)pScr;
    const int16_t * restrict w= ((img_coefup_cubic_t*)pCoef)->coef;
    const int16_t * restrict up=((img_coefup_cubic_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pT0;
    const ae_int16   * restrict pW;
    const int16_t    * restrict pIdx;
    ae_int16x4 x00, x01, x02, x03, x10, x11, x12, x13;
    ae_int16x4 w0, w1, w2, w3;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 y0, y1, t0;
    int16_t k;
    int m, n;
    int wout = out->width,
        hin = in->height,
        hout = out->height,
        stride = out->stride;

    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->width==out->width &&  hout<2*hin && hout>hin && in->stride==out->stride);
    NASSERT(hout > 2);

    stride = stride * sizeof(int16_t);

    /* Process the image by 8 output columns per iteration */
    for (m = 0; m < ((wout+3) & ~7); m+=8)
    {
        /* Move input samples to the scratch */
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (n = 0; n < hin; n++)
        {
            y1 = AE_L16X4_I(pIn, sizeof(ae_int16x4));
            AE_L16X4_XP(y0, pIn, stride);
            AE_S16X4_IP(y1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }
        __Pragma("no_reorder");

        pIn  = (const ae_int16x4 *)(t); /* load input samples from scratch */
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        pW   = (const ae_int16   *)(w);
        pIdx = up;

        __Pragma("loop_count min=2");
        for (n = 0; n < hout; n++)
        {
            /* Load 2x8 elems from the 4 columns */
            k = *pIdx++;
            pT0 = (const ae_int16x4 *)((uintptr_t)pIn + k*2*sizeof(ae_int16x4));
            AE_L16X4_IP(x10, pT0, sizeof(ae_int16x4));  AE_L16X4_IP(x00, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x11, pT0, sizeof(ae_int16x4));  AE_L16X4_IP(x01, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x12, pT0, sizeof(ae_int16x4));  AE_L16X4_IP(x02, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x13, pT0, sizeof(ae_int16x4));  AE_L16X4_IP(x03, pT0, sizeof(ae_int16x4));
            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));

            AE_MUL16X4 (Y0, Y1, x00, w0);
            AE_MULA16X4(Y0, Y1, x01, w1);
            AE_MULA16X4(Y0, Y1, x02, w2);
            AE_MULA16X4(Y0, Y1, x03, w3);
            AE_MUL16X4 (Y2, Y3, x10, w0);
            AE_MULA16X4(Y2, Y3, x11, w1);
            AE_MULA16X4(Y2, Y3, x12, w2);
            AE_MULA16X4(Y2, Y3, x13, w3);

            Y0 = AE_SRAI32R(Y0, 14);
            Y1 = AE_SRAI32R(Y1, 14);
            Y2 = AE_SRAI32R(Y2, 14);
            Y3 = AE_SRAI32R(Y3, 14);
            y0 = AE_SAT16X4(Y0, Y1);
            y1 = AE_SAT16X4(Y2, Y3);
            AE_S16X4_I (y1, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(y0, pOut, stride);
        }
    }
    /* Process last 4 columns */
    if (((wout-1) & 4) == 0)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (n = 0; n < hin; n++)
        {
            AE_L16X4_XP(t0, pIn, stride);
            AE_S16X4_IP(t0, pOut, sizeof(ae_int16x4));
        }
        __Pragma("no_reorder");

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        pW   = (const ae_int16   *)(w);
        pIdx = up;

        __Pragma("loop_count min=2");
        for (n = 0; n < hout; n++)
        {
            /* Load 4x4 elems for the 4 columns */
            k = *pIdx++;
            pT0 = (const ae_int16x4 *)((uintptr_t)pIn + k*sizeof(ae_int16x4));
            AE_L16X4_IP(x00, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x01, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x02, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x03, pT0, sizeof(ae_int16x4));

            /* Load window coefficients */
            AE_L16_IP(w0, pW, sizeof(int16_t));
            AE_L16_IP(w1, pW, sizeof(int16_t));
            AE_L16_IP(w2, pW, sizeof(int16_t));
            AE_L16_IP(w3, pW, sizeof(int16_t));

            AE_MUL16X4 (Y0, Y1, x00, w0);
            AE_MULA16X4(Y0, Y1, x01, w1);
            AE_MULA16X4(Y0, Y1, x02, w2);
            AE_MULA16X4(Y0, Y1, x03, w3);

            Y0 = AE_SRAI32R(Y0, 14);
            Y1 = AE_SRAI32R(Y1, 14);
            y0 = AE_SAT16X4(Y0, Y1);
            AE_S16X4_XP(y0, pOut, stride);
        }
    }
}

const imgresizer_api_t imgresizer_api_upxv_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
