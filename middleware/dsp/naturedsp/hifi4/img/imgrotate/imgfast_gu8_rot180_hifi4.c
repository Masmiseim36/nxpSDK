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
rotation by 0,90,180,270 with no format conversion
Input:
inImg, inSz  image and its size
outSz        output image size
Output:
outImg       image
--------------------------------------------------------------------------*/
void imgfast_gu8_rot180 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  int i, j, w, h, ostride, istride;
  const int8_t * restrict src = (const int8_t *)inImg;
        int8_t * restrict dst = (      int8_t *)outImg;

  (void)pScr;
  ae_valign aD;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  NASSERT_ALIGN(outImg, ALIGNMENT);
  NASSERT_ALIGN(inImg, ALIGNMENT);
  imgsize_validate(inSz, 1, 1);
  imgsize_validate(outSz, 1, 1);
  w = inSz->width;
  h = inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;

  if ((w%2)!=0)
  {

    src = (const int8_t *)((uintptr_t)inImg + ((h - 1)*istride + (w >> 3) * 8)*sizeof(uint8_t)) - 4;
    dst = (      int8_t *)((uintptr_t)outImg + (w & 7)*sizeof(uint8_t)) - 1;
    
    for (i = 0; i<h; i++)
    {
      ae_int64 w0, w1, w2, z0;
      ae_f16x4 x0, x1, y0, y1;
      unsigned char p;
      aD = AE_ZALIGN64();
      z0 = AE_ZERO64();
      for (j = 0; j<(w >> 3); j++) 
      {
        AE_L8X4F_IP(x0, src, -4 * (int)sizeof(uint8_t));
        AE_L8X4F_IP(x1, src, -4 * (int)sizeof(uint8_t));

        y0 = AE_SEL16_7531(x1, x0);
        y1 = AE_SEL16_6420(x1, x0);
        x0 = AE_OR16(y0, AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(y1), 8)));
        w0 = AE_MOVINT64_FROMINT16X4(x0);
        w1 = AE_SRLI64(w0, 56);
        w2 = AE_SLAI64(w0, 8);
        w2 = AE_OR64(z0, w2);
        x0 = AE_MOVINT16X4_FROMINT64(w2); 
        x0 = AE_SHORTSWAP(x0);
        AE_SA16X4_IP(x0, aD, castxcc(ae_int16x4,dst));
        z0 = w1; 
      }
      AE_SA64POS_FP(aD, dst);
      p = AE_MOVAD16_0(AE_MOVINT16X4_FROMINT64(z0));
      XT_S8I(p, (signed char *)dst, 0);
      src = (const int8_t *)((uintptr_t)src - (istride - (w >> 3) * 8)*sizeof(uint8_t));
      dst = (      int8_t *)((uintptr_t)dst + (ostride - (w >> 3) * 8)*sizeof(uint8_t));
    }
    __Pragma("no_reorder");
    for (i = 0; i<(w & 7); i++)
    {
      src = (const int8_t *)((uint8_t*)inImg + (h - 1)*istride + w - 1 - i);
      dst = (      int8_t *)((uint8_t*)outImg + i);
      for (j = 0; j<h; j++)
      {
        unsigned char p;
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const int8_t *)((uintptr_t)src - (istride)*sizeof(uint8_t));
        dst = (      int8_t *)((uintptr_t)dst + (ostride)*sizeof(uint8_t));
      }
    }
  }
  else
  {
    src = (const int8_t *)((uintptr_t)inImg + ((h - 1)*istride + (w >> 3) * 8)*sizeof(uint8_t)) - 4;
    dst = (      int8_t *)((uintptr_t)outImg + (w & 7)*sizeof(uint8_t));
    aD = AE_ZALIGN64();
    for (i = 0; i<h; i++)
    {
      for (j = 0; j<(w >> 3); j++)
      {
        ae_f16x4 x0, x1, y0, y1;
        AE_L8X4F_IP(x0, src, -4 * (int)sizeof(uint8_t));
        AE_L8X4F_IP(x1, src, -4 * (int)sizeof(uint8_t));

        y0 = AE_SEL16_7531(x1, x0);
        y1 = AE_SEL16_6420(x1, x0);
        x0 = AE_OR16(y0, AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(y1), 8)));
        x0 = AE_SHORTSWAP(x0);
        AE_SA16X4_IP(x0, aD, castxcc(ae_int16x4, dst));
      }
      AE_SA64POS_FP(aD, dst);
      src = (const int8_t *)((uintptr_t)src - (istride - (w >> 3) * 8)*sizeof(uint8_t));
      dst = (      int8_t *)((uintptr_t)dst + (ostride - (w >> 3) * 8)*sizeof(uint8_t));
    }
    __Pragma("no_reorder");
    for (i = 0; i<(w & 7); i++)
    {
      src = (const int8_t *)((uint8_t*)inImg + (h - 1)*istride + w - 1 - i);
      dst = (      int8_t *)((uint8_t*)outImg + i);
      for (j = 0; j<h; j++)
      {
        unsigned char p;
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const int8_t *)((uintptr_t)src - (istride)*sizeof(uint8_t));
        dst = (      int8_t *)((uintptr_t)dst + (ostride)*sizeof(uint8_t));
      }
    }
  }
#else
  int i, j, w, h, ostride, istride;
  const uint8_t * restrict in = (const uint8_t *)inImg;
  uint8_t * restrict out = (uint8_t *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  NASSERT_ALIGN(outImg, ALIGNMENT);
  NASSERT_ALIGN(inImg, ALIGNMENT);
  imgsize_validate(inSz, 1, 1);
  imgsize_validate(outSz, 1, 1);
  w = inSz->width;
  h = inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;
  in += (h - 1)*istride;
  for (i = 0; i<h; i++, in -= istride)
  {
    for (j = 0; j<w; j++) 
    {
      *out++ = in[w - 1 - j];    
    }
    out += ostride - w;
  }
#endif
} /* imgfast_gu8_rot180() */
