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
#include "img_common.h"

/*-------------------------------------------------------------------------
  Image interleave
  Functions convert planar images to packed format 

  Image formats:
  8-bit signed or unsigned data
  16-bit signed data

  Input:
  inImgR
  inImgG
  inImgB  planes with R,G,B components
  sz      image size
  Output:
  outImg  packed image (RGB come together)

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imginterleave    (      void * restrict outImg, 
                       const void * restrict inImgR, 
                       const void * restrict inImgG, 
                       const void * restrict inImgB, 
                       const imgsize_t* sz)
{
    int i,j,w=(int)sz->width,h=(int)sz->height,stride=sz->stride;
    imgsize_validate(sz,1,0);
    NASSERT(outImg);
    NASSERT(inImgR);
    NASSERT(inImgG);
    NASSERT(inImgB);
    NASSERT_ALIGN(outImg,1);
    NASSERT_ALIGN(inImgR,1);
    NASSERT_ALIGN(inImgG,1);
    NASSERT_ALIGN(inImgB,1);
    for (i=0; i<h; i++)
    for (j=0; j<w;  j++)
    {
        uint8_t r=((const uint8_t*)inImgR)[i*stride+j];
        uint8_t g=((const uint8_t*)inImgG)[i*stride+j];
        uint8_t b=((const uint8_t*)inImgB)[i*stride+j];
        ((uint8_t*)outImg)[i*stride*3+j*3+0]=r;
        ((uint8_t*)outImg)[i*stride*3+j*3+1]=g;
        ((uint8_t*)outImg)[i*stride*3+j*3+2]=b;
    }
}
