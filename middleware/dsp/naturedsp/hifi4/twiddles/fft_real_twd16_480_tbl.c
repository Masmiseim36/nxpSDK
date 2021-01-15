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

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=480 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x01ad,(int16_t)0x7ffd,
(int16_t)0x035a,(int16_t)0x7ff5,
(int16_t)0x0506,(int16_t)0x7fe7,
(int16_t)0x06b3,(int16_t)0x7fd3,
(int16_t)0x085f,(int16_t)0x7fba,
(int16_t)0x0a0b,(int16_t)0x7f9b,
(int16_t)0x0bb6,(int16_t)0x7f77,
(int16_t)0x0d61,(int16_t)0x7f4c,
(int16_t)0x0f0b,(int16_t)0x7f1d,
(int16_t)0x10b5,(int16_t)0x7ee8,
(int16_t)0x125e,(int16_t)0x7ead,
(int16_t)0x1406,(int16_t)0x7e6d,
(int16_t)0x15ad,(int16_t)0x7e27,
(int16_t)0x1753,(int16_t)0x7ddb,
(int16_t)0x18f9,(int16_t)0x7d8a,
(int16_t)0x1a9d,(int16_t)0x7d34,
(int16_t)0x1c40,(int16_t)0x7cd8,
(int16_t)0x1de2,(int16_t)0x7c77,
(int16_t)0x1f82,(int16_t)0x7c10,
(int16_t)0x2121,(int16_t)0x7ba3,
(int16_t)0x22bf,(int16_t)0x7b32,
(int16_t)0x245b,(int16_t)0x7abb,
(int16_t)0x25f5,(int16_t)0x7a3e,
(int16_t)0x278e,(int16_t)0x79bc,
(int16_t)0x2925,(int16_t)0x7935,
(int16_t)0x2aba,(int16_t)0x78a8,
(int16_t)0x2c4e,(int16_t)0x7817,
(int16_t)0x2ddf,(int16_t)0x7780,
(int16_t)0x2f6e,(int16_t)0x76e3,
(int16_t)0x30fc,(int16_t)0x7642,
(int16_t)0x3287,(int16_t)0x759b,
(int16_t)0x3410,(int16_t)0x74ef,
(int16_t)0x3597,(int16_t)0x743e,
(int16_t)0x371b,(int16_t)0x7388,
(int16_t)0x389d,(int16_t)0x72cd,
(int16_t)0x3a1c,(int16_t)0x720d,
(int16_t)0x3b99,(int16_t)0x7147,
(int16_t)0x3d14,(int16_t)0x707d,
(int16_t)0x3e8b,(int16_t)0x6fae,
(int16_t)0x4000,(int16_t)0x6eda,
(int16_t)0x4172,(int16_t)0x6e01,
(int16_t)0x42e1,(int16_t)0x6d23,
(int16_t)0x444d,(int16_t)0x6c41,
(int16_t)0x45b7,(int16_t)0x6b5a,
(int16_t)0x471d,(int16_t)0x6a6e,
(int16_t)0x4880,(int16_t)0x697d,
(int16_t)0x49e0,(int16_t)0x6888,
(int16_t)0x4b3d,(int16_t)0x678e,
(int16_t)0x4c96,(int16_t)0x668f,
(int16_t)0x4dec,(int16_t)0x658d,
(int16_t)0x4f3e,(int16_t)0x6485,
(int16_t)0x508e,(int16_t)0x637a,
(int16_t)0x51d9,(int16_t)0x6269,
(int16_t)0x5321,(int16_t)0x6155,
(int16_t)0x5465,(int16_t)0x603c,
(int16_t)0x55a6,(int16_t)0x5f1f,
(int16_t)0x56e3,(int16_t)0x5dfe,
(int16_t)0x581c,(int16_t)0x5cd9,
(int16_t)0x5951,(int16_t)0x5bb0,
(int16_t)0x5a82,(int16_t)0x5a82,
(int16_t)0x5bb0,(int16_t)0x5951,
(int16_t)0x5cd9,(int16_t)0x581c,
(int16_t)0x5dfe,(int16_t)0x56e3,
(int16_t)0x5f1f,(int16_t)0x55a6,
(int16_t)0x603c,(int16_t)0x5465,
(int16_t)0x6155,(int16_t)0x5321,
(int16_t)0x6269,(int16_t)0x51d9,
(int16_t)0x637a,(int16_t)0x508e,
(int16_t)0x6485,(int16_t)0x4f3e,
(int16_t)0x658d,(int16_t)0x4dec,
(int16_t)0x668f,(int16_t)0x4c96,
(int16_t)0x678e,(int16_t)0x4b3d,
(int16_t)0x6888,(int16_t)0x49e0,
(int16_t)0x697d,(int16_t)0x4880,
(int16_t)0x6a6e,(int16_t)0x471d,
(int16_t)0x6b5a,(int16_t)0x45b7,
(int16_t)0x6c41,(int16_t)0x444d,
(int16_t)0x6d23,(int16_t)0x42e1,
(int16_t)0x6e01,(int16_t)0x4172,
(int16_t)0x6eda,(int16_t)0x4000,
(int16_t)0x6fae,(int16_t)0x3e8b,
(int16_t)0x707d,(int16_t)0x3d14,
(int16_t)0x7147,(int16_t)0x3b99,
(int16_t)0x720d,(int16_t)0x3a1c,
(int16_t)0x72cd,(int16_t)0x389d,
(int16_t)0x7388,(int16_t)0x371b,
(int16_t)0x743e,(int16_t)0x3597,
(int16_t)0x74ef,(int16_t)0x3410,
(int16_t)0x759b,(int16_t)0x3287,
(int16_t)0x7642,(int16_t)0x30fc,
(int16_t)0x76e3,(int16_t)0x2f6e,
(int16_t)0x7780,(int16_t)0x2ddf,
(int16_t)0x7817,(int16_t)0x2c4e,
(int16_t)0x78a8,(int16_t)0x2aba,
(int16_t)0x7935,(int16_t)0x2925,
(int16_t)0x79bc,(int16_t)0x278e,
(int16_t)0x7a3e,(int16_t)0x25f5,
(int16_t)0x7abb,(int16_t)0x245b,
(int16_t)0x7b32,(int16_t)0x22bf,
(int16_t)0x7ba3,(int16_t)0x2121,
(int16_t)0x7c10,(int16_t)0x1f82,
(int16_t)0x7c77,(int16_t)0x1de2,
(int16_t)0x7cd8,(int16_t)0x1c40,
(int16_t)0x7d34,(int16_t)0x1a9d,
(int16_t)0x7d8a,(int16_t)0x18f9,
(int16_t)0x7ddb,(int16_t)0x1753,
(int16_t)0x7e27,(int16_t)0x15ad,
(int16_t)0x7e6d,(int16_t)0x1406,
(int16_t)0x7ead,(int16_t)0x125e,
(int16_t)0x7ee8,(int16_t)0x10b5,
(int16_t)0x7f1d,(int16_t)0x0f0b,
(int16_t)0x7f4c,(int16_t)0x0d61,
(int16_t)0x7f77,(int16_t)0x0bb6,
(int16_t)0x7f9b,(int16_t)0x0a0b,
(int16_t)0x7fba,(int16_t)0x085f,
(int16_t)0x7fd3,(int16_t)0x06b3,
(int16_t)0x7fe7,(int16_t)0x0506,
(int16_t)0x7ff5,(int16_t)0x035a,
(int16_t)0x7ffd,(int16_t)0x01ad
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr240,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr240,
    __fft_real16_tw
};
const fft_handle_t rnfft16_480 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_480 = (const fft_handle_t)&__rifft_descr;
