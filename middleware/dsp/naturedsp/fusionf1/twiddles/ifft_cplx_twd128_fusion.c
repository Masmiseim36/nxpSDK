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

    twiddle factors for 32x16/16x16 real fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles.h"
#include "common.h"

/*
    Twiddle tables are received by sequential writing twiddle factors used in the original fft loops
    However, last radix2/radix4 stage is omitted because they are processed separately and combined 
    with bitreversing
    Higher halfword of each item is a cosine of a twiddle angle and lower halfword is a sine. All numbers are
    represented in Q15. Twiddle angle is always an integral number multiplied by 2*pi/N where N is FFT length. 
    For example, the sequence of twiddle angles is:
    16-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 -7]/16
    32-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 9 4 8 12 5 10 15 6 12 -14 7 14 -11 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 ]/32
    ....
*/
const int16_t MSC_ALIGNED ifft_twd128[16*12] GCC_ALIGNED = {
(int16_t)0x7fff, (int16_t)0x0000, (int16_t)0x7fff, (int16_t)0x0000, (int16_t)0x7fff, (int16_t)0x0000, (int16_t)0x7fd9, (int16_t)0x0648, (int16_t)0x7f62, (int16_t)0x0c8c, (int16_t)0x7e9d, (int16_t)0x12c8,
(int16_t)0x7f62, (int16_t)0x0c8c, (int16_t)0x7d8a, (int16_t)0x18f9, (int16_t)0x7a7d, (int16_t)0x2528, (int16_t)0x7e9d, (int16_t)0x12c8, (int16_t)0x7a7d, (int16_t)0x2528, (int16_t)0x73b6, (int16_t)0x36ba,
(int16_t)0x7d8a, (int16_t)0x18f9, (int16_t)0x7642, (int16_t)0x30fc, (int16_t)0x6a6e, (int16_t)0x471d, (int16_t)0x7c2a, (int16_t)0x1f1a, (int16_t)0x70e3, (int16_t)0x3c57, (int16_t)0x5ed7, (int16_t)0x55f6,
(int16_t)0x7a7d, (int16_t)0x2528, (int16_t)0x6a6e, (int16_t)0x471d, (int16_t)0x5134, (int16_t)0x62f2, (int16_t)0x7885, (int16_t)0x2b1f, (int16_t)0x62f2, (int16_t)0x5134, (int16_t)0x41ce, (int16_t)0x6dca,
(int16_t)0x7642, (int16_t)0x30fc, (int16_t)0x5a82, (int16_t)0x5a82, (int16_t)0x30fc, (int16_t)0x7642, (int16_t)0x73b6, (int16_t)0x36ba, (int16_t)0x5134, (int16_t)0x62f2, (int16_t)0x1f1a, (int16_t)0x7c2a,
(int16_t)0x70e3, (int16_t)0x3c57, (int16_t)0x471d, (int16_t)0x6a6e, (int16_t)0x0c8c, (int16_t)0x7f62, (int16_t)0x6dca, (int16_t)0x41ce, (int16_t)0x3c57, (int16_t)0x70e3, (int16_t)0xf9b8, (int16_t)0x7fd9,
(int16_t)0x6a6e, (int16_t)0x471d, (int16_t)0x30fc, (int16_t)0x7642, (int16_t)0xe707, (int16_t)0x7d8a, (int16_t)0x66d0, (int16_t)0x4c40, (int16_t)0x2528, (int16_t)0x7a7d, (int16_t)0xd4e1, (int16_t)0x7885,
(int16_t)0x62f2, (int16_t)0x5134, (int16_t)0x18f9, (int16_t)0x7d8a, (int16_t)0xc3a9, (int16_t)0x70e3, (int16_t)0x5ed7, (int16_t)0x55f6, (int16_t)0x0c8c, (int16_t)0x7f62, (int16_t)0xb3c0, (int16_t)0x66d0,
(int16_t)0x5a82, (int16_t)0x5a82, (int16_t)0x0000, (int16_t)0x7fff, (int16_t)0xa57e, (int16_t)0x5a82, (int16_t)0x55f6, (int16_t)0x5ed7, (int16_t)0xf374, (int16_t)0x7f62, (int16_t)0x9930, (int16_t)0x4c40,
(int16_t)0x5134, (int16_t)0x62f2, (int16_t)0xe707, (int16_t)0x7d8a, (int16_t)0x8f1d, (int16_t)0x3c57, (int16_t)0x4c40, (int16_t)0x66d0, (int16_t)0xdad8, (int16_t)0x7a7d, (int16_t)0x877b, (int16_t)0x2b1f,
(int16_t)0x471d, (int16_t)0x6a6e, (int16_t)0xcf04, (int16_t)0x7642, (int16_t)0x8276, (int16_t)0x18f9, (int16_t)0x41ce, (int16_t)0x6dca, (int16_t)0xc3a9, (int16_t)0x70e3, (int16_t)0x8027, (int16_t)0x0648,
(int16_t)0x3c57, (int16_t)0x70e3, (int16_t)0xb8e3, (int16_t)0x6a6e, (int16_t)0x809e, (int16_t)0xf374, (int16_t)0x36ba, (int16_t)0x73b6, (int16_t)0xaecc, (int16_t)0x62f2, (int16_t)0x83d6, (int16_t)0xe0e6,
(int16_t)0x30fc, (int16_t)0x7642, (int16_t)0xa57e, (int16_t)0x5a82, (int16_t)0x89be, (int16_t)0xcf04, (int16_t)0x2b1f, (int16_t)0x7885, (int16_t)0x9d0e, (int16_t)0x5134, (int16_t)0x9236, (int16_t)0xbe32,
(int16_t)0x2528, (int16_t)0x7a7d, (int16_t)0x9592, (int16_t)0x471d, (int16_t)0x9d0e, (int16_t)0xaecc, (int16_t)0x1f1a, (int16_t)0x7c2a, (int16_t)0x8f1d, (int16_t)0x3c57, (int16_t)0xaa0a, (int16_t)0xa129,
(int16_t)0x18f9, (int16_t)0x7d8a, (int16_t)0x89be, (int16_t)0x30fc, (int16_t)0xb8e3, (int16_t)0x9592, (int16_t)0x12c8, (int16_t)0x7e9d, (int16_t)0x8583, (int16_t)0x2528, (int16_t)0xc946, (int16_t)0x8c4a,
(int16_t)0x0c8c, (int16_t)0x7f62, (int16_t)0x8276, (int16_t)0x18f9, (int16_t)0xdad8, (int16_t)0x8583, (int16_t)0x0648, (int16_t)0x7fd9, (int16_t)0x809e, (int16_t)0x0c8c, (int16_t)0xed38, (int16_t)0x8163
};
static const cint32_ptr seqifft_twd128[]={(cint32_ptr)ifft_twd128,(cint32_ptr)ifft_twd32};
static const tFftDescr descr={  128,fft_inc128,(cint32_ptr)seqifft_twd128};
const fft_handle_t cifft16_128=(const void*)&descr;
const fft_handle_t rifft16_256=(const void*)&descr;
#if (!XCHAL_HAVE_FUSION_16BIT_BASEBAND)
const fft_handle_t cifft16x16_128=(const void*)&descr;
const fft_handle_t rifft16x16_256=(const void*)&descr;
#endif
