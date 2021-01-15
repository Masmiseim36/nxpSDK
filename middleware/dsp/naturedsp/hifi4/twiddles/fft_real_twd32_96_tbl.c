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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FB9D759, (int32_t)0xF7A0DEC9, (int32_t)0x7EE7AA4C, (int32_t)0xEF4AEAF1, (int32_t)0x7D8A5F40, (int32_t)0xE70747C4,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D, (int32_t)0x793501A9, (int32_t)0xD6DB1254, (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x72CCB9DB, (int32_t)0xC763158E,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000, (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x658C9A2D, (int32_t)0xB2141B02, (int32_t)0x603C496C, (int32_t)0xAB9A8E6C,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x54657194, (int32_t)0x9FC3B694, (int32_t)0x4DEBE4FE, (int32_t)0x9A7365D3, (int32_t)0x471CECE7, (int32_t)0x9592675C,
    (int32_t)0x40000000, (int32_t)0x9126145F, (int32_t)0x389CEA72, (int32_t)0x8D334625, (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2924EDAC, (int32_t)0x86CAFE57,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3, (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x10B5150F, (int32_t)0x811855B4, (int32_t)0x085F2137, (int32_t)0x804628A7,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr48_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr48_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_96 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_96 = (const fft_handle_t)&__rifft_descr;
