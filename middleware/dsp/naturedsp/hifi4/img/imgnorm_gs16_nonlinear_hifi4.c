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
  Image normalization
  Function normalize the intensity of pixels to the given range

  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input:
  inImg   input image
  sz      image size
  minInt  min intensity on output (for linear normalization)
  maxInt  max intensity on output (for non-linear normalization)
  tbl[64] tabulated values (for non-linear normalization)
  Input:
  outImg   input image

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgnorm_gs16_nonlinear ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t* tbl)
{
    const int16_t * restrict in = (const int16_t *)inImg;
    int16_t * restrict out = (int16_t *)outImg;
    const ae_int16x4 * restrict pIn;
    ae_valign al_in, al_out;
    ae_int16x4 * restrict pOut;
    int i, j;
    int h = (int)sz->height;
    int w = (int)sz->width;
    int istride = sz->stride;

    for (i = 0; i < h; i++)
    {
        ae_int16x4 y00, y01, y02, y03;
        ae_int16x4 y10, y11, y12, y13;
        ae_int16x4 x0, x, ix, z, y0, y1, x1, x2;
        ae_int32x2 d0, d1;
        pIn = (const ae_int16x4*)(in + i*istride);
        al_in = AE_LA64_PP(pIn);
        pOut = (ae_int16x4*)(out + i*istride);
        al_out = AE_ZALIGN64();
        for (j = 0; j < ((w) >> 2); j++)
        {
            AE_LA16X4_IP(x, al_in, pIn);
            ix = AE_SUB16(x, (ae_int16x4)256);
            ix = AE_SRAI16(ix, 9);
            AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
            AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

            x0 = AE_ADD16(AE_SLAI16S(ix, 9), 256);

            ix = AE_ADD16(ix, ix);
            y00 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_3(ix));
            y10 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_3(ix));
            y01 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_2(ix));
            y11 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_2(ix));
            y02 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_1(ix));
            y12 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_1(ix));
            y03 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
            y13 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

            AE_MOVT16X4(y00, y01, AE_MOVAB4(4));
            AE_MOVT16X4(y02, y03, AE_MOVAB4(4));
            y0 = AE_SEL16_7632(y00, y02);

            AE_MOVT16X4(y10, y11, AE_MOVAB4(4));
            AE_MOVT16X4(y12, y13, AE_MOVAB4(4));
            y1 = AE_SEL16_7632(y10, y12);

            AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0),1), AE_SLAI16S(AE_SUB16(x, x0), 5));
            z = AE_ADD16S(y0, AE_ROUND16X4F32SASYM(d0, d1));

            AE_MOVT16X4(z, 0, AE_LT16(z, 0));
            AE_SA16X4_IP(z, al_out, pOut);
        }
        AE_SA64POS_FP(al_out, pOut);
        //tail
        if (w & 3)
        {
            int off1, off2;
            off1 = (w & 3)>1 ? 1 * sizeof(int16_t) : 0;
            off2 = (w & 3)>2 ? 2 * sizeof(int16_t) : 0;
            AE_LA16X4_IP(x, al_in, pIn);
            ix = AE_SUB16(x, (ae_int16x4)256);
            ix = AE_SRAI16(ix, 9);
            AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
            AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

            x0 = AE_ADD16(AE_SLAI16S(ix, 9), 256);

            ix = AE_ADD16(ix, ix);
            y00 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_3(ix));
            y10 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_3(ix));
            y01 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_2(ix));
            y11 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_2(ix));
            y02 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_1(ix));
            y12 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_1(ix));
            y03 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
            y13 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

            AE_MOVT16X4(y00, y01, AE_MOVAB4(4));
            AE_MOVT16X4(y02, y03, AE_MOVAB4(4));
            y0 = AE_SEL16_7632(y00, y02);

            AE_MOVT16X4(y10, y11, AE_MOVAB4(4));
            AE_MOVT16X4(y12, y13, AE_MOVAB4(4));
            y1 = AE_SEL16_7632(y10, y12);

            AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0),1), AE_SLAI16S(AE_SUB16(x, x0), 5));
            z = AE_ADD16S(y0, AE_ROUND16X4F32SASYM(d0, d1));

            AE_MOVT16X4(z, 0, AE_LT16(z, 0));
            x2 = AE_SEL16_4321(z, z);
            x1 = AE_SEL16_5432(z, z);
            x0 = AE_SEL16_6543(z, z);
            AE_S16_0_X(x2, (ae_int16*)pOut, off2);
            AE_S16_0_X(x1, (ae_int16*)pOut, off1);
            AE_S16_0_XP(x0, castxcc(ae_int16, pOut), sizeof(int16_t)*istride);
        }
    }
}
