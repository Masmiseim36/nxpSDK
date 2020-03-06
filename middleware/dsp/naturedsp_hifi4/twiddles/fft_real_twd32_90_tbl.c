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

ALIGN(8) static const int32_t __fft_real_tw[46] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FB02DC6, (int32_t)0xF7123849,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369,
    (int32_t)0x7D33F0CA, (int32_t)0xE5632654,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE,
    (int32_t)0x71046D3E, (int32_t)0xC3E85B18,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05,
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x5C13539B, (int32_t)0xA7156F3C,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x4ECDFEC7, (int32_t)0x9B2276B0,
    (int32_t)0x4793A210, (int32_t)0x95E218C9,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113,
    (int32_t)0x2FF31BDE, (int32_t)0x89520A1A,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3,
    (int32_t)0x1EF74BF3, (int32_t)0x83CD5982,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x0D61304E, (int32_t)0x80B381AC,
    (int32_t)0x04779632, (int32_t)0x8013F61D,
};

/****************** stage 1 radix 3 ******************/
ALIGN(8) static const int32_t __fft45_tw1[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7EC11AA5, (int32_t)0xEE2F9369, (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46,
    (int32_t)0x7B0A9F8D, (int32_t)0xDCB7EA46, (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x6C8CD70B, (int32_t)0xBC2B9B05, (int32_t)0x381C8BB5, (int32_t)0x8CF45113,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0x4793A210, (int32_t)0x95E218C9, (int32_t)0xD00CE422, (int32_t)0x89520A1A,
    (int32_t)0x381C8BB5, (int32_t)0x8CF45113, (int32_t)0xB1320139, (int32_t)0x9B2276B0,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x04779632, (int32_t)0x8013F61D, (int32_t)0x804FD23A, (int32_t)0xF7123849,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC,
    (int32_t)0xE108B40D, (int32_t)0x83CD5982, (int32_t)0x8EFB92C2, (int32_t)0x3C17A4E8,
    (int32_t)0xD00CE422, (int32_t)0x89520A1A, (int32_t)0xA3ECAC65, (int32_t)0x58EA90C4,
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft45_tw2[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x74EF0EBC, (int32_t)0xCBF00DBE, (int32_t)0x55A6125C, (int32_t)0xA0E0A15F,
    (int32_t)0x55A6125C, (int32_t)0xA0E0A15F, (int32_t)0xF29ECFB2, (int32_t)0x80B381AC,
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE,
    (int32_t)0xF29ECFB2, (int32_t)0x80B381AC, (int32_t)0x82CC0F36, (int32_t)0x1A9CD9AC,
};

#define N 45

static const fft_cplx32x32_stage_t s2_tab[] =
{
    fft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] =
{
    ifft_stageS2_DFT3_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT5_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT3_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT5_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1,
};
static const cint32ptr_t tw_tab[] =
{
    __fft45_tw1, __fft45_tw2, NULL
};

static const fft_cplx32x32_descr_t __cfft_descr =
{
    N,
    s2_tab,
    s3_tab,
    tw_step_tab,
    tw_tab
};
static const fft_cplx32x32_descr_t __cifft_descr =
{
    N,
    is2_tab,
    is3_tab,
    tw_step_tab,
    tw_tab
};
static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr,
    __fft_real_tw
};

const fft_handle_t rnfft32_90 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_90 = (const fft_handle_t)&__rifft_descr;
