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
    static const int16_t ALIGN(ALIGNMENT) coefTbl[] =
    {
        /* Half of the window coefficients; the second half is symmetric to the first one */
        /* [-0.0117188, -0.0351563, 0.1132813, 0.4335938] in Q15 */
        -384, -1152, 3712, 14208
    };
    const ae_int16x4 *restrict pIn;
          ae_int16x4 *restrict pOut;
    ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7;
    ae_int16x4 t0, t1, t2, t3, t4, t5, t6, t7;
    ae_int16x4 u0, u1;
    ae_int16x4 cf0, cf1;
    ae_int64 A0, A1, A2, A3;
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
    cf0 = AE_L16X4_I((const ae_int16x4 *)coefTbl, 0);
    cf1 = AE_MOVINT16X4_FROMINT64(AE_L64_I((const ae_int64 *)coefTbl, 0));
    stride = stride * sizeof(int16_t);

    pOut = (      ae_int16x4 *)img;
    pIn  = (const ae_int16x4 *)img;
    /* Process image by 4 columns per iteration */
    for (m=0; m<w; m+=4)
    {
        AE_L16X4_XP(x5, pIn, stride);
        x2 = x3 = x4 = x5;
        AE_L16X4_XP(x6, pIn, stride);
        AE_L16X4_XP(x7, pIn, stride);

        t0 = AE_SEL16_7531(x0, x1);
        t1 = AE_SEL16_6420(x0, x1);
        t2 = AE_SEL16_7531(x2, x3);
        t3 = AE_SEL16_6420(x2, x3);
        t4 = AE_SEL16_7531(x4, x5);
        t5 = AE_SEL16_6420(x4, x5);
        t6 = AE_SEL16_7531(x6, x7);
        t7 = AE_SEL16_6420(x6, x7);

        x0 = AE_SEL16_7531(t0, t2);
        x1 = AE_SEL16_7531(t1, t3);
        x2 = AE_SEL16_6420(t0, t2);
        x3 = AE_SEL16_6420(t1, t3);
        x4 = AE_SEL16_7531(t4, t6);
        x5 = AE_SEL16_7531(t5, t7);
        x6 = AE_SEL16_6420(t4, t6);
        x7 = AE_SEL16_6420(t5, t7);

        /* Process by 1x4 samples per iteration */
        for (n=0; n<hout-2; n++)
        {
            AE_L16X4_XP(t0, pIn, stride);
            AE_L16X4_XP(t1, pIn, stride);
            u0 = AE_SEL16_7362(t0, t1);
            u1 = AE_SEL16_5140(t0, t1);

            x0 = AE_SEL16_5432(x0, x4);
            x1 = AE_SEL16_5432(x1, x5);
            x2 = AE_SEL16_5432(x2, x6);
            x3 = AE_SEL16_5432(x3, x7);
            x4 = AE_SEL16_5432(x4, u0);
            x5 = AE_SEL16_5410(x5, u0);
            x6 = AE_SEL16_5432(x6, u1);
            x7 = AE_SEL16_5410(x7, u1);

            A0 = AE_MULZAAAAQ16(x0, cf0);
            A1 = AE_MULZAAAAQ16(x1, cf0);
            A2 = AE_MULZAAAAQ16(x2, cf0);
            A3 = AE_MULZAAAAQ16(x3, cf0);
            AE_MULAAAAQ16(A0, x4, cf1);
            AE_MULAAAAQ16(A1, x5, cf1);
            AE_MULAAAAQ16(A2, x6, cf1);
            AE_MULAAAAQ16(A3, x7, cf1);

            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_XP(y0, pOut, stride);
        }

        AE_L16X4_XP(t1, pIn, (1-hout*2)*stride + (int)sizeof(ae_int16x4));
        t0 = t1;
        u0 = AE_SEL16_7362(t0, t1);
        u1 = AE_SEL16_5140(t0, t1);

        x0 = AE_SEL16_5432(x0, x4);
        x1 = AE_SEL16_5432(x1, x5);
        x2 = AE_SEL16_5432(x2, x6);
        x3 = AE_SEL16_5432(x3, x7);
        x4 = AE_SEL16_5432(x4, u0);
        x5 = AE_SEL16_5410(x5, u0);
        x6 = AE_SEL16_5432(x6, u1);
        x7 = AE_SEL16_5410(x7, u1);

        A0 = AE_MULZAAAAQ16(x0, cf0);
        A1 = AE_MULZAAAAQ16(x1, cf0);
        A2 = AE_MULZAAAAQ16(x2, cf0);
        A3 = AE_MULZAAAAQ16(x3, cf0);
        AE_MULAAAAQ16(A0, x4, cf1);
        AE_MULAAAAQ16(A1, x5, cf1);
        AE_MULAAAAQ16(A2, x6, cf1);
        AE_MULAAAAQ16(A3, x7, cf1);
        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);

        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_XP(y0, pOut, stride);

        x0 = AE_SEL16_5432(x0, x4);
        x1 = AE_SEL16_5432(x1, x5);
        x2 = AE_SEL16_5432(x2, x6);
        x3 = AE_SEL16_5432(x3, x7);
        x4 = AE_SEL16_5432(x4, u0);
        x5 = AE_SEL16_5410(x5, u0);
        x6 = AE_SEL16_5432(x6, u1);
        x7 = AE_SEL16_5410(x7, u1);

        A0 = AE_MULZAAAAQ16(x0, cf0);
        A1 = AE_MULZAAAAQ16(x1, cf0);
        A2 = AE_MULZAAAAQ16(x2, cf0);
        A3 = AE_MULZAAAAQ16(x3, cf0);
        AE_MULAAAAQ16(A0, x4, cf1);
        AE_MULAAAAQ16(A1, x5, cf1);
        AE_MULAAAAQ16(A2, x6, cf1);
        AE_MULAAAAQ16(A3, x7, cf1);

        A0 = AE_MULZAAAAQ16(x0, cf0);
        A1 = AE_MULZAAAAQ16(x1, cf0);
        A2 = AE_MULZAAAAQ16(x2, cf0);
        A3 = AE_MULZAAAAQ16(x3, cf0);
        AE_MULAAAAQ16(A0, x4, cf1);
        AE_MULAAAAQ16(A1, x5, cf1);
        AE_MULAAAAQ16(A2, x6, cf1);
        AE_MULAAAAQ16(A3, x7, cf1);
        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);

        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_XP(y0, pOut, (1-hout)*stride + (int)sizeof(ae_int16x4));
    }
}

const imgresizer_api_t imgresizer_api_dn2xv_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
