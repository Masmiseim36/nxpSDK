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

/*    image resizer, downsample 2x in horizontal direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return 0;
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    (void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    //return out->width*sizeof(int16_t);// one row
    return 0;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
    static const int16_t ALIGN(ALIGNMENT) coefTbl[] =
    {
        /* [0.125, 0.375, 0.375, 0.125] in Q15 */
        4096, 12288, 12288, 4096,
        /* [0.125, 0.375, 0.5,   0.0] in Q15 */
        4096, 12288, 16384, 0
    };
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    ae_int16x4 x0, x1, x2, x3, t0, t1, t2, y0;
    ae_int16x4 coef, coef_last, coef0, coef1, coef2, coef3;
    ae_int64 A0, A1, A2, A3;
    ae_int32x2 Y0, Y1;
    int m, n, offset_last;
    int h = in->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;
    (void)win;
    (void)pCoef;
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height==out->height &&  win==2*wout && in->stride==out->stride);
    NASSERT(wout >= 2);
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);

    coef = AE_L16X4_I((const ae_int16x4 *)coefTbl, 0);
    coef_last = AE_L16X4_I((const ae_int16x4 *)coefTbl, sizeof(ae_int16x4));
    /* Set the load offset and coefficients for last pixels */
    offset_last = (((wout-1) & 3) > 1) ? 0 : -(int)sizeof(ae_int16x4);
    coef0 = coef; AE_MOVT16X4(coef0, coef_last, AE_LT16((wout-1)&3, 1));
    coef1 = coef; AE_MOVT16X4(coef1, coef_last, AE_LT16((wout-1)&3, 2));
    coef2 = coef; AE_MOVT16X4(coef2, coef_last, AE_LE16((wout-1)&3, 2));
    coef3 = coef_last;

    for (n = 0; n < h; n++)
    {
        pIn =(ae_int16x4 *)((int16_t *)img+n*stride);
        pOut=(ae_int16x4 *)((int16_t *)img+n*stride);
        /* load pixels for the 1st output pixel in special manner */
        AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
        x0 = AE_SEL16_4321(AE_SHORTSWAP(t0), t0);

        /* process other pixels except the last 1...4 pixels */
        for (m = 0; m < (wout-1)/4; m++)
        {
            AE_L16X4_IP(t1, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(t2, pIn, sizeof(ae_int16x4));
            x1 = AE_SEL16_6543(t0, t1);
            x2 = AE_SEL16_4321(t0, t1);
            x3 = AE_SEL16_6543(t1, t2);

            A0 = AE_MULZAAAAQ16(x0, coef);
            A1 = AE_MULZAAAAQ16(x1, coef);
            A2 = AE_MULZAAAAQ16(x2, coef);
            A3 = AE_MULZAAAAQ16(x3, coef);

            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));

            x0 = AE_SEL16_4321(t1, t2);
            t0 = t2;
        }

        /* process last pixels */
        t1 = AE_L16X4_X(pIn, offset_last);

        x1 = AE_SEL16_6543(t0, t1);
        x2 = AE_SEL16_4321(t0, t1);
        x3 = AE_SEL16_6543(t1, t1);

        A0 = AE_MULZAAAAQ16(x0, coef0);
        A1 = AE_MULZAAAAQ16(x1, coef1);
        A2 = AE_MULZAAAAQ16(x2, coef2);
        A3 = AE_MULZAAAAQ16(x3, coef3);

        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
    }
}

const imgresizer_api_t imgresizer_api_dn2xh={NULL,getCoefSz,getCoef,getScratchSize,process};
