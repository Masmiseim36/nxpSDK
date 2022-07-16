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
    horizontal inplace shear functions (16-bit signed data)
    the image is placed beginning from the row PADDING and from the column 
    ALIGNMENT/2. All paddings are written with black pixels
    Input:
    beta    shear scale, Q15
    fillColor filling color
    Input/output
    inoutImg, inoutSz - image and its scale
--------------------------------------------------------------------------*/
void img_gs_hshear(void* pScr, void* inoutImg, imgsize_t * restrict inoutSz,int16_t beta, int fillColor)
{
#if 1
  int i,j,w,h,W,stride;
  const ae_int16x4 * restrict src;
  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
        ae_int16x4 * restrict dst0;
  (void)pScr;
  int16_t * restrict pS = (int16_t *)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  w=inoutSz->width;
  h=inoutSz->height;
  w-=ALIGNMENT;
  h-=PADDING*2;
  stride=inoutSz->stride;
  imgsize_validate(inoutSz,2,1);
  NASSERT_ALIGN(inoutImg, ALIGNMENT);
  ae_int32x2 h0, h1, skewQ16, tw, inc;
  ae_int16x4 al, x0, x1;
  ae_valign aS, aD;
  xtbool2 b_skewQ16;
  
  /* W = w + floorf((h-0.5)*fabsf(beta/32768.f)); */
  al = AE_ABS16S(beta);
  h0 = AE_CVT32X2F16_10(AE_MOVDA16(h));
  h1 = AE_SUB32(h0, 0x8000);// h - (1<<15)
  h1 = AE_MULFP32X16X2S_H(h1, al);
  W = w + AE_MOVAD32_H(AE_SRAI32(h1, 16));

  b_skewQ16 = AE_LT32(beta, AE_ZERO32());
  skewQ16 = AE_MOVDA32((1 << 15));
  tw = AE_SUB32(skewQ16, h0);
  AE_MOVT32X2(skewQ16, tw, b_skewQ16);

  skewQ16 = AE_MULFP32X16X2S_H(skewQ16, beta);
  inc = AE_MOVDA32(2 * beta);
  src = (const ae_int16x4 *)((uintptr_t)inoutImg + (PADDING* stride + (ALIGNMENT / 2)) * sizeof(int16_t));
  for (i=0; i<h; i++)
  {
    int k;
    ae_int32x2 a0, b0;
    ae_int32x2 y0, y1;
    k = AE_MOVAD32_H(AE_SRAI32(skewQ16, 16));
    a0 = AE_AND32(skewQ16, 0xffff); a0 = AE_SLAI32(a0, 15);
    b0 = AE_SUB32(0x7fffffff, a0);
    skewQ16 = AE_ADD32(skewQ16, inc);
    dst0 = (      ae_int16x4 *)((uintptr_t)pS);
    src0 = (const ae_int16x4 *)((uintptr_t)src + i*stride * sizeof(int16_t));
    src1 = (const ae_int16x4 *)((uintptr_t)src + (i*stride + 1) * sizeof(int16_t));
    aS = AE_LA64_PP(src1);
    for (j = 0; j<((w - 1)>>2); j++)
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      AE_LA16X4_IP(x1, aS, src1);
      y0 = AE_MULFP32X16X2S_H(a0, x0);
      y1 = AE_MULFP32X16X2S_L(a0, x0);
      AE_MULAFP32X16X2S_H(y0, b0, x1);
      AE_MULAFP32X16X2S_L(y1, b0, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y1);
      AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
    }
    AE_L16_IP(x0, castxcc(ae_int16, src0), sizeof(int16_t));
    for (j = 0; j<((w - 1)&3); j++)
    {
      AE_L16_IP(x1, castxcc(ae_int16, src0), sizeof(int16_t));
      y0 = AE_MULFP32X16X2S_H(a0, x0);
      AE_MULAFP32X16X2S_H(y0, b0, x1);
      x0 = AE_ROUND16X4F32SASYM(y0, y0);
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
      x0 = x1;
    }
    AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
    src0 = (const ae_int16x4 *)((uintptr_t)pS);
    dst0 = (      ae_int16x4 *)((uintptr_t)src + (i * stride + k) * sizeof(int16_t) );
    aD = AE_ZALIGN64();
    for (j = 0; j<(w>>2); j++)
    {
      AE_L16X4_IP(x0, src0, sizeof(ae_int16x4));
      AE_SA16X4_IP(x0, aD, dst0);
    }
    AE_SA64POS_FP(aD, dst0);
    for (j = 0; j<(w&3); j++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), sizeof(int16_t));
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
    }
    x0 = fillColor;
    dst0 = (      ae_int16x4 *)((uintptr_t)src + (i * stride) * sizeof(int16_t) );
    for (j = 0; j<(k>>2); j++)
    {
      AE_S16X4_IP(x0, dst0, sizeof(ae_int16x4));
    }
    for (j = 0; j<(k&3); j++)
    {
      AE_S16_0_IP(x0, castxcc(ae_int16, dst0), sizeof(int16_t));
    }
    dst0 = (ae_int16x4 *)((uintptr_t)src + ((w + k) + i * stride) * sizeof(int16_t));
    aD = AE_ZALIGN64();
    for (j = 0; j<((W - (w + k)) >> 2); j++)
    {
      AE_SA16X4_IP(x0, aD, dst0);
    }
    for (j = 0; j<((W - (w + k)) & 3); j++)
    {
      AE_S16_0_IP(x0, castxcc(ae_int16,dst0), sizeof(int16_t));
    }
  }
  inoutSz->width=W+ALIGNMENT;
#else
    int i,j,w,h,W,stride;
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
    W=w+(L_mpy_ls(L_deposit_s(h)-(1<<15),S_abs_s(beta))>>16);        //floorf((h-0.5)*fabsf(beta/32768.f));
    for (i=0; i<h; i++)
    {
        int32_t skewQ16,aQ31,bQ31;
        int16_t last;
        int k;
        skewQ16 = beta<0 ?
            L_mpy_ls(L_deposit_s(i-h)+(1<<15),beta):
            L_mpy_ls(L_deposit_s(i  )+(1<<15),beta);
        k=skewQ16>>16;
        aQ31=(skewQ16 & 0xffff)<<15;
        bQ31=L_sub_ll(0x7fffffff,aQ31);
        last=img[w-1+i*stride];
        for (j=0; j<w-1; j++) 
        {           
          pS[j] = S_round_l(L_add_ll(L_mpy_ls(aQ31, img[j + i*stride]), L_mpy_ls(bQ31, img[j + 1 + i*stride])));
        }

        pS[j] = last;
        for (j = 0; j<k; j++) img[j + i*stride] = fillColor;
        for (j = 0; j<w; j++) img[j + k + i*stride] = pS[j];
        for (j = 0; j<W - (w + k); j++) img[j + (w + k) + i*stride] = fillColor;
    }
    inoutSz->width=W+ALIGNMENT;
#endif
} /* img_gs_hshear */
