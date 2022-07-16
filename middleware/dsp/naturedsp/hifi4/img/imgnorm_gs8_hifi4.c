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

void imgnorm_gs8(void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt)
{
    ae_int32x2 dQ31, cQ23;

    const int8_t * restrict in = (const int8_t *)inImg;
    int8_t * restrict out = (int8_t *)outImg;
    int i, j;
    const ae_int16x4 * restrict pIn;
    const signed char * restrict pIn_;
          ae_int16x4 * restrict pOut;
    int16_t pmax, pmin;
    int16_t pmax1, pmin1;
    ae_valign al_in, al_out;
    int h = (int)sz->height;
    int w = (int)sz->width;
    int istride = sz->stride;
    ae_int16x4 pmaxv, pminv;
    imgsize_validate(sz, 1, 0);
    minInt = XT_MAX(minInt, -128);
    maxInt = XT_MIN(maxInt, 127);
    // find min/max values
    {
        ae_int16x4 p;
        pmaxv = -32768; pminv = 32767;
        pmax = -128; pmin = 127;
        int tmp;
        for (i = 0; i<h; i++)
        {
            pIn_ = (const signed char*)(in + i*istride);
            int shift = ((uintptr_t)pIn_ - (((uintptr_t)pIn_)&~3));
            if (shift > 0) shift = 4 - shift;
            pIn_ = (const signed char*)(in + i*istride + shift);
            for (j = 0; j < shift; j++)
            {
                int8_t p = in[i*sz->stride + j];
                pmax = XT_MAX(pmax, p); pmin = XT_MIN(pmin, p);
            }
            tmp = ((w - shift) >> 2) << 2;
            for (j = 0; j<(tmp>>2); j++)
            {
                AE_L8X4F_IP(p, pIn_, 4*sizeof(int8_t));
                AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
                AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));

            }
            for (j = shift + tmp; j < w; j++)
            {
                int8_t p = in[i*sz->stride + j];
                pmax = XT_MAX(pmax, p); pmin = XT_MIN(pmin, p);
            }
        }
        p = AE_SEL16_5432(pmaxv, pmaxv); AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
        p = AE_SEL16_5432(pminv, pminv); AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));
        p = AE_SEL16_4321(pmaxv, pmaxv); AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
        p = AE_SEL16_4321(pminv, pminv); AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));
        pminv = AE_SRAI16(pminv, 8);
        pmaxv = AE_SRAI16(pmaxv, 8);
    }
    pmax1 = AE_MOVAD16_0(pmaxv);
    pmin1 = AE_MOVAD16_0(pminv);
    pmax = XT_MAX(pmax, pmax1);
    pmin = XT_MIN(pmin, pmin1);

    pmax1 = -128; pmin1 = 127;
    for (i = 0; i<(int)sz->height; i++)
        for (j = 0; j<(int)sz->width; j++)
        {
            int8_t p = in[i*sz->stride + j];
            pmax1 = XT_MAX(pmax1, p); pmin1 = XT_MIN(pmin1, p);
        }


    if (pmax == pmin)
    {   // force output to minInt
        maxInt = minInt;
        pmax=-127; pmin=-128;
    }

    //    dQ31=AE_MOVDA32((pmax==pmin) ? 0x7fffffff: ((uint32_t)0x80000000/(pmax-pmin)));
    {
        ae_int64 r = 0x80000000ULL;
        ae_int32x2 t = AE_MOVDA32(pmax - pmin);
        xtbool2 ble1 = AE_LE32(t, 1);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);

        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);
        AE_DIV64D32_H(r, t);    AE_DIV64D32_H(r, t);

        dQ31 = AE_TRUNCA32F64S(AE_AND64(r, 0x7fffffff), 32);
        AE_MOVT32X2(dQ31, 0x7fffffff, ble1);
    }

    cQ23 = AE_MULFP32X2RAS((maxInt - minInt) << 23, dQ31);

    ae_int16x4 p, p0, p1, y0, y1;
    ae_int16x4 t0, t1;
    ae_int32x2 ch0, cl0, ch1, cl1;
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
                p0 = AE_MOVDA16(in[i*istride + j]); p0 = AE_SLAI16S(p0, 7);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(ch0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(AE_SRAI16(y0, 8), 0xff);
                out[i*istride + j] = (int8_t)AE_MOVAD16_0(y0);
            }
            for (j = 0; j < ((w - sh_in) >> 3); j++)
            {
                int shift = i*istride + sh_in;
                AE_LA16X4_IP(p, al_in, pIn);
                p0 = AE_AND16(p, AE_MOVDA16(0xff00));

                p0 = AE_SRAI16(p0, 1);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                cl0 = AE_MULFP32X16X2RAS_L(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(cl0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(y0, AE_MOVDA16(0xff00));

                p = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(p), 8));
                p1 = AE_AND16(p, (int16_t)(0xff00));
                p1 = AE_SRAI16(p1, 1);
                p1 = AE_SUB16(p1, pmin << 7);
                ch1 = AE_MULFP32X16X2RAS_H(cQ23, p1); // ->Q15
                cl1 = AE_MULFP32X16X2RAS_L(cQ23, p1); // ->Q15
                y1 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch1, 8), AE_SLAI32S(cl1, 8)); // ->Q7
                y1 = AE_ADD16S(y1, (ae_int16x4)(minInt << 7));
                y1 = AE_ADD16(y1, y1);
                y1 = AE_AND16(y1, (int16_t)(0xff00));
                //save result
                t1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y1), 8));
                t0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y0), 8));
                out[shift + j * 8 + 0] = (int8_t)AE_MOVAD16_3(t1);
                out[shift + j * 8 + 1] = (int8_t)AE_MOVAD16_3(t0);
                out[shift + j * 8 + 2] = (int8_t)AE_MOVAD16_2(t1);
                out[shift + j * 8 + 3] = (int8_t)AE_MOVAD16_2(t0);
                out[shift + j * 8 + 4] = (int8_t)AE_MOVAD16_1(t1);
                out[shift + j * 8 + 5] = (int8_t)AE_MOVAD16_1(t0);
                out[shift + j * 8 + 6] = (int8_t)AE_MOVAD16_0(t1);
                out[shift + j * 8 + 7] = (int8_t)AE_MOVAD16_0(t0);
            }

            for (j = (((w - sh_in) >> 3) << 3); j < w; j++)
            {
                p0 = AE_MOVDA16(in[i*istride + j]); p0 = AE_SLAI16S(p0, 7);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(ch0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(AE_SRAI16(y0, 8), 0xff);
                out[i*istride + j] = (int8_t)AE_MOVAD16_0(y0);
            }
        }
        else
        {
            pIn = (const ae_int16x4*)(in + i*istride + sh_in);
            pOut = (ae_int16x4*)(out + i*istride + sh_out);
            al_in = AE_LA64_PP(pIn);
            for (j = 0; j < sh_in; j++)
            {
                p0 = AE_MOVDA16(in[i*istride + j]); p0 = AE_SLAI16S(p0, 7);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(ch0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(AE_SRAI16(y0, 8), 0xff);
                out[i*istride + j] = (int8_t)AE_MOVAD16_0(y0);
            }
            for (j = 0; j < ((w - sh_in) >> 3); j++)
            {
                AE_LA16X4_IP(p, al_in, pIn);
                p0 = AE_AND16(p, AE_MOVDA16(0xff00));

                p0 = AE_SRAI16(p0, 1);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                cl0 = AE_MULFP32X16X2RAS_L(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(cl0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(y0, (int16_t)(0xff00));

                p = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(p), 8));
                p1 = AE_AND16(p, (int16_t)(0xff00));
                p1 = AE_SRAI16(p1, 1);
                p1 = AE_SUB16(p1, pmin << 7);
                ch1 = AE_MULFP32X16X2RAS_H(cQ23, p1); // ->Q15
                cl1 = AE_MULFP32X16X2RAS_L(cQ23, p1); // ->Q15
                y1 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch1, 8), AE_SLAI32S(cl1, 8)); // ->Q7
                y1 = AE_ADD16S(y1, (ae_int16x4)(minInt << 7));
                y1 = AE_ADD16(y1, y1);
                y1 = AE_AND16(y1, (int16_t)(0xff00));
                //save result
                t0 = y0;
                t1 = y1;
                t1 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t1), 8));
                t0 = AE_OR16(t0, t1);

                AE_SA16X4_IP(t0, al_out, pOut);
            }
            AE_SA64POS_FP(al_out, pOut);

            for (j = (((w - sh_in) >> 3) << 3); j < w; j++)
            {
                p0 = AE_MOVDA16(in[i*istride + j]); p0 = AE_SLAI16S(p0, 7);
                p0 = AE_SUB16(p0, pmin << 7);
                ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
                y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(ch0, 8)); // ->Q7
                y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
                y0 = AE_ADD16(y0, y0);
                y0 = AE_AND16(AE_SRAI16(y0, 8), 0xff);
                out[i*istride + j] = (int8_t)AE_MOVAD16_0(y0);
            }
        }
    }
}/*imgnorm_gs8()*/
