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
    (int32_t)0x7F834ED0, (int32_t)0xF4D814A4,
    (int32_t)0x7E0E2E32, (int32_t)0xE9C5E582,
    (int32_t)0x7BA3751D, (int32_t)0xDEDF047D,
    (int32_t)0x7847D909, (int32_t)0xD438AF17,
    (int32_t)0x7401E4C1, (int32_t)0xC9E7A512,
    (int32_t)0x6ED9EBA1, (int32_t)0xC0000000,
    (int32_t)0x68D9F964, (int32_t)0xB6950C1E,
    (int32_t)0x620DBE8B, (int32_t)0xADB922B7,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666,
    (int32_t)0x5246DD49, (int32_t)0x9DF24175,
    (int32_t)0x496AF3E2, (int32_t)0x9726069C,
    (int32_t)0x40000000, (int32_t)0x9126145F,
    (int32_t)0x36185AEE, (int32_t)0x8BFE1B3F,
    (int32_t)0x2BC750E9, (int32_t)0x87B826F7,
    (int32_t)0x2120FB83, (int32_t)0x845C8AE3,
    (int32_t)0x163A1A7E, (int32_t)0x81F1D1CE,
    (int32_t)0x0B27EB5C, (int32_t)0x807CB130,
};

static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr36_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr36_32x32,
    __fft_real_tw
};

const fft_handle_t rnfft32_72 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft32_72 = (const fft_handle_t)&__rifft_descr;
