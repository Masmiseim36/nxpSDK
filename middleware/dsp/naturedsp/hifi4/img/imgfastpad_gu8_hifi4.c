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
#include "imgpad_common.h"
/*-------------------------------------------------------------------------
  Image cropping/padding
  Padding function makes left/right/top/bottom padding for input image
  It copies input image to the output at specific position and fills
  borders either with specified color or by duplication of pixels from
  corresponding edge. 
  Region to be copies is set by position of left/upper corner of input
  in the coordinates of output image. If this region is outside the 
  boundaries of output image, then function will crop required region 
  from the input with padding of unused regions
  
  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input
  params      Parameters of image padding operation
  inImage     input image
  Output:
  outImage    output image
  Temporary:
  pScr        Scratch memory area. To determine the scratch area size, 
              use the respective helper functions: xxx_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. each row of output image should be always aligned 
  3. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/
static const imgpad_api imgfastpad_gu8_api=
{
    1,1,
    imgcopyLeftRightBorders_gu8,
    imgcopyUpBtmBorders_gu8,
    imgcopyVert_gu8,
    imgcopyHorz_gu8,
    imgsubcopy_gu8
};
void imgfastpad_gu8  ( void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgpad_params_t* params )
{
    imgpad_process(pScr,outImg,inImg,params,&imgfastpad_gu8_api);
}
/* Returns: size of scratch memory area, in bytes. */
size_t imgfastpad_gu8_getScratchSize        ( const imgpad_params_t * params ){ return imgpad_getScratchSize(params,&imgfastpad_gu8_api);  }
