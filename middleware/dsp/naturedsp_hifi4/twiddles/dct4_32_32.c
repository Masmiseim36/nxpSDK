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

static const complex_fract32 ALIGN(8) dct4_twd_32[]=
{
    {{(int32_t)0X7FF62182UL,(int32_t)0XFCDBD541UL}},
    {{(int32_t)0X7FA736B4UL,(int32_t)0XF6956FB7UL}},
    {{(int32_t)0X7F0991C4UL,(int32_t)0XF054D8D5UL}},
    {{(int32_t)0X7E1D93EAUL,(int32_t)0XEA1DEBBBUL}},
    {{(int32_t)0X7CE3CEB2UL,(int32_t)0XE3F47D96UL}},
    {{(int32_t)0X7B5D039EUL,(int32_t)0XDDDC5B3BUL}},
    {{(int32_t)0X798A23B1UL,(int32_t)0XD7D946D8UL}},
    {{(int32_t)0X776C4EDBUL,(int32_t)0XD1EEF59EUL}},
    {{(int32_t)0X5842DD54UL,(int32_t)0XA34BDF20UL}},
    {{(int32_t)0X539B2AF0UL,(int32_t)0X9F13C7D0UL}},
    {{(int32_t)0X4EBFE8A5UL,(int32_t)0X9B1776DAUL}},
    {{(int32_t)0X49B41533UL,(int32_t)0X9759617FUL}},
    {{(int32_t)0X447ACD50UL,(int32_t)0X93DBD6A0UL}},
    {{(int32_t)0X3F1749B8UL,(int32_t)0X90A0FD4EUL}},
    {{(int32_t)0X398CDD32UL,(int32_t)0X8DAAD37BUL}},
    {{(int32_t)0X33DEF287UL,(int32_t)0X8AFB2CBBUL}},
};

static const complex_fract32 dct3_32[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7F62368FUL,(int32_t)0X0C8BD35EUL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0X18F8B83CUL}},
    {{(int32_t)0X7A7D055BUL,(int32_t)0X25280C5EUL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0X30FBC54DUL}},
    {{(int32_t)0X70E2CBC6UL,(int32_t)0X3C56BA70UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0X471CECE7UL}},
    {{(int32_t)0X62F201ACUL,(int32_t)0X5133CC94UL}},
};

static const complex_fract32 rfft_16[]=
{
    {{(int32_t)0X7641AF3DUL,(int32_t)0X30FBC54DUL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0X5A82799AUL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X7641AF3DUL}},
};

static const complex_fract32 fft_8[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0000000000UL,(int32_t)0X80000000UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0XA57D8666UL}},
};

static const tdct4_twd_fr32 descr = { 32, dct4_twd_32, dct3_32, rfft_16, fft_8 };
const dct_handle_t dct4_32_32=(dct_handle_t)&descr;
const dct_handle_t mdct_32_32=(dct_handle_t)&descr;
