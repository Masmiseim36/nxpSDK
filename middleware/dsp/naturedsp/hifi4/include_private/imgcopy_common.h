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
#ifndef IMGCOPY_COMMON_H__
#define IMGCOPY_COMMON_H__ 1
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"

/* image copy APIs */
typedef struct
{
    size_t (*getScratchSize)(const imgsize_t* in,const imgsize_t* out);
    void (*copy)(void* pScr, void* outImg,const void* inImg,const imgsize_t* in,const imgsize_t* out);
}
imgcopy_api;

/* converters to/from 8-bit unsigned*/
extern const imgcopy_api  imgcopy_api_gu8fast_temp;
extern const imgcopy_api  imgcopy_api_temp_gu8fast;
extern const imgcopy_api  imgcopy_api_gu8_temp;
extern const imgcopy_api  imgcopy_api_temp_gu8;

/* converters to/from 8-bit signed*/
extern const imgcopy_api  imgcopy_api_gs8fast_temp;
extern const imgcopy_api  imgcopy_api_temp_gs8fast;
extern const imgcopy_api  imgcopy_api_gs8_temp;
extern const imgcopy_api  imgcopy_api_temp_gs8;

/* converters to/from 16-bit */
extern const imgcopy_api  imgcopy_api_gs16fast_temp;
extern const imgcopy_api  imgcopy_api_temp_gs16fast;
extern const imgcopy_api  imgcopy_api_gs16_temp;
extern const imgcopy_api  imgcopy_api_temp_gs16;


#endif
