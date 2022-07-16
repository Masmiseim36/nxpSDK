/* ------------------------------------------------------------------------ */
/* Copyright (c) 2020 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2020 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef IMGFFT_COMMON_H__
#define IMGFFT_COMMON_H__
#include "common.h"

/*-----------------------------------------------------------
Apply RFFT to rows, find common scale and rescale results
Input:
x[M*N]  real data
Output:
y[M*N]  spectrum
Temporary:
scr[N+M]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyRFFT_toRows(complex_fract16* scr, complex_fract16 *y, int16_t *x, int M, int N);

/*-----------------------------------------------------------
Apply FFT to columns, find common scale and rescale results
Input:
x[M*N]  row spectra
Output:
y[M*N]  spectrum
Temporary:
scr[2*M+N/2+1]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyFFT_toCols(complex_fract16* scr, complex_fract16 *y, complex_fract16 *x, int M, int N);

/*-----------------------------------------------------------
Apply IFFT to columns, find common scale and rescale results
Input:
x[M*N]  row spectra
Output:
y[M*N]  spectrum
Temporary:
scr[2*M+N/2+1]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyIFFT_toCols(complex_fract16* scr, complex_fract16 *y, complex_fract16 *x, int M, int N);

/*-----------------------------------------------------------
Apply RIFFT to rows, find common scale and rescale results
Input:
x[M*N]  spectrum
Output:
y[M*N]  real data
Temporary:
scr[N+M]
Returns common scale factor
-----------------------------------------------------------*/
int ApplyRIFFT_toRows(complex_fract16* scr, int16_t *y, complex_fract16 *x, int M, int N);

#endif
