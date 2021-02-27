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

    twiddle factors for 24x24 real fwd/rev FFT transforms
*/

#include "NatureDSP_Signal.h"
#include "fft_cplx_twiddles_24x24.h"
#include "common.h"

/*
    Twiddle tables are received by sequential writing twiddle factors used in the original fft loops
    However, last radix2/radix4 stage is omitted because they are processed separately and combined 
    with bitreversing
    Higher word of each item is a cosine of a twiddle angle and lower word is a sine. All numbers are
    represented in Q31 with cleared 8 LSBs. Twiddle angle is always an integral number multiplied by 
    2*pi/N where N is FFT length. 
    For example, the sequence of twiddle angles is:
    16-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 -7]/16
    32-pts transform 
    [0 0 0 1 2 3 2 4 6 3 6 9 4 8 12 5 10 15 6 12 -14 7 14 -11 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 0 0 0 4 8 12 ]/32
    ....
*/
const int32_t MSC_ALIGNED fft24x24_twd128[12*16] GCC_ALIGNED = {
(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fffff00,(int32_t)0x00000000,(int32_t)0x7fd88700,(int32_t)0xF9B82700,(int32_t)0x7f623600,(int32_t)0xF3742D00,(int32_t)0x7e9d5500,(int32_t)0xED37F000,
(int32_t)0x7f623600,(int32_t)0xF3742D00,(int32_t)0x7d8a5f00,(int32_t)0xE7074800,(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x7e9d5500,(int32_t)0xED37F000,(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x73b5eb00,(int32_t)0xC945E000,
(int32_t)0x7d8a5f00,(int32_t)0xE7074800,(int32_t)0x7641af00,(int32_t)0xCF043B00,(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x7c29fb00,(int32_t)0xE0E60700,(int32_t)0x70e2cb00,(int32_t)0xC3A94600,(int32_t)0x5ed77c00,(int32_t)0xAA0A5C00,
(int32_t)0x7a7d0500,(int32_t)0xDAD7F400,(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0x78848400,(int32_t)0xD4E0CC00,(int32_t)0x62f20100,(int32_t)0xAECC3400,(int32_t)0x41ce1e00,(int32_t)0x9235F300,
(int32_t)0x7641af00,(int32_t)0xCF043B00,(int32_t)0x5a827900,(int32_t)0xA57D8700,(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0x73b5eb00,(int32_t)0xC945E000,(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0x1f19f900,(int32_t)0x83D60500,
(int32_t)0x70e2cb00,(int32_t)0xC3A94600,(int32_t)0x471cec00,(int32_t)0x95926800,(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0x6dca0d00,(int32_t)0xBE31E200,(int32_t)0x3c56ba00,(int32_t)0x8F1D3500,(int32_t)0xf9b82600,(int32_t)0x80277900,
(int32_t)0x6a6d9800,(int32_t)0xB8E31400,(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0xe7074700,(int32_t)0x8275A100,(int32_t)0x66cf8100,(int32_t)0xB3C02100,(int32_t)0x25280c00,(int32_t)0x8582FB00,(int32_t)0xd4e0cb00,(int32_t)0x877B7C00,
(int32_t)0x62f20100,(int32_t)0xAECC3400,(int32_t)0x18f8b800,(int32_t)0x8275A100,(int32_t)0xc3a94500,(int32_t)0x8F1D3500,(int32_t)0x5ed77c00,(int32_t)0xAA0A5C00,(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0xb3c02000,(int32_t)0x99307F00,
(int32_t)0x5a827900,(int32_t)0xA57D8700,(int32_t)0x00000000,(int32_t)0x80000000,(int32_t)0xa57d8600,(int32_t)0xA57D8700,(int32_t)0x55f5a400,(int32_t)0xA1288400,(int32_t)0xf3742c00,(int32_t)0x809DCA00,(int32_t)0x99307e00,(int32_t)0xB3C02100,
(int32_t)0x5133cc00,(int32_t)0x9D0DFF00,(int32_t)0xe7074700,(int32_t)0x8275A100,(int32_t)0x8f1d3400,(int32_t)0xC3A94600,(int32_t)0x4c3fdf00,(int32_t)0x99307F00,(int32_t)0xdad7f300,(int32_t)0x8582FB00,(int32_t)0x877b7b00,(int32_t)0xD4E0CC00,
(int32_t)0x471cec00,(int32_t)0x95926800,(int32_t)0xcf043a00,(int32_t)0x89BE5100,(int32_t)0x8275a000,(int32_t)0xE7074800,(int32_t)0x41ce1e00,(int32_t)0x9235F300,(int32_t)0xc3a94500,(int32_t)0x8F1D3500,(int32_t)0x80277800,(int32_t)0xF9B82700,
(int32_t)0x3c56ba00,(int32_t)0x8F1D3500,(int32_t)0xb8e31300,(int32_t)0x95926800,(int32_t)0x809dc900,(int32_t)0x0C8BD400,(int32_t)0x36ba2000,(int32_t)0x8C4A1500,(int32_t)0xaecc3300,(int32_t)0x9D0DFF00,(int32_t)0x83d60400,(int32_t)0x1F19FA00,
(int32_t)0x30fbc500,(int32_t)0x89BE5100,(int32_t)0xa57d8600,(int32_t)0xA57D8700,(int32_t)0x89be5000,(int32_t)0x30FBC600,(int32_t)0x2b1f3400,(int32_t)0x877B7C00,(int32_t)0x9d0dfe00,(int32_t)0xAECC3400,(int32_t)0x9235f200,(int32_t)0x41CE1F00,
(int32_t)0x25280c00,(int32_t)0x8582FB00,(int32_t)0x95926700,(int32_t)0xB8E31400,(int32_t)0x9d0dfe00,(int32_t)0x5133CD00,(int32_t)0x1f19f900,(int32_t)0x83D60500,(int32_t)0x8f1d3400,(int32_t)0xC3A94600,(int32_t)0xaa0a5b00,(int32_t)0x5ED77D00,
(int32_t)0x18f8b800,(int32_t)0x8275A100,(int32_t)0x89be5000,(int32_t)0xCF043B00,(int32_t)0xb8e31300,(int32_t)0x6A6D9900,(int32_t)0x12c81000,(int32_t)0x8162AB00,(int32_t)0x8582fa00,(int32_t)0xDAD7F400,(int32_t)0xc945df00,(int32_t)0x73B5EC00,
(int32_t)0x0c8bd300,(int32_t)0x809DCA00,(int32_t)0x8275a000,(int32_t)0xE7074800,(int32_t)0xdad7f300,(int32_t)0x7A7D0600,(int32_t)0x0647d900,(int32_t)0x80277900,(int32_t)0x809dc900,(int32_t)0xF3742D00,(int32_t)0xed37ef00,(int32_t)0x7E9D5600
};
static const tFftDescr descr={  128,fft_inc128,(cint32_ptr)fft24x24_twd128};
const fft_handle_t cfft24_128=(const void*)&descr;
const fft_handle_t rfft24_256=(const void*)&descr;
