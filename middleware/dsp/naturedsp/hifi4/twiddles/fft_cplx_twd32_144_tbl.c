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
ALIGN(8) static const int32_t __fft144_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, 
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, 
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x5246DD49, (int32_t)0x9DF24175, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, 
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xEF4AEAF1, (int32_t)0x811855B4, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, 
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0xCF043AB3, (int32_t)0x89BE50C3, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, 
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0xB2141B02, (int32_t)0x9A7365D3, (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0xF4D814A4, (int32_t)0x807CB130, (int32_t)0x9A7365D3, (int32_t)0xB2141B02, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, 
    (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, (int32_t)0x89BE50C3, (int32_t)0xCF043AB3, (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0xD438AF17, (int32_t)0x87B826F7, 
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, (int32_t)0x811855B4, (int32_t)0xEF4AEAF1, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, 
    (int32_t)0xB6950C1E, (int32_t)0x9726069C, (int32_t)0x811855B4, (int32_t)0x10B5150F, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xADB922B7, (int32_t)0x9DF24175, 
    (int32_t)0x845C8AE3, (int32_t)0x2120FB83, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0xA57D8666, (int32_t)0xA57D8666, (int32_t)0x89BE50C3, (int32_t)0x30FBC54D, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, 
    (int32_t)0x9726069C, (int32_t)0xB6950C1E, (int32_t)0x9A7365D3, (int32_t)0x4DEBE4FE, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0xA57D8666, (int32_t)0x5A82799A, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, (int32_t)0xB2141B02, (int32_t)0x658C9A2D, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, (int32_t)0x10B5150F, (int32_t)0x811855B4, 
    (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, (int32_t)0xCF043AB3, (int32_t)0x7641AF3D, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, 
    (int32_t)0xDEDF047D, (int32_t)0x7BA3751D, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x807CB130, (int32_t)0xF4D814A4, (int32_t)0xEF4AEAF1, (int32_t)0x7EE7AA4C, 
};

/****************** stage 2 radix 4 ******************/
ALIGN(8) static const int32_t __fft144_tw2[] = 
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

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft144_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, 
};
#define N 144
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT4x4_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT4x4_32x32,
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
    __fft144_tw1, __fft144_tw2, __fft144_tw3, NULL
};
const fft_cplx32x32_descr_t __cfft_descr144_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_cplx32x32_descr_t __cifft_descr144_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_144 = (const fft_handle_t)&__cfft_descr144_32x32;
const fft_handle_t cinfft32_144 = (const fft_handle_t)&__cifft_descr144_32x32;
