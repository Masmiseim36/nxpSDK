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
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
Output:
outImg       image
--------------------------------------------------------------------------*/
void img_gs8_rotq  (void * pScr, void* outImg, const imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q)
{
#if 0
  int i, j, w, h, istride, ostride;
  const int8_t * restrict in = (const int8_t *)inImg;
  int8_t * restrict out = (int8_t *)outImg;
  (void)pScr;
  NASSERT_ALIGN(pScr, ALIGNMENT);
  imgsize_validate(inSz, 1, 0);
  imgsize_validate(outSz, 1, 0);
  NASSERT_ALIGN(outImg, 1);   /* both input and outputs nonaligned */
  NASSERT_ALIGN(inImg, 1);
  switch (q)
  {
  case 0:
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
    break;
  case 1:
    // rotate by 90 degrees
    w = inSz->height;
    h = inSz->width;
    ostride = outSz->stride;
    istride = inSz->stride;
    for (i = 0; i<h; i++)
    {
      for (j = 0; j<w; j++)    out[i*ostride + j] = in[j*istride + (h - 1 - i)];
    }
    break;
  case 2:
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
    break;
  case 3:
    // rotate by 270 degrees
    w = inSz->height;
    h = inSz->width;
    ostride = outSz->stride;
    istride = inSz->stride;
    for (i = 0; i<h; i++)
    {
      for (j = 0; j<w; j++)    out[i*ostride + j] = in[(w - 1 - j)*istride + i];
    }
    break;
}
  NASSERT(outSz->width == w);
  NASSERT(outSz->height == h);

#else
    (void)pScr;
    NASSERT_ALIGN(pScr,ALIGNMENT);
    imgsize_validate(inSz ,1,0);
    imgsize_validate(outSz,1,0);
    NASSERT_ALIGN(outImg,1);   /* both input and outputs nonaligned */ 
    NASSERT_ALIGN(inImg, 1);
    switch(q)
    {
    case 0:
        // copy
        img_gu8_rot0(pScr, outImg, outSz, inImg, inSz);
        break;
    case 1:
        // rotate by 90 degrees
        img_gu8_rot90(pScr, outImg, outSz, inImg, inSz);
        break;
    case 2:
        // flip by 180 degrees
        img_gu8_rot180(pScr, outImg, outSz, inImg, inSz);
        break;
    case 3:
        // rotate by 270 degrees
        img_gu8_rot270(pScr, outImg, outSz, inImg, inSz);
        break;
    }
#endif
} /* img_gs8_rotq() */
