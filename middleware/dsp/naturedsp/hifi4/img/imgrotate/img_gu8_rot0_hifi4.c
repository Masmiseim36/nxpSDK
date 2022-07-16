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

#define ALG_ROT0 1
/*--------------------------------------------------------------------------
rotation by 0,90,180,270 with no format conversion
Input:
inImg, inSz  image and its size
outSz        output image size
Output:
outImg       image
--------------------------------------------------------------------------*/
void img_gu8_rot0 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if ALG_ROT0 == 1
  const ae_int16x4 * restrict src = (const ae_int16x4 *)inImg;
        ae_int16x4 * restrict dst = (      ae_int16x4 *)outImg;

  int i, j, w, h, istride, ostride, s_, d_;
  (void)pScr;
  ae_int16x4 x0;
  ae_valign aD;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  imgsize_validate(inSz, 1, 0);
  imgsize_validate(outSz, 1, 0);
  NASSERT_ALIGN(outImg, 1);   /* both input and outputs nonaligned */
  NASSERT_ALIGN(inImg, 1);
  NASSERT(inSz->width >= 8);
  NASSERT(inSz->height >= 8);
  // copy
  w=inSz->width;
  h=inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;

  for (i = 0; i<h; i++)
  {
    src = (const ae_int16x4 *)((uint8_t*)inImg + i * istride);
    dst = (      ae_int16x4 *)((uint8_t*)outImg + i * ostride);
    s_ = (((uintptr_t)src) & 7);
    if (s_) s_ = 8 - s_;
    for (j = 0; j<s_; j++)
    {
      unsigned char p;
      p = XT_L8UI((unsigned char*)src, 0);
      XT_S8I(p, (signed char *)dst, 0);
      src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
      dst = (      ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
    }
    d_ = (((uintptr_t)dst) & 1);

    if (d_)
    {
      unsigned char p;

      p = XT_L8UI((unsigned char*)src, 0);
      XT_S8I(p, (signed char *)dst, 0);
      src = (const ae_int16x4 *)((uintptr_t)src + (int)sizeof(int8_t));
      dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
      #if 0
      if ((w - s_ - 1)>=8)
      {
        ae_int16x4 x1, y1, t0;
        aD = AE_ZALIGN64();
        for (j = 0; j < ((w - s_ - 1) >> 3); j++)
        {
          AE_L16X4_IP(t0, src, 8 * sizeof(uint8_t));
          x1 = AE_AND16(t0, 0x00ff);
          x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
          x0 = AE_AND16(t0, 0xff00);
          x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
          p = XT_L8UI((unsigned char*)src, 0);
          y1 = AE_MOVDA16(p<<8);
          x1 = AE_SEL16_6543(x1, y1);
          x0 = AE_OR16(x0, x1);
          AE_SA16X4_IP(x0, aD, dst);
        }
        AE_SA64POS_FP(aD, dst);
        src = (const ae_int16x4 *)((uintptr_t)src + (int)sizeof(int8_t));

      }
      #else
      if ((w - s_ - 1)>=8)
      {
        ae_int16x4 y0, y1;
        ae_int16x4 x1, t0, t1;
        aD = AE_ZALIGN64();
        src = (const ae_int16x4 *)((uintptr_t)src - (int)sizeof(int8_t));
        AE_L16X4_IP(t0, src, 8 * sizeof(uint8_t));
        y1 = AE_AND16(t0, 0x00ff);
        y1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(y1), 8));
        y0 = AE_AND16(t0, AE_MOVDA16(0xff00));
        y0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(y0), 8));
        for (j = 0; j < ((w - s_ - 1) >> 3) - 1; j++)
        {
          AE_L16X4_IP(t1, src, 8 * sizeof(uint8_t));
          x1 = AE_AND16(t1, 0x00ff);
          x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
          x0 = AE_AND16(t1, AE_MOVDA16(0xff00));
          x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
          y1 = AE_SEL16_6543(y1, x1);
          y0 = AE_OR16(y0, y1);
          AE_SA16X4_IP(y0, aD, dst);
          y0 = x0;
          y1 = x1;
        }
        /* to avoid speculative load */
        p = XT_L8UI((unsigned char*)src, 0);
        x1 = AE_MOVDA16(p);
        x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
        y1 = AE_SEL16_6543(y1, x1);
        y0 = AE_OR16(y0, y1);
        AE_SA16X4_IP(y0, aD, dst);
        AE_SA64POS_FP(aD, dst);
        src = (const ae_int16x4 *)((uintptr_t)src + (int)sizeof(int8_t));
      }
        #endif
      for (j = 0; j<((w - s_ - 1) & 7); j++)
      {
        unsigned char p;
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
        dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
      }

    }
    else
    {
      aD = AE_ZALIGN64();
      for (j = 0; j<((w - s_) >> 3); j++)
      {
        AE_L16X4_IP(x0, src, 8 * sizeof(uint8_t));
        AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);

      for (j = 0; j<((w - s_)&7); j++)
      {
        unsigned char p; 
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const ae_int16x4 *)((uintptr_t)src +sizeof(uint8_t));
        dst = (      ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
      }

    }

  }
#elif ALG_ROT0==2
 const ae_int16x4 * restrict src = (const ae_int16x4 *)inImg;
       ae_int16x4 * restrict dst = (      ae_int16x4 *)outImg;

  int i, j, w, h, istride, ostride, s_, d_;
  (void)pScr;
  ae_int16x4 x0;
  ae_valign aD, aS;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  imgsize_validate(inSz, 1, 0);
  imgsize_validate(outSz, 1, 0);
  NASSERT_ALIGN(outImg, 1);   /* both input and outputs nonaligned */
  NASSERT_ALIGN(inImg, 1);

  // copy
  w=inSz->width;
  h=inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;

  aD = AE_ZALIGN64();
  for (i = 0; i<h; i++)
  {
    src = (const ae_int16x4 *)((uint8_t*)inImg + i * istride);
    dst = (      ae_int16x4 *)((uint8_t*)outImg + i * ostride);
    d_ = (((uintptr_t)dst) & 1);
    s_ = (((uintptr_t)src) & 1);
    if (d_&s_)
    {
      unsigned char p;
      p = XT_L8UI((unsigned char*)src, 0);
      XT_S8I(p, (signed char *)dst, 0);
      src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
      dst = (       ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));

      aS = AE_LA64_PP(src);
      for (j = 0; j<((w - 1) >> 3); j++)
      {
        AE_LA16X4_IP(x0, aS, src);
        AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);
      for (j = 0; j<((w - 1) & 7); j++)
      {
        unsigned char p;
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
        dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
      }
    }
    if ((d_||s_)==0)
    {
      aS = AE_LA64_PP(src);
      for (j = 0; j<((w) >> 3); j++)
      {
        AE_LA16X4_IP(x0, aS, src);
        AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);

      for (j = 0; j<(w&7); j++)
      {
        unsigned char p; 
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const ae_int16x4 *)((uintptr_t)src +sizeof(uint8_t));
        dst = (      ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
      }
    }
    if ((s_ == 0) & (d_==1))
    {
        unsigned char p;
        ae_int16x4 tmp0, tmp1;
        ae_int16x4 x1;
        /* process 1 element */
        p = XT_L8UI((unsigned char*)src, 0);
        XT_S8I(p, (signed char *)dst, 0);
        src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
        dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
        /* preload 1 element */
        p = XT_L8UI((unsigned char*)src, 0);
        src = (const ae_int16x4 *)((uintptr_t)src + (int)sizeof(int8_t));
        tmp0 = AE_MOVDA16(p);
        aS = AE_LA64_PP(src);
        aD = AE_ZALIGN64();
        for (j = 0; j < ((w-1) >> 3); j++)
        {
            AE_LA16X4_IP(x0, aS, src);
            x1 = AE_AND16(x0, 0x00ff);
            x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
            x0 = AE_AND16(x0, 0xff00);
            x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
            tmp1 = x0;
            x0 = AE_SEL16_4321(tmp0, x0);
            tmp0 = tmp1;
            x0 = AE_OR16(x0, x1);
            AE_SA16X4_IP(x0, aD, dst);
        }
        AE_SA64POS_FP(aD, dst);
        src = (const ae_int16x4 *)((uintptr_t)src - (int)sizeof(int8_t));
        for (j = 0; j<((w-1)& 7); j++)
        {
            unsigned char p;
            p = XT_L8UI((unsigned char*)src, 0);
            XT_S8I(p, (signed char *)dst, 0);
            src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
            dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
        }
    }
    if ((s_ == 1) & (d_ == 0))
    {
        unsigned char p;
        ae_int16x4 tmp0, tmp1;
        ae_int16x4 x1;
        /* preload 1 element */
        p = XT_L8UI((unsigned char*)src, 0);
        src = (const ae_int16x4 *)((uintptr_t)src + (int)sizeof(int8_t));
        tmp0 = AE_MOVDA16(p);
        aS = AE_LA64_PP(src);
        aD = AE_ZALIGN64();
        for (j = 0; j < ((w) >> 3); j++)
        {
            AE_LA16X4_IP(x0, aS, src);
            x1 = AE_AND16(x0, 0x00ff);
            x1 = AE_MOVINT16X4_FROMINT32X2(AE_SLLI32(AE_MOVINT32X2_FROMINT16X4(x1), 8));
            x0 = AE_AND16(x0, 0xff00);
            x0 = AE_MOVINT16X4_FROMINT32X2(AE_SRLI32(AE_MOVINT32X2_FROMINT16X4(x0), 8));
            tmp1 = x0;
            x0 = AE_SEL16_4321(tmp0, x0);
            tmp0 = tmp1;
            x0 = AE_OR16(x0, x1);
            AE_SA16X4_IP(x0, aD, dst);
        }
        AE_SA64POS_FP(aD, dst);
        src = (const ae_int16x4 *)((uintptr_t)src - (int)sizeof(int8_t));
        for (j = 0; j<((w) & 7); j++)
        {
            unsigned char p;
            p = XT_L8UI((unsigned char*)src, 0);
            XT_S8I(p, (signed char *)dst, 0);
            src = (const ae_int16x4 *)((uintptr_t)src + sizeof(uint8_t));
            dst = (ae_int16x4 *)((uintptr_t)dst + sizeof(uint8_t));
        }       
    }
  }
  NASSERT(outSz->width ==w);
  NASSERT(outSz->height==h);
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
  // copy
  w = inSz->width;
  h = inSz->height;
  istride = inSz->stride;
  ostride = outSz->stride;
  for (i = 0; i<h; i++, in += istride)
  {
    for (j = 0; j<w; j++) *out++ = in[j];
    out += ostride - w;
  }
#endif
} /* img_gu8_rot0() */
#undef ALG_ROT0
