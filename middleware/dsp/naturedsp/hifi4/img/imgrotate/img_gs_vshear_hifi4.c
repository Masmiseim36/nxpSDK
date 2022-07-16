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
    vertical inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    alpha    shear scale, Q15
    Input/output:
    inoutImg, inoutSz - image and its scale
--------------------------------------------------------------------------*/
void img_gs_vshear(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t alpha, int fillColor)
{
#if ALG==0
  const ae_int16x4 * restrict src;
  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
        ae_int16x4 * restrict dst0;
  int i, j, w, h, H, stride;
  ae_int16x4 al;
  ae_int32x2 w0, w1, w2, w3, skewQ16, tw, inc;
  ae_int32x2 skewQ16_01;
  ae_int32x2 aQ31_01, bQ31_01;
  ae_int32x2 c0_01, c1_01, c2_01;
  xtbool2 b_skewQ16;
  ae_int16x4 x0, x1;
  int16_t * restrict pS = (int16_t *)pScr;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  w=inoutSz->width;
  h=inoutSz->height;
  w-=ALIGNMENT;
  h-=PADDING*2;
  stride=inoutSz->stride;
  imgsize_validate(inoutSz,2,1);
  NASSERT_ALIGN(inoutImg, ALIGNMENT);

  skewQ16_01 = AE_MOVDA32X2(0, 2 * alpha);
  /* H=h+floorf((w-0.5f)*fabsf(alpha/32768.f)) */
  al = AE_ABS16S(alpha);
  w0 = AE_CVT32X2F16_10(AE_MOVDA16(w));
  inc = AE_MOVDA32( 2 * 2 * alpha);
  w1 = AE_SUB32(w0, 0x8000);// w - (1<<15)
  w1 = AE_MULFP32X16X2S_H(w1, al);  
  H = h + AE_MOVAD32_H(AE_SRAI32(w1, 16));

  b_skewQ16 = AE_LT32(alpha, AE_ZERO32());

  skewQ16 = AE_MOVDA32((1 << 15));
  tw = AE_SUB32(skewQ16, w0);
  AE_MOVT32X2(skewQ16, tw, b_skewQ16);

  skewQ16 = AE_MULFP32X16X2S_H(skewQ16, alpha);
  skewQ16_01 = AE_ADD32(skewQ16, skewQ16_01);
  src = (const ae_int16x4 *)((uintptr_t)inoutImg + (PADDING* stride + (ALIGNMENT / 2)) * sizeof(int16_t));
  if (alpha > 0)
  {
    for (j = 0; j<(w >> 1); j++)
    {
      ae_int32x2 k_01, delta_k;
      int k0, k1;
      xtbool2 bk_1;

      k0 = AE_MOVAD32_H(AE_SRAI32(skewQ16_01, 16));
      k1 = AE_MOVAD32_L(AE_SRAI32(skewQ16_01, 16));

      aQ31_01 = AE_AND32(skewQ16_01, 0xffff); aQ31_01 = AE_SLAI32(aQ31_01, 15);
      bQ31_01 = AE_SUB32(0x7fffffff, aQ31_01);
      k_01 = AE_SRAI32(skewQ16_01, 16);

      c0_01 = AE_ZERO32();
      c1_01 = aQ31_01; 
      c2_01 = bQ31_01; 
      delta_k = AE_SEL32_HH(k_01, k_01);
      bk_1 = AE_LT32(delta_k, k_01);
      AE_MOVT32X2(c0_01, c1_01, bk_1);
      AE_MOVT32X2(c1_01, c2_01, bk_1);
      AE_MOVT32X2(c2_01, AE_ZERO32(), bk_1);

      dst0 = (      ae_int16x4 *)((uintptr_t)pS);
      src0 = (const ae_int16x4 *)((uintptr_t)src + (2 * j - stride) * sizeof(int16_t));
      src1 = (const ae_int16x4 *)((uintptr_t)src + (2 * j)* sizeof(int16_t));

      w0 = AE_L16X2M_I((ae_p16x2s*) src0, 0);
      w0 = AE_SLAI32(w0, 8);

      AE_L16X2M_XU(w1, castxcc(ae_p16x2s, src0), stride * sizeof(int16_t));
      w1 = AE_SLAI32(w1, 8);

      for (i = 0; i<((h - 1)>>1); i++)
      {
        ae_int32x2 y0, y1;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);

        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        
        AE_L16X2M_XU(w3, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w3 = AE_SLAI32(w3, 8);

        y1 = AE_MULFP32X2RAS(c0_01, w1); 
        AE_MULAFP32X2RAS(y1, c1_01, w2);
        AE_MULAFP32X2RAS(y1, c2_01, w3);

        x0 = AE_ROUND16X4F32SASYM(y0, y1);
        AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
        w0 = w2;
        w1 = w3;
      }
      for (i = 0; i<((h - 1)& 1); i++)
      {
        ae_int32x2 y0;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);

        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        x0 = AE_ROUND16X4F32SASYM(y0, y0);
        AE_S16_0_I(x0, (ae_int16 *)dst0, sizeof(int16_t));
        x0 = AE_SEL16_4321(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
        w0 = w1;
        w1 = w2;
      }
      /* last pixels */
      if (k1-k0)
      {
        ae_int32x2 y0, y1;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);
        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        y1 = w1;
        x0 = AE_ROUND16X4F32SASYM(y0, y1);
        x1 = AE_SEL16_7362(x0, x0);
        AE_S16_0_I(x1, (ae_int16 *)dst0, sizeof(int16_t));
        x0 = AE_SEL16_2301(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
      }
      {
        ae_int32x2 y0;
        y0 = w1;
        x0 = AE_ROUND16X4F32SASYM(y0, y0);
        AE_S16_0_I(x0, (ae_int16 *) dst0, sizeof(int16_t)); 
        x0 = AE_SEL16_4321(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
      }

      src0 = (const ae_int16x4 *)((uintptr_t)pS - 2 * sizeof(int16_t));
      dst0 = (      ae_int16x4 *)((uintptr_t)src + (2 * j + k0 * stride - stride) * sizeof(int16_t));
      for (i = 0; i<(h + (k1 - k0)); i++)
      {
        AE_L16X2M_IU(w0, castxcc(ae_p16x2s, src0), 2 * sizeof(int16_t));
        AE_S16X2M_XU(w0, castxcc(ae_p16x2s, dst0), stride * sizeof(int16_t));
      }
      skewQ16_01 = AE_ADD32(skewQ16_01, inc);
    }
  }
  else
  {
    for (j = 0; j<(w >> 1); j++)
    {
      ae_int32x2 k_01, delta_k;
      int k0, k1;
      xtbool2 bk_1;

      k0 = AE_MOVAD32_H(AE_SRAI32(skewQ16_01, 16));
      k1 = AE_MOVAD32_L(AE_SRAI32(skewQ16_01, 16));

      aQ31_01 = AE_AND32(skewQ16_01, 0xffff); aQ31_01 = AE_SLAI32(aQ31_01, 15);
      bQ31_01 = AE_SUB32(0x7fffffff, aQ31_01);
      k_01 = AE_SRAI32(skewQ16_01, 16);

      c2_01 = AE_ZERO32();
      c0_01 = aQ31_01; 
      c1_01 = bQ31_01; 
      delta_k = AE_SEL32_HH(k_01, k_01);
      bk_1 = AE_LT32(k_01, delta_k);
      AE_MOVT32X2(c2_01, c1_01, bk_1);
      AE_MOVT32X2(c1_01, c0_01, bk_1);
      AE_MOVT32X2(c0_01, AE_ZERO32(), bk_1);

      dst0 = (      ae_int16x4 *)((uintptr_t)pS);
      src0 = (const ae_int16x4 *)((uintptr_t)src + (2 * j - 2 * stride) * sizeof(int16_t));
      src1 = (const ae_int16x4 *)((uintptr_t)src + (2 * j + 0 * stride)* sizeof(int16_t));

      AE_L16X2M_XU(w0, castxcc(ae_p16x2s, src0), stride * sizeof(int16_t));
      AE_L16X2M_XU(w1, castxcc(ae_p16x2s, src0), stride * sizeof(int16_t));
      w0 = AE_SLAI32(w0, 8);
      w1 = AE_SLAI32(w1, 8);

      for (i = 0; i<((h - 1)>>1); i++)
      {
        ae_int32x2 y0, y1;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);
        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);

        AE_L16X2M_XU(w3, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w3 = AE_SLAI32(w3, 8);
        y1 = AE_MULFP32X2RAS(c0_01, w1);
        AE_MULAFP32X2RAS(y1, c1_01, w2);
        AE_MULAFP32X2RAS(y1, c2_01, w3);

        x0 = AE_ROUND16X4F32SASYM(y0, y1);
        AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
        w0 = w2;
        w1 = w3;
      }
      for (i = 0; i<((h - 1)& 1); i++)
      {
        ae_int32x2 y0;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);
        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        x0 = AE_ROUND16X4F32SASYM(y0, y0);
        AE_S16_0_I(x0, (ae_int16 *)dst0, sizeof(int16_t));
        x0 = AE_SEL16_4321(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
        w0 = w1;
        w1 = w2;
      }
      /* last pixels */
      if (k0-k1)
      {
        ae_int32x2 y0, y1;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);
        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        y1 = w1;
        x0 = AE_ROUND16X4F32SASYM(y0, y1);
        x1 = AE_SEL16_7362(x0, x0);
        AE_S16_0_I(x0, (ae_int16 *)dst0, sizeof(int16_t));
        x0 = AE_SHORTSWAP(x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
        w0 = w1;
      }
      else
      {
        ae_int32x2 y0;
        AE_L16X2M_XU(w2, castxcc(ae_p16x2s, src1), stride * sizeof(int16_t));
        w2 = AE_SLAI32(w2, 8);
        y0 = AE_MULFP32X2RAS(c0_01, w0);
        AE_MULAFP32X2RAS(y0, c1_01, w1);
        AE_MULAFP32X2RAS(y0, c2_01, w2);
        x0 = AE_ROUND16X4F32SASYM(y0, y0);
        AE_S16_0_I(x0, (ae_int16 *)dst0, sizeof(int16_t));
        x0 = AE_SEL16_4321(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
        w0 = w1;
        w1 = w2;
      }
      { 
        ae_int32x2 y0;
        y0 = w0;
        x0 = AE_ROUND16X4F32SASYM(y0, y0);
        AE_S16_0_I(x0, (ae_int16 *) dst0, sizeof(int16_t)); 
        x0 = AE_SEL16_4321(x0, x0);
        AE_S16_0_IP(x0, castxcc(ae_int16, dst0), 2 * sizeof(int16_t));
      }
      src0 = (const ae_int16x4 *)((uintptr_t)pS - 2 * sizeof(int16_t));
      dst0 = (      ae_int16x4 *)((uintptr_t)src + (2 * j + k0 * stride - 2 * stride) * sizeof(int16_t));
      for (i = 0; i<(h + 1); i++)
      {
        AE_L16X2M_IU(w0, castxcc(ae_p16x2s, src0), 2 * sizeof(int16_t));
        AE_S16X2M_XU(w0, castxcc(ae_p16x2s, dst0), stride * sizeof(int16_t));
      }
      skewQ16_01 = AE_ADD32(skewQ16_01, inc);
    }
  }
  __Pragma("no_reorder");
  if (w&1)
  {
    int k;
    ae_int32x2 a0, b0, y0;
    ae_int16x4 x0, x1;
    skewQ16_01 = AE_SEL32_HH(skewQ16_01, skewQ16_01);
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16_01, 16));
    a0 = AE_AND32(skewQ16_01, 0xffff); a0 = AE_SLAI32(a0, 15);
    b0 = AE_SUB32(0x7fffffff, a0);

    dst0 = (      ae_int16x4 *)((uintptr_t)pS);
    src0 = (const ae_int16x4 *)((uintptr_t)src + (w-1) * sizeof(int16_t));
    AE_L16_XP(x0, castxcc(ae_int16, src0), stride * sizeof(int16_t));
    for (i = 0; i<h - 1; i++)
    {
      AE_L16_XP(x1, castxcc(ae_int16, src0), stride * sizeof(int16_t));
      y0 = AE_MULFP32X16X2S_H(a0, x0);
      AE_MULAFP32X16X2S_H(y0, b0, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y0);
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
      x0 = x1;
    }
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));

    src0 = (const ae_int16x4 *)((uintptr_t)pS);
    dst0 = (ae_int16x4 *)((uintptr_t)src + (k * stride + (w - 1)) * sizeof(int16_t));
    for (i = 0; i<(h >> 2); i++)
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      AE_S16_0_X(x0, (ae_int16 *)dst0, 3 * stride * sizeof(int16_t));
      x1 = AE_SEL16_4321(x0, x0);
      AE_S16_0_X(x1, (ae_int16 *)dst0, 2 * stride * sizeof(int16_t));
      x1 = AE_SEL16_5342(x0, x0);
      AE_S16_0_X(x1, (ae_int16 *)dst0, 1 * stride * sizeof(int16_t));
      x0 = AE_SEL16_6543(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), 4 * stride * sizeof(int16_t));
    }
    for (i = 0; i<(h & 3); i++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }
  
  __Pragma("no_reorder");
  /* filling */
  inc = AE_MOVDA32(2 * alpha);
  x0 = fillColor;
  for (j = 0; j<(w); j++)
  {
    int k;
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16, 16));
    skewQ16 = AE_ADD32(skewQ16, inc);
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j - stride)* sizeof(int16_t));
    for (i = 0; i<(k+1); i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j + (h + k)*stride)* sizeof(int16_t));
    for (i = 0; i<(H - (h + k)); i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }

  inoutSz->height=H+PADDING*2;
#elif ALG==1
  const ae_int16x4 * restrict src;
  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
        ae_int16x4 * restrict dst0;
        ae_int16x4 * restrict dst1;
        ae_int16x4 * restrict dst2;
        ae_int16x4 * restrict dst3;
  int i, j, w, h, H, stride;
  ae_int16x4 al;
  ae_int32x2 w0, w1, skewQ16, tw, inc;
  ae_int32x2 skewQ16_01, skewQ16_23;
  ae_int32x2 aQ31_01, aQ31_23, bQ31_01, bQ31_23;

  xtbool2 b_skewQ16;
  ae_int16x4 x0, x1;
  int16_t * restrict img=(int16_t *)inoutImg;
  int16_t * restrict pS = (int16_t *)pScr;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  w=inoutSz->width;
  h=inoutSz->height;
  w-=ALIGNMENT;
  h-=PADDING*2;
  stride=inoutSz->stride;
  imgsize_validate(inoutSz,2,1);
  NASSERT_ALIGN(img,ALIGNMENT);
  img+=stride*PADDING +ALIGNMENT/2;   // starting point for transform

  skewQ16_01 = AE_MOVDA32X2(0, 2 * alpha);
  skewQ16_23 = AE_MOVDA32X2(2 * 2 * alpha, 3 * 2 * alpha);
  /* H=h+floorf((w-0.5f)*fabsf(alpha/32768.f)) */
  al = AE_ABS16S(alpha);
  w0 = AE_CVT32X2F16_10(AE_MOVDA16(w));
  inc = AE_MOVDA32(4 * 2 * alpha);
  w1 = AE_SUB32(w0, 0x8000);// w - (1<<15)
  w1 = AE_MULFP32X16X2S_H(w1, al);  
  H = h + AE_MOVAD32_H(AE_SRAI32(w1, 16));

  b_skewQ16 = AE_LT32(alpha, AE_ZERO32());

  skewQ16 = AE_MOVDA32((1 << 15));
  tw = AE_SUB32(skewQ16, w0);
  AE_MOVT32X2(skewQ16, tw, b_skewQ16);

  skewQ16 = AE_MULFP32X16X2S_H(skewQ16, alpha);
  skewQ16_01 = AE_ADD32(skewQ16, skewQ16_01);
  skewQ16_23 = AE_ADD32(skewQ16, skewQ16_23);

  /* starting point for transform */
  src = (const ae_int16x4 *)((uintptr_t)inoutImg + (PADDING* stride + (ALIGNMENT / 2)) * sizeof(int16_t));

  for (j = 0; j<(w>>2); j++)
  {
    int k0, k1, k2, k3;
    ae_int32x2 y0, y1;

    k0 = AE_MOVAD32_H(AE_SRAI32(skewQ16_01, 16));
    k1 = AE_MOVAD32_L(AE_SRAI32(skewQ16_01, 16));
    k2 = AE_MOVAD32_H(AE_SRAI32(skewQ16_23, 16));
    k3 = AE_MOVAD32_L(AE_SRAI32(skewQ16_23, 16));

    aQ31_01 = AE_AND32(skewQ16_01, 0xffff); aQ31_01 = AE_SLAI32(aQ31_01, 15);
    aQ31_23 = AE_AND32(skewQ16_23, 0xffff); aQ31_23 = AE_SLAI32(aQ31_23, 15);
    bQ31_01 = AE_SUB32(0x7fffffff, aQ31_01);
    bQ31_23 = AE_SUB32(0x7fffffff, aQ31_23);

    skewQ16_01 = AE_ADD32(skewQ16_01, inc);
    skewQ16_23 = AE_ADD32(skewQ16_23, inc);


    dst0 = (      ae_int16x4 *)((uintptr_t)pS);
    src0 = (const ae_int16x4 *)((uintptr_t)src) + j;
    src1 = (const ae_int16x4 *)((uintptr_t)src + stride * sizeof(int16_t)) + j;
    AE_L16X4_XP(x0, src0, stride * sizeof(int16_t));
    for (i = 0; i<(h - 1); i++)
    {
      AE_L16X4_XP(x1, src0, stride * sizeof(int16_t));
      y0 = AE_MULFP32X16X2S_H(aQ31_01, x0);
      y1 = AE_MULFP32X16X2S_L(aQ31_23, x0);
      AE_MULAFP32X16X2S_H(y0, bQ31_01, x1);
      AE_MULAFP32X16X2S_L(y1, bQ31_23, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y1);
      AE_S16X4_XP(x0, dst0, sizeof(ae_int16x4));
      x0 = x1;
    }
    /* last pixel */
    AE_S16X4_XP(x0, dst0, sizeof(ae_int16x4));

    src0 = (const ae_int16x4 *)((uintptr_t)pS);
    dst0 = (      ae_int16x4 *)((uintptr_t)src + (k0 * stride + 0) *  sizeof(int16_t)) + j;
    dst1 = (      ae_int16x4 *)((uintptr_t)src + (k1 * stride + 1) *  sizeof(int16_t)) + j;
    dst2 = (      ae_int16x4 *)((uintptr_t)src + (k2 * stride + 2) *  sizeof(int16_t)) + j;
    dst3 = (      ae_int16x4 *)((uintptr_t)src + (k3 * stride + 3) *  sizeof(int16_t)) + j;

    for (i = 0; i<h; i++) 
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst3), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst2), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst1), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }

  inc = AE_MOVDA32(2 * alpha);
  skewQ16_01 = AE_SEL32_HH(skewQ16_01, skewQ16_01);
  for (j = (j<<2); j<(w); j++)
  {
    int k;
    ae_int32x2 a0, b0, y0;
    ae_int16x4 x0, x1;
 
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16_01, 16));
    a0 = AE_AND32(skewQ16_01, 0xffff); a0 = AE_SLAI32(a0, 15);
    b0 = AE_SUB32(0x7fffffff, a0);
    skewQ16_01 = AE_ADD32(skewQ16_01, inc);

    dst0 = (      ae_int16x4 *)((uintptr_t)pS);
    src0 = (const ae_int16x4 *)((uintptr_t)src + j * sizeof(int16_t));
    AE_L16_XP(x0, castxcc(ae_int16, src0), stride * sizeof(int16_t));
    for (i = 0; i<h - 1; i++)
    {
      AE_L16_XP(x1, castxcc(ae_int16, src0), stride * sizeof(int16_t));
      y0 = AE_MULFP32X16X2S_H(a0, x0);
      AE_MULAFP32X16X2S_H(y0, b0, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y0);
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
      x0 = x1;
    }
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));

    src0 = (const ae_int16x4 *)((uintptr_t)pS);
    dst0 = (      ae_int16x4 *)((uintptr_t)src + (k * stride + j) * sizeof(int16_t));
    for (i = 0; i<(h >> 2); i++)
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      AE_S16_0_X(x0, (ae_int16 *)dst0, 3 * stride * sizeof(int16_t));
      x1 = AE_SEL16_4321(x0, x0);
      AE_S16_0_X(x1, (ae_int16 *)dst0, 2 * stride * sizeof(int16_t));
      x1 = AE_SEL16_5342(x0, x0);
      AE_S16_0_X(x1, (ae_int16 *)dst0, 1 * stride * sizeof(int16_t));
      x0 = AE_SEL16_6543(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), 4 * stride * sizeof(int16_t));
    }
    for (i = 0; i<(h & 3); i++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }

  /***************************************/

  __Pragma("no_reorder");
  /* filling */
  inc = AE_MOVDA32(2 * alpha);
  x0 = fillColor;
  for (j = 0; j<(w); j++)
  {
    int k;
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16, 16));
    skewQ16 = AE_ADD32(skewQ16, inc);
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j )* sizeof(int16_t));
    for (i = 0; i<(k); i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j + (h + k)*stride)* sizeof(int16_t));
    for (i = 0; i<(H - (h + k)); i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }

  inoutSz->height=H+PADDING*2;
#elif ALG==2
  const ae_int16x4 * restrict src;
  const ae_int16x4 * restrict src0;
        ae_int16x4 * restrict dst0;
  int i, j, w, h, H, stride;
  ae_int32x2 w0, w1, inc, tw, skewQ16;
  ae_int16x4 al;
  xtbool2 b_skewQ16;
  int16_t * restrict img=(int16_t *)inoutImg;
  (void)pScr;
  int16_t * restrict pS = (int16_t *)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  w=inoutSz->width;
  h=inoutSz->height;
  w-=ALIGNMENT;
  h-=PADDING*2;
  stride=inoutSz->stride;
  imgsize_validate(inoutSz,2,1);
  NASSERT_ALIGN(img,ALIGNMENT);
  /* H=h+floorf((w-0.5f)*fabsf(alpha/32768.f)) */
  al = AE_ABS16S(alpha);
  w0 = AE_CVT32X2F16_10(AE_MOVDA16(w));
  w1 = AE_SUB32(w0, 0x8000);// w - (1<<15)
  w1 = AE_MULFP32X16X2S_H(w1, al);
  H = h + AE_MOVAD32_H(AE_SRAI32(w1, 16));

  b_skewQ16 = AE_LT32(alpha, AE_ZERO32());
  skewQ16 = AE_MOVDA32((1 << 15));
  tw = AE_SUB32(skewQ16, w0);
  AE_MOVT32X2(skewQ16, tw, b_skewQ16);

  skewQ16 = AE_MULFP32X16X2S_H(skewQ16, alpha);
  img += stride*PADDING + ALIGNMENT / 2;   // starting point for transform
  inc = AE_MOVDA32(2 * alpha);
  /* starting point for transform */
  src = (const ae_int16x4 *)((uintptr_t)inoutImg + (PADDING* stride + (ALIGNMENT / 2)) * sizeof(int16_t));

  for (j = 0; j<(w); j++)
  {
    int k;
    ae_int32x2 a0, b0, y0;
    ae_int16x4 x0, x1;
 
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16, 16));
    a0 = AE_AND32(skewQ16, 0xffff); a0 = AE_SLAI32(a0, 15);
    b0 = AE_SUB32(0x7fffffff, a0);
    skewQ16 = AE_ADD32(skewQ16, inc);

    dst0 = (      ae_int16x4 *)((uintptr_t)pS);
    src0 = (const ae_int16x4 *)((uintptr_t)src + j * sizeof(int16_t));
    AE_L16_XP(x0, castxcc(ae_int16, src0), stride * sizeof(int16_t));
    for (i = 0; i<h - 1; i++)
    {
      AE_L16_XP(x1, castxcc(ae_int16, src0), stride * sizeof(int16_t));
      y0 = AE_MULFP32X16X2S_H(a0, x0);
      AE_MULAFP32X16X2S_H(y0, b0, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y0);
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
      x0 = x1;
    }
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));

    src0 = (const ae_int16x4 *)((uintptr_t)pS);
    dst0 = (      ae_int16x4 *)((uintptr_t)src + (k * stride + j) * sizeof(int16_t));
    for (i = 0; i<(h >> 2); i++)
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      x0 = AE_SHORTSWAP(x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
    for (i = 0; i<(h & 3); i++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
    x0 = fillColor;
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j) * sizeof(int16_t));
    for (i = 0; i<k; i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
    dst0 = (ae_int16x4 *)((uintptr_t)src + (j + (h + k)*stride)* sizeof(int16_t));
    for (i = 0; i<(H - (h + k)); i++)
    {
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), stride * sizeof(int16_t));
    }
  }
  inoutSz->height=H+PADDING*2;

#else
    int i,j,w,h,H,stride;
    int16_t * restrict img=(int16_t *)inoutImg;
    (void)pScr;
    int16_t * restrict pS = (int16_t *)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    w=inoutSz->width;
    h=inoutSz->height;
    w-=ALIGNMENT;
    h-=PADDING*2;
    stride=inoutSz->stride;
    imgsize_validate(inoutSz,2,1);
    NASSERT_ALIGN(img,ALIGNMENT);
    img+=stride*PADDING +ALIGNMENT/2;   // starting point for transform
    H=h+(L_mpy_ls(L_deposit_s(w)-(1<<15),S_abs_s(alpha))>>16); //H=h+floorf((w-0.5f)*fabsf(alpha/32768.f));
    for (j=0; j<w; j++)
    {
        int32_t skewQ16,aQ31,bQ31;
        int16_t last;
        int k;
        skewQ16 = alpha<0 ?
            L_mpy_ls(L_deposit_s(j-w)+(1<<15),alpha):
            L_mpy_ls(L_deposit_s(j  )+(1<<15),alpha);
        k=skewQ16>>16;
        aQ31=(skewQ16 & 0xffff)<<15;
        bQ31=L_sub_ll(0x7fffffff,aQ31);
        last=img[(h-1)*stride];
        for (i=0; i<h-1; i++)
        {           
          pS[i] = S_round_l(L_add_ll(L_mpy_ls(aQ31, img[i*stride]), L_mpy_ls(bQ31, img[(i + 1)*stride])));
        }
        pS[i] = last;
        for (i=0; i<k; i++) img[i*stride]=fillColor;
        for (i = 0; i<h; i++) img[(i + k)*stride] = pS[i];
        for (i=0; i<H-(h+k); i++) img[(i+(h+k))*stride]=fillColor;
        img++;
    }
    inoutSz->height=H+PADDING*2;
#endif
} /* img_gs_vshear() */
