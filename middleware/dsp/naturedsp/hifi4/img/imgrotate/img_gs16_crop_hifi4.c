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
 out-of-place cropping with conversion from signed 16-bit to destination format
    Input:
    left,up    left/up corner
    inImg,inSz input image in intermediate format
    outSz      output size
    Output:
    outImg     output image
 --------------------------------------------------------------------------*/
void img_gs16_crop(void *pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int left, int up)
{
#if 1
  const ae_int16x4 * restrict src = (const ae_int16x4 *)inImg;
        ae_int16x4 * restrict dst = (      ae_int16x4 *)outImg;
  int i, j, w, h, ostride, istride;
  ae_valign aD;
  ae_int16x4 x0;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg, 2);        /* output non-aligned, input aligned */
  NASSERT_ALIGN(inImg, ALIGNMENT);


  w = outSz->width;
  h = outSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;
  imgsize_validate(inSz, 2, 1);
  imgsize_validate(outSz, 2, 0);
  
  src = (const ae_int16x4 *)((uintptr_t)inImg + (up* istride + left) * sizeof(int16_t));
  for (i = 0; i<h; i++)
  {
      int s_;
      s_ = ((uintptr_t)src & 7) >> 1;
      if (s_) s_ = 4 - s_;
      for (j = 0; j < s_; j++)
      {
          ae_int16x4 p;
          xtbool4 b0;
          AE_L16_IP(p, castxcc(ae_int16, src),sizeof(int16_t));
          b0 = AE_LT16(p, AE_ZERO16());
          AE_MOVT16X4(p, AE_ZERO16(), b0);
          AE_S16_0_IP(p, castxcc(ae_int16, dst), sizeof(int16_t));
      }
      aD = AE_ZALIGN64();
      for (j=0; j<((w-s_)>>2); j++)
      {
          xtbool4 b0;
          AE_L16X4_IP(x0, src, sizeof(ae_int16x4));
          b0 = AE_LT16(x0, AE_ZERO16());
          AE_MOVT16X4(x0, AE_ZERO16(), b0);
          AE_SA16X4_IP(x0, aD, dst);
      }
      AE_SA64POS_FP(aD, dst);
      for (j = 0; j < ((w-s_)&3); j++)
      {
          ae_int16x4 p;
          xtbool4 b0;
          AE_L16_IP(p, castxcc(ae_int16, src), sizeof(int16_t));
          b0 = AE_LT16(p, AE_ZERO16());
          AE_MOVT16X4(p, AE_ZERO16(), b0);
          AE_S16_0_IP(p, castxcc(ae_int16, dst), sizeof(int16_t));
      }     
      src = (const ae_int16x4 *)((uintptr_t)src + (istride - w)*sizeof(int16_t));
      dst = (      ae_int16x4 *)((uintptr_t)dst + (ostride - w)*sizeof(int16_t));
  }

#else
    const int16_t* restrict in;
          int16_t* restrict out;
    int i,j,w,h,ostride,istride;
    (void)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    w=outSz->width;
    h=outSz->height;
    ostride=outSz->stride;
    istride=inSz->stride;
    imgsize_validate(inSz,2,1);
    imgsize_validate(outSz,2,0);
    NASSERT_ALIGN(outImg,2);        /* output non-aligned, input aligned */
    NASSERT_ALIGN(inImg,ALIGNMENT);

    in =(const int16_t*)inImg;
    out=(      int16_t*)outImg;
    in+=up*istride+left;
    for (i=0; i<h; i++,in+=istride)
    {
        for (j=0; j<w; j++) *out++=XT_MAX(0,in[j]);
        out+=ostride-w;
    }
#endif
} /* img_gs16_crop() */
