/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
    Inner FFT stages shared between forward and inverse complex FFT kernels.
	IntegrIT, 2006-2014
*/

#ifndef __FFT_CPLX_COMMON_H
#define __FFT_CPLX_COMMON_H

#include "NatureDSP_types.h"
/* Common utility macros. */
#include "common.h"

/*------------------------------------------------------------------------------
  Inner radix-4 stages of complex-valued FFT of size 2^n, n=4..12.

  Notes:
    1. Inner means second through the next to last stages.
    2. Data are not permuted.
    3. At each stage data are automatically downscaled to avoid overflows.

  Precision: 
    32x32  32-bit input/output data, 32-bit twiddle factors
    32x16  32-bit input/output data, 16-bit twiddle factors
    16x16  16-bit input/output data, 16-bit twiddle factors

  Input:
    x[2*N]/y[2*N]  complex input data. Real and imaginary data are interleaved 
                   and real data goes first.
                   16x16,32x16:                  input data in y[2*N]
                   32x32, (log2(N)+1)/2 is even: input data in y[2*N]
                   32x32, (log2(N)+1)/2 is odd:  input data in x[2*N]
   twd[2*N*3/4]    complex twiddle factor table

  Output:
    y[2*N]         complex output data. Real and imaginary data are interleaved 
                   and real data goes first.

  Input/Output:
    bexp           common block exponent, that is the minimum number of
                   redundant sign bits over input (output) data

  Returned value: total right shift amount applied to dynamically scale the data

  Restrictions:
    x[],y[],twd[] - must not overlap
    x[],y[],twd[] - must be aligned on 8-byte boundary

------------------------------------------------------------------------------*/

int fft_cplx_inner_32x16( int32_t * restrict y,
                          int32_t * restrict x,
                    const int16_t *          twd,
                          int N );

#endif // __FFT_CPLX_COMMON_H
