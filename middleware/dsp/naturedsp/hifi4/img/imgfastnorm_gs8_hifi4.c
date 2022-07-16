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

void imgfastnorm_gs8  ( void * restrict outImg, const void * restrict inImg, const imgsize_t* sz, int minInt, int maxInt)
{
    ae_int32x2 dQ31,cQ23;
    const int8_t * restrict in =(const int8_t *)inImg;
          int8_t * restrict out=(      int8_t *)outImg;
    const signed char * restrict pIn;
          ae_int16x4 * restrict pOut;
    int h=(int)sz->height;
    int w=(int)sz->width;
    int istride=sz->stride;
    int i,j;
    int16_t pmax,pmin;
    ae_int16x4 pmaxv, pminv;
    NASSERT(inImg!=NULL);
    NASSERT(outImg!=NULL);
    NASSERT_ALIGN(inImg,ALIGNMENT);
    NASSERT_ALIGN(outImg,ALIGNMENT);
    imgsize_validate(sz,1,1);
    minInt=XT_MAX(minInt,-128);
    maxInt=XT_MIN(maxInt,127);
    // find min/max values
    pmax=-128; pmin=127;

    // find min/max values
    {
        int k, K;
        xtbool4 bmask;
        ae_int16x4 p, p0, p1;
        pmaxv = -32768; pminv = 32767;
        K = (w - 1) >> 2;
        k = w - (K << 2); //1...4
        bmask = AE_MOVAB4((0xf0 >> k) & 0xf); // 0x1000 for 1, 0x1100 for 2, 0x1110 for 3, 0x1111 for 4
        for (i = 0; i<h; i++)
        {
            pIn = (const signed char*)(in + i*istride);
            for (j = 0; j<K; j++)
            {
                AE_L8X4F_IP(p, pIn, 4 * sizeof(int8_t));
                AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
                AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));
            }
            AE_L8X4F_IP(p, pIn, 4 * sizeof(int8_t));
            p0 = p; p1 = p;
            AE_MOVF16X4(p0, -32768, bmask);
            AE_MOVF16X4(p1, 32767, bmask);
            AE_MOVT16X4(pmaxv, p0, AE_LT16(pmaxv, p0));
            AE_MOVT16X4(pminv, p1, AE_LT16(p1, pminv));
        }
        p = AE_SEL16_5432(pmaxv, pmaxv); AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
        p = AE_SEL16_5432(pminv, pminv); AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));
        p = AE_SEL16_4321(pmaxv, pmaxv); AE_MOVT16X4(pmaxv, p, AE_LT16(pmaxv, p));
        p = AE_SEL16_4321(pminv, pminv); AE_MOVT16X4(pminv, p, AE_LT16(p, pminv));
        pminv = AE_SRAI16(pminv, 8);
        pmaxv = AE_SRAI16(pmaxv, 8);
    }
    
    pmax = AE_MOVAD16_0(pmaxv);
    pmin = AE_MOVAD16_0(pminv);

    if (pmax==pmin) 
    {   // force output to minInt
        maxInt=minInt;
        pmax=-127; pmin=-128;
    }

    {
        ae_int64 r=0x80000000ULL;
        ae_int32x2 t=AE_MOVDA32(pmax-pmin);
        xtbool2 ble1=AE_LE32(t,1);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);

        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
        AE_DIV64D32_H(r,t);    AE_DIV64D32_H(r,t);
    
        dQ31=AE_TRUNCA32F64S(AE_AND64(r,0x7fffffff),32);
        AE_MOVT32X2(dQ31,0x7fffffff,ble1);
    }

    cQ23=AE_MULFP32X2RAS((maxInt-minInt)<<23,dQ31);

    ae_int16x4 p0, p1, y0, y1;
    ae_int16x4 t0, t1;
    ae_int32x2 ch0, cl0, ch1, cl1;
    for (i = 0; i<h; i++)
    {
        pIn = (const signed char*)(in + i*istride);
        pOut = (ae_int16x4*)(out + i*istride);
        for (j = 0; j<(w>>3); j++)
        {
            AE_L8X4F_IP(p0, pIn, 4*sizeof(int8_t));
            p0 = AE_SRAI16(p0, 1);    
            p0 = AE_SUB16(p0, pmin << 7);
            ch0 = AE_MULFP32X16X2RAS_H(cQ23, p0); // ->Q15
            cl0 = AE_MULFP32X16X2RAS_L(cQ23, p0); // ->Q15
            y0 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch0, 8), AE_SLAI32S(cl0, 8)); // ->Q7
            y0 = AE_ADD16S(y0, (ae_int16x4)(minInt << 7));
            y0 = AE_ADD16(y0, y0);
            y0 = AE_AND16(y0, (int16_t)(0xff00));

            AE_L8X4F_IP(p1, pIn, 4 * sizeof(int8_t));
            p1 = AE_SRAI16(p1, 1);
            p1 = AE_SUB16(p1, pmin << 7);
            ch1 = AE_MULFP32X16X2RAS_H(cQ23, p1); // ->Q15
            cl1 = AE_MULFP32X16X2RAS_L(cQ23, p1); // ->Q15
            y1 = AE_ROUND16X4F32SASYM(AE_SLAI32S(ch1, 8), AE_SLAI32S(cl1, 8)); // ->Q7
            y1 = AE_ADD16S(y1, (ae_int16x4)(minInt << 7));
            y1 = AE_ADD16(y1, y1);
            y1 = AE_AND16(y1, (int16_t)(0xff00));
            //save result
            t0 = AE_SEL16_7531(y0, y1);
            t1 = AE_SEL16_6420(y0, y1);
            t0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(t0), 8));
            t0 = AE_OR16(t0, t1);

            AE_S16X4_IP(t0, pOut, sizeof(ae_int16x4));
        }
        for (j = ((w>>3) << 3); j < w; j++)
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
}/*imgfastnorm_gs8()*/

