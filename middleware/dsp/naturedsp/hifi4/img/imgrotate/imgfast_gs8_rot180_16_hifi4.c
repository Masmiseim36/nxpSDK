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
void imgfast_gs8_rot180_16(void* pScr, void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int fillColor)
{
  int i, j, w, h, ostride, istride, h_out;
  const    uint8_t * restrict src  = (const    uint8_t *)inImg;
        ae_int16x4 * restrict dst0 = (      ae_int16x4 *)outImg;
        ae_int16x4 * restrict dst1 = (      ae_int16x4 *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);
  NASSERT_ALIGN(inImg ,ALIGNMENT);
  ae_int16x4 x0;
  ae_valign aD;
  imgsize_validate(inSz,1,1);
  imgsize_validate(outSz,2,1);
  ostride = outSz->stride;
  istride = inSz->stride;
  w = inSz->width;
  h = inSz->height;
  h_out = outSz->height;
  /* filling row PADDING */
  x0 = fillColor;
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

  /* flip by 180 degrees */
  aD = AE_ZALIGN64();
  src  = (const uint8_t *)((uintptr_t)inImg + ((h - 1)*istride + (w >> 2) * 4)*sizeof(uint8_t)) - 4;
  dst0 = (   ae_int16x4 *)((uintptr_t)outImg + (PADDING*ostride + (ALIGNMENT / 2) + (w & 3))*sizeof(int16_t));
  for (i=0; i<h; i++)
  {    
    ae_int16x4 _8000 = AE_MOVDA16(0x8000);
    for (j=0; j<(w>>2); j++)
    {
      AE_L8X4F_IP(x0, castxcc(signed char, src), -4 * (int)sizeof(uint8_t));
      x0 = AE_SHORTSWAP(x0);
      /* signed -> unsigned */
      x0 = AE_XOR16(x0, _8000);
      x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 1));
      AE_SA16X4_IP(x0, aD, dst0);
    }
    AE_SA64POS_FP(aD, dst0);
    src = (const uint8_t *)((uintptr_t)src - (istride - (w >> 2) * 4)*sizeof(uint8_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)dst0+ (ostride-(w>>2)*4)*sizeof(int16_t));
  }  
  for (i=0; i<(w&3); i++)
  {
    ae_int16x4 _80 = AE_MOVDA16(0x80);
    src = (const uint8_t *)((uint8_t*)inImg + (h - 1)*istride + w - 1 - i);
    dst0 = (ae_int16x4 *)((uintptr_t)outImg + (PADDING*ostride + (ALIGNMENT / 2) + i)*sizeof(int16_t));
    for (j=0; j<h; j++)
    {
      ae_int16x4 p;
      unsigned char c0;
      c0 = XT_L8UI((unsigned char*)src, 0);
      p = AE_MOVDA16(c0);
      /* signed -> unsigned */
      p = AE_XOR16(p, _80);
      p = AE_SLLI16S(p, 7);
      src = (const uint8_t *)((uintptr_t)src -(istride)*sizeof(uint8_t));
      AE_S16_0_XP(p,castxcc(ae_int16,dst0),sizeof(int16_t)*ostride);
    }
  }

  outSz->width=w+ALIGNMENT;
  outSz->height=h+PADDING*2;
} /* imgfast_gs8_rot180_16() */
