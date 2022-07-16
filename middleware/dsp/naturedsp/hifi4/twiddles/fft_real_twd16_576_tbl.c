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
ALIGN(8) static const int16_t  __fft_real16_tw576[288] =
{
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0165, (int16_t)0x7FFE, (int16_t)0x02CB, (int16_t)0x7FF8, (int16_t)0x0430, (int16_t)0x7FEE,
    (int16_t)0x0595, (int16_t)0x7FE1, (int16_t)0x06FA, (int16_t)0x7FCF, (int16_t)0x085F, (int16_t)0x7FBA, (int16_t)0x09C4, (int16_t)0x7FA1,
    (int16_t)0x0B28, (int16_t)0x7F83, (int16_t)0x0C8C, (int16_t)0x7F62, (int16_t)0x0DEF, (int16_t)0x7F3D, (int16_t)0x0F52, (int16_t)0x7F14,
    (int16_t)0x10B5, (int16_t)0x7EE8, (int16_t)0x1217, (int16_t)0x7EB7, (int16_t)0x1379, (int16_t)0x7E83, (int16_t)0x14DA, (int16_t)0x7E4A,
    (int16_t)0x163A, (int16_t)0x7E0E, (int16_t)0x179A, (int16_t)0x7DCE, (int16_t)0x18F9, (int16_t)0x7D8A, (int16_t)0x1A57, (int16_t)0x7D43,
    (int16_t)0x1BB4, (int16_t)0x7CF7, (int16_t)0x1D11, (int16_t)0x7CA8, (int16_t)0x1E6C, (int16_t)0x7C55, (int16_t)0x1FC7, (int16_t)0x7BFE,
    (int16_t)0x2121, (int16_t)0x7BA3, (int16_t)0x227A, (int16_t)0x7B45, (int16_t)0x23D1, (int16_t)0x7AE3, (int16_t)0x2528, (int16_t)0x7A7D,
    (int16_t)0x267E, (int16_t)0x7A13, (int16_t)0x27D2, (int16_t)0x79A6, (int16_t)0x2925, (int16_t)0x7935, (int16_t)0x2A77, (int16_t)0x78C0,
    (int16_t)0x2BC7, (int16_t)0x7848, (int16_t)0x2D17, (int16_t)0x77CC, (int16_t)0x2E64, (int16_t)0x774C, (int16_t)0x2FB1, (int16_t)0x76C9,
    (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x3245, (int16_t)0x75B7, (int16_t)0x338D, (int16_t)0x7529, (int16_t)0x34D4, (int16_t)0x7497,
    (int16_t)0x3618, (int16_t)0x7402, (int16_t)0x375B, (int16_t)0x7369, (int16_t)0x389D, (int16_t)0x72CD, (int16_t)0x39DD, (int16_t)0x722D,
    (int16_t)0x3B1B, (int16_t)0x718A, (int16_t)0x3C57, (int16_t)0x70E3, (int16_t)0x3D91, (int16_t)0x7039, (int16_t)0x3EC9, (int16_t)0x6F8B,
    (int16_t)0x4000, (int16_t)0x6EDA, (int16_t)0x4135, (int16_t)0x6E26, (int16_t)0x4267, (int16_t)0x6D6E, (int16_t)0x4398, (int16_t)0x6CB3,
    (int16_t)0x44C6, (int16_t)0x6BF4, (int16_t)0x45F3, (int16_t)0x6B33, (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x4845, (int16_t)0x69A5,
    (int16_t)0x496B, (int16_t)0x68DA, (int16_t)0x4A8F, (int16_t)0x680B, (int16_t)0x4BB0, (int16_t)0x673A, (int16_t)0x4CCF, (int16_t)0x6665,
    (int16_t)0x4DEC, (int16_t)0x658D, (int16_t)0x4F06, (int16_t)0x64B1, (int16_t)0x501E, (int16_t)0x63D3, (int16_t)0x5134, (int16_t)0x62F2,
    (int16_t)0x5247, (int16_t)0x620E, (int16_t)0x5357, (int16_t)0x6126, (int16_t)0x5465, (int16_t)0x603C, (int16_t)0x5571, (int16_t)0x5F4F,
    (int16_t)0x567A, (int16_t)0x5E5F, (int16_t)0x5780, (int16_t)0x5D6C, (int16_t)0x5883, (int16_t)0x5C76, (int16_t)0x5984, (int16_t)0x5B7E,
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x5B7E, (int16_t)0x5984, (int16_t)0x5C76, (int16_t)0x5883, (int16_t)0x5D6C, (int16_t)0x5780,
    (int16_t)0x5E5F, (int16_t)0x567A, (int16_t)0x5F4F, (int16_t)0x5571, (int16_t)0x603C, (int16_t)0x5465, (int16_t)0x6126, (int16_t)0x5357,
    (int16_t)0x620E, (int16_t)0x5247, (int16_t)0x62F2, (int16_t)0x5134, (int16_t)0x63D3, (int16_t)0x501E, (int16_t)0x64B1, (int16_t)0x4F06,
    (int16_t)0x658D, (int16_t)0x4DEC, (int16_t)0x6665, (int16_t)0x4CCF, (int16_t)0x673A, (int16_t)0x4BB0, (int16_t)0x680B, (int16_t)0x4A8F,
    (int16_t)0x68DA, (int16_t)0x496B, (int16_t)0x69A5, (int16_t)0x4845, (int16_t)0x6A6E, (int16_t)0x471D, (int16_t)0x6B33, (int16_t)0x45F3,
    (int16_t)0x6BF4, (int16_t)0x44C6, (int16_t)0x6CB3, (int16_t)0x4398, (int16_t)0x6D6E, (int16_t)0x4267, (int16_t)0x6E26, (int16_t)0x4135,
    (int16_t)0x6EDA, (int16_t)0x4000, (int16_t)0x6F8B, (int16_t)0x3EC9, (int16_t)0x7039, (int16_t)0x3D91, (int16_t)0x70E3, (int16_t)0x3C57,
    (int16_t)0x718A, (int16_t)0x3B1B, (int16_t)0x722D, (int16_t)0x39DD, (int16_t)0x72CD, (int16_t)0x389D, (int16_t)0x7369, (int16_t)0x375B,
    (int16_t)0x7402, (int16_t)0x3618, (int16_t)0x7497, (int16_t)0x34D4, (int16_t)0x7529, (int16_t)0x338D, (int16_t)0x75B7, (int16_t)0x3245,
    (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x76C9, (int16_t)0x2FB1, (int16_t)0x774C, (int16_t)0x2E64, (int16_t)0x77CC, (int16_t)0x2D17,
    (int16_t)0x7848, (int16_t)0x2BC7, (int16_t)0x78C0, (int16_t)0x2A77, (int16_t)0x7935, (int16_t)0x2925, (int16_t)0x79A6, (int16_t)0x27D2,
    (int16_t)0x7A13, (int16_t)0x267E, (int16_t)0x7A7D, (int16_t)0x2528, (int16_t)0x7AE3, (int16_t)0x23D1, (int16_t)0x7B45, (int16_t)0x227A,
    (int16_t)0x7BA3, (int16_t)0x2121, (int16_t)0x7BFE, (int16_t)0x1FC7, (int16_t)0x7C55, (int16_t)0x1E6C, (int16_t)0x7CA8, (int16_t)0x1D11,
    (int16_t)0x7CF7, (int16_t)0x1BB4, (int16_t)0x7D43, (int16_t)0x1A57, (int16_t)0x7D8A, (int16_t)0x18F9, (int16_t)0x7DCE, (int16_t)0x179A,
    (int16_t)0x7E0E, (int16_t)0x163A, (int16_t)0x7E4A, (int16_t)0x14DA, (int16_t)0x7E83, (int16_t)0x1379, (int16_t)0x7EB7, (int16_t)0x1217,
    (int16_t)0x7EE8, (int16_t)0x10B5, (int16_t)0x7F14, (int16_t)0x0F52, (int16_t)0x7F3D, (int16_t)0x0DEF, (int16_t)0x7F62, (int16_t)0x0C8C,
    (int16_t)0x7F83, (int16_t)0x0B28, (int16_t)0x7FA1, (int16_t)0x09C4, (int16_t)0x7FBA, (int16_t)0x085F, (int16_t)0x7FCF, (int16_t)0x06FA,
    (int16_t)0x7FE1, (int16_t)0x0595, (int16_t)0x7FEE, (int16_t)0x0430, (int16_t)0x7FF8, (int16_t)0x02CB, (int16_t)0x7FFE, (int16_t)0x0165,
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr288,
    __fft_real16_tw576
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr288,
    __fft_real16_tw576
};
const fft_handle_t rnfft16_576 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_576 = (const fft_handle_t)&__rifft_descr;
