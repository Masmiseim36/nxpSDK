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

/*    image resizer, downsample 2x in horizontal direction */

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
        -384, -1152, 3712, 14208,
        /* The second half of the coefficients for the penultimate sample */
        /* [0.4335938, 0.1132813, -0.0468751, 0] in Q15 */
        14208, 3712, -1536, 0,
        /* The second half of the coefficients for the last sample */
        /* [0.5, 0, 0, 0] in Q15 */
        16384, 0, 0, 0
    };
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    ae_int16x4 x00, x01, x10, x11, x20, x21, x30, x31;
    ae_int16x4 t0, t1, t2, y0;
    ae_int16x4 cf0, cf1, coef_pnlt, coef_last, coef0, coef1, coef2, coef3, coef4;
    ae_int64 A0, A1, A2, A3;
    ae_int32x2 Y0, Y1;
    int m, n, offset_last0, offset_last1;
    int h = in->height,
        win = in->width,
        wout = out->width,
        stride = out->stride;
    (void)win;
    (void)pCoef;
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height==out->height &&  win==2*wout && in->stride==out->stride);
    NASSERT(wout > 2);
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);

    cf0 = AE_L16X4_I((const ae_int16x4 *)coefTbl, 0);
    cf1 = AE_MOVINT16X4_FROMINT64(AE_L64_I((const ae_int64 *)coefTbl, 0));
    /* Set the load offset and coefficients for last pixels */
    offset_last0 = (((wout-2) & 3) > 0) ? 0 : -(int)sizeof(ae_int16x4);
    offset_last1 = (((wout-2) & 3) > 2) ? (int)sizeof(ae_int16x4) : -(int)sizeof(ae_int16x4);
    coef_pnlt = AE_L16X4_I((const ae_int16x4 *)coefTbl, sizeof(ae_int16x4));
    coef_last = AE_L16X4_I((const ae_int16x4 *)coefTbl, sizeof(ae_int16x4)*2);
    coef0 = cf1;
    coef1 = cf1;
    coef2 = cf1;
    coef3 = cf1;
    AE_MOVT16X4(coef0, coef_pnlt, AE_EQ16((wout-2)&3, 0));
    AE_MOVT16X4(coef1, coef_pnlt, AE_EQ16((wout-2)&3, 1));
    AE_MOVT16X4(coef2, coef_pnlt, AE_EQ16((wout-2)&3, 2));
    AE_MOVT16X4(coef3, coef_pnlt, AE_EQ16((wout-2)&3, 3));

    AE_MOVT16X4(coef1, coef_last, AE_EQ16((wout-2)&3, 0));
    AE_MOVT16X4(coef2, coef_last, AE_EQ16((wout-2)&3, 1));
    AE_MOVT16X4(coef3, coef_last, AE_EQ16((wout-2)&3, 2));
    coef4 = coef_last;

    /* process image by 1 row per iteration */
    for (n = 0; n < h; n++)
    {
        pIn  = (ae_int16x4 *)((int16_t *)img + n*stride);
        pOut = (ae_int16x4 *)((int16_t *)img + n*stride);
        /* load pixels for the 1st two output pixels in special manner */
        x21 = AE_L16_I((const ae_int16 *)pIn, 0);
        AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
        AE_L16X4_IP(t1, pIn, 0);
        x31 = AE_SEL16_5432(x21, AE_SEL16_6543(t0, t1));

        /* process other pixels except the last 2...5 pixels */
        for (m = 0; m < (wout-2)/4; m++)
        {
            AE_L16X4_IP(t1, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(t2, pIn, sizeof(ae_int16x4));
            x00 = x21;
            x01 = AE_SEL16_6543(t0, t1);
            x10 = x31;
            x11 = AE_SEL16_4321(t0, t1);
            x20 = x01;
            x21 = AE_SEL16_6543(t1, t2);
            x30 = x11;
            x31 = AE_SEL16_4321(t1, t2);

            A0 = AE_MULZAAAAQ16(x00, cf0);
            A1 = AE_MULZAAAAQ16(x10, cf0);
            A2 = AE_MULZAAAAQ16(x20, cf0);
            A3 = AE_MULZAAAAQ16(x30, cf0);
            AE_MULAAAAQ16(A0, x01, cf1);
            AE_MULAAAAQ16(A1, x11, cf1);
            AE_MULAAAAQ16(A2, x21, cf1);
            AE_MULAAAAQ16(A3, x31, cf1);

            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));

            t0 = t2;
        }

        /* process last 2...5 pixels */
        t1 = AE_L16X4_X(pIn, offset_last0);
        t2 = AE_L16X4_X(pIn, offset_last1);
        x00 = x21;
        x01 = AE_SEL16_6543(t0, t1);
        x10 = x31;
        x11 = AE_SEL16_4321(t0, t1);
        x20 = x01;
        x21 = AE_SEL16_6543(t1, t2);
        x30 = x11;
        x31 = AE_SEL16_4321(t1, t2);

        A0 = AE_MULZAAAAQ16(x00, cf0);
        A1 = AE_MULZAAAAQ16(x10, cf0);
        A2 = AE_MULZAAAAQ16(x20, cf0);
        A3 = AE_MULZAAAAQ16(x30, cf0);
        AE_MULAAAAQ16(A0, x01, coef0);
        AE_MULAAAAQ16(A1, x11, coef1);
        AE_MULAAAAQ16(A2, x21, coef2);
        AE_MULAAAAQ16(A3, x31, coef3);

        Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
        Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
        AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));

        x00 = x21;
        x01 = AE_SEL16_6543(t2, t2);

        A0 = AE_MULZAAAAQ16(x00, cf0);
        AE_MULAAAAQ16(A0, x01, coef4);

        Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
        y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
        AE_S16_0_I(y0, (ae_int16 *)pOut, 0);
    }
}

const imgresizer_api_t imgresizer_api_dn2xh_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
