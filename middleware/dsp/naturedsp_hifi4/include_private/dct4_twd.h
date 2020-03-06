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
/*          Copyright (C) 2015-2017 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
    NatureDSP Signal Processing Library. FFT part
    DCT-IV twiddles
    Integrit, 2006-2011
*/
#ifndef DCT4_TWD__
#define DCT4_TWD__
#include "NatureDSP_Signal_fft.h"
#include "common.h"

/* DCT-IV twiddles */
typedef struct
{
    int   N;                        /* DCT-IV size                                                     */
    const complex_fract16* split;   /* split part twiddles: exp(-j*pi*[(1:2:N/2) (1:2:N/2)+N]/(4*N))   */
    const complex_fract16* dct3;    /* DCT-III twiddles: exp(j*pi(1:N/4)/N                             */
    const complex_fract16* rfft;    /* real FFT split phase: exp(pi*j(1:N/8)/(N/2)                     */
    const complex_fract16* fft;     /* FFT twddles exp(-j*pi *[1;2;3]*(0:N/4-1)/(N/4)                  */
}
tdct4_twd_fr16;

typedef struct
{
    int   N;                        /* DCT-IV size                                                     */
    const complex_fract32* split;   /* split part twiddles: exp(-j*pi*[(1:2:N/2) (1:2:N/2)+N]/(4*N))   */
    const complex_fract32* dct3;    /* DCT-III twiddles: exp(j*pi*(1:N/4)/N)                           */
    const complex_fract32* rfft;    /* real FFT split phase: exp(j*pi*(1:N/8-1)/(N/4))                 */
    const complex_fract32* fft;     /* FFT twddles exp(-j*pi*[1;2;3]*(0:N/4-1))/(N/4)                  */
}
tdct4_twd_fr32;

#endif
