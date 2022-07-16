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
#include "common.h"
#include "fft_x16_common.h"



/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=96 */
ALIGN(8) static const int16_t __fft_real16_tw96[48] =
{
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x085F, (int16_t)0x7FBA, (int16_t)0x10B5, (int16_t)0x7EE8, (int16_t)0x18F9, (int16_t)0x7D8A,
    (int16_t)0x2121, (int16_t)0x7BA3, (int16_t)0x2925, (int16_t)0x7935, (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x389D, (int16_t)0x72CD,
    (int16_t)0x4000, (int16_t)0x6EDA, (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x4DEC, (int16_t)0x658D, (int16_t)0x5465, (int16_t)0x603C,
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x603C, (int16_t)0x5465, (int16_t)0x658D, (int16_t)0x4DEC, (int16_t)0x6A6E, (int16_t)0x471D,
    (int16_t)0x6EDA, (int16_t)0x4000, (int16_t)0x72CD, (int16_t)0x389D, (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x7935, (int16_t)0x2925,
    (int16_t)0x7BA3, (int16_t)0x2121, (int16_t)0x7D8A, (int16_t)0x18F9, (int16_t)0x7EE8, (int16_t)0x10B5, (int16_t)0x7FBA, (int16_t)0x085F, 
};


static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr48,
    __fft_real16_tw96
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr48,
    __fft_real16_tw96
};
const fft_handle_t  rnfft16_96 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_96 = (const fft_handle_t)&__rifft_descr;
