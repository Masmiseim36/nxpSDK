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
    Discrete Cosine Transform, Type II 
    Twiddle factors
   Integrit, 2006-2018
*/
/* Signal Processing Library API. */
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "dct2_twd.h"

/*
exp(i*pi/2*(0:N-1)/N), N=32
*/
static const union ufloat32uint32 ALIGN(8) dct_twd32[64]=
{
{0x3f800000UL},{0x00000000UL},{0x3f7fb10fUL},{0x3d48fb30UL},{0x3f7ec46dUL},{0x3dc8bd36UL},{0x3f7d3aacUL},{0x3e164083UL},
{0x3f7b14beUL},{0x3e47c5c2UL},{0x3f7853f8UL},{0x3e78cfccUL},{0x3f74fa0bUL},{0x3e94a031UL},{0x3f710908UL},{0x3eac7cd4UL},
{0x3f6c835eUL},{0x3ec3ef15UL},{0x3f676bd8UL},{0x3edae880UL},{0x3f61c598UL},{0x3ef15aeaUL},{0x3f5b941aUL},{0x3f039c3dUL},
{0x3f54db31UL},{0x3f0e39daUL},{0x3f4d9f02UL},{0x3f187fc0UL},{0x3f45e403UL},{0x3f226799UL},{0x3f3daef9UL},{0x3f2beb4aUL},
{0x3f3504f3UL},{0x3f3504f3UL},{0x3f2beb4aUL},{0x3f3daef9UL},{0x3f226799UL},{0x3f45e403UL},{0x3f187fc0UL},{0x3f4d9f02UL},
{0x3f0e39daUL},{0x3f54db31UL},{0x3f039c3dUL},{0x3f5b941aUL},{0x3ef15aeaUL},{0x3f61c598UL},{0x3edae880UL},{0x3f676bd8UL},
{0x3ec3ef15UL},{0x3f6c835eUL},{0x3eac7cd4UL},{0x3f710908UL},{0x3e94a031UL},{0x3f74fa0bUL},{0x3e78cfccUL},{0x3f7853f8UL},
{0x3e47c5c2UL},{0x3f7b14beUL},{0x3e164083UL},{0x3f7d3aacUL},{0x3dc8bd36UL},{0x3f7ec46dUL},{0x3d48fb30UL},{0x3f7fb10fUL}
};
/*
N=16;  
twd = exp(-2j*pi*[1;2;3]*(0:N/4-1)/N);
twd_ri = reshape([real(twd(:).');imag(twd(:).')],1,2*numel(twd));
*/
static const union ufloat32uint32 ALIGN(8) fft_twd16[24]=
{
{0x3f800000UL},{0x00000000UL},{0x3f800000UL},{0x00000000UL},{0x3f800000UL},{0x00000000UL},
{0x3f6c835eUL},{0xbec3ef15UL},{0x3f3504f3UL},{0xbf3504f3UL},{0x3ec3ef15UL},{0xbf6c835eUL},
{0x3f3504f3UL},{0xbf3504f3UL},{0x248d3132UL},{0xbf800000UL},{0xbf3504f3UL},{0xbf3504f3UL},
{0x3ec3ef15UL},{0xbf6c835eUL},{0xbf3504f3UL},{0xbf3504f3UL},{0xbf6c835eUL},{0x3ec3ef15UL}
};

/* DCT-II twiddles */
static const tdct2_twd twd32={MAGIC_DCT2_F,32,(void*)dct_twd32,(void*)fft_twd16};
const dct_handle_t dct2_f_32=(const dct_handle_t*)&twd32;
