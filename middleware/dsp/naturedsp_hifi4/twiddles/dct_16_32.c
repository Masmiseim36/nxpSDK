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
    Q=2^15;
    COSI=min(Q-1,max(-Q,round(COSI*Q)));
    W=min(Q-1,max(-Q,round(W*Q)));
    idx=find(COSI<0);COSI(idx)=COSI(idx)+2*Q;
    idx=find(W<0);W(idx)=W(idx)+2*Q;

    fprintf(1,'static const uint32_t cosi[]={\n');
    for k=1:N/4
        c=COSI(2*(k-1)+1);
        s=COSI(2*k);
        fprintf(1,'0x%04s%04s,',dec2hex(s),dec2hex(c));
    end
    fprintf(1,'};\n');
    fprintf(1,'static const uint32_t w[]={\n');
    for k=1:N/2+1
        c=W(2*(k-1)+1);
        s=W(2*k);
        fprintf(1,'0x%04s%04s,',dec2hex(s),dec2hex(c));
    end
    fprintf(1,'};\n');
*/

static const uint32_t ALIGN(8) twd_split16[8]=
{
    /*
       NOTE: 1st number is skipped and
       16-bit halves of numbers are swapped
       for optimization purposes
    */
    /*0x00007FFF,*/
    0x7D8A18F9UL,
    0x764230FCUL,
    0x6A6E471DUL,
    0x5A825A82UL,
    0x471D6A6EUL,
    0x30FC7642UL,
    0x18F97D8AUL,
    0x0UL
};
static const int16_t twd_dct16[2*16]=
{
    /*
       NOTE: numbers are interleaved
       for optimization purposes
    */
    (int16_t)0x7FFFU,    (int16_t)0x5A82U,
    (int16_t)0x7FD9U,    (int16_t)0xF9B8U,
    (int16_t)0x5ED7U,   -(int16_t)0xAA0AU,
    (int16_t)0x7F62U,    (int16_t)0xF374U,
    (int16_t)0x62F2U,   -(int16_t)0xAECCU,
    (int16_t)0x7E9DU,    (int16_t)0xED38U,
    (int16_t)0x66D0U,   -(int16_t)0xB3C0U,
    (int16_t)0x7D8AU,    (int16_t)0xE707U,
    (int16_t)0x6A6EU,   -(int16_t)0xB8E3U,
    (int16_t)0x7C2AU,    (int16_t)0xE0E6U,
    (int16_t)0x6DCAU,   -(int16_t)0xBE32U,
    (int16_t)0x7A7DU,    (int16_t)0xDAD8U,
    (int16_t)0x70E3U,   -(int16_t)0xC3A9U,
    (int16_t)0x7885U,    (int16_t)0xD4E1U,
    (int16_t)0x73B6U,   -(int16_t)0xC946U,
    (int16_t)0x7642U,   -(int16_t)0xCF04U
};
static const int16_t ALIGN(8) twd_fft16[2*3*4] = 
{
    (int16_t)0x7fffU,    (int16_t) 0x0000,
    (int16_t)0x7fffU,    (int16_t) 0x0000, 
    (int16_t)0x7fffU,    (int16_t) 0x0000, 
    (int16_t)0x7642U,    (int16_t)-0x30fc,
    (int16_t)0x5a82U,    (int16_t)-0x5a82, 
    (int16_t)0x30fcU,    (int16_t)-0x7642, 
    (int16_t)0x5a82U,    (int16_t)-0x5a82, 
    (int16_t)0x0000U,    (int16_t)-MIN_INT16,
   -(int16_t)0x5a82U,    (int16_t)-0x5a82, 
    (int16_t)0x30fcU,    (int16_t)-0x7642, 
   -(int16_t)0x5a82U,    (int16_t)-0x5a82, 
   -(int16_t)0x7642U,    (int16_t) 0x30fc
};

static const tdct2_twd twd_N32={MAGIC_DCT2_16,32,(void*)twd_split16,(void*)twd_fft16,(void*)twd_dct16};
const dct_handle_t dct2_16_32=(const dct_handle_t*)&twd_N32;
