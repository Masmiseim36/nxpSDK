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
    static const int16_t ALIGN(ALIGNMENT) coefTbl[] =
    {
        /* Half of the window coefficients; the second half is symmetric to the first one */
        /* [-0.0234375, 0.2265625, 0.8671875, -0.0703125] in Q15 */
        -768, 7424, 28416, -2304
    };
    int m, n;
    const ae_int16   * restrict pIn0;
    const ae_int16   * restrict pIn1;
    const ae_int16   * restrict pIn2;
    const ae_int16   * restrict pIn3;
          ae_int16x4 * restrict pOut;
    int w = in->width,
        hin = in->height,
        hout = out->height,
        stride = out->stride;
    ae_int16x4 x0, x1, x2, x3;
    ae_int16x4 t0, t1, t2, t3;
    ae_int16x4 w0, w1;
    ae_int64 A0, A1, A2, A3, A4, A5, A6, A7;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 y0, y1;

    (void)pCoef;
    imgsize_validate(in, 2, 1);
    imgsize_validate(out, 2, 1);
    NASSERT_ALIGN(img, ALIGNMENT);
    NASSERT_ALIGN(pScr, ALIGNMENT);
    NASSERT(in->width == out->width &&  hout == 2 * hin && in->stride == out->stride);

    w0 = AE_L16X4_I((const ae_int16x4 *)coefTbl, 0);
    w1 = AE_SHORTSWAP(w0);
    stride = stride*sizeof(int16_t);

    /* Process the image by 4 columns per iteration */
    /* Process samples in reverse order */
    for (m = 0; m < w; m+=4)
    {
        pIn0 = (const ae_int16   *)((uintptr_t)img + m*sizeof(int16_t) + (hin - 1)*stride + 0*sizeof(int16_t));
        pIn1 = (const ae_int16   *)((uintptr_t)img + m*sizeof(int16_t) + (hin - 1)*stride + 1*sizeof(int16_t));
        pIn2 = (const ae_int16   *)((uintptr_t)img + m*sizeof(int16_t) + (hin - 1)*stride + 2*sizeof(int16_t));
        pIn3 = (const ae_int16   *)((uintptr_t)img + m*sizeof(int16_t) + (hin - 1)*stride + 3*sizeof(int16_t));
        pOut = (      ae_int16x4 *)((uintptr_t)img + m*sizeof(int16_t) + (hout - 1)*stride);

        AE_L16_XP(x0, pIn0, -stride);
        AE_L16_XP(x1, pIn1, -stride);
        AE_L16_XP(x2, pIn2, -stride);
        AE_L16_XP(x3, pIn3, -stride);
        AE_L16_XP(t0, pIn0, -stride);
        AE_L16_XP(t1, pIn1, -stride);
        AE_L16_XP(t2, pIn2, -stride);
        AE_L16_XP(t3, pIn3, -stride);

        x0 = AE_SEL16_6543(x0, t0);
        x1 = AE_SEL16_6543(x1, t1);
        x2 = AE_SEL16_6543(x2, t2);
        x3 = AE_SEL16_6543(x3, t3);

        A0 = AE_MULZAAAAQ16(x0, w0);
        A1 = AE_MULZAAAAQ16(x1, w0);
        A2 = AE_MULZAAAAQ16(x2, w0);
        A3 = AE_MULZAAAAQ16(x3, w0);

        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_XP(y0, pOut, -stride);

        /* Process by 2x4 output samples per iteration */
        for (n = 0; n < hin - 2; n++)
        {
            AE_L16_XP(t0, pIn0, -stride);
            AE_L16_XP(t1, pIn1, -stride);
            AE_L16_XP(t2, pIn2, -stride);
            AE_L16_XP(t3, pIn3, -stride);

            x0 = AE_SEL16_6543(x0, t0);
            x1 = AE_SEL16_6543(x1, t1);
            x2 = AE_SEL16_6543(x2, t2);
            x3 = AE_SEL16_6543(x3, t3);

            A0 = AE_MULZAAAAQ16(x0, w1);
            A1 = AE_MULZAAAAQ16(x1, w1);
            A2 = AE_MULZAAAAQ16(x2, w1);
            A3 = AE_MULZAAAAQ16(x3, w1);
            A4 = AE_MULZAAAAQ16(x0, w0);
            A5 = AE_MULZAAAAQ16(x1, w0);
            A6 = AE_MULZAAAAQ16(x2, w0);
            A7 = AE_MULZAAAAQ16(x3, w0);

            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            Y2 = AE_TRUNCA32X2F64S(A4, A5, 33);
            Y3 = AE_TRUNCA32X2F64S(A6, A7, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            AE_S16X4_XP(y0, pOut, -stride);
            AE_S16X4_XP(y1, pOut, -stride);
        }

        x0 = AE_SEL16_6543(x0, t0);
        x1 = AE_SEL16_6543(x1, t1);
        x2 = AE_SEL16_6543(x2, t2);
        x3 = AE_SEL16_6543(x3, t3);

        A0 = AE_MULZAAAAQ16(x0, w1);
        A1 = AE_MULZAAAAQ16(x1, w1);
        A2 = AE_MULZAAAAQ16(x2, w1);
        A3 = AE_MULZAAAAQ16(x3, w1);
        A4 = AE_MULZAAAAQ16(x0, w0);
        A5 = AE_MULZAAAAQ16(x1, w0);
        A6 = AE_MULZAAAAQ16(x2, w0);
        A7 = AE_MULZAAAAQ16(x3, w0);

        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
        Y2 = AE_TRUNCA32X2F64S(A4, A5, 33);
        Y3 = AE_TRUNCA32X2F64S(A6, A7, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
        AE_S16X4_XP(y0, pOut, -stride);
        AE_S16X4_XP(y1, pOut, -stride);


        x0 = AE_SEL16_6543(x0, t0);
        x1 = AE_SEL16_6543(x1, t1);
        x2 = AE_SEL16_6543(x2, t2);
        x3 = AE_SEL16_6543(x3, t3);

        A0 = AE_MULZAAAAQ16(x0, w1);
        A1 = AE_MULZAAAAQ16(x1, w1);
        A2 = AE_MULZAAAAQ16(x2, w1);
        A3 = AE_MULZAAAAQ16(x3, w1);

        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_XP(y0, pOut, -stride);
    }
}

const imgresizer_api_t imgresizer_api_up2xv_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
