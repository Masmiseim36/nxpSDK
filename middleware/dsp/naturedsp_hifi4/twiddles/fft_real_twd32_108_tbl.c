/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_Signal_fft.h"
#include "fft_twiddles32x32.h"
#include "common.h"

ALIGN(8) static const int32_t __fft_real_tw[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7A9F667D, (int32_t)0xDB4A081C,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x75881AFB, (int32_t)0xCD4D421B,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x66ABF908, (int32_t)0xB3904EA9,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x5D1A9459, (int32_t)0xA8293BE8,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x46564F69, (int32_t)0x950EBC43,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x39724065, (int32_t)0x8D9D7216,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x1D84D223, (int32_t)0x83734404,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
};
/****************** stage 1 radix 2 ******************/
ALIGN(8) static const int32_t __fft54_tw1[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032,
    (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE,
    (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0xCD4D421B, (int32_t)0x8A77E505,
    (int32_t)0xC0000000, (int32_t)0x9126145F,
    (int32_t)0xB3904EA9, (int32_t)0x995406F8,
    (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0x9DF24175, (int32_t)0xADB922B7,
    (int32_t)0x950EBC43, (int32_t)0xB9A9B097,
    (int32_t)0x8D9D7216, (int32_t)0xC68DBF9B,
    (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x83734404, (int32_t)0xE27B2DDD,
    (int32_t)0x80DD90C4, (int32_t)0xF123DE0D,
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft54_tw2[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0x0EDC21F3,
    (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0x46564F69,
};

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft54_tw3[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
};
static const fft_cplx32x32_stage_t s2_tab[] =
{
    fft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] =
{
    ifft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT2_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1,
};
static const cint32ptr_t tw_tab[] =
{
    __fft54_tw1, __fft54_tw2, __fft54_tw3, NULL
};
static const fft_cplx32x32_descr_t __cfft_descr54 =
{
    54,
    s2_tab,
    s3_tab,
    tw_step_tab,
    tw_tab
};
static const fft_cplx32x32_descr_t __cifft_descr54 =
{
    54,
    is2_tab,
    is3_tab,
    tw_step_tab,
    tw_tab
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr54,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr54,
    __fft_real_tw
};

const fft_handle_t rnfft32_108 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_108 = (const fft_handle_t)&__rifft_descr;
