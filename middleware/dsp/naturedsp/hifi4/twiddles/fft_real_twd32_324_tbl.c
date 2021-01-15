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
    (int32_t)0x7FF9D6B2, (int32_t)0xFD8495B8,
    (int32_t)0x7FE75B5E, (int32_t)0xFB09689C,
    (int32_t)0x7FC88FCE, (int32_t)0xF88EB5D3,
    (int32_t)0x7F9D76F6, (int32_t)0xF614BA76,
    (int32_t)0x7F6614FF, (int32_t)0xF39BB38E,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7ED28C31, (int32_t)0xEEAD76C7,
    (int32_t)0x7E76738E, (int32_t)0xEC38BA6B,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582,
    (int32_t)0x7D99C626, (int32_t)0xE7553464,
    (int32_t)0x7D19469F, (int32_t)0xE4E6E334,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7BF433C5, (int32_t)0xE0125006,
    (int32_t)0x7B4FBCA9, (int32_t)0xDDAC8513,
    (int32_t)0x7A9F667D, (int32_t)0xDB4A081C,
    (int32_t)0x79E3423B, (int32_t)0xD8EB13E5,
    (int32_t)0x791B61FF, (int32_t)0xD68FE2DD,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x7768BBB5, (int32_t)0xD1E5B242,
    (int32_t)0x767E1F7F, (int32_t)0xCF9725A5,
    (int32_t)0x75881AFB, (int32_t)0xCD4D421B,
    (int32_t)0x7486C5D9, (int32_t)0xCB08400C,
    (int32_t)0x737A38E0, (int32_t)0xC8C85766,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x713FDFE4, (int32_t)0xC458AF9A,
    (int32_t)0x70124AC9, (int32_t)0xC2295DC9,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000,
    (int32_t)0x6D96E081, (int32_t)0xBDDCCB86,
    (int32_t)0x6C494880, (int32_t)0xBBBFF508,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x698EF356, (int32_t)0xB79A31A4,
    (int32_t)0x68227968, (int32_t)0xB591AAF5,
    (int32_t)0x66ABF908, (int32_t)0xB3904EA9,
    (int32_t)0x652B9645, (int32_t)0xB1964E2A,
    (int32_t)0x63A17621, (int32_t)0xAFA3DA30,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x60709663, (int32_t)0xABD656FD,
    (int32_t)0x5ECA256F, (int32_t)0xA9FBA57C,
    (int32_t)0x5D1A9459, (int32_t)0xA8293BE8,
    (int32_t)0x5B620CAF, (int32_t)0xA65F4728,
    (int32_t)0x59A0B8D8, (int32_t)0xA49DF351,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x56045A84, (int32_t)0xA135DA91,
    (int32_t)0x5429A903, (int32_t)0x9F8F699D,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175,
    (int32_t)0x505C25D0, (int32_t)0x9C5E89DF,
    (int32_t)0x4E69B1D6, (int32_t)0x9AD469BB,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x4A6E550B, (int32_t)0x97DD8698,
    (int32_t)0x4865CE5C, (int32_t)0x96710CAA,
    (int32_t)0x46564F69, (int32_t)0x950EBC43,
    (int32_t)0x44400AF8, (int32_t)0x93B6B780,
    (int32_t)0x4223347A, (int32_t)0x92691F7F,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x3DD6A237, (int32_t)0x8FEDB537,
    (int32_t)0x3BA75066, (int32_t)0x8EC0201C,
    (int32_t)0x39724065, (int32_t)0x8D9D7216,
    (int32_t)0x3737A89A, (int32_t)0x8C85C720,
    (int32_t)0x34F7BFF4, (int32_t)0x8B793A27,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x3068DA5B, (int32_t)0x8981E081,
    (int32_t)0x2E1A4DBE, (int32_t)0x8897444B,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7,
    (int32_t)0x29701D23, (int32_t)0x86E49E01,
    (int32_t)0x2714EC1B, (int32_t)0x861CBDC5,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x22537AED, (int32_t)0x84B04357,
    (int32_t)0x1FEDAFFA, (int32_t)0x840BCC3B,
    (int32_t)0x1D84D223, (int32_t)0x83734404,
    (int32_t)0x1B191CCC, (int32_t)0x82E6B961,
    (int32_t)0x18AACB9C, (int32_t)0x826639DA,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x13C74595, (int32_t)0x81898C72,
    (int32_t)0x11528939, (int32_t)0x812D73CF,
    (int32_t)0x0EDC21F3, (int32_t)0x80DD90C4,
    (int32_t)0x0C644C72, (int32_t)0x8099EB01,
    (int32_t)0x09EB458A, (int32_t)0x8062890A,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
    (int32_t)0x04F69764, (int32_t)0x8018A4A2,
    (int32_t)0x027B6A48, (int32_t)0x8006294E,
};

/****************** stage 1 radix 2 ******************/
ALIGN(8) static const int32_t __fft162_tw1[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7FE75B5E, (int32_t)0xFB09689C,
    (int32_t)0x7F9D76F6, (int32_t)0xF614BA76,
    (int32_t)0x7F226F3C, (int32_t)0xF123DE0D,
    (int32_t)0x7E76738E, (int32_t)0xEC38BA6B,
    (int32_t)0x7D99C626, (int32_t)0xE7553464,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD,
    (int32_t)0x7B4FBCA9, (int32_t)0xDDAC8513,
    (int32_t)0x79E3423B, (int32_t)0xD8EB13E5,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x767E1F7F, (int32_t)0xCF9725A5,
    (int32_t)0x7486C5D9, (int32_t)0xCB08400C,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x70124AC9, (int32_t)0xC2295DC9,
    (int32_t)0x6D96E081, (int32_t)0xBDDCCB86,
    (int32_t)0x6AF143BD, (int32_t)0xB9A9B097,
    (int32_t)0x68227968, (int32_t)0xB591AAF5,
    (int32_t)0x652B9645, (int32_t)0xB1964E2A,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x5ECA256F, (int32_t)0xA9FBA57C,
    (int32_t)0x5B620CAF, (int32_t)0xA65F4728,
    (int32_t)0x57D6C418, (int32_t)0xA2E56BA7,
    (int32_t)0x5429A903, (int32_t)0x9F8F699D,
    (int32_t)0x505C25D0, (int32_t)0x9C5E89DF,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x4865CE5C, (int32_t)0x96710CAA,
    (int32_t)0x44400AF8, (int32_t)0x93B6B780,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x3BA75066, (int32_t)0x8EC0201C,
    (int32_t)0x3737A89A, (int32_t)0x8C85C720,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505,
    (int32_t)0x2E1A4DBE, (int32_t)0x8897444B,
    (int32_t)0x29701D23, (int32_t)0x86E49E01,
    (int32_t)0x24B5F7E4, (int32_t)0x85609983,
    (int32_t)0x1FEDAFFA, (int32_t)0x840BCC3B,
    (int32_t)0x1B191CCC, (int32_t)0x82E6B961,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x11528939, (int32_t)0x812D73CF,
    (int32_t)0x0C644C72, (int32_t)0x8099EB01,
    (int32_t)0x07714A2D, (int32_t)0x80377032,
    (int32_t)0x027B6A48, (int32_t)0x8006294E,
    (int32_t)0xFD8495B8, (int32_t)0x8006294E,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032,
    (int32_t)0xF39BB38E, (int32_t)0x8099EB01,
    (int32_t)0xEEAD76C7, (int32_t)0x812D73CF,
    (int32_t)0xE9C5E582, (int32_t)0x81F1D1CE,
    (int32_t)0xE4E6E334, (int32_t)0x82E6B961,
    (int32_t)0xE0125006, (int32_t)0x840BCC3B,
    (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0xD68FE2DD, (int32_t)0x86E49E01,
    (int32_t)0xD1E5B242, (int32_t)0x8897444B,
    (int32_t)0xCD4D421B, (int32_t)0x8A77E505,
    (int32_t)0xC8C85766, (int32_t)0x8C85C720,
    (int32_t)0xC458AF9A, (int32_t)0x8EC0201C,
    (int32_t)0xC0000000, (int32_t)0x9126145F,
    (int32_t)0xBBBFF508, (int32_t)0x93B6B780,
    (int32_t)0xB79A31A4, (int32_t)0x96710CAA,
    (int32_t)0xB3904EA9, (int32_t)0x995406F8,
    (int32_t)0xAFA3DA30, (int32_t)0x9C5E89DF,
    (int32_t)0xABD656FD, (int32_t)0x9F8F699D,
    (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0xA49DF351, (int32_t)0xA65F4728,
    (int32_t)0xA135DA91, (int32_t)0xA9FBA57C,
    (int32_t)0x9DF24175, (int32_t)0xADB922B7,
    (int32_t)0x9AD469BB, (int32_t)0xB1964E2A,
    (int32_t)0x97DD8698, (int32_t)0xB591AAF5,
    (int32_t)0x950EBC43, (int32_t)0xB9A9B097,
    (int32_t)0x92691F7F, (int32_t)0xBDDCCB86,
    (int32_t)0x8FEDB537, (int32_t)0xC2295DC9,
    (int32_t)0x8D9D7216, (int32_t)0xC68DBF9B,
    (int32_t)0x8B793A27, (int32_t)0xCB08400C,
    (int32_t)0x8981E081, (int32_t)0xCF9725A5,
    (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x861CBDC5, (int32_t)0xD8EB13E5,
    (int32_t)0x84B04357, (int32_t)0xDDAC8513,
    (int32_t)0x83734404, (int32_t)0xE27B2DDD,
    (int32_t)0x826639DA, (int32_t)0xE7553464,
    (int32_t)0x81898C72, (int32_t)0xEC38BA6B,
    (int32_t)0x80DD90C4, (int32_t)0xF123DE0D,
    (int32_t)0x8062890A, (int32_t)0xF614BA76,
    (int32_t)0x8018A4A2, (int32_t)0xFB09689C,
};

/****************** stage 2 radix 3 ******************/
ALIGN(8) static const int32_t __fft162_tw2[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x7F9D76F6, (int32_t)0xF614BA76, (int32_t)0x7E76738E, (int32_t)0xEC38BA6B,
    (int32_t)0x7E76738E, (int32_t)0xEC38BA6B, (int32_t)0x79E3423B, (int32_t)0xD8EB13E5,
    (int32_t)0x7C8CBBFC, (int32_t)0xE27B2DDD, (int32_t)0x72628DEA, (int32_t)0xC68DBF9B,
    (int32_t)0x79E3423B, (int32_t)0xD8EB13E5, (int32_t)0x68227968, (int32_t)0xB591AAF5,
    (int32_t)0x767E1F7F, (int32_t)0xCF9725A5, (int32_t)0x5B620CAF, (int32_t)0xA65F4728,
    (int32_t)0x72628DEA, (int32_t)0xC68DBF9B, (int32_t)0x4C6FB157, (int32_t)0x995406F8,
    (int32_t)0x6D96E081, (int32_t)0xBDDCCB86, (int32_t)0x3BA75066, (int32_t)0x8EC0201C,
    (int32_t)0x68227968, (int32_t)0xB591AAF5, (int32_t)0x29701D23, (int32_t)0x86E49E01,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x5B620CAF, (int32_t)0xA65F4728, (int32_t)0x027B6A48, (int32_t)0x8006294E,
    (int32_t)0x5429A903, (int32_t)0x9F8F699D, (int32_t)0xEEAD76C7, (int32_t)0x812D73CF,
    (int32_t)0x4C6FB157, (int32_t)0x995406F8, (int32_t)0xDB4A081C, (int32_t)0x85609983,
    (int32_t)0x44400AF8, (int32_t)0x93B6B780, (int32_t)0xC8C85766, (int32_t)0x8C85C720,
    (int32_t)0x3BA75066, (int32_t)0x8EC0201C, (int32_t)0xB79A31A4, (int32_t)0x96710CAA,
    (int32_t)0x32B2BDE5, (int32_t)0x8A77E505, (int32_t)0xA8293BE8, (int32_t)0xA2E56BA7,
    (int32_t)0x29701D23, (int32_t)0x86E49E01, (int32_t)0x9AD469BB, (int32_t)0xB1964E2A,
    (int32_t)0x1FEDAFFA, (int32_t)0x840BCC3B, (int32_t)0x8FEDB537, (int32_t)0xC2295DC9,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
    (int32_t)0x0C644C72, (int32_t)0x8099EB01, (int32_t)0x826639DA, (int32_t)0xE7553464,
    (int32_t)0x027B6A48, (int32_t)0x8006294E, (int32_t)0x8018A4A2, (int32_t)0xFB09689C,
    (int32_t)0xF88EB5D3, (int32_t)0x80377032, (int32_t)0x80DD90C4, (int32_t)0x0EDC21F3,
    (int32_t)0xEEAD76C7, (int32_t)0x812D73CF, (int32_t)0x84B04357, (int32_t)0x22537AED,
    (int32_t)0xE4E6E334, (int32_t)0x82E6B961, (int32_t)0x8B793A27, (int32_t)0x34F7BFF4,
    (int32_t)0xDB4A081C, (int32_t)0x85609983, (int32_t)0x950EBC43, (int32_t)0x46564F69,
    (int32_t)0xD1E5B242, (int32_t)0x8897444B, (int32_t)0xA135DA91, (int32_t)0x56045A84,
    (int32_t)0xC8C85766, (int32_t)0x8C85C720, (int32_t)0xAFA3DA30, (int32_t)0x63A17621,
};

/****************** stage 3 radix 3 ******************/
ALIGN(8) static const int32_t __fft162_tw3[] =
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

/****************** stage 4 radix 3 ******************/
ALIGN(8) static const int32_t __fft162_tw4[] =
{
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FFFFFFF, (int32_t)0x00000000,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x87B826F7, (int32_t)0xD438AF17,
};
#define N 162
static const fft_cplx32x32_stage_t s2_tab[] =
{
    fft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
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
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const fft_cplx32x32_stage_t is2_tab[] =
{
    ifft_stageS2_DFT2_first_32x32,
    fft_stageS2_DFT3_32x32,
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
    fft_stageS3_DFT3_32x32,
    fft_stageS3_DFT3_last_32x32,
    NULL
};
static const int tw_step_tab[] =
{
    1, 1, 1, 1, 1,
};
static const cint32ptr_t tw_tab[] =
{
    __fft162_tw1, __fft162_tw2, __fft162_tw3, __fft162_tw4, NULL
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

const fft_handle_t rnfft32_324 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_324 = (const fft_handle_t)&__rifft_descr;
