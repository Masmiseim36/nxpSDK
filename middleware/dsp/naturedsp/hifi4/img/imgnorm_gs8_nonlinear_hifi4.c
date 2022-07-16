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

void imgnorm_gs8_nonlinear ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, const int16_t* tbl)
{
    const int8_t * restrict in = (const int8_t *)inImg;
    int8_t * restrict out = (int8_t *)outImg;
    const ae_int16x4 * restrict pIn;
    ae_int16x4 * restrict pOut;
    int i, j;
    ae_valign al_in, al_out;
    int h = (int)sz->height;
    int w = (int)sz->width;
    int istride = sz->stride;
    NASSERT(inImg != NULL);
    NASSERT(outImg != NULL);
    NASSERT_ALIGN(inImg, 1);
    NASSERT_ALIGN(outImg, 1);
    imgsize_validate(sz, 1, 0);
    al_out = AE_ZALIGN64();
    int sh_in, sh_out;

    for (i = 0; i < h; i++)
    {
        pIn = (const ae_int16x4*)(in + i*istride);
        pOut = (ae_int16x4*)(out + i*istride);
        sh_in = (uintptr_t)pIn & 1;
        sh_out = (uintptr_t)pOut & 1;
        //can't use any vector load/store in this case
        if (sh_in != sh_out)
        {
            pIn = (const ae_int16x4*)(in + i*istride + sh_in);
            al_in = AE_LA64_PP(pIn);
            for (j = 0; j < sh_in; j++)
            {
                int8_t p = in[i*sz->stride + j];
                ae_int16x4 y0, y1;
                ae_int16x4 x, ix, x0, z;
                ae_int32x2 d0, d1;
                x = AE_MOVDA16(p);
                x = AE_ADD16(x, 128);
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);

                ix = AE_ADD16(ix, ix);
                y0 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
                y1 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                z = AE_ADD16(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, -128, AE_LT16(z, -128));
                AE_MOVT16X4(z, 127, AE_LT16(127, z));

                out[i*sz->stride + j] = AE_MOVAD16_0(z);
            }
            for (j = 0; j < ((w - sh_in) >> 3); j++)
            {
                int shift = i*istride + sh_in;
                ae_int16x4 y00, y01, y02, y03;
                ae_int16x4 y10, y11, y12, y13;
                ae_int16x4 x, ix, x0, p, z, y0, y1;
                ae_int32x2 d0, d1;
                ae_int16x4  y00_, y10_, y01_, y11_;
                ae_int16x4  y02_, y12_, y03_, y13_;
                ae_int16x4 x_, ix_, x0_, z_, y0_, y1_;
                ae_int32x2 d0_, d1_;
                AE_LA16X4_IP(p, al_in, pIn);
                x = AE_AND16(p, (int16_t)(0xff00));
                x = AE_SRAI16(x, 8);
                x = AE_ADD16(x, 128);
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));
                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);

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

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                z = AE_ADD16S(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, -128, AE_LT16(z, -128));
                AE_MOVT16X4(z, 127, AE_LT16(127, z));

                p = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(p), 8));
                x_ = AE_AND16(p, (int16_t)(0xff00));
                x_ = AE_SRAI16(x_, 8);
                x_ = AE_ADD16(x_, 128);
                ix_ = AE_SRAI16(AE_SUB16(x_, 2), 2);
                AE_MOVT16X4(ix_, 62, AE_LT16(62, ix_));
                AE_MOVT16X4(ix_, 0, AE_LT16(ix_, 0));
                x0_ = AE_ADD16(AE_SLAI16S(ix_, 2), 2);

                ix_ = AE_ADD16(ix_, ix_);
                y00_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_3(ix_));
                y10_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_3(ix_));
                y01_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_2(ix_));
                y11_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_2(ix_));
                y02_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_1(ix_));
                y12_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_1(ix_));
                y03_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix_));
                y13_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix_));

                AE_MOVT16X4(y00_, y01_, AE_MOVAB4(4));
                AE_MOVT16X4(y02_, y03_, AE_MOVAB4(4));
                y0_ = AE_SEL16_7632(y00_, y02_);

                AE_MOVT16X4(y10_, y11_, AE_MOVAB4(4));
                AE_MOVT16X4(y12_, y13_, AE_MOVAB4(4));
                y1_ = AE_SEL16_7632(y10_, y12_);

                AE_MULF16X4SS(d0_, d1_, AE_SLAI16S(AE_SUB16(y1_, y0_), 1), AE_SLAI16S(AE_SUB16(x_, x0_), 12));
                z_ = AE_ADD16S(y0_, AE_ROUND16X4F32SASYM(d0_, d1_));

                AE_MOVT16X4(z_, -128, AE_LT16(z_, -128));
                AE_MOVT16X4(z_, 127, AE_LT16(127, z_));

                out[shift + j * 8 + 0] = AE_MOVAD16_3(z_);
                out[shift + j * 8 + 1] = AE_MOVAD16_3(z);
                out[shift + j * 8 + 2] = AE_MOVAD16_2(z_);
                out[shift + j * 8 + 3] = AE_MOVAD16_2(z);
                out[shift + j * 8 + 4] = AE_MOVAD16_1(z_);
                out[shift + j * 8 + 5] = AE_MOVAD16_1(z);
                out[shift + j * 8 + 6] = AE_MOVAD16_0(z_);
                out[shift + j * 8 + 7] = AE_MOVAD16_0(z);
            }

            for (j = (((w - sh_in) >> 3) << 3); j < w; j++)
            {
                int8_t p = in[i*sz->stride + j];
                ae_int16x4 y0, y1;
                ae_int16x4 x, ix, x0, z;
                ae_int32x2 d0, d1;
                x = AE_MOVDA16(p);
                x = AE_ADD16(x, 128);
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);
                ix = AE_ADD16(ix, ix);
                y0 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
                y1 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                z = AE_ADD16(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, -128, AE_LT16(z, -128));
                AE_MOVT16X4(z, 127, AE_LT16(127, z));

                out[i*sz->stride + j] = AE_MOVAD16_0(z);
            }
        }
        else
        {
            pIn = (const ae_int16x4*)(in + i*istride + sh_in);
            pOut = (ae_int16x4*)(out + i*istride + sh_out);
            al_in = AE_LA64_PP(pIn);
            for (j = 0; j < sh_in; j++)
            {
                int8_t p = in[i*sz->stride + j];
                ae_int16x4 y0, y1;
                ae_int16x4 x, ix, x0, z;
                ae_int32x2 d0, d1;
                x = AE_MOVDA16(p);
                x = AE_ADD16(x, 128);
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);
                ix = AE_ADD16(ix, ix);
                y0 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
                y1 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                z = AE_ADD16(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, -128, AE_LT16(z, -128));
                AE_MOVT16X4(z, 127, AE_LT16(127, z));

                out[i*sz->stride + j] = AE_MOVAD16_0(z);
            }
            for (j = 0; j < ((w - sh_in) >> 3); j++)
            {
                ae_int16x4 y00, y01, y02, y03;
                ae_int16x4 y10, y11, y12, y13;
                ae_int16x4 x, ix, x0, p, z, y0, y1, t0;
                ae_int32x2 d0, d1;
                ae_int16x4  y00_, y10_, y01_, y11_;
                ae_int16x4  y02_, y12_, y03_, y13_;
                ae_int16x4 x_, ix_, x0_, z_, y0_, y1_;
                ae_int32x2 d0_, d1_;
                AE_LA16X4_IP(p, al_in, pIn);
                p = AE_XOR16(p, (int16_t)(0x8080));
                x = AE_AND16(p, (int16_t)(0xff00));
                x = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x), 8));
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));
                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);

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

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                y0 = AE_ADD16(y0, 128);
                z = AE_ADD16(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, 0, AE_LT16(z, 0));
                AE_MOVT16X4(z, 255, AE_LT16(255, z));


                x_ = AE_AND16(p, 0x00ff);
                ix_ = AE_SRAI16(AE_SUB16(x_, 2), 2);
                AE_MOVT16X4(ix_, 62, AE_LT16(62, ix_));
                AE_MOVT16X4(ix_, 0, AE_LT16(ix_, 0));
                x0_ = AE_ADD16(AE_SLAI16S(ix_, 2), 2);

                ix_ = AE_ADD16(ix_, ix_);
                y00_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_3(ix_));
                y10_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_3(ix_));
                y01_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_2(ix_));
                y11_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_2(ix_));
                y02_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_1(ix_));
                y12_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_1(ix_));
                y03_ = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix_));
                y13_ = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix_));

                AE_MOVT16X4(y00_, y01_, AE_MOVAB4(4));
                AE_MOVT16X4(y02_, y03_, AE_MOVAB4(4));
                y0_ = AE_SEL16_7632(y00_, y02_);

                AE_MOVT16X4(y10_, y11_, AE_MOVAB4(4));
                AE_MOVT16X4(y12_, y13_, AE_MOVAB4(4));
                y1_ = AE_SEL16_7632(y10_, y12_);

                AE_MULF16X4SS(d0_, d1_, AE_SLAI16S(AE_SUB16(y1_, y0_), 1), AE_SLAI16S(AE_SUB16(x_, x0_), 12));
                y0_ = AE_ADD16(y0_, 128);
                z_ = AE_ADD16S(y0_, AE_ROUND16X4F32SASYM(d0_, d1_));

                AE_MOVT16X4(z_, 0, AE_LT16(z_, 0));
                AE_MOVT16X4(z_, 255, AE_LT16(255, z_));

                z = AE_MOVINT16X4_FROMINT32X2(AE_SRLA32(AE_MOVINT32X2_FROMINT16X4(z), -8));
                t0 = AE_OR16(z, z_);

                t0 = AE_XOR16(t0, (int16_t)(0x8080));
                AE_SA16X4_IP(t0, al_out, pOut);

            }
            AE_SA64POS_FP(al_out, pOut);

            for (j = (((w - sh_in) >> 3) << 3); j < w; j++)
            {
                int8_t p = in[i*sz->stride + j];
                ae_int16x4 y0, y1;
                ae_int16x4 x, ix, x0, z;
                ae_int32x2 d0, d1;
                x = AE_MOVDA16(p);
                x = AE_ADD16(x, 128);
                ix = AE_SRAI16(AE_SUB16(x, 2), 2);
                AE_MOVT16X4(ix, 62, AE_LT16(62, ix));
                AE_MOVT16X4(ix, 0, AE_LT16(ix, 0));

                x0 = AE_ADD16(AE_SLAI16S(ix, 2), 2);
                ix = AE_ADD16(ix, ix);
                y0 = AE_L16_X((const ae_int16 *)tbl, AE_MOVAD16_0(ix));
                y1 = AE_L16_X((const ae_int16 *)(tbl + 1), AE_MOVAD16_0(ix));

                AE_MULF16X4SS(d0, d1, AE_SLAI16S(AE_SUB16(y1, y0), 1), AE_SLAI16S(AE_SUB16(x, x0), 12));
                z = AE_ADD16(y0, AE_ROUND16X4F32SASYM(d0, d1));

                AE_MOVT16X4(z, -128, AE_LT16(z, -128));
                AE_MOVT16X4(z, 127, AE_LT16(127, z));

                out[i*sz->stride + j] = AE_MOVAD16_0(z);
            }
        }
    }
}/*imgnorm_gs8_nonlinear()*/

