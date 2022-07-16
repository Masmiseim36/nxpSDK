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
void imgfastinterleave16(    void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz)
{
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    const ae_int16x4 * restrict pInR;
    const int16_t * restrict inR = (const int16_t *)inImgR;
    const ae_int16x4 * restrict pInG;
    const int16_t * restrict inG = (const int16_t *)inImgG;
    const ae_int16x4 * restrict pInB;
    const int16_t * restrict inB = (const int16_t *)inImgB;
    int16_t * restrict out = (int16_t *)outImg;
    ae_int16x4 * restrict pOut;
    imgsize_validate(sz, 2, 1);
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
        for (j = 0; j < (w >> 2); j++)
        {
            ae_int16x4 r, g, b, _12, _23, _31, _1, _2, _3;
            AE_L16X4_IP(r, pInR, 4 * sizeof(int16_t));
            AE_L16X4_IP(g, pInG, 4 * sizeof(int16_t));
            AE_L16X4_IP(b, pInB, 4 * sizeof(int16_t));
            r = AE_SEL16_7520(r, r);
            g = AE_SEL16_7520(g, g);
            b = AE_SEL16_7520(b, b);
            _12 = AE_SEL16_7362(r, g);
            _23 = AE_SEL16_5140(g, b);
            _31 = AE_SEL16_7160(b, r);
            _1 = AE_SEL16_7632(_12, _31);
            _2 = AE_SEL16_7610(_23, _12);
            _3 = AE_SEL16_5410(_31, _23);

            AE_S16X4_IP(_1, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(_2, pOut, sizeof(ae_int16x4));
            AE_S16X4_IP(_3, pOut, sizeof(ae_int16x4));
        }
    }
    for (i = 0; i < h; i++)
    {
        for (j = w&(~3); j < w; j++)
        {
            int16_t r = inR[i*stride + j];
            int16_t g = inG[i*stride + j];
            int16_t b = inB[i*stride + j];
            out[i*stride * 3 + j * 3 + 0] = r;
            out[i*stride * 3 + j * 3 + 1] = g;
            out[i*stride * 3 + j * 3 + 2] = b;
        }
    }
}
