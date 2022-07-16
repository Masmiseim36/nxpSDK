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
void img_gs16_rot90  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  xtbool4 b0 = AE_MOVBA4(0x3);
  xtbool4 b1 = AE_MOVBA4(0x5);

  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
  //const ae_int16x4 * restrict src2;
  //const ae_int16x4 * restrict src3;

        ae_int16x4 * restrict dst0;
        ae_int16x4 * restrict dst1;
        ae_int16x4 * restrict dst2;
        ae_int16x4 * restrict dst3;
  int i, j, w, h, ostride, istride;
  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 x0_, x1_, x2_, x3_;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  imgsize_validate(inSz, 2, 0);
  imgsize_validate(outSz, 2, 0);
  NASSERT_ALIGN(outImg, 2);   /* both input and outputs nonaligned */
  NASSERT_ALIGN(inImg, 2);
  ae_valign aD0, aD1;
  ae_valign aS0;
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;

  for (i = 0; i<(h>>1); i++)
  {
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + (h - 2 - 2*i) * sizeof(int16_t));
    src1 = (const ae_int16x4 *)((uintptr_t)inImg + (h - 1 - 2*i) * sizeof(int16_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)outImg + ( 2*i * ostride)*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((2*i+1) * ostride)*sizeof(int16_t));
    aD0 = AE_ZALIGN64();
    aD1 = AE_ZALIGN64();
    for (j = 0; j<(w >> 2); j++)
    {
        AE_L16_XP(x0, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
        AE_L16_XP(x1, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
        AE_L16_XP(x2, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
        AE_L16_XP(x3, castxcc(ae_int16, src0), istride *  sizeof(int16_t));

        AE_L16_XP(x0_, castxcc(ae_int16, src1), istride *  sizeof(int16_t));
        AE_L16_XP(x1_, castxcc(ae_int16, src1), istride *  sizeof(int16_t));
        AE_L16_XP(x2_, castxcc(ae_int16, src1), istride *  sizeof(int16_t));
        AE_L16_XP(x3_, castxcc(ae_int16, src1), istride *  sizeof(int16_t));

        x0 = AE_SEL16_7362(x0, x1);
        x2 = AE_SEL16_7362(x2, x3);
        x0 = AE_SEL16_7632(x0, x2);

        x0_ = AE_SEL16_7362(x0_, x1_);
        x2_ = AE_SEL16_7362(x2_, x3_);
        x0_ = AE_SEL16_7632(x0_, x2_);

        AE_SA16X4_IP(x0_,aD0,  dst0);
        AE_SA16X4_IP(x0, aD1, dst1);
    }
    AE_SA64POS_FP(aD0, dst0); 
    AE_SA64POS_FP(aD1, dst1);
  }

  if (h&1)
  {
      src0 = (const ae_int16x4 *)((uintptr_t)inImg);
      dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((h-1) * ostride)*sizeof(int16_t));
      aD0 = AE_ZALIGN64();
      for (j = 0; j<(w >> 2); j++) //xcc:8/u2
      {

          AE_L16_XP(x0, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
          AE_L16_XP(x1, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
          AE_L16_XP(x2, castxcc(ae_int16, src0), istride *  sizeof(int16_t));
          AE_L16_XP(x3, castxcc(ae_int16, src0), istride *  sizeof(int16_t));

          AE_MOVT16X4(x0, x2, b0);
          AE_MOVT16X4(x1, x3, b0);
          AE_MOVT16X4(x0, x1, b1);
          AE_SA16X4_IP(x0, aD0, dst0);
      }
      AE_SA64POS_FP(aD0, dst0);
  }


  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {

    src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - i)* istride + (h)-(h & 3) - 1) * sizeof(int16_t));
    dst0 = (ae_int16x4 *)((uintptr_t)outImg + (((h & 3) + 0)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + (((h & 3) + 1)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst2 = (ae_int16x4 *)((uintptr_t)outImg + (((h & 3) + 2)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst3 = (ae_int16x4 *)((uintptr_t)outImg + (((h & 3) + 3)* ostride + (w - 1 - i))*sizeof(int16_t));
    aS0 = AE_LA64_PP(src0);
    for (j = 0; j<(h >> 2); j++)
    {
      AE_LA16X4_RIP(x0, aS0, src0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst3), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst2), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst1), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), 4 * ostride*sizeof(int16_t));
    }
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - i) * istride + (h - 1)) * sizeof(int16_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)outImg + (w - 1 - i) *sizeof(int16_t));
    for (j = 0; j<(h & 3); j++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), -(int)sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), ostride*sizeof(int16_t));
    }
  }
  NASSERT(outSz->width == w);
  NASSERT(outSz->height==h);
#else
    int i,j,w,h,istride,ostride;
    const int16_t * restrict in =(const int16_t *)inImg;
          int16_t * restrict out=(      int16_t *)outImg;
    (void)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    imgsize_validate(inSz,2,0);
    imgsize_validate(outSz,2,0);
    NASSERT_ALIGN(outImg,2);   /* both input and outputs nonaligned */ 
    NASSERT_ALIGN(inImg, 2);

    // rotate by 90 degrees
    w = inSz->height;
    h = inSz->width;
    ostride = outSz->stride;
    istride = inSz->stride;
    for (i = 0; i<h; i++)
    {
      for (j = 0; j<w; j++)    out[i*ostride + j] = in[j*istride + (h - 1 - i)];
    }
 #endif  
} /* img_gs16_rot90() */
