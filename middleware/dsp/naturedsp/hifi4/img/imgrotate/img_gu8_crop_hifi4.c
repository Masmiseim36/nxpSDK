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
#include "imgrotate_common.h"

/*--------------------------------------------------------------------------
 out-of-place cropping with conversion from signed 16-bit to destination format
    Input:
    left,up    left/up corner
    inImg,inSz input image in intermediate format
    outSz      output size
    Output:
    outImg     output image
 --------------------------------------------------------------------------*/
void img_gu8_crop(void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up)
{
#if 1
  const ae_int16x4 * restrict src = (const ae_int16x4 *)inImg;
        ae_int16x4 * restrict dst = (      ae_int16x4 *)outImg;

  int i, j, w, h, ostride, istride, _d;
  ae_valign aS, aD;
  ae_int16x4 x0, x1, y0, y1;
  ae_int16x4 _64;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  w=outSz->width;
  h=outSz->height;
  ostride=outSz->stride;
  istride=inSz->stride;
  imgsize_validate(inSz,2,1);
  imgsize_validate(outSz,1,0);
  NASSERT_ALIGN(outImg,1);        /* output non-aligned, input aligned */
  NASSERT_ALIGN(inImg,ALIGNMENT);
  _64 = 64;
  src = (const ae_int16x4 *)((uintptr_t)inImg + (up* istride + left) * sizeof(int16_t));
  for (i = 0; i<h; i++)
  {
    src = (const ae_int16x4 *)((uintptr_t)inImg + (up * istride + left + i* istride) * sizeof(int16_t));
    dst = (      ae_int16x4 *)((uint8_t*)outImg + i * ostride);

    _d = (((uintptr_t)dst) & 1);
    if (_d == 0)
    {
      aS = AE_LA64_PP(src);
      aD = AE_ZALIGN64();
      for (j = 0; j<(w >> 3); j++)
      {
        xtbool4 b0, b1;
        AE_LA16X4_IP(x0, aS, src);
        AE_LA16X4_IP(x1, aS, src);
        b0 = AE_LT16(x0, AE_ZERO16());
        b1 = AE_LT16(x1, AE_ZERO16());
        x0 = AE_ADD16S(x0, _64);
        x1 = AE_ADD16S(x1, _64);
        x0 = AE_SRAI16(x0, 7);
        x1 = AE_SRAI16(x1, 7);
        AE_MOVT16X4(x0, AE_ZERO16(), b0);
        AE_MOVT16X4(x1, AE_ZERO16(), b1);
        y0 = AE_SEL16_7531(x0, x1);
        y1 = AE_SEL16_6420(x0, x1);
        x0 = AE_OR16(y0, AE_MOVINT16X4_FROMINT64(AE_SLLI64(AE_MOVINT64_FROMINT16X4(y1), 8)));
        AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);
      for (j = 0; j<(w & 7); j++)
      {
        int16_t c0;
        xtbool4 b0;
        AE_L16_IP(x0, castxcc(ae_int16, src), sizeof(int16_t));
        b0 = AE_LT16(x0, AE_ZERO16());
        x0 = AE_ADD16S(x0, _64);
        x0 = AE_SRAI16(x0, 7);
        AE_MOVT16X4(x0, AE_ZERO16(), b0);
        c0 = AE_MOVAD16_0(x0);
        XT_S8I(c0, (signed char *)dst, 0);
        dst = (ae_int16x4 *)XT_ADD((uintptr_t)dst, sizeof(int8_t));
      }
    }
    else
    {
      {
        int16_t c0;
        xtbool4 b0;
        AE_L16_IP(x0, castxcc(ae_int16, src), sizeof(int16_t));
        b0 = AE_LT16(x0, AE_ZERO16());
        x0 = AE_ADD16S(x0, _64);
        x0 = AE_SRAI16(x0, 7);
        AE_MOVT16X4(x0, AE_ZERO16(), b0);
        c0 = AE_MOVAD16_0(x0);
        XT_S8I(c0, (signed char *)dst, 0);
        dst = (ae_int16x4 *)XT_ADD((uintptr_t)dst, sizeof(int8_t));
      }
      aS = AE_LA64_PP(src); 
      aD = AE_ZALIGN64();
      for (j = 0; j<((w - 1) >> 3); j++)
      {
        xtbool4 b0, b1;
        AE_LA16X4_IP(x0, aS, src);
        AE_LA16X4_IP(x1, aS, src);
        b0 = AE_LT16(x0, AE_ZERO16());
        b1 = AE_LT16(x1, AE_ZERO16());
        x0 = AE_ADD16S(x0, _64);
        x1 = AE_ADD16S(x1, _64);
        x0 = AE_SRAI16(x0, 7);
        x1 = AE_SRAI16(x1, 7);
        AE_MOVT16X4(x0, AE_ZERO16(), b0);
        AE_MOVT16X4(x1, AE_ZERO16(), b1);
        y0 = AE_SEL16_7531(x0, x1);
        y1 = AE_SEL16_6420(x0, x1);
        x0 = AE_OR16(y0, AE_MOVINT16X4_FROMINT64(AE_SLLI64(AE_MOVINT64_FROMINT16X4(y1), 8)));
        AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);
      for (j = 0; j<((w - 1)& 7); j++)
      {
        int16_t c0;
        xtbool4 b0;
        AE_L16_IP(x0, castxcc(ae_int16, src), sizeof(int16_t));
        b0 = AE_LT16(x0, AE_ZERO16());
        x0 = AE_ADD16S(x0, _64);
        x0 = AE_SRAI16(x0, 7);
        AE_MOVT16X4(x0, AE_ZERO16(), b0);
        c0 = AE_MOVAD16_0(x0);
        XT_S8I(c0, (signed char *)dst, 0);
        dst = (ae_int16x4 *)XT_ADD((uintptr_t)dst, sizeof(int8_t));
      }
    }
  }

#else
    const int16_t* restrict in;
          uint8_t* restrict out;
    int i,j,w,h,ostride,istride;
    (void)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    w=outSz->width;
    h=outSz->height;
    ostride=outSz->stride;
    istride=inSz->stride;
    imgsize_validate(inSz,2,1);
    imgsize_validate(outSz,1,0);
    NASSERT_ALIGN(outImg,1);        /* output non-aligned, input aligned */
    NASSERT_ALIGN(inImg,ALIGNMENT);

    in =(const int16_t*)inImg;
    out=(      uint8_t*)outImg;
    in+=up*istride+left;
    for (i=0; i<h; i++,in+=istride)
    {
        for (j=0; j<w; j++)
        {
          *out++ = (uint8_t)(S_shr_s(S_add_ss(XT_MAX(in[j], 0), (1 << 6)), 7));
          //*out++ = (uint8_t)((XT_MAX(in[j], 0) + (1 << 6)) >> 7);
        }
        out+=ostride-w;
    }
#endif
} /* img_gu8_crop() */
