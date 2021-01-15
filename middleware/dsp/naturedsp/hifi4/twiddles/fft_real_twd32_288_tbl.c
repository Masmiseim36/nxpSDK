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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FF833BD, (int32_t)0xFD352B7B, (int32_t)0x7FE0CFE7, (int32_t)0xFA6AAE0F, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9,
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4, (int32_t)0x7F3D3CF4, (int32_t)0xF210A676, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7E82A146, (int32_t)0xEC87388C,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7CF7447F, (int32_t)0xE44BB4EC, (int32_t)0x7C54EFDC, (int32_t)0xE1938239,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x7AE2E9E4, (int32_t)0xDC2E9018, (int32_t)0x7A1365A5, (int32_t)0xD98278ED, (int32_t)0x793501A9, (int32_t)0xD6DB1254,
    (int32_t)0x7847D909, (int32_t)0xD438AF17, (int32_t)0x774C08AB, (int32_t)0xD19BA15F, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7528ED32, (int32_t)0xCC72CBE5,
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512, (int32_t)0x72CCB9DB, (int32_t)0xC763158E, (int32_t)0x7189922C, (int32_t)0xC4E56BE4, (int32_t)0x70389514, (int32_t)0xC26EF5C4,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6D6DC08F, (int32_t)0xBD98D67F, (int32_t)0x6BF4403B, (int32_t)0xBB39C435, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319,
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E, (int32_t)0x673993A9, (int32_t)0xB44FF727, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x63D34137, (int32_t)0xAFE1BD5B,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C, (int32_t)0x5E5F1A91, (int32_t)0xA9864294, (int32_t)0x5C766C1C, (int32_t)0xA77C800C,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x58837FF4, (int32_t)0xA38993E4, (int32_t)0x5679BD6C, (int32_t)0xA1A0E56F, (int32_t)0x54657194, (int32_t)0x9FC3B694,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175, (int32_t)0x501E42A5, (int32_t)0x9C2CBEC9, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x4BB008D9, (int32_t)0x98C66C57,
    (int32_t)0x496AF3E2, (int32_t)0x9726069C, (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x44C63BCB, (int32_t)0x940BBFC5, (int32_t)0x42672981, (int32_t)0x92923F71,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x3D910A3C, (int32_t)0x8FC76AEC, (int32_t)0x3B1A941C, (int32_t)0x8E766DD4, (int32_t)0x389CEA72, (int32_t)0x8D334625,
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F, (int32_t)0x338D341B, (int32_t)0x8AD712CE, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2E645EA1, (int32_t)0x88B3F755,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57, (int32_t)0x267D8713, (int32_t)0x85EC9A5B, (int32_t)0x23D16FE8, (int32_t)0x851D161C,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x1E6C7DC7, (int32_t)0x83AB1024, (int32_t)0x1BB44B14, (int32_t)0x8308BB81, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE, (int32_t)0x1378C774, (int32_t)0x817D5EBA, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x0DEF598A, (int32_t)0x80C2C30C,
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130, (int32_t)0x085F2137, (int32_t)0x804628A7, (int32_t)0x059551F1, (int32_t)0x801F3019, (int32_t)0x02CAD485, (int32_t)0x8007CC43,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr144_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr144_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_288=  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_288 = (const fft_handle_t)&__rifft_descr;
