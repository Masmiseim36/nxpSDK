/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2009-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef IMG_COMMON_H__
#define IMG_COMMON_H__
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"

#define ALIGNMENT HIFI_SIMD_WIDTH

/*------------------------------------------
Validate the size of the image
  sz   - image size structure
  bpp  - bytes per pixel
  fast - 'fast' version of image
------------------------------------------*/
inline_ void imgsize_validate(const imgsize_t* sz, int bpp, int fast)
{
    (void)bpp;
    (void)sz;
    (void)fast;
    NASSERT(sz);
    NASSERT(bpp>=1 && bpp<=3);
    NASSERT(sz->width >=8 && sz->width<=sz->stride);
    NASSERT(sz->height >=8 );
    if (fast) 
    {
        NASSERT(sz->stride*bpp % ALIGNMENT==0);
    }
    else
    {
        //NASSERT(sz->width==sz->stride);
    }
}
#endif
