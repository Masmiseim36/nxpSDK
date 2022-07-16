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
#ifndef IMGRESIZE_COMMON_H__
#define IMGRESIZE_COMMON_H__
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"
#include "imgcopy_common.h"
#include "imgresizer_common.h"

/*-------------------------------------------------------------------------
  Image resize
  Resize process is divided onto initialization and processing phases. On the 
  initialization, the function returns the size of memory requesting for 
  persistent data (i.e. coefficients) needed for dowsampling/upsampling 
  operations. In the processing phase, input image is converted to the output 
  image according to the transformation parameters. Additionally, image resize
  requires some amount of temporary (scratch) data which can not be kept between 
  successive calls of imgresize_process() function and might be reused 
  
  Image formats:
  gs     8-bit grayscale data
  gs16   16-bit grayscale data

  Input
  objmem      Memory block allocated for the instance object:
  params      Parameters of image resize operation
  inImage     input image
  Output:
  outImage    output image
  Temporary:
  pScr        Scratch memory area for the processing function. To 
              determine the scratch area size, use the respective
              helper function: imgresize_getScratchSize()
  Restrictions:
  1. see general restrictions applied for all images for fast/generic 
     functions
  2. pointer to the scratch memory should be aligned on 8-byte boundary
-------------------------------------------------------------------------*/

// allocation routine
size_t imgresize_alloc      ( const imgresize_params_t * params );
// general imitialization
imgresize_handle_t imgresize_init ( void * objmem, const imgresize_params_t * params, 
                                    const imgcopy_api* fncopy_input,const imgcopy_api* fncopy_output, int fast);

// general processing function 
void imgresize_process ( imgresize_handle_t handle, void * restrict pScr, void * restrict outImg, const void * restrict inImg);

// general scratch size 
size_t imgresize_getScratchSize( const imgresize_params_t * params,const imgcopy_api* fncopy_input,const imgcopy_api* fncopy_output);

#endif
