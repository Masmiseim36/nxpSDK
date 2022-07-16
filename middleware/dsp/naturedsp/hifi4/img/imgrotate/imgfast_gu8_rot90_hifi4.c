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
void imgfast_gu8_rot90 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  const int8_t * restrict src0;
  const int8_t * restrict src1;
  const int8_t * restrict src2;
  const int8_t * restrict src3;
      ae_int32 * restrict dst0;
      ae_int32 * restrict dst1;
      ae_int32 * restrict dst2;
      ae_int32 * restrict dst3;
  int i, j, w, h, ostride, istride;
  (void)pScr;
  ae_f16x4 x0, x1, x2, x3;
  ae_int16x4 y0, y1, y2, y3;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  NASSERT_ALIGN(outImg, ALIGNMENT);
  NASSERT_ALIGN(inImg, ALIGNMENT);
  imgsize_validate(inSz, 1, 1);
  imgsize_validate(outSz, 1, 1);
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;

  for (i = 0; i<(w >> 2); i++)
  {
    src0 = (const int8_t *)((uintptr_t)inImg + ((0 + 4 * i) * istride + (h)-(h & 3)) * sizeof(int8_t)) - 4;
    src1 = (const int8_t *)((uintptr_t)inImg + ((1 + 4 * i) * istride + (h)-(h & 3)) * sizeof(int8_t)) - 4;
    src2 = (const int8_t *)((uintptr_t)inImg + ((2 + 4 * i) * istride + (h)-(h & 3)) * sizeof(int8_t)) - 4;
    src3 = (const int8_t *)((uintptr_t)inImg + ((3 + 4 * i) * istride + (h)-(h & 3)) * sizeof(int8_t)) - 4;


    dst0 = (ae_int32 *)((uintptr_t)outImg + (((h & 3) + 0)* ostride)*sizeof(uint8_t)) + i;
    dst1 = (ae_int32 *)((uintptr_t)outImg + (((h & 3) + 1)* ostride)*sizeof(uint8_t)) + i;
    dst2 = (ae_int32 *)((uintptr_t)outImg + (((h & 3) + 2)* ostride)*sizeof(uint8_t)) + i;
    dst3 = (ae_int32 *)((uintptr_t)outImg + (((h & 3) + 3)* ostride)*sizeof(uint8_t)) + i;
    for (j=0; j<(h>>2); j++)
    {
      AE_L8X4F_IP(x0, src0, -4*(int)sizeof(uint8_t));
      AE_L8X4F_IP(x1, src1, -4*(int)sizeof(uint8_t));
      AE_L8X4F_IP(x2, src2, -4*(int)sizeof(uint8_t));
      AE_L8X4F_IP(x3, src3, -4*(int)sizeof(uint8_t));

      y0 = AE_SEL16_7362(x2, x0);
      y1 = AE_SEL16_7362(x3, x1); 
      y2 = AE_SEL16_5140(x2, x0);
      y3 = AE_SEL16_5140(x3, x1);

      x0 = AE_OR16(y3, AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(y2), 8)));
      x1 = AE_OR16(y1, AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(y0), 8)));
    
      AE_S32_L_XP(AE_MOVINT32X2_FROMINT16X4(x0), dst0, 4 * ostride*sizeof(uint8_t));
      AE_S32_L_XP(AE_MOVINT32X2_FROMINT16X4(AE_SEL16_7632(x0, x0)), dst1, 4 * ostride*sizeof(uint8_t));
      AE_S32_L_XP(AE_MOVINT32X2_FROMINT16X4(x1), dst2, 4 * ostride*sizeof(uint8_t) );
      AE_S32_L_XP(AE_MOVINT32X2_FROMINT16X4(AE_SEL16_7632(x1, x1)), dst3, 4 * ostride*sizeof(uint8_t) );
    }
  }
  __Pragma("no_reorder");
  for (j = 0; j<(h & 3); j++)
  {
    src0 = (const int8_t *)((uintptr_t)inImg + (0 * istride + (h - 1 - j)) * sizeof(uint8_t));
    src1 = (const int8_t *)((uintptr_t)inImg + (1 * istride + (h - 1 - j)) * sizeof(uint8_t));
    src2 = (const int8_t *)((uintptr_t)inImg + (2 * istride + (h - 1 - j)) * sizeof(uint8_t));
    src3 = (const int8_t *)((uintptr_t)inImg + (3 * istride + (h - 1 - j)) * sizeof(uint8_t));

    dst0 = (ae_int32 *)((uintptr_t)outImg + (j * ostride)*sizeof(uint8_t));
    for (i = 0; i<(w >> 2); i++)
    {
      unsigned char  p0, p1, p2, p3;
      p0 = XT_L8UI(( unsigned char*)src0, 0 * (int)(sizeof(uint8_t) * 4 * istride));
      p1 = XT_L8UI(( unsigned char*)src1, 0 * (int)(sizeof(uint8_t) * 4 * istride));
      p2 = XT_L8UI(( unsigned char*)src2, 0 * (int)(sizeof(uint8_t) * 4 * istride));
      p3 = XT_L8UI(( unsigned char*)src3, 0 * (int)(sizeof(uint8_t) * 4 * istride));

      XT_S8I(p0,( signed char *)dst0,0 * sizeof(uint8_t));
      XT_S8I(p1,( signed char *)dst0,1 * sizeof(uint8_t));
      XT_S8I(p2,( signed char *)dst0,2 * sizeof(uint8_t));
      XT_S8I(p3,( signed char *)dst0,3 * sizeof(uint8_t));

      src0 = (const int8_t *)((uintptr_t)src0 + 4 * (istride)*sizeof(uint8_t));
      src1 = (const int8_t *)((uintptr_t)src1 + 4 * (istride)*sizeof(uint8_t));
      src2 = (const int8_t *)((uintptr_t)src2 + 4 * (istride)*sizeof(uint8_t));
      src3 = (const int8_t *)((uintptr_t)src3 + 4 * (istride)*sizeof(uint8_t));
      dst0 = (    ae_int32 *)((uintptr_t)dst0 + 4 * sizeof(uint8_t));
    }
  }
  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {
    src0 = (const int8_t *)((uintptr_t)inImg + ((w - 1 - i) * istride + (h - 1)) * sizeof(int8_t));
    dst0 = (ae_int32 *)((uintptr_t)outImg + (w - 1 - i) *sizeof(int8_t));
    for (j = 0; j<h; j++)
    {
      unsigned char p;
      p = XT_L8UI(( unsigned char*)src0,0);
      XT_S8I(p,( signed char *)dst0, 0); 
      src0 = (const int8_t *)((uintptr_t)src0 - (int)sizeof(int8_t));
      dst0 = (    ae_int32 *)((uintptr_t)dst0 + (ostride)*sizeof(uint8_t));
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
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i = 0; i<h; i++)
  {
    for (j = 0; j<w; j++)    out[i*ostride + j] = in[j*istride + (h - 1 - i)];
  }
#endif
} /* imgfast_gu8_rot90() */
