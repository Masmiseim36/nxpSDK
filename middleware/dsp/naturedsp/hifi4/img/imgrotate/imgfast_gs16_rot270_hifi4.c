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
void imgfast_gs16_rot270 (void* pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz)
{
#if 1
  const ae_int16x4 * restrict src0;
  const ae_int16x4 * restrict src1;
  const ae_int16x4 * restrict src2;
  const ae_int16x4 * restrict src3;
        ae_int16x4 * restrict dst0;
        ae_int16x4 * restrict dst1;
        ae_int16x4 * restrict dst2;
        ae_int16x4 * restrict dst3;
  int i, j, w, h, ostride, istride;
  ae_int16x4 x0, x1, x2, x3;
  ae_int16x4 y0, y1, y2, y3;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);
  NASSERT_ALIGN(inImg ,ALIGNMENT);

  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;

  for (i=0; i<(w>>2); i++)
  {
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 4 * i - 0) * istride) * sizeof(int16_t));
    src1 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 4 * i - 1) * istride) * sizeof(int16_t));
    src2 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 4 * i - 2) * istride) * sizeof(int16_t));
    src3 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 4 * i - 3) * istride) * sizeof(int16_t));

    dst0 = (ae_int16x4 *)((uintptr_t)outImg + (0* ostride)*sizeof(int16_t)) + i;
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + (1* ostride)*sizeof(int16_t)) + i;
    dst2 = (ae_int16x4 *)((uintptr_t)outImg + (2* ostride)*sizeof(int16_t)) + i;
    dst3 = (ae_int16x4 *)((uintptr_t)outImg + (3* ostride)*sizeof(int16_t)) + i;
    for (j = 0; j<(h >> 2); j++)
    {
      /* AE_L16X4_XP for better scheduling*/
      AE_L16X4_XP(x0, src0, sizeof(ae_int16x4));
      AE_L16X4_XP(x1, src1, sizeof(ae_int16x4));
      AE_L16X4_XP(x2, src2, sizeof(ae_int16x4));
      AE_L16X4_XP(x3, src3, sizeof(ae_int16x4));

      y0 = AE_SEL16_7362(x0, x1);
      y1 = AE_SEL16_7362(x2, x3);
      y2 = AE_SEL16_5140(x0, x1);
      y3 = AE_SEL16_5140(x2, x3);

      x3 = AE_SEL16_5410(y2, y3);
      x2 = AE_SEL16_7632(y2, y3);
      x1 = AE_SEL16_5410(y0, y1);
      x0 = AE_SEL16_7632(y0, y1);

      AE_S16X4_XP(x0, dst0, 4 * ostride*sizeof(int16_t));
      AE_S16X4_XP(x1, dst1, 4 * ostride*sizeof(int16_t));
      AE_S16X4_XP(x2, dst2, 4 * ostride*sizeof(int16_t));
      AE_S16X4_XP(x3, dst3, 4 * ostride*sizeof(int16_t));
    }
  }
  __Pragma("no_reorder");
  for (j = 0; j<(h&3); j++)
  {
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 0) * istride + (h - 1 - j)) * sizeof(int16_t));
    src1 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 1) * istride + (h - 1 - j)) * sizeof(int16_t));
    src2 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 2) * istride + (h - 1 - j)) * sizeof(int16_t));
    src3 = (const ae_int16x4 *)((uintptr_t)inImg + ((w - 1 - 3) * istride + (h - 1 - j)) * sizeof(int16_t));

    dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((h - 1 - j) * ostride)*sizeof(int16_t));  
    for (i = 0; i<(w >> 2); i++)
    {
      ae_int16x4 p0, p1, p2, p3;
      AE_L16_XP(p0,castxcc(ae_int16,src0),-(int)(sizeof(int16_t)*4*istride));
      AE_L16_XP(p1,castxcc(ae_int16,src1),-(int)(sizeof(int16_t)*4*istride));
      AE_L16_XP(p2,castxcc(ae_int16,src2),-(int)(sizeof(int16_t)*4*istride));
      AE_L16_XP(p3,castxcc(ae_int16,src3),-(int)(sizeof(int16_t)*4*istride));

      AE_S16_0_IP(p0,castxcc(ae_int16,dst0),sizeof(int16_t));
      AE_S16_0_IP(p1,castxcc(ae_int16,dst0),sizeof(int16_t));
      AE_S16_0_IP(p2,castxcc(ae_int16,dst0),sizeof(int16_t));
      AE_S16_0_IP(p3,castxcc(ae_int16,dst0),sizeof(int16_t));
    }
  }
  __Pragma("no_reorder");
  for (i = 0; i<(w & 3); i++)
  {

    src0 = (const ae_int16x4 *)((uintptr_t)inImg + (i * istride) * sizeof(int16_t));
    dst0 = (ae_int16x4 *)((uintptr_t)outImg + ((0)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst1 = (ae_int16x4 *)((uintptr_t)outImg + ((1)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst2 = (ae_int16x4 *)((uintptr_t)outImg + ((2)* ostride + (w - 1 - i))*sizeof(int16_t));
    dst3 = (ae_int16x4 *)((uintptr_t)outImg + ((3)* ostride + (w - 1 - i))*sizeof(int16_t));
    for (j = 0; j<(h >> 2); j++)
    {
      AE_L16X4_XP(x0, src0, (int)sizeof(ae_int16x4));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst3), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst2), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst1), 4 * ostride*sizeof(int16_t));
      x0 = AE_SEL16_4321(x0, x0);
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), 4 * ostride*sizeof(int16_t));
    }
    src0 = (const ae_int16x4 *)((uintptr_t)inImg + (i* istride + (h - 1)) * sizeof(int16_t));
    dst0 = (      ae_int16x4 *)((uintptr_t)outImg + ((h - 1)* ostride + (w - 1 - i))*sizeof(int16_t));
    for (j = 0; j<(h & 3); j++)
    {
      AE_L16_IP(x0, castxcc(ae_int16, src0), -(int)sizeof(int16_t));
      AE_S16_0_XP(x0, castxcc(ae_int16, dst0), -ostride*sizeof(int16_t));
    }
  }
#else
  int i,j,w,h,ostride,istride;
  const int16_t * restrict in =(const int16_t *)inImg;
        int16_t * restrict out=(      int16_t *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);
  NASSERT_ALIGN(inImg ,ALIGNMENT);
  imgsize_validate(inSz,2,1);
  imgsize_validate(outSz,2,1);

  // copy
  w = inSz->height;
  h = inSz->width;
  ostride = outSz->stride;
  istride = inSz->stride;
  for (i = 0; i<h; i++)
  {
    for (j = 0; j<w; j++)    out[i*ostride + j] = in[(w - 1 - j)*istride + i];
  }
  NASSERT(outSz->width ==w);
  NASSERT(outSz->height==h);
#endif
} /* imgfast_gs16_rot270() */
