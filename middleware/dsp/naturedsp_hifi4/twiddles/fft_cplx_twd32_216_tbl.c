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
ALIGN(8) static const int32_t __fft216_tw1[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, 
    (int32_t)0x7FF22333, (int32_t)0xFC46F3B2, (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3, (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, 
    (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3, (int32_t)0x7F226F3C, (int32_t)0xF123DE0D, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, 
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, 
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D, (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x7847D909, (int32_t)0xD438AF17, 
    (int32_t)0x7EA6060C, (int32_t)0xED72DF72, (int32_t)0x7A9F667D, (int32_t)0xDB4A081C, (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, 
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, 
    (int32_t)0x7D5B0893, (int32_t)0xE61DBC0E, (int32_t)0x75881AFB, (int32_t)0xCD4D421B, (int32_t)0x68D9F964, (int32_t)0xB6950C1E, 
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, 
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x5A82799A, (int32_t)0xA57D8666, 
    (int32_t)0x7A9F667D, (int32_t)0xDB4A081C, (int32_t)0x6AF143BD, (int32_t)0xB9A9B097, (int32_t)0x5246DD49, (int32_t)0x9DF24175, 
    (int32_t)0x7980C86F, (int32_t)0xD7BCFF58, (int32_t)0x66ABF908, (int32_t)0xB3904EA9, (int32_t)0x496AF3E2, (int32_t)0x9726069C, 
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x40000000, (int32_t)0x9126145F, 
    (int32_t)0x76F4DC14, (int32_t)0xD0BDDA5C, (int32_t)0x5D1A9459, (int32_t)0xA8293BE8, (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, 
    (int32_t)0x75881AFB, (int32_t)0xCD4D421B, (int32_t)0x57D6C418, (int32_t)0xA2E56BA7, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, 
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x2120FB83, (int32_t)0x845C8AE3, 
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, 
    (int32_t)0x70AA706F, (int32_t)0xC3404B8B, (int32_t)0x46564F69, (int32_t)0x950EBC43, (int32_t)0x0B27EB5C, (int32_t)0x807CB130, 
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x00000000, (int32_t)0x80000000, 
    (int32_t)0x6CF16421, (int32_t)0xBCCD9142, (int32_t)0x39724065, (int32_t)0x8D9D7216, (int32_t)0xF4D814A4, (int32_t)0x807CB130, 
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097, (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, 
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0xDEDF047D, (int32_t)0x845C8AE3, 
    (int32_t)0x66ABF908, (int32_t)0xB3904EA9, (int32_t)0x24B5F7E4, (int32_t)0x85609983, (int32_t)0xD438AF17, (int32_t)0x87B826F7, 
    (int32_t)0x6467BB86, (int32_t)0xB09C1F99, (int32_t)0x1D84D223, (int32_t)0x83734404, (int32_t)0xC9E7A512, (int32_t)0x8BFE1B3F, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x5F9E847D, (int32_t)0xAAE7F815, (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4, (int32_t)0xB6950C1E, (int32_t)0x9726069C, 
    (int32_t)0x5D1A9459, (int32_t)0xA8293BE8, (int32_t)0x07714A2D, (int32_t)0x80377032, (int32_t)0xADB922B7, (int32_t)0x9DF24175, 
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x00000000, (int32_t)0x80000000, (int32_t)0xA57D8666, (int32_t)0xA57D8666, 
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7, (int32_t)0xF88EB5D3, (int32_t)0x80377032, (int32_t)0x9DF24175, (int32_t)0xADB922B7, 
    (int32_t)0x551807EB, (int32_t)0xA0617B83, (int32_t)0xF123DE0D, (int32_t)0x80DD90C4, (int32_t)0x9726069C, (int32_t)0xB6950C1E, 
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x9126145F, (int32_t)0xC0000000, 
    (int32_t)0x4F63E067, (int32_t)0x9B98447A, (int32_t)0xE27B2DDD, (int32_t)0x83734404, (int32_t)0x8BFE1B3F, (int32_t)0xC9E7A512, 
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x87B826F7, (int32_t)0xD438AF17, 
    (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0xD438AF17, (int32_t)0x87B826F7, (int32_t)0x845C8AE3, (int32_t)0xDEDF047D, 
    (int32_t)0x46564F69, (int32_t)0x950EBC43, (int32_t)0xCD4D421B, (int32_t)0x8A77E505, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, 
    (int32_t)0x43326EBE, (int32_t)0x930E9BDF, (int32_t)0xC68DBF9B, (int32_t)0x8D9D7216, (int32_t)0x807CB130, (int32_t)0xF4D814A4, 
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, (int32_t)0x80000000, (int32_t)0x00000000, 
    (int32_t)0x3CBFB475, (int32_t)0x8F558F91, (int32_t)0xB9A9B097, (int32_t)0x950EBC43, (int32_t)0x807CB130, (int32_t)0x0B27EB5C, 
    (int32_t)0x39724065, (int32_t)0x8D9D7216, (int32_t)0xB3904EA9, (int32_t)0x995406F8, (int32_t)0x81F1D1CE, (int32_t)0x163A1A7E, 
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0xADB922B7, (int32_t)0x9DF24175, (int32_t)0x845C8AE3, (int32_t)0x2120FB83, 
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7, (int32_t)0x87B826F7, (int32_t)0x2BC750E9, 
    (int32_t)0x2F4225A4, (int32_t)0x890B23EC, (int32_t)0xA2E56BA7, (int32_t)0xA8293BE8, (int32_t)0x8BFE1B3F, (int32_t)0x36185AEE, 
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x9DF24175, (int32_t)0xADB922B7, (int32_t)0x9126145F, (int32_t)0x40000000, 
    (int32_t)0x284300A8, (int32_t)0x867F3791, (int32_t)0x995406F8, (int32_t)0xB3904EA9, (int32_t)0x9726069C, (int32_t)0x496AF3E2, 
    (int32_t)0x24B5F7E4, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0xB9A9B097, (int32_t)0x9DF24175, (int32_t)0x5246DD49, 
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0xA57D8666, (int32_t)0x5A82799A, 
    (int32_t)0x1D84D223, (int32_t)0x83734404, (int32_t)0x8D9D7216, (int32_t)0xC68DBF9B, (int32_t)0xADB922B7, (int32_t)0x620DBE8B, 
    (int32_t)0x19E243F2, (int32_t)0x82A4F76D, (int32_t)0x8A77E505, (int32_t)0xCD4D421B, (int32_t)0xB6950C1E, (int32_t)0x68D9F964, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xC0000000, (int32_t)0x6ED9EBA1, 
    (int32_t)0x128D208E, (int32_t)0x8159F9F4, (int32_t)0x85609983, (int32_t)0xDB4A081C, (int32_t)0xC9E7A512, (int32_t)0x7401E4C1, 
    (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4, (int32_t)0x83734404, (int32_t)0xE27B2DDD, (int32_t)0xD438AF17, (int32_t)0x7847D909, 
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x81F1D1CE, (int32_t)0xE9C5E582, (int32_t)0xDEDF047D, (int32_t)0x7BA3751D, 
    (int32_t)0x07714A2D, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0xF123DE0D, (int32_t)0xE9C5E582, (int32_t)0x7E0E2E32, 
    (int32_t)0x03B90C4E, (int32_t)0x800DDCCD, (int32_t)0x80377032, (int32_t)0xF88EB5D3, (int32_t)0xF4D814A4, (int32_t)0x7F834ED0, 
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft216_tw2[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7F226F3C, (int32_t)0xF123DE0D, (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, 
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, 
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0x6AF143BD, (int32_t)0xB9A9B097, (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x57D6C418, (int32_t)0xA2E56BA7, (int32_t)0xF88EB5D3, (int32_t)0x80377032, 
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7, (int32_t)0x24B5F7E4, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0xB9A9B097, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0x07714A2D, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0xF123DE0D, 
    (int32_t)0xF88EB5D3, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0x0EDC21F3, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0x2BC750E9, 
    (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0x46564F69, (int32_t)0xCD4D421B, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0x5D1A9459, 
};

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft216_tw3[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x620DBE8B, (int32_t)0xADB922B7, 
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, 
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17, (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0x2BC750E9, 
};

/****************** stage 4 radix 3 ******************/
ALIGN(8) static const int32_t __fft216_tw4[] = 
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0xC0000000, (int32_t)0x9126145F, 
};
#define N 216
static const fft_cplx32x32_stage_t s2_tab[] = 
{
    fft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT2_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t s3_tab[] =
{
    fft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT2_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] = 
{
    ifft_stageS2_DFT4_first_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT3_32x32,
    fft_stageS2_DFT2_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is3_tab[] =
{
    ifft_stageS3_DFT4_first_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT2_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 1, 
}; 
static const cint32ptr_t tw_tab[] = 
{
    __fft216_tw1, __fft216_tw2, __fft216_tw3, __fft216_tw4, NULL
};
 const fft_cplx32x32_descr_t __cfft_descr216_32x32 =
{
    N, 
    s2_tab, 
    s3_tab, 
    tw_step_tab,
    tw_tab
};     
 const fft_cplx32x32_descr_t __cifft_descr216_32x32 =
{
    N, 
    is2_tab, 
    is3_tab, 
    tw_step_tab,
    tw_tab
};     
const fft_handle_t cnfft32_216 = (const fft_handle_t)&__cfft_descr216_32x32;
const fft_handle_t cinfft32_216 = (const fft_handle_t)&__cifft_descr216_32x32;
