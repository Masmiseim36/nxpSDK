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
rotation by 0,90,180,270 conterclockwise with format conversion
the image is placed beginning from the row PADDING and from the column
ALIGNMENT/2. All paddings are written with black pixels
8-bit gs  -> signed 16-bit 1 pixel per 16-bit word
16-bit gs -> signed 16-bit 1 pixel per 16-bit word
RGB       -> signed 16-bit 1 pixel per 64-bit word
Input:
inImg, inSz  image and its size
outSz        output image size
fillColor    filling color
Output:
outImg       image
--------------------------------------------------------------------------*/
void img_gs16_rot270_16(void* pScr,void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor)
{
#if 1
  xtbool4 b0 = AE_MOVBA4(0x3);
  xtbool4 b1 = AE_MOVBA4(0x5);

  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
        ae_int16x4 * restrict dst0;
        ae_int16x4 * restrict dst1;
        ae_int16x4 * restrict dst2;
        ae_int16x4 * restrict dst3;
  int i, j, w, h, ostride, istride, h_out;
  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 x0_, x1_, x2_, x3_;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  NASSERT_ALIGN(outImg, ALIGNMENT);    /* inImg - not aligned, outImg - aligned */
  NASSERT_ALIGN(outImg, 1);
  (void)pScr;
  imgsize_validate(inSz, 2, 0);
  imgsize_validate(outSz, 2, 0);
  ae_valign aS0;
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;

  h_out = outSz->height;
  x0 = fillColor;
  /* filling row PADDING */
  dst0 = (ae_int16x4 *)outImg;
  dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((h_out - (h + PADDING))*ostride)*sizeof(int16_t));
  for (j = 0; j<((PADDING*ostride) >> 2); j++)
  {
    AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
    AE_S16X4_IP(x0, dst1, sizeof(ae_int16x4));
  }
  for (j = 0; j<((PADDING*ostride) & 3); j++)
  {
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
    AE_S16_0_IP(x0, castxcc(ae_int16, dst1), sizeof(int16_t));
  }
  dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((PADDING + h)*ostride)*sizeof(int16_t));
  for (j = 0; j<(int)(((h_out - 2 * PADDING - h)*ostride) >> 2); j++)
  {
    AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
  }
  for (j = 0; j<((h_out - 2 * PADDING - h) & 3); j++)
  {
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
  }
  __Pragma("no_reorder");
  /* filling column PADDING */
  dst0 = (ae_int16x4 *)((uintptr_t)outImg + (PADDING * ostride)*sizeof(int16_t));
  dst1 = (ae_int16x4 *)((uintptr_t)outImg + (PADDING * ostride + ostride - ALIGNMENT / 2)*sizeof(int16_t));
  for (i = 0; i<h; i++)
  {
    AE_S16X4_XP(x0, dst0, ostride * sizeof(int16_t));
    AE_S16X4_XP(x0, dst1, ostride * sizeof(int16_t));
  }
  for (j = 0; j<((ostride - ALIGNMENT - w + 3) >> 2); j++)
  {
    dst0 = (ae_int16x4 *)((uintptr_t)outImg + (PADDING * ostride + ostride - ALIGNMENT / 2)*sizeof(int16_t)) - 1 - j;
    for (i = 0; i<h; i++)
    {
      AE_S16X4_XP(x0, dst0, ostride * sizeof(ae_int16));
    }
  }
  /***************************************************/
  __Pragma("no_reorder");

  for (i = 0; i<(h>>1); i++)
  {
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1) * istride + 2*i) * sizeof(int16_t));
    src1 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1) * istride + 2 * i + 1) * sizeof(int16_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)outImg + ((2*i) * ostride +  (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((2*i+1) * ostride + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    for (j = 0; j<(w >> 2); j++)
    {
      AE_L16_XP(x0, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
      AE_L16_XP(x1, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
      AE_L16_XP(x2, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
      AE_L16_XP(x3, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));

      AE_L16_XP(x0_, castxcc(ae_int16, src1), -istride *  sizeof(int16_t));
      AE_L16_XP(x1_, castxcc(ae_int16, src1), -istride *  sizeof(int16_t));
      AE_L16_XP(x2_, castxcc(ae_int16, src1), -istride *  sizeof(int16_t));
      AE_L16_XP(x3_, castxcc(ae_int16, src1), -istride *  sizeof(int16_t));

      x0 = AE_SEL16_7362(x0, x1);
      x2 = AE_SEL16_7362(x2, x3);
      x0 = AE_SEL16_7632(x0, x2);

      x0_ = AE_SEL16_7362(x0_, x1_);
      x2_ = AE_SEL16_7362(x2_, x3_);
      x0_ = AE_SEL16_7632(x0_, x2_);

      AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
      AE_S16X4_IP(x0_, dst1, sizeof(ae_int16x4));
    }
  }

  if (h&1)
  {
      src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1) * istride + (h-1)) * sizeof(int16_t));
      dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((h-1) * ostride + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
      for (j = 0; j<(w >> 2); j++) //xcc:8/u2
      {
          AE_L16_XP(x0, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
          AE_L16_XP(x1, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
          AE_L16_XP(x2, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));
          AE_L16_XP(x3, castxcc(ae_int16, src0), -istride *  sizeof(int16_t));

          AE_MOVT16X4(x0, x2, b0);
          AE_MOVT16X4(x1, x3, b0);
          AE_MOVT16X4(x0, x1, b1);
          AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
      }
  }

  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + (i * istride) * sizeof(int16_t));
    dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((0)* ostride +  (PADDING*ostride + (ALIGNMENT / 2)) + (w - 1 - i))*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((1)* ostride +  (PADDING*ostride + (ALIGNMENT / 2)) + (w - 1 - i))*sizeof(int16_t));
    dst2 = (ae_int16x4 *)((uintptr_t)outImg + ((2)* ostride +  (PADDING*ostride + (ALIGNMENT / 2)) + (w - 1 - i))*sizeof(int16_t));
    dst3 = (ae_int16x4 *)((uintptr_t)outImg + ((3)* ostride +  (PADDING*ostride + (ALIGNMENT / 2)) + (w - 1 - i))*sizeof(int16_t));
    aS0 = AE_LA64_PP(src0);
    for (j = 0; j<(h >> 2); j++)
    {
      AE_LA16X4_IP(x0, aS0, src0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst3), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst2), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst1), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), 4 * ostride*sizeof(int16_t));
    }
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + (i* istride + (h - 1)) * sizeof(int16_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)outImg + ((h - 1)* ostride + (w - 1 - i) + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    for (j = 0; j<(h & 3); j++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), -(int)sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), -ostride*sizeof(int16_t));
    }
  }
  outSz->width=w+ALIGNMENT;
  outSz->height=h+PADDING*2;
#else
    int i,j,w,h,ostride,istride;
    const int16_t * restrict in =(const int16_t *)inImg;
          int16_t * restrict out=(      int16_t *)outImg;
    (void)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    NASSERT_ALIGN(outImg,ALIGNMENT);    /* inImg - not aligned, outImg - aligned */    
    NASSERT_ALIGN(outImg,1);
    imgsize_validate(inSz,2,0);
    imgsize_validate(outSz,2,1);
    ostride = outSz->stride;
    istride = inSz->stride;
    for (i=0; i<PADDING*ostride; i++) *out++=fillColor;
    // rotate by 270 degrees counterclockwise
    w=inSz->height;
    h=inSz->width;
    for (i=0; i<h; i++)
    {
        for (j=0; j<ALIGNMENT/2; j++) *out++=fillColor;
        for (j=0; j<w; j++) *out++=in[(w-1-j)*istride+i];
        for (; j<(ostride-ALIGNMENT/2); j++) *out++=fillColor;
    }
    for (i=0; i<(int)(outSz->height-(h+PADDING))*ostride; i++) *out++=fillColor;
    outSz->width=w+ALIGNMENT;
    outSz->height=h+PADDING*2;
#endif
} /* img_gs16_rot270_16() */
