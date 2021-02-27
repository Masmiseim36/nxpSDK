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
#ifndef FFT_CPLX_TWIDDLES_24x24_H__
#define FFT_CPLX_TWIDDLES_24x24_H__
#include "NatureDSP_Signal.h"
#include "common.h"
#include "fft_cplx_twiddles.h"

extern const int32_t fft24x24_twd16[];
extern const int32_t fft24x24_twd32[];
extern const int32_t fft24x24_twd64[];
extern const int32_t fft24x24_twd128[];
extern const int32_t fft24x24_twd256[];
extern const int32_t fft24x24_twd512[];
extern const int32_t fft24x24_twd1024[];
extern const int32_t fft24x24_twd2048[];
extern const int32_t fft24x24_twd4096[];

extern const int32_t ifft24x24_twd16[];
extern const int32_t ifft24x24_twd32[];
extern const int32_t ifft24x24_twd64[];
extern const int32_t ifft24x24_twd128[];
extern const int32_t ifft24x24_twd256[];
extern const int32_t ifft24x24_twd512[];
extern const int32_t ifft24x24_twd1024[];
extern const int32_t ifft24x24_twd2048[];
extern const int32_t ifft24x24_twd4096[];

/*
    function returns
    pTwd[0] - pointer to the twiddle factors
    pInc[0] - pointer to the increments
    pDigrevtbl[0] - pointer to the digit reversal table
    Input:
    N - FFT size
*/
void fft_getTables_24x24(cuint64_ptr *pTwd,
                          cint16_ptr *pInc,
                          cint16_ptr *pDigrevtbl,
                          int N);

const tFftDescr* fft_getTables_24x24_hifi3 (int N);
const tFftDescr* ifft_getTables_24x24_hifi3(int N);

#endif
