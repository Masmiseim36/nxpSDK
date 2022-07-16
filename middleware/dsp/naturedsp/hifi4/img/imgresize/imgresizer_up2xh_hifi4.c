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

/*    image resizer, upsample 2x in horizontal direction */

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
    //return out->width*sizeof(int16_t);  // one row
    return 0;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    const ae_int16 * restrict pIn1;
    const ae_int16 * restrict pIn0;
          ae_int16 * restrict pOut;
    int m, n;
    int h = in->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;
    ae_int16x4 x0, x1;
    ae_int16x4 t0, t1, y0, y1;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 c0, c1;
    ae_valign alIn0, alIn1, alOut;

    (void)pCoef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height==out->height && wout==2*win && in->stride==out->stride);
    NASSERT(win >= 4);

    /* Set window coefficients */
    c0 = 24576; /* 0.75, Q15 */
    c1 = 8192;  /* 0.25, Q15 */

    /* Process the image by 1 row per iteration */
    /* Process samples in reverse order */
    for (n = 0; n < h; n++)
    {
        pIn0 = (const ae_int16 *)img + n*stride + win-1;
        pOut = (      ae_int16 *)img + n*stride + wout-1;
        AE_L16_IP(x1, pIn0, -(int)sizeof(int16_t));
        AE_S16_0_IP(x1, pOut, -(int)sizeof(int16_t));
        __Pragma("no_unroll");
        for (m = 0; m < ((win-1)&3); m++)
        {
            x0 = x1;
            AE_L16_IP(x1, pIn0, -(int)sizeof(int16_t));

            Y0 = AE_MULF16SS_00(x0, c0);
            AE_MULAF16SS_00(Y0, x1, c1);
            Y1 = AE_MULF16SS_00(x0, c1);
            AE_MULAF16SS_00(Y1, x1, c0);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
            y1 = AE_ROUND16X4F32SASYM(Y1, Y1);
            AE_S16_0_IP(y0, pOut, -(int)sizeof(int16_t));
            AE_S16_0_IP(y1, pOut, -(int)sizeof(int16_t));
        }

        pIn1 = pIn0 + 1;
        alIn0 = AE_LA64_PP(pIn0);
        alIn1 = AE_LA64_PP(pIn1);
        alOut = AE_ZALIGN64();
        /* Main loop: process by 8 output samples per iteration */
        for (m = 0; m < ((win-1)/4); m++)
        {
            AE_LA16X4_RIP(x1, alIn1, castxcc(ae_int16x4,pIn1));
            AE_LA16X4_RIP(x0, alIn0, castxcc(ae_int16x4,pIn0));

            AE_MULF16X4SS (Y0, Y1, x0, c1);
            AE_MULAF16X4SS(Y0, Y1, x1, c0);
            AE_MULF16X4SS (Y2, Y3, x0, c0);
            AE_MULAF16X4SS(Y2, Y3, x1, c1);

            t0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            t1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            y0 = AE_SEL16_7362(t0, t1);
            y1 = AE_SEL16_5140(t0, t1);

            AE_SA16X4_RIP(y0, alOut, castxcc(ae_int16x4,pOut));
            AE_SA16X4_RIP(y1, alOut, castxcc(ae_int16x4,pOut));
        }
        AE_SA64NEG_FP(alOut, pOut);
    }
}

const imgresizer_api_t imgresizer_api_up2xh={NULL,getCoefSz,getCoef,getScratchSize,process};
