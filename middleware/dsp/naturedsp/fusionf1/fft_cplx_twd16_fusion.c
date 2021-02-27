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
const int16_t MSC_ALIGNED fft_twd16[2*12] GCC_ALIGNED = {
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7642,(int16_t)0xCF04,(int16_t)0x5a82,(int16_t)0xA57E,(int16_t)0x30fc,(int16_t)0x89BE,
(int16_t)0x5a82,(int16_t)0xA57E,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xA57E,(int16_t)0x30fc,(int16_t)0x89BE,(int16_t)0xa57e,(int16_t)0xA57E,(int16_t)0x89be,(int16_t)0x30FC,
};

static const cint32_ptr seqfft_twd16[]={(cint32_ptr)fft_twd16};
static const tFftDescr descr={  16,NULL,(cint32_ptr)seqfft_twd16};
const fft_handle_t cfft16_16=(const void*)&descr;
const fft_handle_t rfft16_32=(const void*)&descr;
#if (!XCHAL_HAVE_FUSION_16BIT_BASEBAND)
const fft_handle_t cfft16x16_16=(const void*)&descr;
const fft_handle_t rfft16x16_32=(const void*)&descr;
#endif
