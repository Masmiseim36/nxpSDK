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
ALIGN(8) static const int32_t __fft36_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, 
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft36_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, 
};
#define N 36
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft36_tw1, __fft36_tw2, NULL
};
const fft_cplx32x32_descr_t __cfft_descr36_32x32 = 
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx32x32_descr_t __cifft_descr36_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_36 = (const fft_handle_t)&__cfft_descr36_32x32;
const fft_handle_t cinfft32_36 = (const fft_handle_t)&__cifft_descr36_32x32;
