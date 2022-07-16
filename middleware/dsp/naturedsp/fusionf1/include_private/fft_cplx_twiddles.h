/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
/*
	NatureDSP_Signal library. FFT part
    C code with generic optimization
	Integrit, 2006-2015

    twiddle factors for 32x16/16x16 real fwd/rev FFT transforms
*/
#ifndef FFT_CPLX_TWIDDLES_H__
#define FFT_CPLX_TWIDDLES_H__
#include "NatureDSP_Signal.h"
#include "common.h"

typedef struct
{
    int N;
    cint16_ptr inc;
    cint32_ptr twd;
}
tFftDescr;

/*
    function returns
    pTwd[0] - pointer to the twiddle factors
    pInc[0] - pointer to the increments
    pDigrevtbl[0] - pointer to the digit reversal table
    Input:
    N - FFT size
*/
const tFftDescr* fft_cplx_getTables(int N);
const tFftDescr* ifft_cplx_getTables(int N);


extern const short fft_inc64[];
extern const short fft_inc128[];
extern const short fft_inc256[];
extern const short fft_inc512[];
extern const short fft_inc1024[];
extern const short fft_inc2048[];
extern const short fft_inc4096[];

extern const short fft_twd16[];
extern const short fft_twd32[];
extern const short fft_twd64[];
extern const short fft_twd128[];
extern const short fft_twd256[];
extern const short fft_twd512[];
extern const short fft_twd1024[];
extern const short fft_twd2048[];
extern const short fft_twd4096[];

extern const short ifft_twd16[];
extern const short ifft_twd32[];
extern const short ifft_twd64[];
extern const short ifft_twd128[];
extern const short ifft_twd256[];
extern const short ifft_twd512[];
extern const short ifft_twd1024[];
extern const short ifft_twd2048[];
extern const short ifft_twd4096[];

extern  const int16_t _fft_twiddle_table_8_[];
extern  const int16_t _fft_twiddle_table_16_[];
extern  const int16_t _fft_twiddle_table_32_[];
extern  const int16_t _fft_twiddle_table_64_[];
extern  const int16_t _fft_twiddle_table_128_[];
extern  const int16_t _fft_twiddle_table_256_[];
extern  const int16_t _fft_twiddle_table_512_[];
extern  const int16_t _fft_twiddle_table_1024_[];
extern  const int16_t _fft_twiddle_table_2048_[];
extern  const int16_t _fft_twiddle_table_4096_[];

extern  const int16_t _ifft_twiddle_table_8_[];
extern  const int16_t _ifft_twiddle_table_16_[];
extern  const int16_t _ifft_twiddle_table_32_[];
extern  const int16_t _ifft_twiddle_table_64_[];
extern  const int16_t _ifft_twiddle_table_128_[];
extern  const int16_t _ifft_twiddle_table_256_[];
extern  const int16_t _ifft_twiddle_table_512_[];
extern  const int16_t _ifft_twiddle_table_1024_[];
extern  const int16_t _ifft_twiddle_table_2048_[];
extern  const int16_t _ifft_twiddle_table_4096_[];
#endif
