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
ALIGN(8) static const int16_t   __fft_real16_tw352[176] =
{
    (int16_t)0x0000, (int16_t)0x7FFF, (int16_t)0x0249, (int16_t)0x7FFB, (int16_t)0x0492, (int16_t)0x7FEB, (int16_t)0x06DA, (int16_t)0x7FD1,
    (int16_t)0x0922, (int16_t)0x7FAD, (int16_t)0x0B69, (int16_t)0x7F7E, (int16_t)0x0DAF, (int16_t)0x7F44, (int16_t)0x0FF4, (int16_t)0x7F01,
    (int16_t)0x1237, (int16_t)0x7EB2, (int16_t)0x147A, (int16_t)0x7E5A, (int16_t)0x16BA, (int16_t)0x7DF7, (int16_t)0x18F9, (int16_t)0x7D8A,
    (int16_t)0x1B35, (int16_t)0x7D13, (int16_t)0x1D70, (int16_t)0x7C92, (int16_t)0x1FA8, (int16_t)0x7C06, (int16_t)0x21DD, (int16_t)0x7B70,
    (int16_t)0x2410, (int16_t)0x7AD1, (int16_t)0x2640, (int16_t)0x7A27, (int16_t)0x286C, (int16_t)0x7973, (int16_t)0x2A95, (int16_t)0x78B5,
    (int16_t)0x2CBB, (int16_t)0x77EE, (int16_t)0x2EDD, (int16_t)0x771D, (int16_t)0x30FC, (int16_t)0x7642, (int16_t)0x3316, (int16_t)0x755D,
    (int16_t)0x352C, (int16_t)0x746F, (int16_t)0x373E, (int16_t)0x7377, (int16_t)0x394C, (int16_t)0x7276, (int16_t)0x3B54, (int16_t)0x716C,
    (int16_t)0x3D58, (int16_t)0x7058, (int16_t)0x3F57, (int16_t)0x6F3B, (int16_t)0x4151, (int16_t)0x6E15, (int16_t)0x4345, (int16_t)0x6CE6,
    (int16_t)0x4534, (int16_t)0x6BAE, (int16_t)0x471D, (int16_t)0x6A6E, (int16_t)0x4900, (int16_t)0x6924, (int16_t)0x4ADE, (int16_t)0x67D2,
    (int16_t)0x4CB5, (int16_t)0x6678, (int16_t)0x4E86, (int16_t)0x6515, (int16_t)0x5051, (int16_t)0x63AB, (int16_t)0x5215, (int16_t)0x6237,
    (int16_t)0x53D2, (int16_t)0x60BC, (int16_t)0x5589, (int16_t)0x5F39, (int16_t)0x5739, (int16_t)0x5DAF, (int16_t)0x58E1, (int16_t)0x5C1C,
    (int16_t)0x5A82, (int16_t)0x5A82, (int16_t)0x5C1C, (int16_t)0x58E1, (int16_t)0x5DAF, (int16_t)0x5739, (int16_t)0x5F39, (int16_t)0x5589,
    (int16_t)0x60BC, (int16_t)0x53D2, (int16_t)0x6237, (int16_t)0x5215, (int16_t)0x63AB, (int16_t)0x5051, (int16_t)0x6515, (int16_t)0x4E86,
    (int16_t)0x6678, (int16_t)0x4CB5, (int16_t)0x67D2, (int16_t)0x4ADE, (int16_t)0x6924, (int16_t)0x4900, (int16_t)0x6A6E, (int16_t)0x471D,
    (int16_t)0x6BAE, (int16_t)0x4534, (int16_t)0x6CE6, (int16_t)0x4345, (int16_t)0x6E15, (int16_t)0x4151, (int16_t)0x6F3B, (int16_t)0x3F57,
    (int16_t)0x7058, (int16_t)0x3D58, (int16_t)0x716C, (int16_t)0x3B54, (int16_t)0x7276, (int16_t)0x394C, (int16_t)0x7377, (int16_t)0x373E,
    (int16_t)0x746F, (int16_t)0x352C, (int16_t)0x755D, (int16_t)0x3316, (int16_t)0x7642, (int16_t)0x30FC, (int16_t)0x771D, (int16_t)0x2EDD,
    (int16_t)0x77EE, (int16_t)0x2CBB, (int16_t)0x78B5, (int16_t)0x2A95, (int16_t)0x7973, (int16_t)0x286C, (int16_t)0x7A27, (int16_t)0x2640,
    (int16_t)0x7AD1, (int16_t)0x2410, (int16_t)0x7B70, (int16_t)0x21DD, (int16_t)0x7C06, (int16_t)0x1FA8, (int16_t)0x7C92, (int16_t)0x1D70,
    (int16_t)0x7D13, (int16_t)0x1B35, (int16_t)0x7D8A, (int16_t)0x18F9, (int16_t)0x7DF7, (int16_t)0x16BA, (int16_t)0x7E5A, (int16_t)0x147A,
    (int16_t)0x7EB2, (int16_t)0x1237, (int16_t)0x7F01, (int16_t)0x0FF4, (int16_t)0x7F44, (int16_t)0x0DAF, (int16_t)0x7F7E, (int16_t)0x0B69,
    (int16_t)0x7FAD, (int16_t)0x0922, (int16_t)0x7FD1, (int16_t)0x06DA, (int16_t)0x7FEB, (int16_t)0x0492, (int16_t)0x7FFB, (int16_t)0x0249,
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr176,
    __fft_real16_tw352
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr176,
    __fft_real16_tw352
};
const fft_handle_t rnfft16_352 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_352 = (const fft_handle_t)&__rifft_descr;
