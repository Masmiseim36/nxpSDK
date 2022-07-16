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
  Image interleave
  Functions convert planar images to packed format 

  Image formats:
  8-bit signed or unsigned data
  16-bit signed data

  Input:
  inImgR
  inImgG
  inImgB  planes with R,G,B components
  sz      image size
  Output:
  outImg  packed image (RGB come together)

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgfastinterleave(      void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz)
{
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    const ae_int16x4 * restrict pInR;
    const uint8_t * restrict inR = (const uint8_t *)inImgR;
    const ae_int16x4 * restrict pInG;
    const uint8_t * restrict inG = (const uint8_t *)inImgG;
    const ae_int16x4 * restrict pInB;
    const uint8_t * restrict inB = (const uint8_t *)inImgB;
    uint8_t * restrict out = (uint8_t *)outImg;
    ae_int16x4 * restrict pOut;
    ae_int16x4 r, g, b;
    imgsize_validate(sz, 1, 1);
    NASSERT(outImg);
    NASSERT(inImgR);
    NASSERT(inImgG);
    NASSERT(inImgB);
    NASSERT_ALIGN(outImg, ALIGNMENT);
    NASSERT_ALIGN(inImgR, ALIGNMENT);
    NASSERT_ALIGN(inImgG, ALIGNMENT);
    NASSERT_ALIGN(inImgB, ALIGNMENT);
    for (i = 0; i < h; i++)
    {
        pInR = (const ae_int16x4*)(inR + i*stride);
        pInG = (const ae_int16x4*)(inG + i*stride);
        pInB = (const ae_int16x4*)(inB + i*stride);
        pOut = (ae_int16x4 *)(out + 3 * i*stride);
        for (j = 0; j < (w >> 3); j++)
        {
            ae_int16x4 r0, r1, g0, g1, b0, b1;
            ae_int16x4 r0b0r4b4, g1r2g5r6, g0r1g4r5, b1g2b5g6, r3b3r7b7, b2g3b6g7;
            ae_int16x4 _0, _1, _2, _3, _4, _5;
            AE_L16X4_IP(r, pInR, 4 * sizeof(uint16_t));
            AE_L16X4_IP(g, pInG, 4 * sizeof(uint16_t));
            AE_L16X4_IP(b, pInB, 4 * sizeof(uint16_t));

            r0 = AE_AND16(r, 0x00ff);
            g0 = AE_AND16(g, 0x00ff);
            b0 = AE_AND16(b, 0x00ff);

            r1 = AE_AND16(r, AE_MOVDA16(0xff00));
            g1 = AE_AND16(g, AE_MOVDA16(0xff00));
            b1 = AE_AND16(b, AE_MOVDA16(0xff00));

            r1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(r1), 8));
            g1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(g1), 8));
            b1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(b1), 8));

            r0b0r4b4 = AE_SEL16_7531(r0, b0);
            r0b0r4b4 = AE_SEL16_7520(r0b0r4b4, r0b0r4b4);

            g1r2g5r6 = AE_SEL16_7520(g1, r0);
            g1r2g5r6 = AE_SEL16_7520(g1r2g5r6, g1r2g5r6);

            g0r1g4r5 = AE_SEL16_7531(g0, r1);
            g0r1g4r5 = AE_SEL16_7520(g0r1g4r5, g0r1g4r5);

            b1g2b5g6 = AE_SEL16_7520(b1, g0);
            b1g2b5g6 = AE_SEL16_7520(b1g2b5g6, b1g2b5g6);

            r3b3r7b7 = AE_SEL16_6420(r1, b1);
            r3b3r7b7 = AE_SEL16_7520(r3b3r7b7, r3b3r7b7);

            b2g3b6g7 = AE_SEL16_6420(b0, g1);
            b2g3b6g7 = AE_SEL16_7520(b2g3b6g7, b2g3b6g7);

            _0 = AE_SEL16_7632(r0b0r4b4, g1r2g5r6);
            _1 = AE_SEL16_7632(g0r1g4r5, b1g2b5g6);

            _2 = AE_SEL16_7610(b2g3b6g7, r0b0r4b4);
            _3 = AE_SEL16_7610(r3b3r7b7, g0r1g4r5);

            _4 = AE_SEL16_5410(g1r2g5r6, b2g3b6g7);
            _5 = AE_SEL16_5410(b1g2b5g6, r3b3r7b7);

            _1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(_1), 8));
            _0 = AE_OR16(_0, _1);
            AE_S16X4_IP(_0, pOut, sizeof(ae_int16x4));

            _3 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(_3), 8));
            _2 = AE_OR16(_2, _3);
            AE_S16X4_IP(_2, pOut, sizeof(ae_int16x4));

            _5 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(_5), 8));
            _4 = AE_OR16(_4, _5);
            AE_S16X4_IP(_4, pOut, sizeof(ae_int16x4));

        }
    }
    for (i = 0; i < h; i++)
    {
        for (j = w&(~7); j < w; j++)
        {
            uint8_t r = ((const uint8_t*)inImgR)[i*stride + j];
            uint8_t g = ((const uint8_t*)inImgG)[i*stride + j];
            uint8_t b = ((const uint8_t*)inImgB)[i*stride + j];
            ((uint8_t*)outImg)[i*stride * 3 + j * 3 + 0] = r;
            ((uint8_t*)outImg)[i*stride * 3 + j * 3 + 1] = g;
            ((uint8_t*)outImg)[i*stride * 3 + j * 3 + 2] = b;
        }
    }
}
