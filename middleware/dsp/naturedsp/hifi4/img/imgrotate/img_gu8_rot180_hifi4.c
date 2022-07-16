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
void img_gu8_rot180 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  int i, j, w, h, istride, ostride, s_, d_;
  const int8_t * restrict src = (const int8_t *)inImg;
       uint8_t * restrict dst = (uint8_t *)outImg;
  (void)pScr;
  ae_valign aS, aD;
  ae_int16x4 x0, x1;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  imgsize_validate(inSz, 1, 0);
  imgsize_validate(outSz, 1, 0);
  NASSERT_ALIGN(outImg, 1);   /* both input and outputs nonaligned */
  NASSERT_ALIGN(inImg, 1);
  unsigned char p;
  // flip by 180 degrees
  w = inSz->width;
  h = inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;
#if 1
  for (i = 0; i<h; i++)
  {
      src = (const int8_t *)((uint8_t*)inImg + i * istride);
      dst = (uint8_t *)((uint8_t*)outImg + (h - 1 - i) * ostride + (w - 8));
      s_ = (((uintptr_t)src) & 1);
      d_ = ((((uintptr_t)dst)) & 1);
      if (s_ == 0)
      {
          aS = AE_LA64_PP(src);
          aD = AE_ZALIGN64();
          if (d_ == 0)
          {
              dst = (uint8_t *)((uintptr_t)dst + 6 * sizeof(uint8_t));
              for (j = 0; j < (w >> 3); j++)
              {
                  AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4, src));
                  x1 = AE_AND16(x0, (int16_t)(0xff00));
                  x0 = AE_AND16(x0, 0x00ff);
                  x1 = AE_MOVINT16X4_FROMINT64(AE_SRLI64(AE_MOVINT64_FROMINT16X4(x1), 8));
                  x0 = AE_MOVINT16X4_FROMINT64(AE_SLLI64(AE_MOVINT64_FROMINT16X4(x0), 8));
                  x0 = AE_OR16(x0, x1);
                  AE_SA16X4_RIP(x0, aD, castxcc(ae_int16x4, dst));
              }
              AE_SA64NEG_FP(aD, dst);
              dst = (uint8_t *)((uintptr_t)dst + 1 * sizeof(uint8_t));
              for (j = 0; j<(w & 7); j++)
              {
                  p = XT_L8UI((unsigned char*)src, 0);
                  XT_S8I(p, (signed char *)dst, 0);
                  src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
                  dst = (uint8_t *)((uintptr_t)dst - (int)sizeof(uint8_t));
              }
          }
          else
          {
              for (j = 0; j < (w >> 3); j++)
              {
                  AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4, src));
                  x1 = AE_SRAI16(x0, 8);
                  p = AE_MOVAD16_0(x1); XT_S8I(p, (signed char *)dst, 0);
                  p = AE_MOVAD16_0(x0); XT_S8I(p, (signed char *)dst, 1);
                  p = AE_MOVAD16_1(x1); XT_S8I(p, (signed char *)dst, 2);
                  p = AE_MOVAD16_1(x0); XT_S8I(p, (signed char *)dst, 3);
                  p = AE_MOVAD16_2(x1); XT_S8I(p, (signed char *)dst, 4);
                  p = AE_MOVAD16_2(x0); XT_S8I(p, (signed char *)dst, 5);
                  p = AE_MOVAD16_3(x1); XT_S8I(p, (signed char *)dst, 6);
                  p = AE_MOVAD16_3(x0); XT_S8I(p, (signed char *)dst, 7);
                  dst = (uint8_t *)((uintptr_t)dst - 8 * sizeof(uint8_t));
              }
              dst = (uint8_t *)((uintptr_t)dst + 7 * sizeof(uint8_t));
              for (j = 0; j<(w & 7); j++)
              {
                  p = XT_L8UI((unsigned char*)src, 0);
                  XT_S8I(p, (signed char *)dst, 0);
                  src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
                  dst = (uint8_t *)((uintptr_t)dst - (int)sizeof(uint8_t));
              }
          }         
      }
      else
      {
          if (d_ == 0)
          {
              unsigned char p;
              ae_int16x4 tmp0, tmp1;
              /* preload 1 element */
              p = XT_L8UI((unsigned char*)src, 0);
              src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
              tmp0 = AE_MOVDA16(p);
              tmp0 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(tmp0), 8));
              aS = AE_LA64_PP(src);
              aD = AE_ZALIGN64();
              dst = (uint8_t *)((uintptr_t)dst + 6 * sizeof(uint8_t));
              for (j = 0; j < ((w) >> 3); j++)
              {
                  AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4, src));
                  x1 = AE_AND16(x0, 0x00ff);
                  x0 = AE_AND16(x0, (int16_t)(0xff00));
                  tmp1 = x0;
                  x0 = AE_SEL16_4321(tmp0, x0);
                  tmp0 = tmp1;
                  x0 = AE_OR16(x0, x1);
                  AE_SA16X4_RIP(x0, aD, castxcc(ae_int16x4, dst));
              }
              AE_SA64NEG_FP(aD, dst);
              dst = (uint8_t *)((uintptr_t)dst + 1 * sizeof(uint8_t));
              src = (const int8_t *)((uintptr_t)src - 1*(int)sizeof(int8_t));
              for (j = 0; j < ((w) & 7); j++)
              {
                  p = XT_L8UI((unsigned char*)src, 0);
                  XT_S8I(p, (signed char *)dst, 0);
                  src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
                  dst = (uint8_t *)((uintptr_t)dst - (int)sizeof(uint8_t));
              }
          }
          else
          {
              /* copy first pixel*/
              p = XT_L8UI((unsigned char*)src, 0);
              XT_S8I(p, (signed char *)dst, 7);
              src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
              dst = (uint8_t *)((uintptr_t)dst - (int)sizeof(uint8_t));
              /*************************/
              aS = AE_LA64_PP(src);
              for (j = 0; j < ((w - 1) >> 3); j++)
              {
                  AE_LA16X4_IP(x0, aS, castxcc(ae_int16x4, src));
                  x1 = AE_SRAI16(x0, 8);
                  p = AE_MOVAD16_0(x1); XT_S8I(p, (signed char *)dst, 0);
                  p = AE_MOVAD16_0(x0); XT_S8I(p, (signed char *)dst, 1);
                  p = AE_MOVAD16_1(x1); XT_S8I(p, (signed char *)dst, 2);
                  p = AE_MOVAD16_1(x0); XT_S8I(p, (signed char *)dst, 3);
                  p = AE_MOVAD16_2(x1); XT_S8I(p, (signed char *)dst, 4);
                  p = AE_MOVAD16_2(x0); XT_S8I(p, (signed char *)dst, 5);
                  p = AE_MOVAD16_3(x1); XT_S8I(p, (signed char *)dst, 6);
                  p = AE_MOVAD16_3(x0); XT_S8I(p, (signed char *)dst, 7);
                  dst = (uint8_t *)((uintptr_t)dst - 8 * sizeof(uint8_t));
              }
              dst = (uint8_t *)((uintptr_t)dst + 7 * sizeof(uint8_t));
              for (j = 0; j < ((w - 1) & 7); j++)
              {
                  p = XT_L8UI((unsigned char*)src, 0);
                  XT_S8I(p, (signed char *)dst, 0);
                  src = (const int8_t *)((uintptr_t)src + (int)sizeof(int8_t));
                  dst = (uint8_t *)((uintptr_t)dst - (int)sizeof(uint8_t));
              }
          }
      }
  }

  NASSERT(outSz->width ==w);
  NASSERT(outSz->height==h);
#else
  for (i = 0; i<h; i++)
  {
    src = (const int8_t *)((uint8_t*)inImg + (h - 1 - i)*istride + w - 1);
    dst=(       uint8_t *)((uint8_t*)outImg + ostride * i) ;
    for (j=0; j<w; j++)
    {
      unsigned char p;
      p = XT_L8UI(( unsigned char*)src,0);
      XT_S8I(p,( signed char *)dst, 0); 
      src = (const int8_t *)((uintptr_t)src -sizeof(uint8_t));
      dst = (      uint8_t *)((uintptr_t)dst+ sizeof(uint8_t));   
    }
  }
  #endif
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
  // flip by 180 degrees
  w = inSz->width;
  h = inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;
  in += (h - 1)*istride;
  for (i = 0; i<h; i++, in -= istride)
  {
    for (j = 0; j<w; j++) *out++ = in[w - 1 - j];
    out += ostride - w;
  }
#endif
} /* img_gu8_rot180() */
