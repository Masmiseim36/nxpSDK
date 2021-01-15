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

static const complex_fract32 ALIGN(8) dct4_twd_128[]=
{
    {{(int32_t)0X7FFF6216UL,(int32_t)0xFF36F078UL}},
    {{(int32_t)0X7FFA72D1UL,(int32_t)0xFDA4D929UL}},
    {{(int32_t)0X7FF09478UL,(int32_t)0xFC12D91AUL}},
    {{(int32_t)0X7FE1C76BUL,(int32_t)0xFA80FFCBUL}},
    {{(int32_t)0X7FCE0C3EUL,(int32_t)0xF8EF5CBBUL}},
    {{(int32_t)0X7FB563B3UL,(int32_t)0xF75DFF66UL}},
    {{(int32_t)0X7F97CEBDUL,(int32_t)0xF5CCF743UL}},
    {{(int32_t)0X7F754E80UL,(int32_t)0xF43C53CBUL}},
    {{(int32_t)0X7F4DE451UL,(int32_t)0xF2AC246EUL}},
    {{(int32_t)0X7F2191B4UL,(int32_t)0xF11C789AUL}},
    {{(int32_t)0X7EF05860UL,(int32_t)0xEF8D5FB8UL}},
    {{(int32_t)0X7EBA3A39UL,(int32_t)0xEDFEE92BUL}},
    {{(int32_t)0X7E7F3957UL,(int32_t)0xEC71244FUL}},
    {{(int32_t)0X7E3F57FFUL,(int32_t)0xEAE4207AUL}},
    {{(int32_t)0X7DFA98A8UL,(int32_t)0xE957ECFBUL}},
    {{(int32_t)0X7DB0FDF8UL,(int32_t)0xE7CC9917UL}},
    {{(int32_t)0X7D628AC6UL,(int32_t)0xE642340DUL}},
    {{(int32_t)0X7D0F4218UL,(int32_t)0xE4B8CD11UL}},
    {{(int32_t)0X7CB72724UL,(int32_t)0xE330734DUL}},
    {{(int32_t)0X7C5A3D50UL,(int32_t)0xE1A935E2UL}},
    {{(int32_t)0X7BF88830UL,(int32_t)0xE02323E5UL}},
    {{(int32_t)0X7B920B89UL,(int32_t)0xDE9E4C60UL}},
    {{(int32_t)0X7B26CB4FUL,(int32_t)0xDD1ABE51UL}},
    {{(int32_t)0X7AB6CBA4UL,(int32_t)0xDB9888A8UL}},
    {{(int32_t)0X7A4210D8UL,(int32_t)0xDA17BA4AUL}},
    {{(int32_t)0X79C89F6EUL,(int32_t)0xD898620CUL}},
    {{(int32_t)0X794A7C12UL,(int32_t)0xD71A8EB5UL}},
    {{(int32_t)0X78C7ABA2UL,(int32_t)0xD59E4EFFUL}},
    {{(int32_t)0X78403329UL,(int32_t)0xD423B191UL}},
    {{(int32_t)0X77B417DFUL,(int32_t)0xD2AAC504UL}},
    {{(int32_t)0X77235F2DUL,(int32_t)0xD13397E2UL}},
    {{(int32_t)0X768E0EA6UL,(int32_t)0xCFBE389FUL}},
    {{(int32_t)0X59F3DE12UL,(int32_t)0xA4EFCA31UL}},
    {{(int32_t)0X58D40E8CUL,(int32_t)0xA3D6F534UL}},
    {{(int32_t)0X57B0D256UL,(int32_t)0xA2C1ADC9UL}},
    {{(int32_t)0X568A34A9UL,(int32_t)0xA1AFFEA3UL}},
    {{(int32_t)0X556040E2UL,(int32_t)0xA0A1F24DUL}},
    {{(int32_t)0X5433027DUL,(int32_t)0x9F979331UL}},
    {{(int32_t)0X53028518UL,(int32_t)0x9E90EB94UL}},
    {{(int32_t)0X51CED46EUL,(int32_t)0x9D8E0597UL}},
    {{(int32_t)0X5097FC5EUL,(int32_t)0x9C8EEB34UL}},
    {{(int32_t)0X4F5E08E3UL,(int32_t)0x9B93A641UL}},
    {{(int32_t)0X4E210617UL,(int32_t)0x9A9C406EUL}},
    {{(int32_t)0X4CE10034UL,(int32_t)0x99A8C345UL}},
    {{(int32_t)0X4B9E0390UL,(int32_t)0x98B93828UL}},
    {{(int32_t)0X4A581C9EUL,(int32_t)0x97CDA855UL}},
    {{(int32_t)0X490F57EEUL,(int32_t)0x96E61CE0UL}},
    {{(int32_t)0X47C3C22FUL,(int32_t)0x96029EB6UL}},
    {{(int32_t)0X46756828UL,(int32_t)0x9523369CUL}},
    {{(int32_t)0X452456BDUL,(int32_t)0x9447ED2FUL}},
    {{(int32_t)0X43D09AEDUL,(int32_t)0x9370CAE4UL}},
    {{(int32_t)0X427A41D0UL,(int32_t)0x929DD806UL}},
    {{(int32_t)0X4121589BUL,(int32_t)0x91CF1CB6UL}},
    {{(int32_t)0X3FC5EC98UL,(int32_t)0x9104A0EEUL}},
    {{(int32_t)0X3E680B2CUL,(int32_t)0x903E6C7BUL}},
    {{(int32_t)0X3D07C1D6UL,(int32_t)0x8F7C8701UL}},
    {{(int32_t)0X3BA51E29UL,(int32_t)0x8EBEF7FBUL}},
    {{(int32_t)0X3A402DD2UL,(int32_t)0x8E05C6B7UL}},
    {{(int32_t)0X38D8FE93UL,(int32_t)0x8D50FA59UL}},
    {{(int32_t)0X376F9E46UL,(int32_t)0x8CA099DAUL}},
    {{(int32_t)0X36041AD9UL,(int32_t)0x8BF4AC05UL}},
    {{(int32_t)0X34968250UL,(int32_t)0x8B4D377CUL}},
    {{(int32_t)0X3326E2C3UL,(int32_t)0x8AAA42B4UL}},
    {{(int32_t)0X31B54A5EUL,(int32_t)0x8A0BD3F5UL}},
};

static const complex_fract32 dct3_128[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0x00000000UL}},
    {{(int32_t)0X7FF62182UL,(int32_t)0x03242ABFUL}},
    {{(int32_t)0X7FD8878EUL,(int32_t)0x0647D97CUL}},
    {{(int32_t)0X7FA736B4UL,(int32_t)0x096A9049UL}},
    {{(int32_t)0X7F62368FUL,(int32_t)0x0C8BD35EUL}},
    {{(int32_t)0X7F0991C4UL,(int32_t)0x0FAB272BUL}},
    {{(int32_t)0X7E9D55FCUL,(int32_t)0x12C8106FUL}},
    {{(int32_t)0X7E1D93EAUL,(int32_t)0x15E21445UL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0x18F8B83CUL}},
    {{(int32_t)0X7CE3CEB2UL,(int32_t)0x1C0B826AUL}},
    {{(int32_t)0X7C29FBEEUL,(int32_t)0x1F19F97BUL}},
    {{(int32_t)0X7B5D039EUL,(int32_t)0x2223A4C5UL}},
    {{(int32_t)0X7A7D055BUL,(int32_t)0x25280C5EUL}},
    {{(int32_t)0X798A23B1UL,(int32_t)0x2826B928UL}},
    {{(int32_t)0X78848414UL,(int32_t)0x2B1F34EBUL}},
    {{(int32_t)0X776C4EDBUL,(int32_t)0x2E110A62UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0x30FBC54DUL}},
    {{(int32_t)0X7504D345UL,(int32_t)0x33DEF287UL}},
    {{(int32_t)0X73B5EBD1UL,(int32_t)0x36BA2014UL}},
    {{(int32_t)0X72552C85UL,(int32_t)0x398CDD32UL}},
    {{(int32_t)0X70E2CBC6UL,(int32_t)0x3C56BA70UL}},
    {{(int32_t)0X6F5F02B2UL,(int32_t)0x3F1749B8UL}},
    {{(int32_t)0X6DCA0D14UL,(int32_t)0x41CE1E65UL}},
    {{(int32_t)0X6C242960UL,(int32_t)0x447ACD50UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0x471CECE7UL}},
    {{(int32_t)0X68A69E81UL,(int32_t)0x49B41533UL}},
    {{(int32_t)0X66CF8120UL,(int32_t)0x4C3FDFF4UL}},
    {{(int32_t)0X64E88926UL,(int32_t)0x4EBFE8A5UL}},
    {{(int32_t)0X62F201ACUL,(int32_t)0x5133CC94UL}},
    {{(int32_t)0X60EC3830UL,(int32_t)0x539B2AF0UL}},
    {{(int32_t)0X5ED77C8AUL,(int32_t)0x55F5A4D2UL}},
    {{(int32_t)0X5CB420E0UL,(int32_t)0x5842DD54UL}},
};

static const complex_fract32 rfft_64[]=
{
    {{(int32_t)0X7F62368FUL,(int32_t)0x0C8BD35EUL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0x18F8B83CUL}},
    {{(int32_t)0X7A7D055BUL,(int32_t)0x25280C5EUL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0x30FBC54DUL}},
    {{(int32_t)0X70E2CBC6UL,(int32_t)0x3C56BA70UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0x471CECE7UL}},
    {{(int32_t)0X62F201ACUL,(int32_t)0x5133CC94UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0x5A82799AUL}},
    {{(int32_t)0X5133CC94UL,(int32_t)0x62F201ACUL}},
    {{(int32_t)0X471CECE7UL,(int32_t)0x6A6D98A4UL}},
    {{(int32_t)0X3C56BA70UL,(int32_t)0x70E2CBC6UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0x7641AF3DUL}},
    {{(int32_t)0X25280C5EUL,(int32_t)0x7A7D055BUL}},
    {{(int32_t)0X18F8B83CUL,(int32_t)0x7D8A5F40UL}},
    {{(int32_t)0X0C8BD35EUL,(int32_t)0x7F62368FUL}},
};

static const complex_fract32 fft_32[]=
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0x00000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0x00000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0x00000000UL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0xE70747C4UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0xCF043AB3UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0xB8E31319UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0xCF043AB3UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0xA57D8666UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0x89BE50C3UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0xB8E31319UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0x89BE50C3UL}},
    {{(int32_t)0XE70747C4UL,(int32_t)0x8275A0C0UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0xA57D8666UL}},
    {{(int32_t)0000000000UL,(int32_t)0x80000000UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0xA57D8666UL}},
    {{(int32_t)0X471CECE7UL,(int32_t)0x9592675CUL}},
    {{(int32_t)0XCF043AB3UL,(int32_t)0x89BE50C3UL}},
    {{(int32_t)0X8275A0C0UL,(int32_t)0xE70747C4UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0x89BE50C3UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0xA57D8666UL}},
    {{(int32_t)0X89BE50C3UL,(int32_t)0x30FBC54DUL}},
    {{(int32_t)0X18F8B83CUL,(int32_t)0x8275A0C0UL}},
    {{(int32_t)0X89BE50C3UL,(int32_t)0xCF043AB3UL}},
    {{(int32_t)0XB8E31319UL,(int32_t)0x6A6D98A4UL}},
};

static const tdct4_twd_fr32 descr = { 128, dct4_twd_128, dct3_128, rfft_64, fft_32 };
const dct_handle_t dct4_32_128=(dct_handle_t)&descr;
const dct_handle_t mdct_32_128=(dct_handle_t)&descr;
