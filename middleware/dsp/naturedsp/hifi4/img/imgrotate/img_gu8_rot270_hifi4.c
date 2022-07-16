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
void img_gu8_rot270 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  int i, j, w, h, istride, ostride, s_;
  const int8_t * restrict src;
       uint8_t * restrict dst;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  imgsize_validate(inSz ,1,0);
  imgsize_validate(outSz,1,0);
  NASSERT_ALIGN(outImg,1);   /* both input and outputs nonaligned */ 
  NASSERT_ALIGN(inImg, 1);
  ae_valign aS;
  unsigned char p;
  ae_int16x4 x0, x1;
  // rotate by 270 degrees
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i = 0; i<w; i++)
  {
    src = (const int8_t *)((uint8_t*)inImg + i * istride);
    dst = (      uint8_t *)((uint8_t*)outImg + (w - 1 - i));
    s_ = (((uintptr_t)src) & 1);
    if (s_==0)
    {
      aS = AE_LA64_PP(src);
      for (j = 0; j<(h >> 3); j++)
      {
        AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4,src));
        x1 = AE_SRAI16(x0, 8);
        p = AE_MOVAD16_3(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_3(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_2(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_2(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_1(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_1(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_0(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_0(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
      }
      for (j = 0; j<(h&7); j++)
      {
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
        dst = (uint8_t *)((uintptr_t)dst + (ostride)*sizeof(uint8_t));
      }
    }
    else
    {
      /* copy first pixel*/
      p = XT_L8UI((unsigned char*)src, 0);
      XT_S8I(p, (signed char *)dst, 0);
      src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
      dst = (     uint8_t *)((uintptr_t)dst + (ostride)*sizeof(uint8_t));
      /*************************/
      aS = AE_LA64_PP(src);
      for (j = 0; j<((h - 1) >> 3); j++)
      {
        AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4, src));
        x1 = AE_SRAI16(x0, 8);
        p = AE_MOVAD16_3(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_3(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_2(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_2(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_1(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_1(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_0(x0);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
        p = AE_MOVAD16_0(x1);
        XT_S8I(p, (signed char *)dst, 0); dst = (uint8_t *)((uintptr_t)dst + ostride * sizeof(uint8_t));
      }
      for (j = 0; j<((h - 1) & 7); j++)
      {
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
        dst = (     uint8_t *)((uintptr_t)dst + (ostride)*sizeof(uint8_t));
      }
    }
  }
  NASSERT(outSz->width ==w);
  NASSERT(outSz->height==h);
#elif 1
  int i, j, w, h, istride, ostride;
  const int8_t * restrict src0;
      ae_int32 * restrict dst0; 
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  imgsize_validate(inSz ,1,0);
  imgsize_validate(outSz,1,0);
  NASSERT_ALIGN(outImg,1);   /* both input and outputs nonaligned */ 
  NASSERT_ALIGN(inImg, 1);
  // rotate by 270 degrees
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i = 0; i<(w); i++)
  {
    src0 = (const int8_t *)((uintptr_t)inImg + (i* istride + (h - 1)) * sizeof(int8_t));
    dst0 = (    ae_int32 *)((uintptr_t)outImg + ((h - 1)* ostride + (w - 1 - i)) *sizeof(int8_t));
    for (j = 0; j<h; j++)
    {
      unsigned char p;
      p = XT_L8UI((unsigned char*)src0, 0);
      XT_S8I(p, (signed char *)dst0, 0);
      src0 = (const int8_t *)((uintptr_t)src0 - (int)sizeof(int8_t));
      dst0 = (ae_int32 *)((uintptr_t)dst0 - (ostride)*sizeof(uint8_t));
    }
  }
#else
  int i,j,w,h, istride,ostride;
  const uint8_t * restrict in =(const uint8_t *)inImg;
        uint8_t * restrict out=(      uint8_t *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  imgsize_validate(inSz ,1,0);
  imgsize_validate(outSz,1,0);
  NASSERT_ALIGN(outImg,1);   /* both input and outputs nonaligned */ 
  NASSERT_ALIGN(inImg, 1);
  // rotate by 270 degrees
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i = 0; i<h; i++)
  {
    for (j = 0; j<w; j++)    out[i*ostride + j] = in[(w - 1 - j)*istride + i];
  }
#endif
} /* img_gu8_rot270() */
