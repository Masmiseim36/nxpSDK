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
    N=32;
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

static const uint64_t twd_split16[8]=
{
  /*
     NOTE: 32-bit halves of numbers are swapped
     for optimization purposes
  */
  0x7FFFFFFF00000000ULL,0x7D8A5F4018F8B83CULL,
  0x7641AF3D30FBC54DULL,0x6A6D98A4471CECE7ULL,
  0x5A82799A5A82799AULL,0x471CECE76A6D98A4ULL,
  0x30FBC54D7641AF3DULL,0x18F8B83C7D8A5F40ULL
};
static const int32_t twd_dct16[17*2]=
{
  /*
     NOTE: numbers are interleaved
     for optimization purposes
  */
  (int32_t)0x7FFFFFFFUL, (int32_t)0x5A82799AUL,
  (int32_t)0x7FD8878EUL, (int32_t)0xF9B82684UL, (int32_t)0x5ED77C8AUL, (int32_t)0x55F5A4D2UL,
  (int32_t)0x7F62368FUL, (int32_t)0xF3742CA2UL, (int32_t)0x62F201ACUL, (int32_t)0x5133CC94UL,
  (int32_t)0x7E9D55FCUL, (int32_t)0xED37EF91UL, (int32_t)0x66CF8120UL, (int32_t)0x4C3FDFF4UL,
  (int32_t)0x7D8A5F40UL, (int32_t)0xE70747C4UL, (int32_t)0x6A6D98A4UL, (int32_t)0x471CECE7UL,
  (int32_t)0x7C29FBEEUL, (int32_t)0xE0E60685UL, (int32_t)0x6DCA0D14UL, (int32_t)0x41CE1E65UL,
  (int32_t)0x7A7D055BUL, (int32_t)0xDAD7F3A2UL, (int32_t)0x70E2CBC6UL, (int32_t)0x3C56BA70UL,
  (int32_t)0x78848414UL, (int32_t)0xD4E0CB15UL, (int32_t)0x73B5EBD1UL, (int32_t)0x36BA2015UL,
  (int32_t)0x7641AF3DUL, (int32_t)0x30FBC54DUL
};
static const complex_fract32 twd_fft16[]=
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

static const tdct2_twd twd_N32={MAGIC_DCT2_32,32,(void*)twd_split16,(void*)twd_fft16,(void*)twd_dct16};
const dct_handle_t dct2_32_32=(const dct_handle_t*)&twd_N32;
