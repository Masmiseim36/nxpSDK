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

/*    image resizer, downsample 1...2x in horizontal direction */

/* returns size of coefficients */
static size_t getCoefSz(const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    NASSERT(in->height==out->height &&  win<2*wout && win>wout && in->stride==out->stride);
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    return img_getCoef_dn_cubic_alloc(win,wout);
}
/* returns coefficients */
static void getCoef(void* coef, const imgsize_t* in,const imgsize_t* out)
{
    int win=in->width,wout=out->width;
    NASSERT(in->height==out->height &&  win<2*wout && win>wout && in->stride==out->stride);
    (void)in,(void)out,(void)coef;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    img_getCoef_dn_cubic_init(coef,win,wout);
}

static size_t getScratchSize(const imgsize_t* in,const imgsize_t* out)
{
    (void)in,(void)out;
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);
    /* 2 aligned rows */
    return ((out->width*sizeof(int16_t) + ALIGNMENT-1) & ~(ALIGNMENT-1))*2;
}

/* in-place image resize */
static void process(void *pScr, void* pCoef, const void* img, void*imgOut, const imgsize_t* in, const imgsize_t* out, int fast)
{
          int16_t * restrict t    = (int16_t *)pScr;
    const int16_t * restrict w    = ((img_coefdn_cubic_t*)pCoef)->coef;
    const int16_t * restrict left = ((img_coefdn_cubic_t*)pCoef)->left;
    const int16_t * restrict pL;
    const ae_int16x4 * restrict pW;
          ae_int16x4 * restrict pT;
    const ae_int16x4 * restrict pIn0;
    const ae_int16x4 * restrict pIn1;
          ae_int16x4 * restrict pOut0;
          ae_int16x4 * restrict pOut1;
    ae_int16x4 x000, x001, x010, x011, x020, x021, x030, x031;
    ae_int16x4 x100, x101, x110, x111, x120, x121, x130, x131;
    ae_int16x4 w00, w01, w10, w11, w20, w21, w30, w31;
    ae_int16x4 y0, y1, t0, t1;
    ae_int64 A0, A1, A2, A3;
    ae_int32x2 Y0, Y1;
    ae_valign al0;
    int m, n, k0, k1, k2, k3;
    int hout = in->height,
        wout = out->width,
        stride = out->stride;

    NASSERT_ALIGN(img,ALIGNMENT);
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT(in->height == out->height &&  in->width<2 * out->width && in->width>out->width && in->stride == out->stride);
    NASSERT(wout >= 1);
    imgsize_validate(in,2,1);
    imgsize_validate(out,2,1);

    for (n = 0; n < (hout & ~1); n+=2)
    {
        pW = (const ae_int16x4 *)w;
        pL = left;
        pIn0  = (ae_int16x4 *)((int16_t *)img + (n + 0)*stride);
        pIn1  = (ae_int16x4 *)((int16_t *)img + (n + 1)*stride);
        pOut0 = (ae_int16x4 *)t;

        /* Process by 2x4 samples per iteration */
        for (m = 0; m < wout/4; m++)
        {
            /* Load indexes */
            k0 = *pL++;  k1 = *pL++;  k2 = *pL++;  k3 = *pL++;
            /* Load 4x8 elems from the 1st row */
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x000, al0, pT);  AE_LA16X4_IP(x001, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k1, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x010, al0, pT);  AE_LA16X4_IP(x011, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k2, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x020, al0, pT);  AE_LA16X4_IP(x021, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k3, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x030, al0, pT);  AE_LA16X4_IP(x031, al0, pT);
            /* Load 4x8 elems from the 2nd row */
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn1);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x100, al0, pT);  AE_LA16X4_IP(x101, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k1, (uintptr_t)pIn1);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x110, al0, pT);  AE_LA16X4_IP(x111, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k2, (uintptr_t)pIn1);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x120, al0, pT);  AE_LA16X4_IP(x121, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k3, (uintptr_t)pIn1);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x130, al0, pT);  AE_LA16X4_IP(x131, al0, pT);
            /* Load window coefficients */
            AE_L16X4_IP(w00, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w01, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w10, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w11, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w20, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w21, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w30, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w31, pW, sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x000, w00);
            A1 = AE_MULZAAAAQ16(x010, w10);
            A2 = AE_MULZAAAAQ16(x020, w20);
            A3 = AE_MULZAAAAQ16(x030, w30);
            AE_MULAAAAQ16(A0, x001, w01);
            AE_MULAAAAQ16(A1, x011, w11);
            AE_MULAAAAQ16(A2, x021, w21);
            AE_MULAAAAQ16(A3, x031, w31);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut0, sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x100, w00);
            A1 = AE_MULZAAAAQ16(x110, w10);
            A2 = AE_MULZAAAAQ16(x120, w20);
            A3 = AE_MULZAAAAQ16(x130, w30);
            AE_MULAAAAQ16(A0, x101, w01);
            AE_MULAAAAQ16(A1, x111, w11);
            AE_MULAAAAQ16(A2, x121, w21);
            AE_MULAAAAQ16(A3, x131, w31);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y1 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y1, pOut0, sizeof(ae_int16x4));
        }
        /* Process last 0...3 samples, for 2 rows */
        __Pragma("no_unroll");
        for (m = 0; m < (wout & 3); m++)
        {
            k0 = *pL++;
            /* Load 1x8 elems from the 1st row */
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x000, al0, pT);  AE_LA16X4_IP(x001, al0, pT);
            /* Load 1x8 elems from the 2nd row */
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn1);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x100, al0, pT);  AE_LA16X4_IP(x101, al0, pT);
            /* Load window coefficients */
            AE_L16X4_IP(w00, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w01, pW, sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x000, w00);
            A1 = AE_MULZAAAAQ16(x100, w00);
            AE_MULAAAAQ16(A0, x001, w01);
            AE_MULAAAAQ16(A1, x101, w01);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
            AE_S16_0_I (y0, (ae_int16 *)pOut0, sizeof(ae_int16x4));
            AE_S16_0_IP(AE_SHORTSWAP(y0), castxcc(ae_int16,pOut0), sizeof(int16_t));
        }
        /* Move computed samples from the scratch to the result image */
        pIn0 = (ae_int16x4 *)t;
        pOut0 = (ae_int16x4 *)((int16_t *)img + (n + 0)*stride);
        pOut1 = (ae_int16x4 *)((int16_t *)img + (n + 1)*stride);
        __Pragma("loop_count min=1");
        for (m=0; m<wout; m+=4)
        {
            AE_L16X4_IP(t0, pIn0, sizeof(ae_int16x4));
            AE_L16X4_IP(t1, pIn0, sizeof(ae_int16x4));
            AE_S16X4_IP(t0, pOut0, sizeof(ae_int16x4));
            AE_S16X4_IP(t1, pOut1, sizeof(ae_int16x4));
        }
    }
    /* Process the last row */
    if (hout & 1)
    {
        n = hout - 1;
        pW = (const ae_int16x4 *)w;
        pL = left;
        pIn0  = (ae_int16x4 *)((int16_t *)img + n*stride);
        pOut0 = (ae_int16x4 *)t;

        for (m = 0; m < wout/4; m++)
        {
            /* Load indexes */
            k0 = *pL++;  k1 = *pL++;  k2 = *pL++;  k3 = *pL++;
            /* Load 4x8 elems from the row */
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x000, al0, pT);  AE_LA16X4_IP(x001, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k1, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x010, al0, pT);  AE_LA16X4_IP(x011, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k2, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x020, al0, pT);  AE_LA16X4_IP(x021, al0, pT);
            pT = (ae_int16x4 *)XT_ADDX2(k3, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x030, al0, pT);  AE_LA16X4_IP(x031, al0, pT);
            /* Load window coefficients */
            AE_L16X4_IP(w00, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w01, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w10, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w11, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w20, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w21, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w30, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w31, pW, sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x000, w00);
            A1 = AE_MULZAAAAQ16(x010, w10);
            A2 = AE_MULZAAAAQ16(x020, w20);
            A3 = AE_MULZAAAAQ16(x030, w30);
            AE_MULAAAAQ16(A0, x001, w01);
            AE_MULAAAAQ16(A1, x011, w11);
            AE_MULAAAAQ16(A2, x021, w21);
            AE_MULAAAAQ16(A3, x031, w31);
            Y0 = AE_TRUNCA32X2F64S(A0, A1, 33);
            Y1 = AE_TRUNCA32X2F64S(A2, A3, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y1);
            AE_S16X4_IP(y0, pOut0, sizeof(ae_int16x4));
        }
        __Pragma("no_unroll");
        for (m = 0; m < (wout & 3); m++)
        {
            k0 = *pL++;
            pT = (ae_int16x4 *)XT_ADDX2(k0, (uintptr_t)pIn0);
            al0 = AE_LA64_PP(pT);  AE_LA16X4_IP(x000, al0, pT);  AE_LA16X4_IP(x001, al0, pT);

            AE_L16X4_IP(w00, pW, sizeof(ae_int16x4));
            AE_L16X4_IP(w01, pW, sizeof(ae_int16x4));

            A0 = AE_MULZAAAAQ16(x000, w00);
            AE_MULAAAAQ16(A0, x001, w01);
            Y0 = AE_TRUNCA32X2F64S(A0, A0, 33);
            y0 = AE_ROUND16X4F32SASYM(Y0, Y0);
            AE_S16_0_IP(y0, castxcc(ae_int16,pOut0), sizeof(int16_t));
        }
        /* Move computed samples from the scratch to the result image */
        pIn0 = (ae_int16x4 *)t;
        pOut0 = (ae_int16x4 *)((int16_t *)img + n*stride);
        __Pragma("loop_count min=1");
        for (m=0; m<wout; m+=4)
        {
            AE_L16X4_IP(t0, pIn0, sizeof(ae_int16x4));
            AE_S16X4_IP(t0, pOut0, sizeof(ae_int16x4));
        }
    }
}

const imgresizer_api_t imgresizer_api_dnxh_cubic={NULL,getCoefSz,getCoef,getScratchSize,process};
