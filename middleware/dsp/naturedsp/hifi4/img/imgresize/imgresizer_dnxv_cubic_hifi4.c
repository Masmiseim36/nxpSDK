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
#include "img_getCoef_dn_cubic.h"
#include "imgresizer_bicubic_common.h"

/*    image resizer, downsample 1...2x in vertical direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT(in->width==out->width &&  hin<2*hout && hin>hout && in->stride==out->stride);
    return img_getCoef_dn_cubic_alloc(hin,hout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int hin=in->height,hout=out->height;
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT(in->width==out->width &&  hin<2*hout && hin>hout && in->stride==out->stride);
    img_getCoef_dn_cubic_init(coef,hin,hout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    /* 4 columns */
    return out->height*sizeof(int16_t)*4;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
#if 1
    int16_t * restrict t=(int16_t *)pScr;
    const int16_t * restrict w= ((img_coefdn_cubic_t*)pCoef)->coef;
    const int16_t * restrict up=((img_coefdn_cubic_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pT;
    const ae_int16x4 * restrict pW;
    ae_int16x4 x0, x1, x2, x3, x4, x5, x6, x7;
    ae_int16x4 w0, w1;
    ae_int32x2 Y0, Y1;
    ae_int16x4 y0, t0, t1, t2, t3, t4, t5, t6, t7;
    ae_int64 A0, A1, A2, A3;
    int m, n;
    int wout = out->width,
        hout = out->height,
        stride = out->stride;

    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->width == wout && in->height<2*out->height && in->height>out->height && in->stride == out->stride);
    NASSERT(hout >= 1);

    stride = stride * sizeof(int16_t);

    for (m = 0; m < wout; m+=4)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        pW   = (const ae_int16x4 *)(w);

        /* Process by 1x4 samples per iteration */
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            int16_t ix;
            /* Load 8x4 elems from the 4 columns */
            ix = up[n];
            pT = (const ae_int16x4 *)((uintptr_t)pIn + stride*ix);
            AE_L16X4_XP(x0, pT, stride);
            AE_L16X4_XP(x1, pT, stride);
            AE_L16X4_XP(x2, pT, stride);
            AE_L16X4_XP(x3, pT, stride);
            AE_L16X4_XP(x4, pT, stride);
            AE_L16X4_XP(x5, pT, stride);
            AE_L16X4_XP(x6, pT, stride);
            AE_L16X4_XP(x7, pT, stride);
            /* Load window coefficients */
            AE_L16X4_IP(w0, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w1, pW, sizeof(ae_int16x4));

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

            A0 = AE_MULZAAAAQ16(x0, w0);
            A1 = AE_MULZAAAAQ16(x1, w0);
            A2 = AE_MULZAAAAQ16(x2, w0);
            A3 = AE_MULZAAAAQ16(x3, w0);
            AE_MULAAAAQ16(A0, x4, w1);
            AE_MULAAAAQ16(A1, x5, w1);
            AE_MULAAAAQ16(A2, x6, w1);
            AE_MULAAAAQ16(A3, x7, w1);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }
        /* Move computed samples from the scratch to the result image */
        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
            AE_S16X4_XP(t0, pOut, stride);
        }
    }
#else
    int16_t * restrict t=(int16_t *)pScr;
    const int16_t * restrict w= ((img_coefdn_cubic_t*)pCoef)->coef;
    const int16_t * restrict up=((img_coefdn_cubic_t*)pCoef)->left;
    const ae_int16x4 * restrict pIn;
          ae_int16x4 * restrict pOut;
    const ae_int16x4 * restrict pT;
    const ae_int16x4 * restrict pT1;
    const ae_int16x4 * restrict pW;
    ae_int16x4 x00, x10, x20, x30, x40, x50, x60, x70;
    ae_int16x4 x01, x11, x21, x31, x41, x51, x61, x71;
    ae_int16x4 w0, w1, w2, w3, w4, w5, w6, w7;
    ae_int32x2 Y0, Y1, Y2, Y3;
    ae_int16x4 y0, y1, t0, t1, t2, t3, t4, t5, t6, t7;
    ae_int64 A0, A1, A2, A3;
    int m, n, k;
    int wout = out->width,
        hin = in->height,
        hout = out->height,
        stride = out->stride;

    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->width==wout && hin<2*hout && hin>hout && in->stride==out->stride);
    NASSERT(hout >= 1);
    NASSERT(getScratchSize(in, out) == hout*sizeof(int16_t)*8); /* must be 8 columns */

    stride = stride * sizeof(int16_t);

    for (m = 0; m < ((wout+3) & ~7); m+=8)
    {
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        pW   = (const ae_int16x4 *)(w);

        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            int16_t k;
            /* Load 8x8 elems from the 8 columns */
            k = up[n];
            pT = (const ae_int16x4 *)((uintptr_t)pIn + stride*k);
            pT1 = pT + 1;
            AE_L16X4_XP(x00, pT, stride);  AE_L16X4_XP(x01, pT1, stride);
            AE_L16X4_XP(x10, pT, stride);  AE_L16X4_XP(x11, pT1, stride);
            AE_L16X4_XP(x20, pT, stride);  AE_L16X4_XP(x21, pT1, stride);
            AE_L16X4_XP(x30, pT, stride);  AE_L16X4_XP(x31, pT1, stride);
            AE_L16X4_XP(x40, pT, stride);  AE_L16X4_XP(x41, pT1, stride);
            AE_L16X4_XP(x50, pT, stride);  AE_L16X4_XP(x51, pT1, stride);
            AE_L16X4_XP(x60, pT, stride);  AE_L16X4_XP(x61, pT1, stride);
            AE_L16X4_XP(x70, pT, stride);  AE_L16X4_XP(x71, pT1, stride);
            /* Load window coefficients */
            AE_L16_IP(w0, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w1, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w2, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w3, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w4, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w5, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w6, castxcc(ae_int16,pW), sizeof(int16_t));
            AE_L16_IP(w7, castxcc(ae_int16,pW), sizeof(int16_t));

            AE_MUL16X4 (Y0, Y1, x00, w0);
            AE_MULA16X4(Y0, Y1, x10, w1);
            AE_MULA16X4(Y0, Y1, x20, w2);
            AE_MULA16X4(Y0, Y1, x30, w3);
            AE_MULA16X4(Y0, Y1, x40, w4);
            AE_MULA16X4(Y0, Y1, x50, w5);
            AE_MULA16X4(Y0, Y1, x60, w6);
            AE_MULA16X4(Y0, Y1, x70, w7);
            AE_MUL16X4 (Y2, Y3, x01, w0);
            AE_MULA16X4(Y2, Y3, x11, w1);
            AE_MULA16X4(Y2, Y3, x21, w2);
            AE_MULA16X4(Y2, Y3, x31, w3);
            AE_MULA16X4(Y2, Y3, x41, w4);
            AE_MULA16X4(Y2, Y3, x51, w5);
            AE_MULA16X4(Y2, Y3, x61, w6);
            AE_MULA16X4(Y2, Y3, x71, w7);
            Y0 = AE_ADD32S(Y0, Y0);
            Y1 = AE_ADD32S(Y1, Y1);
            Y2 = AE_ADD32S(Y2, Y2);
            Y3 = AE_ADD32S(Y3, Y3);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            y1 = AE_ROUND16X4F32SASYM(Y2, Y3);
            AE_S16X4_IP(y1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            AE_L16X4_IP(t1, pIn, sizeof(ae_int16x4));
            AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
            AE_S16X4_I (t1, pOut, sizeof(ae_int16x4));
            AE_S16X4_XP(t0, pOut, stride);
        }
    }
    if (((wout-1) & 4) == 0)
    {
        m = wout & ~7;
        pIn  = (const ae_int16x4 *)((int16_t *)img + m);
        pOut = (      ae_int16x4 *)(t);
        pW   = (const ae_int16x4 *)(w);

        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            /* Load 8x4 elems from the 4 columns */
            k = up[n];
            pT = (const ae_int16x4 *)((uintptr_t)pIn + k*stride);
            AE_L16X4_XP(x00, pT, stride);
            AE_L16X4_XP(x10, pT, stride);
            AE_L16X4_XP(x20, pT, stride);
            AE_L16X4_XP(x30, pT, stride);
            AE_L16X4_XP(x40, pT, stride);
            AE_L16X4_XP(x50, pT, stride);
            AE_L16X4_XP(x60, pT, stride);
            AE_L16X4_XP(x70, pT, stride);
            /* Load window coefficients */
            AE_L16X4_IP(w0, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w1, pW, sizeof(ae_int16x4));

            t0 = AE_SEL16_7531(x00, x10);
            t1 = AE_SEL16_6420(x00, x10);
            t2 = AE_SEL16_7531(x20, x30);
            t3 = AE_SEL16_6420(x20, x30);
            t4 = AE_SEL16_7531(x40, x50);
            t5 = AE_SEL16_6420(x40, x50);
            t6 = AE_SEL16_7531(x60, x70);
            t7 = AE_SEL16_6420(x60, x70);

            x00 = AE_SEL16_7531(t0, t2);
            x10 = AE_SEL16_7531(t1, t3);
            x20 = AE_SEL16_6420(t0, t2);
            x30 = AE_SEL16_6420(t1, t3);
            x40 = AE_SEL16_7531(t4, t6);
            x50 = AE_SEL16_7531(t5, t7);
            x60 = AE_SEL16_6420(t4, t6);
            x70 = AE_SEL16_6420(t5, t7);

            A0 = AE_MULZAAAAQ16(x00, w0);
            A1 = AE_MULZAAAAQ16(x10, w0);
            A2 = AE_MULZAAAAQ16(x20, w0);
            A3 = AE_MULZAAAAQ16(x30, w0);
            AE_MULAAAAQ16(A0, x40, w1);
            AE_MULAAAAQ16(A1, x50, w1);
            AE_MULAAAAQ16(A2, x60, w1);
            AE_MULAAAAQ16(A3, x70, w1);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);

            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut, sizeof(ae_int16x4));
        }

        pIn  = (const ae_int16x4 *)(t);
        pOut = (      ae_int16x4 *)((int16_t *)img + m);
        __Pragma("loop_count min=1");
        for (n = 0; n < hout; n++)
        {
            AE_L16X4_IP(t0, pIn, sizeof(ae_int16x4));
            AE_S16X4_XP(t0, pOut, stride);
        }
    }
#endif
}

const imgresizer_api_t imgresizer_api_dnxv_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
