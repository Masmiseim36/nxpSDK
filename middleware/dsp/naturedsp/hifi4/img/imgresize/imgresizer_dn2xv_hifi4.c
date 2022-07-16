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
#include "imgresizer_bilinear_common.h"

/*    image resizer, downsample 2x in vertical direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return 0;
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);

}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return 0;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    const ae_int16x4 *restrict pIn;
          ae_int16x4 *restrict pOut;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 coef0, coef1;
    ae_int32x2 Y0, Y1;
    ae_int16x4 y0;
    int m, n;
    int w = in->width,
        hin = in->height,
        hout = out->height,
        stride = out->stride;
    (void)hin;
    (void)pCoef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(w==out->width &&  hin==2*hout && stride==out->stride);
    NASSERT(hout >= 2);

    /* Set window coefficients */
    coef0 = 4096;  /* 0.125, Q15 */
    coef1 = 12288; /* 0.375, Q15 */
    stride = stride * sizeof(int16_t);

    pOut = (      ae_int16x4 *)img;
    pIn  = (const ae_int16x4 *)img;
    /* Process image by 4 columns per iteration */
    for (m=0; m<w; m+=4)
    {
        AE_L16X4_XP(x0, pIn, stride);
        x1 = x0;

        /* Process by 1x4 samples per iteration */
        __Pragma("loop_count min=1");
        for (n=0; n<hout-1; n++)
        {
            AE_L16X4_XP(x2, pIn, stride);
            AE_L16X4_XP(x3, pIn, stride);

            AE_MULF16X4SS (Y0, Y1, x0, coef0);
            AE_MULAF16X4SS(Y0, Y1, x1, coef1);
            AE_MULAF16X4SS(Y0, Y1, x2, coef1);
            AE_MULAF16X4SS(Y0, Y1, x3, coef0);

            /* use here truncation instead of rounding operation
               for better loop scheduling */
            //y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y0 = AE_TRUNC16X4F32(Y0, Y1);
            AE_S16X4_XP(y0, pOut, stride);

            x0 = x2;
            x1 = x3;
        }
        /* Process last 1x4 samples */
        AE_L16X4_XP(x2, pIn, (1-hout*2)*stride + (int)sizeof(ae_int16x4));
        x3 = x2;

        AE_MULF16X4SS (Y0, Y1, x0, coef0);
        AE_MULAF16X4SS(Y0, Y1, x1, coef1);
        AE_MULAF16X4SS(Y0, Y1, x2, coef1);
        AE_MULAF16X4SS(Y0, Y1, x3, coef0);

        //y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        y0 = AE_TRUNC16X4F32(Y0, Y1);
        AE_S16X4_XP(y0, pOut, (1-hout)*stride + (int)sizeof(ae_int16x4));
    }
}

const imgresizer_api_t imgresizer_api_dn2xv={NULL,getCoefSz,getCoef,getScratchSize,process};
