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

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=384 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x0218,(int16_t)0x7ffc,
(int16_t)0x0430,(int16_t)0x7fee,
(int16_t)0x0648,(int16_t)0x7fd9,
(int16_t)0x085f,(int16_t)0x7fba,
(int16_t)0x0a76,(int16_t)0x7f92,
(int16_t)0x0c8c,(int16_t)0x7f62,
(int16_t)0x0ea1,(int16_t)0x7f29,
(int16_t)0x10b5,(int16_t)0x7ee8,
(int16_t)0x12c8,(int16_t)0x7e9d,
(int16_t)0x14da,(int16_t)0x7e4a,
(int16_t)0x16ea,(int16_t)0x7def,
(int16_t)0x18f9,(int16_t)0x7d8a,
(int16_t)0x1b06,(int16_t)0x7d1d,
(int16_t)0x1d11,(int16_t)0x7ca8,
(int16_t)0x1f1a,(int16_t)0x7c2a,
(int16_t)0x2121,(int16_t)0x7ba3,
(int16_t)0x2326,(int16_t)0x7b14,
(int16_t)0x2528,(int16_t)0x7a7d,
(int16_t)0x2728,(int16_t)0x79dd,
(int16_t)0x2925,(int16_t)0x7935,
(int16_t)0x2b1f,(int16_t)0x7885,
(int16_t)0x2d17,(int16_t)0x77cc,
(int16_t)0x2f0b,(int16_t)0x770b,
(int16_t)0x30fc,(int16_t)0x7642,
(int16_t)0x32e9,(int16_t)0x7570,
(int16_t)0x34d4,(int16_t)0x7497,
(int16_t)0x36ba,(int16_t)0x73b6,
(int16_t)0x389d,(int16_t)0x72cd,
(int16_t)0x3a7c,(int16_t)0x71dc,
(int16_t)0x3c57,(int16_t)0x70e3,
(int16_t)0x3e2d,(int16_t)0x6fe2,
(int16_t)0x4000,(int16_t)0x6eda,
(int16_t)0x41ce,(int16_t)0x6dca,
(int16_t)0x4398,(int16_t)0x6cb3,
(int16_t)0x455d,(int16_t)0x6b94,
(int16_t)0x471d,(int16_t)0x6a6e,
(int16_t)0x48d8,(int16_t)0x6940,
(int16_t)0x4a8f,(int16_t)0x680b,
(int16_t)0x4c40,(int16_t)0x66d0,
(int16_t)0x4dec,(int16_t)0x658d,
(int16_t)0x4f93,(int16_t)0x6443,
(int16_t)0x5134,(int16_t)0x62f2,
(int16_t)0x52cf,(int16_t)0x619a,
(int16_t)0x5465,(int16_t)0x603c,
(int16_t)0x55f6,(int16_t)0x5ed7,
(int16_t)0x5780,(int16_t)0x5d6c,
(int16_t)0x5904,(int16_t)0x5bfa,
(int16_t)0x5a82,(int16_t)0x5a82,
(int16_t)0x5bfa,(int16_t)0x5904,
(int16_t)0x5d6c,(int16_t)0x5780,
(int16_t)0x5ed7,(int16_t)0x55f6,
(int16_t)0x603c,(int16_t)0x5465,
(int16_t)0x619a,(int16_t)0x52cf,
(int16_t)0x62f2,(int16_t)0x5134,
(int16_t)0x6443,(int16_t)0x4f93,
(int16_t)0x658d,(int16_t)0x4dec,
(int16_t)0x66d0,(int16_t)0x4c40,
(int16_t)0x680b,(int16_t)0x4a8f,
(int16_t)0x6940,(int16_t)0x48d8,
(int16_t)0x6a6e,(int16_t)0x471d,
(int16_t)0x6b94,(int16_t)0x455d,
(int16_t)0x6cb3,(int16_t)0x4398,
(int16_t)0x6dca,(int16_t)0x41ce,
(int16_t)0x6eda,(int16_t)0x4000,
(int16_t)0x6fe2,(int16_t)0x3e2d,
(int16_t)0x70e3,(int16_t)0x3c57,
(int16_t)0x71dc,(int16_t)0x3a7c,
(int16_t)0x72cd,(int16_t)0x389d,
(int16_t)0x73b6,(int16_t)0x36ba,
(int16_t)0x7497,(int16_t)0x34d4,
(int16_t)0x7570,(int16_t)0x32e9,
(int16_t)0x7642,(int16_t)0x30fc,
(int16_t)0x770b,(int16_t)0x2f0b,
(int16_t)0x77cc,(int16_t)0x2d17,
(int16_t)0x7885,(int16_t)0x2b1f,
(int16_t)0x7935,(int16_t)0x2925,
(int16_t)0x79dd,(int16_t)0x2728,
(int16_t)0x7a7d,(int16_t)0x2528,
(int16_t)0x7b14,(int16_t)0x2326,
(int16_t)0x7ba3,(int16_t)0x2121,
(int16_t)0x7c2a,(int16_t)0x1f1a,
(int16_t)0x7ca8,(int16_t)0x1d11,
(int16_t)0x7d1d,(int16_t)0x1b06,
(int16_t)0x7d8a,(int16_t)0x18f9,
(int16_t)0x7def,(int16_t)0x16ea,
(int16_t)0x7e4a,(int16_t)0x14da,
(int16_t)0x7e9d,(int16_t)0x12c8,
(int16_t)0x7ee8,(int16_t)0x10b5,
(int16_t)0x7f29,(int16_t)0x0ea1,
(int16_t)0x7f62,(int16_t)0x0c8c,
(int16_t)0x7f92,(int16_t)0x0a76,
(int16_t)0x7fba,(int16_t)0x085f,
(int16_t)0x7fd9,(int16_t)0x0648,
(int16_t)0x7fee,(int16_t)0x0430,
(int16_t)0x7ffc,(int16_t)0x0218
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr192,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr192,
    __fft_real16_tw
};
const fft_handle_t rnfft16_384 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_384 = (const fft_handle_t)&__rifft_descr;
