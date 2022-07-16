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

/*    image resizer, upsample 2x in vertical direction */

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
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    ae_int16x4 s0, s1, w0, w1;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 y0, y1;
    int m, n;
    int w = in->width,
        hin = in->height,
        hout = out->height,
        stride = out->stride;

    (void)pCoef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->width==out->width &&  hout==2*hin && in->stride==out->stride);
    NASSERT(hin >= 2);

    stride = stride * sizeof(int16_t);
    /* Set window coefficients */
    w0 = 24576; /* 0.75, Q15 */
    w1 = 8192;  /* 0.25, Q15 */

    /* Process the image by 4 columns per iteration */
    /* Process samples in reverse order */
    for (m = 0; m < w; m+=4)
    {
        pOut = (      ae_int16x4 *)((uintptr_t)img + (hout - 1)*stride + m*sizeof(int16_t));
        pIn  = (const ae_int16x4 *)((uintptr_t)img + (hin  - 1)*stride + m*sizeof(int16_t));

        AE_L16X4_XP(s1, pIn, -stride);
        AE_S16X4_XP(s1, pOut, -stride);

        /* Process by 2x4 output samples per iteration */
        __Pragma("loop_count min=1");
        for (n = 0; n < hin-1; n++)
        {
            s0 = s1;
            AE_L16X4_XP(s1, pIn, -stride);

            AE_MULF16X4SS (Y0, Y1, s0, w0);
            AE_MULAF16X4SS(Y0, Y1, s1, w1);
            AE_MULF16X4SS (Y2, Y3, s0, w1);
            AE_MULAF16X4SS(Y2, Y3, s1, w0);

            /* use here truncation instead of rounding operation
               for better loop scheduling */
            //y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            //y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            y0 = AE_TRUNC16X4F32(Y0, Y1);
            y1 = AE_TRUNC16X4F32(Y2, Y3);

            AE_S16X4_XP(y0, pOut, -stride);
            AE_S16X4_XP(y1, pOut, -stride);
        }
    }
}

const imgresizer_api_t imgresizer_api_up2xv={NULL,getCoefSz,getCoef,getScratchSize,process};
