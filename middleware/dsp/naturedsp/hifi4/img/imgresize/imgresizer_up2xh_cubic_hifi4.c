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
#include "imgresizer_bicubic_common.h"

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
    return 0;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    static const int16_t ALIGN(ALIGNMENT) coefTbl[] =
    {
        /* Window coefficients */
        /* [-0.0234375, 0.2265625, 0.8671875, -0.0703125] in Q15 */
        -768, 7424, 28416, -2304
    };
    const ae_int16 * restrict pIn0;
          ae_int16 * restrict pOut;
    int m, n;
    int h = in->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;
    ae_int16x4 x0, x1, x2, x3, x4;
    ae_int16x4 y0, y1;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int64 A0, A1, A2, A3, A4, A5, A6, A7;
    ae_int16x4 c0, c1;
    ae_valign alIn0, alOut;

    (void)pCoef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height==out->height && wout==2*win && in->stride==out->stride);

    c0 = AE_L16X4_I((const ae_int16x4 *)coefTbl, 0);
    c1 = AE_SHORTSWAP(c0);

    /* Process the image by 1 row per iteration */
    /* Process samples in reverse order */
    for (n = 0; n < h; n++)
    {
        pIn0 = (const ae_int16 *)img + n*stride + win-1;
        pOut = (      ae_int16 *)img + n*stride + wout-1;
        AE_L16_IP(x0, pIn0, -(int)sizeof(int16_t));
        AE_L16_IP(x1, pIn0, -(int)sizeof(int16_t));
        x0 = AE_SEL16_6543(x0, x1);
        /* Compute the 1st sample in special manner */
        A0 = AE_MULZAAAAQ16(x0, c0);
        Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
        AE_S16_0_IP(y0, pOut, -(int)sizeof(int16_t));

        for (m = 0; m < ((win-2)&3); m++)
        {
            AE_L16_IP(x1, pIn0, -(int)sizeof(int16_t));
            x0 = AE_SEL16_6543(x0, x1);

            A0 = AE_MULZAAAAQ16(x0, c1);
            A1 = AE_MULZAAAAQ16(x0, c0);

            Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
            Y1 = AE_TRUNCA32X2F64S(A1, A1, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
            y1 = AE_ROUND16X4F32SASYM(Y1, Y1);
            AE_S16_0_IP(y0, pOut, -(int)sizeof(int16_t));
            AE_S16_0_IP(y1, pOut, -(int)sizeof(int16_t));
        }

        alIn0 = AE_LA64_PP(pIn0);
        alOut = AE_ZALIGN64();
        /* Main loop: process by 8 output samples per iteration */
        for (m = 0; m < ((win-2)/4); m++)
        {
            AE_LA16X4_RIP(x4, alIn0, castxcc(ae_int16x4,pIn0));
            x1 = AE_SEL16_6543(x0, x4);
            x2 = AE_SEL16_5432(x0, x4);
            x3 = AE_SEL16_4321(x0, x4);

            A0 = AE_MULZAAAAQ16(x1, c1);
            A1 = AE_MULZAAAAQ16(x1, c0);
            A2 = AE_MULZAAAAQ16(x2, c1);
            A3 = AE_MULZAAAAQ16(x2, c0);
            A4 = AE_MULZAAAAQ16(x3, c1);
            A5 = AE_MULZAAAAQ16(x3, c0);
            A6 = AE_MULZAAAAQ16(x4, c1);
            A7 = AE_MULZAAAAQ16(x4, c0);

            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            Y2 = AE_TRUNCA32X2F64S(A4, A5, 33);
            Y3 = AE_TRUNCA32X2F64S(A6, A7, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            AE_SA16X4_RIP(y0, alOut, castxcc(ae_int16x4,pOut));
            AE_SA16X4_RIP(y1, alOut, castxcc(ae_int16x4,pOut));

            x0 = x4;
        }
        AE_SA64NEG_FP(alOut, pOut);

        /* Compute last 3 samples */
        x0 = AE_SEL16_6543(x0, AE_SHORTSWAP(x0));
        A0 = AE_MULZAAAAQ16(x0, c1);
        A1 = AE_MULZAAAAQ16(x0, c0);
        Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
        Y1 = AE_TRUNCA32X2F64S(A1, A1, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
        y1 = AE_ROUND16X4F32SASYM(Y1, Y1);
        AE_S16_0_IP(y0, pOut, -(int)sizeof(int16_t));
        AE_S16_0_IP(y1, pOut, -(int)sizeof(int16_t));

        x0 = AE_SEL16_6543(x0, AE_SHORTSWAP(x0));
        A0 = AE_MULZAAAAQ16(x0, c1);
        Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
        AE_S16_0_IP(y0, pOut, -(int)sizeof(int16_t));
    }
}

const imgresizer_api_t imgresizer_api_up2xh_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
