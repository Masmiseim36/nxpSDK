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
void img_gs16_rot0  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  const ae_int16x4 * restrict src = (const ae_int16x4 *)inImg;
        ae_int16x4 * restrict dst = (      ae_int16x4 *)outImg;

  int i,j,w,h,istride,ostride;
  int sh;
  (void)pScr;
  ae_int16x4 x0;
  ae_valign aD, aS;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  imgsize_validate(inSz,2,0);
  imgsize_validate(outSz,2,0);
  NASSERT_ALIGN(outImg,2);   /* both input and outputs nonaligned */ 
  NASSERT_ALIGN(inImg, 2);

  // copy
  w=inSz->width;
  h=inSz->height;
  ostride = outSz->stride;
  istride = inSz->stride;

  aD = AE_ZALIGN64();
  for (i=0; i<h; i++)
  { 
    src = (const ae_int16x4 *)((int16_t*)inImg + i*istride);
    dst = (ae_int16x4 *)((int16_t*)outImg + i*ostride);
    aS = AE_LA64_PP(src);
    /* align input */
    sh = ((uintptr_t)src & 7) >> 1; 
    AE_LA16X4_IP(x0, aS, src);
    AE_SA16X4_IP(x0, aD, dst);
    AE_SA64POS_FP(aD, dst);
    src = (const ae_int16x4*)((int16_t*)src - sh);
    dst = (ae_int16x4*)((int16_t*)dst - sh);
    for (j = 0; j<((w >> 2)-1); j++)
    {
        AE_L16X4_IP(x0, src, sizeof(ae_int16x4));
        AE_SA16X4_IP(x0, aD, dst);
    }
    AE_SA64POS_FP(aD, dst);
    for (j = 0; j < sh; j++)
    {
        ae_int16x4 p;
        AE_L16_IP(p, castxcc(ae_int16, src), sizeof(int16_t));
        AE_S16_0_IP(p, castxcc(ae_int16, dst), sizeof(int16_t));
    }
  }  
  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {
    src = (const ae_int16x4 *)((int16_t*)inImg + w - 1 - i);
    dst = (      ae_int16x4 *)((int16_t*)outImg + w - 1 - i);
    for (j = 0; j<h; j++)
    {
      ae_int16x4 p;
      AE_L16_XP(p, castxcc(ae_int16, src), sizeof(int16_t)*istride);
      AE_S16_0_XP(p, castxcc(ae_int16, dst), sizeof(int16_t)*ostride);
    }
  }
  NASSERT(outSz->width ==w);
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

    // copy
    w=inSz->width;
    h=inSz->height;
    ostride = outSz->stride;
    istride = inSz->stride;
    for (i=0; i<h; i++, in+=istride)
    {
        for (j=0; j<w; j++) *out++=in[j];
        out+=ostride-w;
    }
#endif 
} /* img_gs16_rot0() */
