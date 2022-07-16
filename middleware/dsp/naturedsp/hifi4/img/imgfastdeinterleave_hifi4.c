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
void imgfastdeinterleave  
                      (       void * restrict outImgR, 
                              void * restrict outImgG, 
                              void * restrict outImgB, 
                        const void * restrict inImg, 
                        const imgsize_t* sz)
{
    int i,j,w=(int)sz->width,h=(int)sz->height,stride=sz->stride;
    ae_int16x4 * restrict pOutR;
    uint8_t * restrict outR = (uint8_t *)outImgR;
    ae_int16x4 * restrict pOutG;
    uint8_t * restrict outG = (uint8_t *)outImgG;
    ae_int16x4 * restrict pOutB;
    uint8_t * restrict outB = (uint8_t *)outImgB;
    const uint8_t * restrict in = (const uint8_t *)inImg;
    const signed char * restrict pIn;
    imgsize_validate(sz,1,1);
    NASSERT(outImgR);
    NASSERT(outImgG);
    NASSERT(outImgB);
    NASSERT(inImg  );
    NASSERT_ALIGN(outImgR,ALIGNMENT);
    NASSERT_ALIGN(outImgG,ALIGNMENT);
    NASSERT_ALIGN(outImgB,ALIGNMENT);
    NASSERT_ALIGN(inImg  ,ALIGNMENT);
    for (i = 0; i < h; i++)
    {
        pOutR = (ae_int16x4*)(outR + i*stride);
        pOutG = (ae_int16x4*)(outG + i*stride);
        pOutB = (ae_int16x4*)(outB + i*stride);
        pIn = (const signed char *)(in + 3 * i*stride);
        for (j = 0; j < (w >> 3); j++)
        {
            ae_int16x4 r0, g0, b0, r1, g1, b1, _0, _1, _2, _3, _4, _5, r0g0r2g2, b0r1b2r3, g1b1g3b3, r4g4r6g6, b4r5b6r7, g5b5g7b7;
            AE_L8X4F_IP(_0, pIn, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(_1, pIn, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(_2, pIn, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(_3, pIn, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(_4, pIn, 4 * sizeof(uint8_t));
            AE_L8X4F_IP(_5, pIn, 4 * sizeof(uint8_t));

            r0g0r2g2 = AE_SEL16_7610(_0, _1);
            b0r1b2r3 = AE_SEL16_5432(_0, _2);
            g1b1g3b3 = AE_SEL16_7610(_1, _2);

            r4g4r6g6 = AE_SEL16_7610(_3, _4);
            b4r5b6r7 = AE_SEL16_5432(_3, _5);
            g5b5g7b7 = AE_SEL16_7610(_4, _5);

            r0 = AE_SEL16_7531(r0g0r2g2, r4g4r6g6);
            g0 = AE_SEL16_6420(r0g0r2g2, r4g4r6g6);

            b0 = AE_SEL16_7531(b0r1b2r3, b4r5b6r7);
            r1 = AE_SEL16_6420(b0r1b2r3, b4r5b6r7);

            g1 = AE_SEL16_7531(g1b1g3b3, g5b5g7b7);
            b1 = AE_SEL16_6420(g1b1g3b3, g5b5g7b7);

            r0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r0), 8));
            g0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g0), 8));
            b0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b0), 8));

            r0 = AE_OR16(r0, r1);
            g0 = AE_OR16(g0, g1);
            b0 = AE_OR16(b0, b1);

            AE_S16X4_IP(r0, pOutR, sizeof(ae_int16x4));
            AE_S16X4_IP(g0, pOutG, sizeof(ae_int16x4));
            AE_S16X4_IP(b0, pOutB, sizeof(ae_int16x4));
        }
    }
    for (i = 0; i < h; i++)
    {
        for (j = w&(~7); j < w; j++)
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
