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

/*-------------------------------------------------------------------------
  Image deinterleave
  Functions convert packed images to planar format 

  Image formats:
  8-bit signed or unsigned data
  16-bit signed data

  Input:
  inImg   packed image (RGB come together)
  sz      image size
  Output:
  outImgR
  outImgG
  outImgB planes with R,G,B components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgdeinterleave16(       void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg  , 
                        const imgsize_t* sz)
{
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 * restrict pOutR;
    int16_t * restrict outR = (int16_t *)outImgR;
    ae_int16x4 * restrict pOutG;
    int16_t * restrict outG = (int16_t *)outImgG;
    ae_int16x4 * restrict pOutB;
    int16_t * restrict outB = (int16_t *)outImgB;
    const ae_int16x4 * restrict pIn;
    const int16_t * restrict in = (const int16_t *)inImg;
    ae_valign al_r, al_g, al_b, al_in;
    imgsize_validate(sz, 2, 0);
    NASSERT(outImgR);
    NASSERT(outImgG);
    NASSERT(outImgB);
    NASSERT(inImg);
    NASSERT_ALIGN(outImgR, 2);
    NASSERT_ALIGN(outImgG, 2);
    NASSERT_ALIGN(outImgB, 2);
    NASSERT_ALIGN(inImg, 2);
    al_r = AE_ZALIGN64();
    al_g = AE_ZALIGN64();
    al_b = AE_ZALIGN64();
    for (i = 0; i < h; i++)
    {
        pOutR = (ae_int16x4*)(outR + i*stride);
        pOutG = (ae_int16x4*)(outG + i*stride);
        pOutB = (ae_int16x4*)(outB + i*stride);
        pIn = (const ae_int16x4 *)(in + 3 * i*stride);
        al_in = AE_LA64_PP(pIn);
        for (j = 0; j < (w >> 2); j++)
        {
            ae_int16x4 r, g, b, _0, _1, _2, r0g0r2g2, b0r1b2r3, g1b1g3b3;
            AE_LA16X4_IP(_0, al_in, pIn);
            AE_LA16X4_IP(_1, al_in, pIn);
            AE_LA16X4_IP(_2, al_in, pIn);

            r0g0r2g2 = AE_SEL16_7610(_0, _1);
            b0r1b2r3 = AE_SEL16_5432(_0, _2);
            g1b1g3b3 = AE_SEL16_7610(_1, _2);

            r = AE_SEL16_7520(r0g0r2g2, b0r1b2r3);
            r = AE_SEL16_7520(r, r);

            g = AE_SEL16_7520(g1b1g3b3, r0g0r2g2);
            g = AE_SEL16_5342(g, g);

            b = AE_SEL16_7520(b0r1b2r3, g1b1g3b3);
            b = AE_SEL16_7520(b, b);

            AE_SA16X4_IP(r, al_r, pOutR);
            AE_SA16X4_IP(g, al_g, pOutG); 
            AE_SA16X4_IP(b, al_b, pOutB);
        }
        AE_SA64POS_FP(al_r, pOutR);
        AE_SA64POS_FP(al_g, pOutG);
        AE_SA64POS_FP(al_b, pOutB);
    }
    for (i = 0; i < h; i++)
    {
        for (j = w&(~3); j < w; j++)
        {
            int16_t r = in[i*stride * 3 + j * 3 + 0];
            int16_t g = in[i*stride * 3 + j * 3 + 1];
            int16_t b = in[i*stride * 3 + j * 3 + 2];
            outR[i*stride + j] = r;
            outG[i*stride + j] = g;
            outB[i*stride + j] = b;
        }
    }
}
