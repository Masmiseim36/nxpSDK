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
void img_gu8_rot270_16(void* pScr,void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor)
{
#if 1
  int i, j, w, h, ostride, istride, h_out;
  const uint8_t * restrict src0;
     ae_int16x4 * restrict dst0;
     ae_int16x4 * restrict dst1;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);    /* inImg - not aligned, outImg - aligned */    
  NASSERT_ALIGN(outImg,1);
  ae_int16x4 x0;
  xtbool4 b0 = AE_MOVBA4(0x3);
  xtbool4 b1 = AE_MOVBA4(0x5);

  imgsize_validate(inSz,1,0);
  imgsize_validate(outSz,2,1);
  ostride = outSz->stride;
  istride = inSz->stride;
  w = inSz->height;
  h = inSz->width;
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
  __Pragma("no_reorder");
  /***************************************************/

  /* rotate by 270 degrees counterclockwise */
  for (i = 0; i<(h>>1); i++)
  {
    src0 = (const uint8_t *)((uintptr_t)inImg + ((w - 1) * istride + 2*i) * sizeof(uint8_t));
    dst0 = (   ae_int16x4 *)((uintptr_t)outImg + ((2*i) * ostride + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((2 * i+1) * ostride + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    for (j = 0; j<(w >> 2); j++)
    {
      unsigned char c0, c1, c2, c3;
      unsigned char c0_, c1_, c2_, c3_;
      ae_int16x4 x0, x1, x2, x3;
      ae_int16x4 x0_, x1_, x2_, x3_;
      c0 = XT_L8UI((unsigned char*)src0, 0); 
      c0_ = XT_L8UI((unsigned char*)src0, 1);
      src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
      c1 = XT_L8UI((unsigned char*)src0, 0); 
      c1_ = XT_L8UI((unsigned char*)src0, 1);
      src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
      c2 = XT_L8UI((unsigned char*)src0, 0); 
      c2_ = XT_L8UI((unsigned char*)src0, 1);
      src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
      c3 = XT_L8UI((unsigned char*)src0, 0); 
      c3_ = XT_L8UI((unsigned char*)src0, 1);
      src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
      x0 = AE_MOVDA16(c0);
      x1 = AE_MOVDA16(c1);
      x2 = AE_MOVDA16(c2);
      x3 = AE_MOVDA16(c3);

      x0_ = AE_MOVDA16(c0_);
      x1_ = AE_MOVDA16(c1_);
      x2_ = AE_MOVDA16(c2_);
      x3_ = AE_MOVDA16(c3_);

      AE_MOVT16X4(x0, x2, b0);
      AE_MOVT16X4(x1, x3, b0);
      AE_MOVT16X4(x0, x1, b1);

      AE_MOVT16X4(x0_, x2_, b0);
      AE_MOVT16X4(x1_, x3_, b0);
      AE_MOVT16X4(x0_, x1_, b1);

      x0 = AE_SLLI16S(x0, 7);
      x0_ = AE_SLLI16S(x0_, 7);
      AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
      AE_S16X4_IP(x0_, dst1, sizeof(ae_int16x4));
    }
  }

  if (h&1)
  {
      src0 = (const uint8_t *)((uintptr_t)inImg + ((w - 1) * istride + (h-1)) * sizeof(uint8_t));
      dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((h-1) * ostride + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
      for (j = 0; j<(w >> 2); j++)
      {
          unsigned char c0, c1, c2, c3;
          ae_int16x4 x0, x1, x2, x3;
          c0 = XT_L8UI((unsigned char*)src0, 0); src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
          c1 = XT_L8UI((unsigned char*)src0, 0); src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
          c2 = XT_L8UI((unsigned char*)src0, 0); src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
          c3 = XT_L8UI((unsigned char*)src0, 0); src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -istride * sizeof(uint8_t));
          x0 = AE_MOVDA16(c0);
          x1 = AE_MOVDA16(c1);
          x2 = AE_MOVDA16(c2);
          x3 = AE_MOVDA16(c3);

          AE_MOVT16X4(x0, x2, b0);
          AE_MOVT16X4(x1, x3, b0);
          AE_MOVT16X4(x0, x1, b1);
          x0 = AE_SLLI16S(x0, 7);
          AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
      }
  }

  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {
    src0 = (const uint8_t *)((uintptr_t)inImg + (i* istride + (h - 1)) * sizeof(uint8_t));
    dst0 = (   ae_int16x4 *)((uintptr_t)outImg + ((h - 1)* ostride + (w - 1 - i) + (PADDING*ostride + (ALIGNMENT / 2)))*sizeof(int16_t));
    for (j = 0; j<(h); j++)
    {
      unsigned char c0;
      c0 = XT_L8UI((unsigned char*)src0, 0); src0 = (const uint8_t *)XT_ADD((uintptr_t)src0, -(int)sizeof(uint8_t));
      x0 = AE_MOVDA16(c0);
      x0 = AE_SLLI16S(x0, 7);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), -ostride*sizeof(int16_t));
    }
  }
  outSz->width=w+ALIGNMENT;
  outSz->height=h+PADDING*2;

#else
  int i,j,w,h,ostride,istride;
  const uint8_t * restrict in =(const uint8_t *)inImg;
        int16_t * restrict out=(      int16_t *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);    /* inImg - not aligned, outImg - aligned */    
  NASSERT_ALIGN(outImg,1);
  imgsize_validate(inSz,1,0);
  imgsize_validate(outSz,2,1);
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i=0; i<PADDING*ostride; i++) *out++=fillColor;
  // rotate by 270 degrees counterclockwise
  w = inSz->height;
  h = inSz->width;
  for (i = 0; i<h; i++)
  {
    for (j = 0; j<ALIGNMENT / 2; j++) *out++ = fillColor;
    for (j = 0; j<w; j++) *out++ = ((int16_t)in[(w - 1 - j)*istride + i]) << 7;
    for (; j<(ostride - ALIGNMENT / 2); j++) *out++ = fillColor;
  }
  for (i=0; i<(int)(outSz->height-(h+PADDING))*ostride; i++) *out++=fillColor;
  outSz->width=w+ALIGNMENT;
  outSz->height=h+PADDING*2;
#endif
} /* img_gu8_rot270_16() */
