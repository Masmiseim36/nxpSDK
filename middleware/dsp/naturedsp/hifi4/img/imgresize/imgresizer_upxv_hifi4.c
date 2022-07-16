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

/*    image resizer, upsample 1...2x in vertical direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    NASSERT(hout<2*hin && hout>hin);
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return img_getCoef_up_alloc(hin,hout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    NASSERT(hout<2*hin && hout>hin);
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    img_getCoef_up_init(coef,hin,hout);
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
    const int16_t * restrict w= ((img_coefup_t*)pCoef)->coef;
    const int16_t * restrict up=((img_coefup_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pT0;
    const ae_int16x4 * restrict pT1;
    const ae_int16   * restrict pW;
    const int16_t    * restrict pIdx;
    ae_int16x4 x000, x001, x010, x011, x100, x101, x110, x111;
    ae_int16x4 w00, w01, w10, w11;
    ae_int32x2 Y00, Y01, Y02, Y03, Y10, Y11, Y12, Y13;
    ae_int16x4 y00, y01, y10, y11, t0;
    int m, n, k0;
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
    w = ((img_coefup_t*)pCoef)->coef;

    /* Process the image by 8 output columns per iteration */
    for (m = 0; m < ((wout+3) & ~7); m+=8)
    {
        /* Move input samples to the scratch */
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=1");
        for (n = 0; n < hin; n++)
        {
            AE_L16X4_IP(y00, pIn, sizeof(ae_int16x4));
            AE_L16X4_XP(y01, pIn, stride-sizeof(ae_int16x4));
            AE_S16X4_IP(y00, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(y01, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t); /* load input samples from scratch */
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        pW   = (const ae_int16   *)(w);
        pIdx = (const int16_t    *)(up);

        __Pragma("loop_count min=1");
        for (n = 0; n < (hout>>1); n++)
        {
            ae_int32x2 idx;
            ae_int16x4 ofs;
            /* Load 4x8 elems for the 8 columns */
            AE_L32_IP(idx, castxcc(ae_int32,pIdx), 2*sizeof(int16_t));
            ofs = AE_SLAI16S(AE_MOVINT16X4_FROMINT32X2(idx), 4); /* offset = idx*sizeof(ae_int16x4)*2 */
            pT0 = (const ae_int16x4 *)((uintptr_t)pIn + AE_MOVAD16_0(ofs));
            pT1 = (const ae_int16x4 *)((uintptr_t)pIn + AE_MOVAD16_1(ofs));

            AE_L16X4_IP(x000, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x010, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x001, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x011, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x100, pT1, sizeof(ae_int16x4));
            AE_L16X4_IP(x110, pT1, sizeof(ae_int16x4));
            AE_L16X4_IP(x101, pT1, sizeof(ae_int16x4));
            AE_L16X4_IP(x111, pT1, sizeof(ae_int16x4));

            /* Load window coefficients */
            AE_L16_IP(w00, pW, sizeof(int16_t));
            AE_L16_IP(w01, pW, sizeof(int16_t));
            AE_L16_IP(w10, pW, sizeof(int16_t));
            AE_L16_IP(w11, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y00, Y01, x000, w00);
            AE_MULAF16X4SS(Y00, Y01, x001, w01);
            AE_MULF16X4SS (Y02, Y03, x010, w00);
            AE_MULAF16X4SS(Y02, Y03, x011, w01);
            AE_MULF16X4SS (Y10, Y11, x100, w10);
            AE_MULAF16X4SS(Y10, Y11, x101, w11);
            AE_MULF16X4SS (Y12, Y13, x110, w10);
            AE_MULAF16X4SS(Y12, Y13, x111, w11);

            y00 = AE_ROUND16X4F32SASYM(Y00, Y01);
            y01 = AE_ROUND16X4F32SASYM(Y02, Y03);
            y10 = AE_ROUND16X4F32SASYM(Y10, Y11);
            y11 = AE_ROUND16X4F32SASYM(Y12, Y13);
            AE_S16X4_IP(y00, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(y01, pOut, stride-sizeof(ae_int16x4));
            AE_S16X4_IP(y10, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(y11, pOut, stride-sizeof(ae_int16x4));
        }
        /* Process last output row*/
        if (hout & 1)
        {
            /* Load 2x8 elems from the 4 columns */
            k0 = *pIdx;
            pT0 = (const ae_int16x4 *)((uintptr_t)pIn + k0*sizeof(ae_int16x4)*2);
            AE_L16X4_IP(x000, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x010, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x001, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x011, pT0, sizeof(ae_int16x4));
            /* Load window coefficients */
            AE_L16_IP(w00, pW, sizeof(int16_t));
            AE_L16_IP(w01, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y00, Y01, x000, w00);
            AE_MULAF16X4SS(Y00, Y01, x001, w01);
            AE_MULF16X4SS (Y02, Y03, x010, w00);
            AE_MULAF16X4SS(Y02, Y03, x011, w01);

            y00 = AE_ROUND16X4F32SASYM(Y00, Y01);
            y01 = AE_ROUND16X4F32SASYM(Y02, Y03);
            AE_S16X4_IP(y00, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(y01, pOut, stride-sizeof(ae_int16x4));
        }
    }
    /* Process last 4 columns */
    if (((wout-1) & 4) == 0)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        __Pragma("loop_count min=2");
        for (n = 0; n < hin; n++)
        {
            AE_L16X4_XP(t0, pIn, stride);
            AE_S16X4_IP(t0, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        pW   = (const ae_int16   *)(w);
        pIdx = (const int16_t    *)(up);

        __Pragma("loop_count min=2");
        for (n = 0; n < hout; n++)
        {
            /* Load 4x4 elems for the 4 columns */
            k0 = *pIdx++;
            pT0 = (const ae_int16x4 *)((uintptr_t)pIn + k0*sizeof(ae_int16x4));
            AE_L16X4_IP(x000, pT0, sizeof(ae_int16x4));
            AE_L16X4_IP(x001, pT0, sizeof(ae_int16x4));

            /* Load window coefficients */
            AE_L16_IP(w00, pW, sizeof(int16_t));
            AE_L16_IP(w01, pW, sizeof(int16_t));

            AE_MULF16X4SS (Y00, Y01, x000, w00);
            AE_MULAF16X4SS(Y00, Y01, x001, w01);

            y00 = AE_ROUND16X4F32SASYM(Y00, Y01);
            AE_S16X4_XP(y00, pOut, stride);
        }
    }
}

const imgresizer_api_t imgresizer_api_upxv={"imgresizer_api_upxv",getCoefSz,getCoef,getScratchSize,process};
