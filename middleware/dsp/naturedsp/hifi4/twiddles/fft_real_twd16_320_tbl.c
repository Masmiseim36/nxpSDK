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

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=320 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x0283,(int16_t)0x7ffa,
(int16_t)0x0506,(int16_t)0x7fe7,
(int16_t)0x0789,(int16_t)0x7fc7,
(int16_t)0x0a0b,(int16_t)0x7f9b,
(int16_t)0x0c8c,(int16_t)0x7f62,
(int16_t)0x0f0b,(int16_t)0x7f1d,
(int16_t)0x118a,(int16_t)0x7ecb,
(int16_t)0x1406,(int16_t)0x7e6d,
(int16_t)0x1680,(int16_t)0x7e02,
(int16_t)0x18f9,(int16_t)0x7d8a,
(int16_t)0x1b6e,(int16_t)0x7d07,
(int16_t)0x1de2,(int16_t)0x7c77,
(int16_t)0x2052,(int16_t)0x7bda,
(int16_t)0x22bf,(int16_t)0x7b32,
(int16_t)0x2528,(int16_t)0x7a7d,
(int16_t)0x278e,(int16_t)0x79bc,
(int16_t)0x29f0,(int16_t)0x78ef,
(int16_t)0x2c4e,(int16_t)0x7817,
(int16_t)0x2ea7,(int16_t)0x7732,
(int16_t)0x30fc,(int16_t)0x7642,
(int16_t)0x334c,(int16_t)0x7546,
(int16_t)0x3597,(int16_t)0x743e,
(int16_t)0x37dc,(int16_t)0x732b,
(int16_t)0x3a1c,(int16_t)0x720d,
(int16_t)0x3c57,(int16_t)0x70e3,
(int16_t)0x3e8b,(int16_t)0x6fae,
(int16_t)0x40b9,(int16_t)0x6e6e,
(int16_t)0x42e1,(int16_t)0x6d23,
(int16_t)0x4502,(int16_t)0x6bce,
(int16_t)0x471d,(int16_t)0x6a6e,
(int16_t)0x4930,(int16_t)0x6903,
(int16_t)0x4b3d,(int16_t)0x678e,
(int16_t)0x4d41,(int16_t)0x660f,
(int16_t)0x4f3e,(int16_t)0x6485,
(int16_t)0x5134,(int16_t)0x62f2,
(int16_t)0x5321,(int16_t)0x6155,
(int16_t)0x5506,(int16_t)0x5fae,
(int16_t)0x56e3,(int16_t)0x5dfe,
(int16_t)0x58b7,(int16_t)0x5c45,
(int16_t)0x5a82,(int16_t)0x5a82,
(int16_t)0x5c45,(int16_t)0x58b7,
(int16_t)0x5dfe,(int16_t)0x56e3,
(int16_t)0x5fae,(int16_t)0x5506,
(int16_t)0x6155,(int16_t)0x5321,
(int16_t)0x62f2,(int16_t)0x5134,
(int16_t)0x6485,(int16_t)0x4f3e,
(int16_t)0x660f,(int16_t)0x4d41,
(int16_t)0x678e,(int16_t)0x4b3d,
(int16_t)0x6903,(int16_t)0x4930,
(int16_t)0x6a6e,(int16_t)0x471d,
(int16_t)0x6bce,(int16_t)0x4502,
(int16_t)0x6d23,(int16_t)0x42e1,
(int16_t)0x6e6e,(int16_t)0x40b9,
(int16_t)0x6fae,(int16_t)0x3e8b,
(int16_t)0x70e3,(int16_t)0x3c57,
(int16_t)0x720d,(int16_t)0x3a1c,
(int16_t)0x732b,(int16_t)0x37dc,
(int16_t)0x743e,(int16_t)0x3597,
(int16_t)0x7546,(int16_t)0x334c,
(int16_t)0x7642,(int16_t)0x30fc,
(int16_t)0x7732,(int16_t)0x2ea7,
(int16_t)0x7817,(int16_t)0x2c4e,
(int16_t)0x78ef,(int16_t)0x29f0,
(int16_t)0x79bc,(int16_t)0x278e,
(int16_t)0x7a7d,(int16_t)0x2528,
(int16_t)0x7b32,(int16_t)0x22bf,
(int16_t)0x7bda,(int16_t)0x2052,
(int16_t)0x7c77,(int16_t)0x1de2,
(int16_t)0x7d07,(int16_t)0x1b6e,
(int16_t)0x7d8a,(int16_t)0x18f9,
(int16_t)0x7e02,(int16_t)0x1680,
(int16_t)0x7e6d,(int16_t)0x1406,
(int16_t)0x7ecb,(int16_t)0x118a,
(int16_t)0x7f1d,(int16_t)0x0f0b,
(int16_t)0x7f62,(int16_t)0x0c8c,
(int16_t)0x7f9b,(int16_t)0x0a0b,
(int16_t)0x7fc7,(int16_t)0x0789,
(int16_t)0x7fe7,(int16_t)0x0506,
(int16_t)0x7ffa,(int16_t)0x0283
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr160,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr160,
    __fft_real16_tw
};
const fft_handle_t rnfft16_320 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_320 = (const fft_handle_t)&__rifft_descr;
