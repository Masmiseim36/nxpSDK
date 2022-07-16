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
rotation by 0,90,180,270 with format conversion
the image is placed beginning from the row PADDING and from the column 
ALIGNMENT/2. All paddings are written with black pixels
8-bit gs  -> signed 16-bit 1 pixel per 16-bit word
16-bit gs -> signed 16-bit 1 pixel per 16-bit word
RGB       -> signed 16-bit 1 pixel per 64-bit word
Input:
inImg, inSz  image and its size
outSz        output image size
q            rotation (0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 
             3 - 270 degrees conterclockwise
fillColor    filling color
Output:
outImg       image
--------------------------------------------------------------------------*/
void img_gu8_rotq16(void* pScr,void* outImg, imgsize_t * restrict outSz, const void* inImg, const imgsize_t * restrict inSz, int q, int fillColor)
{
  (void)pScr;
  NASSERT_ALIGN(pScr,ALIGNMENT);
  NASSERT_ALIGN(outImg,ALIGNMENT);    /* inImg - not aligned, outImg - aligned */    
  NASSERT_ALIGN(outImg,1);
  imgsize_validate(inSz,1,0);
  imgsize_validate(outSz,2,1);
  switch(q&3)
  {
  case 0:
      // copy
      img_gu8_rot0_16(pScr, outImg, outSz, inImg, inSz, fillColor);
      break;
  case 1:
      // rotate by 90 degrees counterclockwise
      img_gu8_rot90_16(pScr, outImg, outSz, inImg, inSz, fillColor);
      break;
  case 2:
      // flip by 180 degrees
      img_gu8_rot180_16(pScr, outImg, outSz, inImg, inSz, fillColor);
      break;
  case 3:
      // rotate by 270 degrees counterclockwise
      img_gu8_rot270_16(pScr, outImg, outSz, inImg, inSz, fillColor);
      break;
  }
} /* img_gu8_rotq16() */
