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
    (int32_t)0x7FFFFFFF, (int32_t)0x00000000, (int32_t)0x7FF62182, (int32_t)0xFCDBD541, (int32_t)0x7FD8878E, (int32_t)0xF9B82684, (int32_t)0x7FA736B4, (int32_t)0xF6956FB7,
    (int32_t)0x7F62368F, (int32_t)0xF3742CA2, (int32_t)0x7F0991C4, (int32_t)0xF054D8D5, (int32_t)0x7E9D55FC, (int32_t)0xED37EF91, (int32_t)0x7E1D93EA, (int32_t)0xEA1DEBBB,
    (int32_t)0x7D8A5F40, (int32_t)0xE70747C4, (int32_t)0x7CE3CEB2, (int32_t)0xE3F47D96, (int32_t)0x7C29FBEE, (int32_t)0xE0E60685, (int32_t)0x7B5D039E, (int32_t)0xDDDC5B3B,
    (int32_t)0x7A7D055B, (int32_t)0xDAD7F3A2, (int32_t)0x798A23B1, (int32_t)0xD7D946D8, (int32_t)0x78848414, (int32_t)0xD4E0CB15, (int32_t)0x776C4EDB, (int32_t)0xD1EEF59E,
    (int32_t)0x7641AF3D, (int32_t)0xCF043AB3, (int32_t)0x7504D345, (int32_t)0xCC210D79, (int32_t)0x73B5EBD1, (int32_t)0xC945DFEC, (int32_t)0x72552C85, (int32_t)0xC67322CE,
    (int32_t)0x70E2CBC6, (int32_t)0xC3A94590, (int32_t)0x6F5F02B2, (int32_t)0xC0E8B648, (int32_t)0x6DCA0D14, (int32_t)0xBE31E19B, (int32_t)0x6C242960, (int32_t)0xBB8532B0,
    (int32_t)0x6A6D98A4, (int32_t)0xB8E31319, (int32_t)0x68A69E81, (int32_t)0xB64BEACD, (int32_t)0x66CF8120, (int32_t)0xB3C0200C, (int32_t)0x64E88926, (int32_t)0xB140175B,
    (int32_t)0x62F201AC, (int32_t)0xAECC336C, (int32_t)0x60EC3830, (int32_t)0xAC64D510, (int32_t)0x5ED77C8A, (int32_t)0xAA0A5B2E, (int32_t)0x5CB420E0, (int32_t)0xA7BD22AC,
    (int32_t)0x5A82799A, (int32_t)0xA57D8666, (int32_t)0x5842DD54, (int32_t)0xA34BDF20, (int32_t)0x55F5A4D2, (int32_t)0xA1288376, (int32_t)0x539B2AF0, (int32_t)0x9F13C7D0,
    (int32_t)0x5133CC94, (int32_t)0x9D0DFE54, (int32_t)0x4EBFE8A5, (int32_t)0x9B1776DA, (int32_t)0x4C3FDFF4, (int32_t)0x99307EE0, (int32_t)0x49B41533, (int32_t)0x9759617F,
    (int32_t)0x471CECE7, (int32_t)0x9592675C, (int32_t)0x447ACD50, (int32_t)0x93DBD6A0, (int32_t)0x41CE1E65, (int32_t)0x9235F2EC, (int32_t)0x3F1749B8, (int32_t)0x90A0FD4E,
    (int32_t)0x3C56BA70, (int32_t)0x8F1D343A, (int32_t)0x398CDD32, (int32_t)0x8DAAD37B, (int32_t)0x36BA2014, (int32_t)0x8C4A142F, (int32_t)0x33DEF287, (int32_t)0x8AFB2CBB,
    (int32_t)0x30FBC54D, (int32_t)0x89BE50C3, (int32_t)0x2E110A62, (int32_t)0x8893B125, (int32_t)0x2B1F34EB, (int32_t)0x877B7BEC, (int32_t)0x2826B928, (int32_t)0x8675DC4F,
    (int32_t)0x25280C5E, (int32_t)0x8582FAA5, (int32_t)0x2223A4C5, (int32_t)0x84A2FC62, (int32_t)0x1F19F97B, (int32_t)0x83D60412, (int32_t)0x1C0B826A, (int32_t)0x831C314E,
    (int32_t)0x18F8B83C, (int32_t)0x8275A0C0, (int32_t)0x15E21445, (int32_t)0x81E26C16, (int32_t)0x12C8106F, (int32_t)0x8162AA04, (int32_t)0x0FAB272B, (int32_t)0x80F66E3C,
    (int32_t)0x0C8BD35E, (int32_t)0x809DC971, (int32_t)0x096A9049, (int32_t)0x8058C94C, (int32_t)0x0647D97C, (int32_t)0x80277872, (int32_t)0x03242ABF, (int32_t)0x8009DE7E,
};


static const fft_real32x32_descr_t __rfft_descr =
{
    (const fft_handle_t)&__cfft_descr128_32x32,
    __fft_real_tw
};

static const fft_real32x32_descr_t __rifft_descr =
{
    (const fft_handle_t)&__cifft_descr128_32x32,
    __fft_real_tw
};

const fft_handle_t rfft32_256 = (const fft_handle_t)&__rfft_descr;
const fft_handle_t rifft32_256 = (const fft_handle_t)&__rifft_descr;
