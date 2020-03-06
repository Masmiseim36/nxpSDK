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

static const uint32_t twd_split32[16]=
{
    /*
       NOTE: 1st number is skipped and
       16-bit halves of numbers are swapped
       for optimization purposes
    */
    /*0x00007FFF,*/
    0x7F620C8CUL,
    0x7D8A18F9UL,
    0x7A7D2528UL,
    0x764230FCUL,
    0x70E33C57UL,
    0x6A6E471DUL,
    0x62F25134UL,
    0x5A825A82UL,
    0x513462F2UL,
    0x471D6A6EUL,
    0x3C5770E3UL,
    0x30FC7642UL,
    0x25287A7DUL,
    0x18F97D8AUL,
    0x0C8C7F62UL, 
    0x0UL
};
static const int16_t twd_dct32[]=
{
    /*
       NOTE: numbers are interleaved
       for optimization purposes
    */
    (int16_t)0x7FFFU, (int16_t)0x5A82U,
    (int16_t)0x7FF6U, (int16_t)0xFCDCU,
    (int16_t)0x5CB4U,-(int16_t)0xA7BDU,
    (int16_t)0x7FD9U, (int16_t)0xF9B8U,
    (int16_t)0x5ED7U,-(int16_t)0xAA0AU,
    (int16_t)0x7FA7U, (int16_t)0xF695U,
    (int16_t)0x60ECU,-(int16_t)0xAC65U,
    (int16_t)0x7F62U, (int16_t)0xF374U,
    (int16_t)0x62F2U,-(int16_t)0xAECCU,
    (int16_t)0x7F0AU, (int16_t)0xF055U,
    (int16_t)0x64E9U,-(int16_t)0xB140U,
    (int16_t)0x7E9DU, (int16_t)0xED38U,
    (int16_t)0x66D0U,-(int16_t)0xB3C0U,
    (int16_t)0x7E1EU, (int16_t)0xEA1EU,
    (int16_t)0x68A7U,-(int16_t)0xB64CU,
    (int16_t)0x7D8AU, (int16_t)0xE707U,
    (int16_t)0x6A6EU,-(int16_t)0xB8E3U,
    (int16_t)0x7CE4U, (int16_t)0xE3F4U,
    (int16_t)0x6C24U,-(int16_t)0xBB85U,
    (int16_t)0x7C2AU, (int16_t)0xE0E6U,
    (int16_t)0x6DCAU,-(int16_t)0xBE32U,
    (int16_t)0x7B5DU, (int16_t)0xDDDCU,
    (int16_t)0x6F5FU,-(int16_t)0xC0E9U,
    (int16_t)0x7A7DU, (int16_t)0xDAD8U,
    (int16_t)0x70E3U,-(int16_t)0xC3A9U,
    (int16_t)0x798AU, (int16_t)0xD7D9U,
    (int16_t)0x7255U,-(int16_t)0xC673U,
    (int16_t)0x7885U, (int16_t)0xD4E1U,
    (int16_t)0x73B6U,-(int16_t)0xC946U,
    (int16_t)0x776CU, (int16_t)0xD1EFU,
    (int16_t)0x7505U,-(int16_t)0xCC21U,
    (int16_t)0x7642U,-(int16_t)0xCF04U,
};
static const complex_fract16 twd_fft32[3*8] = 
{
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 32767, 0    }},
    {{ 32138,-6393 }},
    {{ 30274,-12540}},
    {{ 27246,-18205}},
    {{ 30274,-12540}},
    {{ 23170,-23170}},
    {{ 12540,-30274}},
    {{ 27246,-18205}},
    {{ 12540,-30274}},
    {{-6393 ,-32138}},
    {{ 23170,-23170}},
    {{ 0    ,-32768}},
    {{-23170,-23170}},
    {{ 18205,-27246}},
    {{-12540,-30274}},
    {{-32138,-6393 }},
    {{ 12540,-30274}},
    {{-23170,-23170}},
    {{-30274, 12540}},
    {{ 6393 ,-32138}},
    {{-30274,-12540}},
    {{-18205, 27246}}
};

static const tdct2_twd twd_N64={MAGIC_DCT2_16,64,(void*)twd_split32,(void*)twd_fft32,(void*)twd_dct32};
const dct_handle_t dct2_16_64=(const dct_handle_t*)&twd_N64;
