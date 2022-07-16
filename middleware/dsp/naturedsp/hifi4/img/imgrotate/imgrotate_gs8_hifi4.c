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

size_t img_gs8_getScratchSize     (const imgrotate_params_t* params) 
{ 
  /* if rotation is a multiple of 90 degrees, than ScratchSize = 0 */
  if ((params->angleQ15 & 0x3fff) == 0)
  {
    return 0;
  }
  else
  {
    int w, h;
    h = params->in.height;
    w = params->in.width;
    if (ALG == 1)  { return 4 * (w + h) * sizeof(int16_t); }
    else { return 2 * (w + h + 2) * sizeof(int16_t); }
  }

}
static const tImgRotateApi imgrotate_gs8_api=
{
    img_gs8_rotq,
    img_gs8_rotq16, 
    img_gs_vshear,
    img_gs_hshear,
    img_gs8_crop,
    img_gs8_getScratchSize,
    IMGROTATE_GS8_MAGIC, /* magic number for identification     */
    1,                  /* bytes per pixel                     */
    0,                  /* 1 - fast, 0 - generic               */
    2                   /* bytes per pixel for temporary image */
};

/*-------------------------------------------------------------------------
  Image rotate
  Image rotation function rotates input image counter clockwise about their 
  center
  Rotation following algorithms:
  - dedicated variants for 90, 180 and -90 degree rotations
  - for small angles in range -pi/4...pi/4 - 3-shear steps
  - for another angles - proper combinations of above variants. 
  Since all of these algorithms show very different computational 
  speed, overall transformation performance depends not only from
  the image size, but from the rotation angle as well.
  The size of resulted image depends on the angle and all unused portions 
  of image will be filled with fill color. Required output size will be 
  requested by getOutSize API. User should use this size as an input for 
  process function, but may select equal or bigger stride between image 
  rows
  
  Image formats:
  gu8    8-bit unsigned grayscale data
  gs8    8-bit signed grayscale data
  gs16   16-bit grayscale data

  Input
  objmem      Memory block allocated for the instance object:
  params      Parameters of image rotate operation
  inImage     input image

  Output:
  outImage    output image
  outSz       resulted size of output image 

  Temporary:
  pScr        Scratch memory area. To determine the scratch area size, 
              use the respective helper function: imgrotate_xxx_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/
size_t             imgrotate_gs8_alloc         ( const imgrotate_params_t * params ) { return imgrotate_alloc(params,&imgrotate_gs8_api); }
imgrotate_handle_t imgrotate_gs8_init          ( void * objmem, const imgrotate_params_t * params ) { return imgrotate_init(objmem,params,&imgrotate_gs8_api); }
void               imgrotate_gs8_getOutSize    (imgsize_t * restrict outSz, const imgrotate_params_t * params) { imgrotate_getOutSize(outSz,params,&imgrotate_gs8_api); }
void               imgrotate_gs8_process       ( imgrotate_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg, const imgsize_t *outSz) { imgrotate_process  ( handle, pScr, outImg, inImg,outSz); }
size_t             imgrotate_gs8_getScratchSize( const imgrotate_params_t * params ) { return imgrotate_getScratchSize(params,&imgrotate_gs8_api); }
