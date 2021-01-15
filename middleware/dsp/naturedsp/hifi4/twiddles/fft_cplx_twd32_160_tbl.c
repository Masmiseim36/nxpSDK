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
#include "fft_twiddles32x32.h"
#include "common.h"

/* Twiddles tables for fft_cplx32x32, ifft_cplx32x32 */

/****************** stage 1 radix 4 ******************/
ALIGN(8) static const int32_t __fft160_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FE6BCB0, (int32_t)0xFAF988CC, 
    (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, (int32_t)0x7F9AFCB9, (int32_t)0xF5F50D67, (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, 
    (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7F1CDE01, (int32_t)0xF0F488D9, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, 
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x7C769E18, (int32_t)0xE21E765A, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, 
    (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x7B31BBB2, (int32_t)0xDD417079, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x56E2F15D, (int32_t)0xA201B853, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x7816A759, (int32_t)0xD3B26FB0, 
    (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x3E8B240E, (int32_t)0x90520D04, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, 
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x743E0918, (int32_t)0xCA695B94, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x6FADF2FC, (int32_t)0xC174DBF2, 
    (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0x05067734, (int32_t)0x80194350, (int32_t)0x6D23501B, (int32_t)0xBD1EC45C, (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, 
    (int32_t)0xF5F50D67, (int32_t)0x80650347, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xE70747C4, (int32_t)0x8275A0C0, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x648543E4, (int32_t)0xB0C1878B, 
    (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0xCA695B94, (int32_t)0x8BC1F6E8, (int32_t)0x6154FB91, (int32_t)0xACDEE2E8, (int32_t)0x14060B68, (int32_t)0x81936DAF, 
    (int32_t)0xBD1EC45C, (int32_t)0x92DCAFE5, (int32_t)0x5DFE47AD, (int32_t)0xA91D0EA3, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0xB0C1878B, (int32_t)0x9B7ABC1C, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x56E2F15D, (int32_t)0xA201B853, 
    (int32_t)0xF5F50D67, (int32_t)0x80650347, (int32_t)0x9B7ABC1C, (int32_t)0xB0C1878B, (int32_t)0x53211D18, (int32_t)0x9EAB046F, (int32_t)0xEBF9F498, (int32_t)0x81936DAF, 
    (int32_t)0x92DCAFE5, (int32_t)0xBD1EC45C, (int32_t)0x4F3E7875, (int32_t)0x9B7ABC1C, (int32_t)0xE21E765A, (int32_t)0x838961E8, (int32_t)0x8BC1F6E8, (int32_t)0xCA695B94, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x471CECE7, (int32_t)0x9592675C, 
    (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, (int32_t)0x8275A0C0, (int32_t)0xE70747C4, (int32_t)0x42E13BA4, (int32_t)0x92DCAFE5, (int32_t)0xC5E3A3A9, (int32_t)0x8DF37F8B, 
    (int32_t)0x80650347, (int32_t)0xF5F50D67, (int32_t)0x3E8B240E, (int32_t)0x90520D04, (int32_t)0xBD1EC45C, (int32_t)0x92DCAFE5, (int32_t)0x80194350, (int32_t)0x05067734, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0x3596A46C, (int32_t)0x8BC1F6E8, 
    (int32_t)0xACDEE2E8, (int32_t)0x9EAB046F, (int32_t)0x84CE444E, (int32_t)0x22BE8F87, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, 
    (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, (int32_t)0x2C4D9050, (int32_t)0x87E958A7, (int32_t)0x9EAB046F, (int32_t)0xACDEE2E8, (int32_t)0x90520D04, (int32_t)0x3E8B240E, 
    (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x4B3C8C12, (int32_t)0x22BE8F87, (int32_t)0x84CE444E, 
    (int32_t)0x92DCAFE5, (int32_t)0xBD1EC45C, (int32_t)0xA201B853, (int32_t)0x56E2F15D, (int32_t)0x1DE189A6, (int32_t)0x838961E8, (int32_t)0x8DF37F8B, (int32_t)0xC5E3A3A9, 
    (int32_t)0xACDEE2E8, (int32_t)0x6154FB91, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0xB8E31319, (int32_t)0x6A6D98A4, 
    (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0xC5E3A3A9, (int32_t)0x720C8075, (int32_t)0x0F0B7727, (int32_t)0x80E321FF, 
    (int32_t)0x838961E8, (int32_t)0xE21E765A, (int32_t)0xD3B26FB0, (int32_t)0x7816A759, (int32_t)0x0A0AF299, (int32_t)0x80650347, (int32_t)0x81936DAF, (int32_t)0xEBF9F498, 
    (int32_t)0xE21E765A, (int32_t)0x7C769E18, (int32_t)0x05067734, (int32_t)0x80194350, (int32_t)0x80650347, (int32_t)0xF5F50D67, (int32_t)0xF0F488D9, (int32_t)0x7F1CDE01, 
};

/****************** stage 2 radix 5 ******************/
ALIGN(8) static const int32_t __fft160_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7E6C9251, (int32_t)0xEBF9F498, (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, 
    (int32_t)0x79BC384D, (int32_t)0xD8722192, (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, 
    (int32_t)0x720C8075, (int32_t)0xC5E3A3A9, (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0x14060B68, (int32_t)0x81936DAF, (int32_t)0xD8722192, (int32_t)0x8643C7B3, 
    (int32_t)0x678DDE6E, (int32_t)0xB4C373EE, (int32_t)0x278DDE6E, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x98722192, (int32_t)0xB4C373EE, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x4B3C8C12, (int32_t)0x98722192, (int32_t)0xD8722192, (int32_t)0x8643C7B3, (int32_t)0x8643C7B3, (int32_t)0xD8722192, (int32_t)0x98722192, (int32_t)0x4B3C8C12, 
    (int32_t)0x3A1C5C57, (int32_t)0x8DF37F8B, (int32_t)0xB4C373EE, (int32_t)0x98722192, (int32_t)0x81936DAF, (int32_t)0x14060B68, (int32_t)0xD8722192, (int32_t)0x79BC384D, 
};
#define N 160
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT5_32x32,
    fft_stageS2_DFT8_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT5_32x32,
    fft_stageS3_DFT8_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft160_tw1, __fft160_tw2, NULL
};
const fft_cplx32x32_descr_t __cfft_descr160_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx32x32_descr_t __cifft_descr160_32x32 = 
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_160  = (const fft_handle_t)&__cfft_descr160_32x32;
const fft_handle_t cinfft32_160 = (const fft_handle_t)&__cifft_descr160_32x32;
