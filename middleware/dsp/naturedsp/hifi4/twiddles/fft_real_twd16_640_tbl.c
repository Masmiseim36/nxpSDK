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
ALIGN(8) static const int16_t __fft_real16_tw640[320] =
{
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0142, (int16_t)0x7FFE, (int16_t)0x0283, (int16_t)0x7FFA, (int16_t)0x03C5, (int16_t)0x7FF2,
    (int16_t)0x0506, (int16_t)0x7FE7, (int16_t)0x0648, (int16_t)0x7FD9, (int16_t)0x0789, (int16_t)0x7FC7, (int16_t)0x08CA, (int16_t)0x7FB3,
    (int16_t)0x0A0B, (int16_t)0x7F9B, (int16_t)0x0B4C, (int16_t)0x7F80, (int16_t)0x0C8C, (int16_t)0x7F62, (int16_t)0x0DCC, (int16_t)0x7F41,
    (int16_t)0x0F0B, (int16_t)0x7F1D, (int16_t)0x104B, (int16_t)0x7EF5, (int16_t)0x118A, (int16_t)0x7ECB, (int16_t)0x12C8, (int16_t)0x7E9D,
    (int16_t)0x1406, (int16_t)0x7E6D, (int16_t)0x1544, (int16_t)0x7E39, (int16_t)0x1680, (int16_t)0x7E02, (int16_t)0x17BD, (int16_t)0x7DC8,
    (int16_t)0x18F9, (int16_t)0x7D8A, (int16_t)0x1A34, (int16_t)0x7D4A, (int16_t)0x1B6E, (int16_t)0x7D07, (int16_t)0x1CA8, (int16_t)0x7CC0,
    (int16_t)0x1DE2, (int16_t)0x7C77, (int16_t)0x1F1A, (int16_t)0x7C2A, (int16_t)0x2052, (int16_t)0x7BDA, (int16_t)0x2189, (int16_t)0x7B88,
    (int16_t)0x22BF, (int16_t)0x7B32, (int16_t)0x23F4, (int16_t)0x7AD9, (int16_t)0x2528, (int16_t)0x7A7D, (int16_t)0x265B, (int16_t)0x7A1E,
    (int16_t)0x278E, (int16_t)0x79BC, (int16_t)0x28BF, (int16_t)0x7957, (int16_t)0x29F0, (int16_t)0x78EF, (int16_t)0x2B1F, (int16_t)0x7885,
    (int16_t)0x2C4E, (int16_t)0x7817, (int16_t)0x2D7B, (int16_t)0x77A6, (int16_t)0x2EA7, (int16_t)0x7732, (int16_t)0x2FD2, (int16_t)0x76BB,
    (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x3224, (int16_t)0x75C5, (int16_t)0x334C, (int16_t)0x7546, (int16_t)0x3472, (int16_t)0x74C3,
    (int16_t)0x3597, (int16_t)0x743E, (int16_t)0x36BA, (int16_t)0x73B6, (int16_t)0x37DC, (int16_t)0x732B, (int16_t)0x38FD, (int16_t)0x729D,
    (int16_t)0x3A1C, (int16_t)0x720D, (int16_t)0x3B3A, (int16_t)0x7179, (int16_t)0x3C57, (int16_t)0x70E3, (int16_t)0x3D72, (int16_t)0x704A,
    (int16_t)0x3E8B, (int16_t)0x6FAE, (int16_t)0x3FA3, (int16_t)0x6F0F, (int16_t)0x40B9, (int16_t)0x6E6E, (int16_t)0x41CE, (int16_t)0x6DCA,
    (int16_t)0x42E1, (int16_t)0x6D23, (int16_t)0x43F3, (int16_t)0x6C7A, (int16_t)0x4502, (int16_t)0x6BCE, (int16_t)0x4611, (int16_t)0x6B1F,
    (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x4828, (int16_t)0x69BA, (int16_t)0x4930, (int16_t)0x6903, (int16_t)0x4A37, (int16_t)0x684A,
    (int16_t)0x4B3D, (int16_t)0x678E, (int16_t)0x4C40, (int16_t)0x66D0, (int16_t)0x4D41, (int16_t)0x660F, (int16_t)0x4E41, (int16_t)0x654B,
    (int16_t)0x4F3E, (int16_t)0x6485, (int16_t)0x503A, (int16_t)0x63BD, (int16_t)0x5134, (int16_t)0x62F2, (int16_t)0x522B, (int16_t)0x6225,
    (int16_t)0x5321, (int16_t)0x6155, (int16_t)0x5415, (int16_t)0x6083, (int16_t)0x5506, (int16_t)0x5FAE, (int16_t)0x55F6, (int16_t)0x5ED7,
    (int16_t)0x56E3, (int16_t)0x5DFE, (int16_t)0x57CE, (int16_t)0x5D23, (int16_t)0x58B7, (int16_t)0x5C45, (int16_t)0x599E, (int16_t)0x5B65,
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x5B65, (int16_t)0x599E, (int16_t)0x5C45, (int16_t)0x58B7, (int16_t)0x5D23, (int16_t)0x57CE,
    (int16_t)0x5DFE, (int16_t)0x56E3, (int16_t)0x5ED7, (int16_t)0x55F6, (int16_t)0x5FAE, (int16_t)0x5506, (int16_t)0x6083, (int16_t)0x5415,
    (int16_t)0x6155, (int16_t)0x5321, (int16_t)0x6225, (int16_t)0x522B, (int16_t)0x62F2, (int16_t)0x5134, (int16_t)0x63BD, (int16_t)0x503A,
    (int16_t)0x6485, (int16_t)0x4F3E, (int16_t)0x654B, (int16_t)0x4E41, (int16_t)0x660F, (int16_t)0x4D41, (int16_t)0x66D0, (int16_t)0x4C40,
    (int16_t)0x678E, (int16_t)0x4B3D, (int16_t)0x684A, (int16_t)0x4A37, (int16_t)0x6903, (int16_t)0x4930, (int16_t)0x69BA, (int16_t)0x4828,
    (int16_t)0x6A6E, (int16_t)0x471D, (int16_t)0x6B1F, (int16_t)0x4611, (int16_t)0x6BCE, (int16_t)0x4502, (int16_t)0x6C7A, (int16_t)0x43F3,
    (int16_t)0x6D23, (int16_t)0x42E1, (int16_t)0x6DCA, (int16_t)0x41CE, (int16_t)0x6E6E, (int16_t)0x40B9, (int16_t)0x6F0F, (int16_t)0x3FA3,
    (int16_t)0x6FAE, (int16_t)0x3E8B, (int16_t)0x704A, (int16_t)0x3D72, (int16_t)0x70E3, (int16_t)0x3C57, (int16_t)0x7179, (int16_t)0x3B3A,
    (int16_t)0x720D, (int16_t)0x3A1C, (int16_t)0x729D, (int16_t)0x38FD, (int16_t)0x732B, (int16_t)0x37DC, (int16_t)0x73B6, (int16_t)0x36BA,
    (int16_t)0x743E, (int16_t)0x3597, (int16_t)0x74C3, (int16_t)0x3472, (int16_t)0x7546, (int16_t)0x334C, (int16_t)0x75C5, (int16_t)0x3224,
    (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x76BB, (int16_t)0x2FD2, (int16_t)0x7732, (int16_t)0x2EA7, (int16_t)0x77A6, (int16_t)0x2D7B,
    (int16_t)0x7817, (int16_t)0x2C4E, (int16_t)0x7885, (int16_t)0x2B1F, (int16_t)0x78EF, (int16_t)0x29F0, (int16_t)0x7957, (int16_t)0x28BF,
    (int16_t)0x79BC, (int16_t)0x278E, (int16_t)0x7A1E, (int16_t)0x265B, (int16_t)0x7A7D, (int16_t)0x2528, (int16_t)0x7AD9, (int16_t)0x23F4,
    (int16_t)0x7B32, (int16_t)0x22BF, (int16_t)0x7B88, (int16_t)0x2189, (int16_t)0x7BDA, (int16_t)0x2052, (int16_t)0x7C2A, (int16_t)0x1F1A,
    (int16_t)0x7C77, (int16_t)0x1DE2, (int16_t)0x7CC0, (int16_t)0x1CA8, (int16_t)0x7D07, (int16_t)0x1B6E, (int16_t)0x7D4A, (int16_t)0x1A34,
    (int16_t)0x7D8A, (int16_t)0x18F9, (int16_t)0x7DC8, (int16_t)0x17BD, (int16_t)0x7E02, (int16_t)0x1680, (int16_t)0x7E39, (int16_t)0x1544,
    (int16_t)0x7E6D, (int16_t)0x1406, (int16_t)0x7E9D, (int16_t)0x12C8, (int16_t)0x7ECB, (int16_t)0x118A, (int16_t)0x7EF5, (int16_t)0x104B,
    (int16_t)0x7F1D, (int16_t)0x0F0B, (int16_t)0x7F41, (int16_t)0x0DCC, (int16_t)0x7F62, (int16_t)0x0C8C, (int16_t)0x7F80, (int16_t)0x0B4C,
    (int16_t)0x7F9B, (int16_t)0x0A0B, (int16_t)0x7FB3, (int16_t)0x08CA, (int16_t)0x7FC7, (int16_t)0x0789, (int16_t)0x7FD9, (int16_t)0x0648,
    (int16_t)0x7FE7, (int16_t)0x0506, (int16_t)0x7FF2, (int16_t)0x03C5, (int16_t)0x7FFA, (int16_t)0x0283, (int16_t)0x7FFE, (int16_t)0x0142,
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr320,
    __fft_real16_tw640
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr320,
    __fft_real16_tw640
};
const fft_handle_t rnfft16_640 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_640 = (const fft_handle_t)&__rifft_descr;
