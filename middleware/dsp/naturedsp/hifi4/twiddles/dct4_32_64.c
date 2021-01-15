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
/*
    NatureDSP Signal Processing Library. FFT part
    DCT-IV twiddles
    Integrit, 2006-2018
*/

#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct4_twd.h"

static const complex_fract32 ALIGN(8) dct4_twd_64[]=
{
    {{(int32_t)0X7FFD885AUL,(int32_t)0XFE6DE2E0UL}},
    {{(int32_t)0X7FE9CBC0UL,(int32_t)0XFB49E6A3UL}},
    {{(int32_t)0X7FC25596UL,(int32_t)0XF826A462UL}},
    {{(int32_t)0X7F872BF3UL,(int32_t)0XF50497FBUL}},
    {{(int32_t)0X7F3857F6UL,(int32_t)0XF1E43D1CUL}},
    {{(int32_t)0X7ED5E5C6UL,(int32_t)0XEEC60F31UL}},
    {{(int32_t)0X7E5FE493UL,(int32_t)0XEBAA894FUL}},
    {{(int32_t)0X7DD6668FUL,(int32_t)0XE8922622UL}},
    {{(int32_t)0X7D3980ECUL,(int32_t)0XE57D5FDAUL}},
    {{(int32_t)0X7C894BDEUL,(int32_t)0XE26CB01BUL}},
    {{(int32_t)0X7BC5E290UL,(int32_t)0XDF608FE4UL}},
    {{(int32_t)0X7AEF6323UL,(int32_t)0XDC597781UL}},
    {{(int32_t)0X7A05EEADUL,(int32_t)0XD957DE7AUL}},
    {{(int32_t)0X7909A92DUL,(int32_t)0XD65C3B7BUL}},
    {{(int32_t)0X77FAB989UL,(int32_t)0XD3670446UL}},
    {{(int32_t)0X76D94989UL,(int32_t)0XD078AD9EUL}},
    {{(int32_t)0X59646498UL,(int32_t)0XA462EEACUL}},
    {{(int32_t)0X571DEEFAUL,(int32_t)0XA2386284UL}},
    {{(int32_t)0X54CA0A4BUL,(int32_t)0XA01C4C73UL}},
    {{(int32_t)0X5269126EUL,(int32_t)0X9E0EFFC1UL}},
    {{(int32_t)0X4FFB654DUL,(int32_t)0X9C10CD70UL}},
    {{(int32_t)0X4D8162C4UL,(int32_t)0X9A22042DUL}},
    {{(int32_t)0X4AFB6C98UL,(int32_t)0X9842F043UL}},
    {{(int32_t)0X4869E665UL,(int32_t)0X9673DB94UL}},
    {{(int32_t)0X45CD358FUL,(int32_t)0X94B50D87UL}},
    {{(int32_t)0X4325C135UL,(int32_t)0X9306CB04UL}},
    {{(int32_t)0X4073F21DUL,(int32_t)0X91695663UL}},
    {{(int32_t)0X3DB832A6UL,(int32_t)0X8FDCEF66UL}},
    {{(int32_t)0X3AF2EEB7UL,(int32_t)0X8E61D32EUL}},
    {{(int32_t)0X382493B0UL,(int32_t)0X8CF83C30UL}},
    {{(int32_t)0X354D9057UL,(int32_t)0X8BA0622FUL}},
    {{(int32_t)0X326E54C7UL,(int32_t)0X8A5A7A31UL}},
};

static const complex_fract32 dct3_64[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FD8878EUL,(int32_t)0X0647D97CUL}},
    {{(int32_t)0X7F62368FUL,(int32_t)0X0C8BD35EUL}},
    {{(int32_t)0X7E9D55FCUL,(int32_t)0X12C8106FUL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0X18F8B83CUL}},
    {{(int32_t)0X7C29FBEEUL,(int32_t)0X1F19F97BUL}},
    {{(int32_t)0X7A7D055BUL,(int32_t)0X25280C5EUL}},
    {{(int32_t)0X78848414UL,(int32_t)0X2B1F34EBUL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0X30FBC54DUL}},
    {{(int32_t)0X73B5EBD1UL,(int32_t)0X36BA2014UL}},
    {{(int32_t)0X70E2CBC6UL,(int32_t)0X3C56BA70UL}},
    {{(int32_t)0X6DCA0D14UL,(int32_t)0X41CE1E65UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0X471CECE7UL}},
    {{(int32_t)0X66CF8120UL,(int32_t)0X4C3FDFF4UL}},
    {{(int32_t)0X62F201ACUL,(int32_t)0X5133CC94UL}},
    {{(int32_t)0X5ED77C8AUL,(int32_t)0X55F5A4D2UL}},
};

static const complex_fract32 rfft_32[]=
{
    {{(int32_t)0X7D8A5F40UL,(int32_t)0X18F8B83CUL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0X30FBC54DUL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0X471CECE7UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0X5A82799AUL}},
    {{(int32_t)0X471CECE7UL,(int32_t)0X6A6D98A4UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X7641AF3DUL}},
    {{(int32_t)0X18F8B83CUL,(int32_t)0X7D8A5F40UL}},
};

static const complex_fract32 fft_16[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0XCF043AB3UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0000000000UL,(int32_t)0X80000000UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X89BE50C3UL,(int32_t)0X30FBC54DUL}},
};

static const tdct4_twd_fr32 descr = { 64, dct4_twd_64, dct3_64, rfft_32, fft_16 };
const dct_handle_t dct4_32_64=(dct_handle_t)&descr;
const dct_handle_t mdct_32_64=(dct_handle_t)&descr;
