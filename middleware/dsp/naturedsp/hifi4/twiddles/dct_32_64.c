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
    DCT-II twiddles
    Integrit, 2006-2018
*/

#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct2_twd.h"

/*
    pregenerated tables:
    N=64;
    cosi=exp(2*i*pi*(0:N/4-1)/(N));
    w=exp(-i*pi/2*(0:N/2)/N);
    COSI(1:2:N/2)=real(cosi);
    COSI(2:2:N/2)=imag(cosi);
    W(1:2:N+2)=real(w);
    W(2:2:N+2)=imag(w);
    Q=2^31;
    COSI=min(Q-1,max(-Q,round(COSI*Q)));
    W=min(Q-1,max(-Q,round(W*Q)));
    idx=find(COSI<0);COSI(idx)=COSI(idx)+2*Q;
    idx=find(W<0);W(idx)=W(idx)+2*Q;

    fprintf(1,'static const uint64_t cosi[]={\n');
    for k=1:N/4
        c=COSI(2*(k-1)+1);
        s=COSI(2*k);
        fprintf(1,'0x%08s%08sULL,',dec2hex(s),dec2hex(c));
    end
    fprintf(1,'};\n');
    fprintf(1,'static const uint64_t w[]={\n');
    for k=1:N/2+1
        c=W(2*(k-1)+1);
        s=W(2*k);
        fprintf(1,'0x%08s%08sULL,',dec2hex(s),dec2hex(c));
    end
    fprintf(1,'};\n');
*/

static const uint64_t twd_split32[16]=
{
  /*
     NOTE: 32-bit halves of numbers are swapped
     for optimization purposes
  */
  0x7FFFFFFF00000000ULL,
  0x7F62368F0C8BD35EULL,
  0x7D8A5F4018F8B83CULL,
  0x7A7D055B25280C5EULL,
  0x7641AF3D30FBC54DULL,
  0x70E2CBC63C56BA70ULL,
  0x6A6D98A4471CECE7ULL,
  0x62F201AC5133CC94ULL,
  0x5A82799A5A82799AULL,
  0x5133CC9462F201ACULL,
  0x471CECE76A6D98A4ULL,
  0x3C56BA7070E2CBC6ULL,
  0x30FBC54D7641AF3DULL,
  0x25280C5E7A7D055BULL,
  0x18F8B83C7D8A5F40ULL,
  0x0C8BD35E7F62368FULL
};
static const int32_t twd_dct32[32*2]=
{
  /*
     NOTE: numbers are interleaved
     for optimization purposes
  */
  (int32_t)0x7FFFFFFFUL, (int32_t)0x5A82799AUL,
  (int32_t)0x7FF62182UL, (int32_t)0xFCDBD541UL,
  (int32_t)0x5CB420E0UL,-(int32_t)0xA7BD22ACUL,
  (int32_t)0x7FD8878EUL, (int32_t)0xF9B82684UL,
  (int32_t)0x5ED77C8AUL,-(int32_t)0xAA0A5B2EUL,
  (int32_t)0x7FA736B4UL, (int32_t)0xF6956FB7UL,
  (int32_t)0x60EC3830UL,-(int32_t)0xAC64D510UL,
  (int32_t)0x7F62368FUL, (int32_t)0xF3742CA2UL,
  (int32_t)0x62F201ACUL,-(int32_t)0xAECC336CUL,
  (int32_t)0x7F0991C4UL, (int32_t)0xF054D8D5UL,
  (int32_t)0x64E88926UL,-(int32_t)0xB140175BUL,
  (int32_t)0x7E9D55FCUL, (int32_t)0xED37EF91UL,
  (int32_t)0x66CF8120UL,-(int32_t)0xB3C0200CUL,
  (int32_t)0x7E1D93EAUL, (int32_t)0xEA1DEBBBUL,
  (int32_t)0x68A69E81UL,-(int32_t)0xB64BEACDUL,
  (int32_t)0x7D8A5F40UL, (int32_t)0xE70747C4UL,
  (int32_t)0x6A6D98A4UL,-(int32_t)0xB8E31319UL,
  (int32_t)0x7CE3CEB2UL, (int32_t)0xE3F47D96UL,
  (int32_t)0x6C242960UL,-(int32_t)0xBB8532B0UL,
  (int32_t)0x7C29FBEEUL, (int32_t)0xE0E60685UL,
  (int32_t)0x6DCA0D14UL,-(int32_t)0xBE31E19BUL,
  (int32_t)0x7B5D039EUL, (int32_t)0xDDDC5B3BUL,
  (int32_t)0x6F5F02B2UL,-(int32_t)0xC0E8B648UL,
  (int32_t)0x7A7D055BUL, (int32_t)0xDAD7F3A2UL,
  (int32_t)0x70E2CBC6UL,-(int32_t)0xC3A94590UL,
  (int32_t)0x798A23B1UL, (int32_t)0xD7D946D8UL,
  (int32_t)0x72552C85UL,-(int32_t)0xC67322CEUL,
  (int32_t)0x78848414UL, (int32_t)0xD4E0CB15UL,
  (int32_t)0x73B5EBD1UL,-(int32_t)0xC945DFECUL,
  (int32_t)0x776C4EDBUL, (int32_t)0xD1EEF59EUL,
  (int32_t)0x7504D345UL,-(int32_t)0xCC210D79UL,
  (int32_t)0x7641AF3DUL,-(int32_t)0xCF043AB3UL
};
static const complex_fract32 twd_fft32[2*3*8] = 
{
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7FFFFFFFUL,(int32_t)0000000000UL}},
    {{(int32_t)0X7D8A5F40UL,(int32_t)0XE70747C4UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0XCF043AB3UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0XB8E31319UL}},
    {{(int32_t)0X7641AF3DUL,(int32_t)0XCF043AB3UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0X6A6D98A4UL,(int32_t)0XB8E31319UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0XE70747C4UL,(int32_t)0X8275A0C0UL}},
    {{(int32_t)0X5A82799AUL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0000000000UL,(int32_t)0X80000000UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X471CECE7UL,(int32_t)0X9592675CUL}},
    {{(int32_t)0XCF043AB3UL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0X8275A0C0UL,(int32_t)0XE70747C4UL}},
    {{(int32_t)0X30FBC54DUL,(int32_t)0X89BE50C3UL}},
    {{(int32_t)0XA57D8666UL,(int32_t)0XA57D8666UL}},
    {{(int32_t)0X89BE50C3UL,(int32_t)0X30FBC54DUL}},
    {{(int32_t)0X18F8B83CUL,(int32_t)0X8275A0C0UL}},
    {{(int32_t)0X89BE50C3UL,(int32_t)0XCF043AB3UL}},
    {{(int32_t)0XB8E31319UL,(int32_t)0X6A6D98A4UL}},
};

static const tdct2_twd twd_N64={MAGIC_DCT2_32,64,(void*)twd_split32,(void*)twd_fft32,(void*)twd_dct32};
const dct_handle_t dct2_32_64=(const dct_handle_t*)&twd_N64;
