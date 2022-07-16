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
const int16_t MSC_ALIGNED fft_twd512[64*12] GCC_ALIGNED = {
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7ffe,(int16_t)0xFE6E,(int16_t)0x7ff6,(int16_t)0xFCDC,(int16_t)0x7fea,(int16_t)0xFB4A,
(int16_t)0x7ff6,(int16_t)0xFCDC,(int16_t)0x7fd9,(int16_t)0xF9B8,(int16_t)0x7fa7,(int16_t)0xF695,(int16_t)0x7fea,(int16_t)0xFB4A,(int16_t)0x7fa7,(int16_t)0xF695,(int16_t)0x7f38,(int16_t)0xF1E4,
(int16_t)0x7fd9,(int16_t)0xF9B8,(int16_t)0x7f62,(int16_t)0xF374,(int16_t)0x7e9d,(int16_t)0xED38,(int16_t)0x7fc2,(int16_t)0xF827,(int16_t)0x7f0a,(int16_t)0xF055,(int16_t)0x7dd6,(int16_t)0xE892,
(int16_t)0x7fa7,(int16_t)0xF695,(int16_t)0x7e9d,(int16_t)0xED38,(int16_t)0x7ce4,(int16_t)0xE3F4,(int16_t)0x7f87,(int16_t)0xF505,(int16_t)0x7e1e,(int16_t)0xEA1E,(int16_t)0x7bc6,(int16_t)0xDF61,
(int16_t)0x7f62,(int16_t)0xF374,(int16_t)0x7d8a,(int16_t)0xE707,(int16_t)0x7a7d,(int16_t)0xDAD8,(int16_t)0x7f38,(int16_t)0xF1E4,(int16_t)0x7ce4,(int16_t)0xE3F4,(int16_t)0x790a,(int16_t)0xD65C,
(int16_t)0x7f0a,(int16_t)0xF055,(int16_t)0x7c2a,(int16_t)0xE0E6,(int16_t)0x776c,(int16_t)0xD1EF,(int16_t)0x7ed6,(int16_t)0xEEC6,(int16_t)0x7b5d,(int16_t)0xDDDC,(int16_t)0x75a6,(int16_t)0xCD92,
(int16_t)0x7e9d,(int16_t)0xED38,(int16_t)0x7a7d,(int16_t)0xDAD8,(int16_t)0x73b6,(int16_t)0xC946,(int16_t)0x7e60,(int16_t)0xEBAB,(int16_t)0x798a,(int16_t)0xD7D9,(int16_t)0x719e,(int16_t)0xC50D,
(int16_t)0x7e1e,(int16_t)0xEA1E,(int16_t)0x7885,(int16_t)0xD4E1,(int16_t)0x6f5f,(int16_t)0xC0E9,(int16_t)0x7dd6,(int16_t)0xE892,(int16_t)0x776c,(int16_t)0xD1EF,(int16_t)0x6cf9,(int16_t)0xBCDA,
(int16_t)0x7d8a,(int16_t)0xE707,(int16_t)0x7642,(int16_t)0xCF04,(int16_t)0x6a6e,(int16_t)0xB8E3,(int16_t)0x7d3a,(int16_t)0xE57D,(int16_t)0x7505,(int16_t)0xCC21,(int16_t)0x67bd,(int16_t)0xB505,
(int16_t)0x7ce4,(int16_t)0xE3F4,(int16_t)0x73b6,(int16_t)0xC946,(int16_t)0x64e9,(int16_t)0xB140,(int16_t)0x7c89,(int16_t)0xE26D,(int16_t)0x7255,(int16_t)0xC673,(int16_t)0x61f1,(int16_t)0xAD97,
(int16_t)0x7c2a,(int16_t)0xE0E6,(int16_t)0x70e3,(int16_t)0xC3A9,(int16_t)0x5ed7,(int16_t)0xAA0A,(int16_t)0x7bc6,(int16_t)0xDF61,(int16_t)0x6f5f,(int16_t)0xC0E9,(int16_t)0x5b9d,(int16_t)0xA69C,
(int16_t)0x7b5d,(int16_t)0xDDDC,(int16_t)0x6dca,(int16_t)0xBE32,(int16_t)0x5843,(int16_t)0xA34C,(int16_t)0x7aef,(int16_t)0xDC59,(int16_t)0x6c24,(int16_t)0xBB85,(int16_t)0x54ca,(int16_t)0xA01C,
(int16_t)0x7a7d,(int16_t)0xDAD8,(int16_t)0x6a6e,(int16_t)0xB8E3,(int16_t)0x5134,(int16_t)0x9D0E,(int16_t)0x7a06,(int16_t)0xD958,(int16_t)0x68a7,(int16_t)0xB64C,(int16_t)0x4d81,(int16_t)0x9A22,
(int16_t)0x798a,(int16_t)0xD7D9,(int16_t)0x66d0,(int16_t)0xB3C0,(int16_t)0x49b4,(int16_t)0x9759,(int16_t)0x790a,(int16_t)0xD65C,(int16_t)0x64e9,(int16_t)0xB140,(int16_t)0x45cd,(int16_t)0x94B5,
(int16_t)0x7885,(int16_t)0xD4E1,(int16_t)0x62f2,(int16_t)0xAECC,(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0x77fb,(int16_t)0xD367,(int16_t)0x60ec,(int16_t)0xAC65,(int16_t)0x3db8,(int16_t)0x8FDD,
(int16_t)0x776c,(int16_t)0xD1EF,(int16_t)0x5ed7,(int16_t)0xAA0A,(int16_t)0x398d,(int16_t)0x8DAB,(int16_t)0x76d9,(int16_t)0xD079,(int16_t)0x5cb4,(int16_t)0xA7BD,(int16_t)0x354e,(int16_t)0x8BA0,
(int16_t)0x7642,(int16_t)0xCF04,(int16_t)0x5a82,(int16_t)0xA57E,(int16_t)0x30fc,(int16_t)0x89BE,(int16_t)0x75a6,(int16_t)0xCD92,(int16_t)0x5843,(int16_t)0xA34C,(int16_t)0x2c99,(int16_t)0x8805,
(int16_t)0x7505,(int16_t)0xCC21,(int16_t)0x55f6,(int16_t)0xA129,(int16_t)0x2827,(int16_t)0x8676,(int16_t)0x7460,(int16_t)0xCAB2,(int16_t)0x539b,(int16_t)0x9F14,(int16_t)0x23a7,(int16_t)0x8511,
(int16_t)0x73b6,(int16_t)0xC946,(int16_t)0x5134,(int16_t)0x9D0E,(int16_t)0x1f1a,(int16_t)0x83D6,(int16_t)0x7308,(int16_t)0xC7DB,(int16_t)0x4ec0,(int16_t)0x9B17,(int16_t)0x1a83,(int16_t)0x82C6,
(int16_t)0x7255,(int16_t)0xC673,(int16_t)0x4c40,(int16_t)0x9930,(int16_t)0x15e2,(int16_t)0x81E2,(int16_t)0x719e,(int16_t)0xC50D,(int16_t)0x49b4,(int16_t)0x9759,(int16_t)0x113a,(int16_t)0x812A,
(int16_t)0x70e3,(int16_t)0xC3A9,(int16_t)0x471d,(int16_t)0x9592,(int16_t)0x0c8c,(int16_t)0x809E,(int16_t)0x7023,(int16_t)0xC248,(int16_t)0x447b,(int16_t)0x93DC,(int16_t)0x07d9,(int16_t)0x803E,
(int16_t)0x6f5f,(int16_t)0xC0E9,(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0x0324,(int16_t)0x800A,(int16_t)0x6e97,(int16_t)0xBF8C,(int16_t)0x3f17,(int16_t)0x90A1,(int16_t)0xfe6e,(int16_t)0x8002,
(int16_t)0x6dca,(int16_t)0xBE32,(int16_t)0x3c57,(int16_t)0x8F1D,(int16_t)0xf9b8,(int16_t)0x8027,(int16_t)0x6cf9,(int16_t)0xBCDA,(int16_t)0x398d,(int16_t)0x8DAB,(int16_t)0xf505,(int16_t)0x8079,
(int16_t)0x6c24,(int16_t)0xBB85,(int16_t)0x36ba,(int16_t)0x8C4A,(int16_t)0xf055,(int16_t)0x80F6,(int16_t)0x6b4b,(int16_t)0xBA33,(int16_t)0x33df,(int16_t)0x8AFB,(int16_t)0xebab,(int16_t)0x81A0,
(int16_t)0x6a6e,(int16_t)0xB8E3,(int16_t)0x30fc,(int16_t)0x89BE,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x698c,(int16_t)0xB796,(int16_t)0x2e11,(int16_t)0x8894,(int16_t)0xe26d,(int16_t)0x8377,
(int16_t)0x68a7,(int16_t)0xB64C,(int16_t)0x2b1f,(int16_t)0x877B,(int16_t)0xdddc,(int16_t)0x84A3,(int16_t)0x67bd,(int16_t)0xB505,(int16_t)0x2827,(int16_t)0x8676,(int16_t)0xd958,(int16_t)0x85FA,
(int16_t)0x66d0,(int16_t)0xB3C0,(int16_t)0x2528,(int16_t)0x8583,(int16_t)0xd4e1,(int16_t)0x877B,(int16_t)0x65de,(int16_t)0xB27F,(int16_t)0x2224,(int16_t)0x84A3,(int16_t)0xd079,(int16_t)0x8927,
(int16_t)0x64e9,(int16_t)0xB140,(int16_t)0x1f1a,(int16_t)0x83D6,(int16_t)0xcc21,(int16_t)0x8AFB,(int16_t)0x63ef,(int16_t)0xB005,(int16_t)0x1c0c,(int16_t)0x831C,(int16_t)0xc7db,(int16_t)0x8CF8,
(int16_t)0x62f2,(int16_t)0xAECC,(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0xc3a9,(int16_t)0x8F1D,(int16_t)0x61f1,(int16_t)0xAD97,(int16_t)0x15e2,(int16_t)0x81E2,(int16_t)0xbf8c,(int16_t)0x9169,
(int16_t)0x60ec,(int16_t)0xAC65,(int16_t)0x12c8,(int16_t)0x8163,(int16_t)0xbb85,(int16_t)0x93DC,(int16_t)0x5fe4,(int16_t)0xAB36,(int16_t)0x0fab,(int16_t)0x80F6,(int16_t)0xb796,(int16_t)0x9674,
(int16_t)0x5ed7,(int16_t)0xAA0A,(int16_t)0x0c8c,(int16_t)0x809E,(int16_t)0xb3c0,(int16_t)0x9930,(int16_t)0x5dc8,(int16_t)0xA8E2,(int16_t)0x096b,(int16_t)0x8059,(int16_t)0xb005,(int16_t)0x9C11,
(int16_t)0x5cb4,(int16_t)0xA7BD,(int16_t)0x0648,(int16_t)0x8027,(int16_t)0xac65,(int16_t)0x9F14,(int16_t)0x5b9d,(int16_t)0xA69C,(int16_t)0x0324,(int16_t)0x800A,(int16_t)0xa8e2,(int16_t)0xA238,
(int16_t)0x5a82,(int16_t)0xA57E,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xA57E,(int16_t)0x5964,(int16_t)0xA463,(int16_t)0xfcdc,(int16_t)0x800A,(int16_t)0xa238,(int16_t)0xA8E2,
(int16_t)0x5843,(int16_t)0xA34C,(int16_t)0xf9b8,(int16_t)0x8027,(int16_t)0x9f14,(int16_t)0xAC65,(int16_t)0x571e,(int16_t)0xA238,(int16_t)0xf695,(int16_t)0x8059,(int16_t)0x9c11,(int16_t)0xB005,
(int16_t)0x55f6,(int16_t)0xA129,(int16_t)0xf374,(int16_t)0x809E,(int16_t)0x9930,(int16_t)0xB3C0,(int16_t)0x54ca,(int16_t)0xA01C,(int16_t)0xf055,(int16_t)0x80F6,(int16_t)0x9674,(int16_t)0xB796,
(int16_t)0x539b,(int16_t)0x9F14,(int16_t)0xed38,(int16_t)0x8163,(int16_t)0x93dc,(int16_t)0xBB85,(int16_t)0x5269,(int16_t)0x9E0F,(int16_t)0xea1e,(int16_t)0x81E2,(int16_t)0x9169,(int16_t)0xBF8C,
(int16_t)0x5134,(int16_t)0x9D0E,(int16_t)0xe707,(int16_t)0x8276,(int16_t)0x8f1d,(int16_t)0xC3A9,(int16_t)0x4ffb,(int16_t)0x9C11,(int16_t)0xe3f4,(int16_t)0x831C,(int16_t)0x8cf8,(int16_t)0xC7DB,
(int16_t)0x4ec0,(int16_t)0x9B17,(int16_t)0xe0e6,(int16_t)0x83D6,(int16_t)0x8afb,(int16_t)0xCC21,(int16_t)0x4d81,(int16_t)0x9A22,(int16_t)0xdddc,(int16_t)0x84A3,(int16_t)0x8927,(int16_t)0xD079,
(int16_t)0x4c40,(int16_t)0x9930,(int16_t)0xdad8,(int16_t)0x8583,(int16_t)0x877b,(int16_t)0xD4E1,(int16_t)0x4afb,(int16_t)0x9843,(int16_t)0xd7d9,(int16_t)0x8676,(int16_t)0x85fa,(int16_t)0xD958,
(int16_t)0x49b4,(int16_t)0x9759,(int16_t)0xd4e1,(int16_t)0x877B,(int16_t)0x84a3,(int16_t)0xDDDC,(int16_t)0x486a,(int16_t)0x9674,(int16_t)0xd1ef,(int16_t)0x8894,(int16_t)0x8377,(int16_t)0xE26D,
(int16_t)0x471d,(int16_t)0x9592,(int16_t)0xcf04,(int16_t)0x89BE,(int16_t)0x8276,(int16_t)0xE707,(int16_t)0x45cd,(int16_t)0x94B5,(int16_t)0xcc21,(int16_t)0x8AFB,(int16_t)0x81a0,(int16_t)0xEBAB,
(int16_t)0x447b,(int16_t)0x93DC,(int16_t)0xc946,(int16_t)0x8C4A,(int16_t)0x80f6,(int16_t)0xF055,(int16_t)0x4326,(int16_t)0x9307,(int16_t)0xc673,(int16_t)0x8DAB,(int16_t)0x8079,(int16_t)0xF505,
(int16_t)0x41ce,(int16_t)0x9236,(int16_t)0xc3a9,(int16_t)0x8F1D,(int16_t)0x8027,(int16_t)0xF9B8,(int16_t)0x4074,(int16_t)0x9169,(int16_t)0xc0e9,(int16_t)0x90A1,(int16_t)0x8002,(int16_t)0xFE6E,
(int16_t)0x3f17,(int16_t)0x90A1,(int16_t)0xbe32,(int16_t)0x9236,(int16_t)0x800a,(int16_t)0x0324,(int16_t)0x3db8,(int16_t)0x8FDD,(int16_t)0xbb85,(int16_t)0x93DC,(int16_t)0x803e,(int16_t)0x07D9,
(int16_t)0x3c57,(int16_t)0x8F1D,(int16_t)0xb8e3,(int16_t)0x9592,(int16_t)0x809e,(int16_t)0x0C8C,(int16_t)0x3af3,(int16_t)0x8E62,(int16_t)0xb64c,(int16_t)0x9759,(int16_t)0x812a,(int16_t)0x113A,
(int16_t)0x398d,(int16_t)0x8DAB,(int16_t)0xb3c0,(int16_t)0x9930,(int16_t)0x81e2,(int16_t)0x15E2,(int16_t)0x3825,(int16_t)0x8CF8,(int16_t)0xb140,(int16_t)0x9B17,(int16_t)0x82c6,(int16_t)0x1A83,
(int16_t)0x36ba,(int16_t)0x8C4A,(int16_t)0xaecc,(int16_t)0x9D0E,(int16_t)0x83d6,(int16_t)0x1F1A,(int16_t)0x354e,(int16_t)0x8BA0,(int16_t)0xac65,(int16_t)0x9F14,(int16_t)0x8511,(int16_t)0x23A7,
(int16_t)0x33df,(int16_t)0x8AFB,(int16_t)0xaa0a,(int16_t)0xA129,(int16_t)0x8676,(int16_t)0x2827,(int16_t)0x326e,(int16_t)0x8A5A,(int16_t)0xa7bd,(int16_t)0xA34C,(int16_t)0x8805,(int16_t)0x2C99,
(int16_t)0x30fc,(int16_t)0x89BE,(int16_t)0xa57e,(int16_t)0xA57E,(int16_t)0x89be,(int16_t)0x30FC,(int16_t)0x2f87,(int16_t)0x8927,(int16_t)0xa34c,(int16_t)0xA7BD,(int16_t)0x8ba0,(int16_t)0x354E,
(int16_t)0x2e11,(int16_t)0x8894,(int16_t)0xa129,(int16_t)0xAA0A,(int16_t)0x8dab,(int16_t)0x398D,(int16_t)0x2c99,(int16_t)0x8805,(int16_t)0x9f14,(int16_t)0xAC65,(int16_t)0x8fdd,(int16_t)0x3DB8,
(int16_t)0x2b1f,(int16_t)0x877B,(int16_t)0x9d0e,(int16_t)0xAECC,(int16_t)0x9236,(int16_t)0x41CE,(int16_t)0x29a4,(int16_t)0x86F6,(int16_t)0x9b17,(int16_t)0xB140,(int16_t)0x94b5,(int16_t)0x45CD,
(int16_t)0x2827,(int16_t)0x8676,(int16_t)0x9930,(int16_t)0xB3C0,(int16_t)0x9759,(int16_t)0x49B4,(int16_t)0x26a8,(int16_t)0x85FA,(int16_t)0x9759,(int16_t)0xB64C,(int16_t)0x9a22,(int16_t)0x4D81,
(int16_t)0x2528,(int16_t)0x8583,(int16_t)0x9592,(int16_t)0xB8E3,(int16_t)0x9d0e,(int16_t)0x5134,(int16_t)0x23a7,(int16_t)0x8511,(int16_t)0x93dc,(int16_t)0xBB85,(int16_t)0xa01c,(int16_t)0x54CA,
(int16_t)0x2224,(int16_t)0x84A3,(int16_t)0x9236,(int16_t)0xBE32,(int16_t)0xa34c,(int16_t)0x5843,(int16_t)0x209f,(int16_t)0x843A,(int16_t)0x90a1,(int16_t)0xC0E9,(int16_t)0xa69c,(int16_t)0x5B9D,
(int16_t)0x1f1a,(int16_t)0x83D6,(int16_t)0x8f1d,(int16_t)0xC3A9,(int16_t)0xaa0a,(int16_t)0x5ED7,(int16_t)0x1d93,(int16_t)0x8377,(int16_t)0x8dab,(int16_t)0xC673,(int16_t)0xad97,(int16_t)0x61F1,
(int16_t)0x1c0c,(int16_t)0x831C,(int16_t)0x8c4a,(int16_t)0xC946,(int16_t)0xb140,(int16_t)0x64E9,(int16_t)0x1a83,(int16_t)0x82C6,(int16_t)0x8afb,(int16_t)0xCC21,(int16_t)0xb505,(int16_t)0x67BD,
(int16_t)0x18f9,(int16_t)0x8276,(int16_t)0x89be,(int16_t)0xCF04,(int16_t)0xb8e3,(int16_t)0x6A6E,(int16_t)0x176e,(int16_t)0x822A,(int16_t)0x8894,(int16_t)0xD1EF,(int16_t)0xbcda,(int16_t)0x6CF9,
(int16_t)0x15e2,(int16_t)0x81E2,(int16_t)0x877b,(int16_t)0xD4E1,(int16_t)0xc0e9,(int16_t)0x6F5F,(int16_t)0x1455,(int16_t)0x81A0,(int16_t)0x8676,(int16_t)0xD7D9,(int16_t)0xc50d,(int16_t)0x719E,
(int16_t)0x12c8,(int16_t)0x8163,(int16_t)0x8583,(int16_t)0xDAD8,(int16_t)0xc946,(int16_t)0x73B6,(int16_t)0x113a,(int16_t)0x812A,(int16_t)0x84a3,(int16_t)0xDDDC,(int16_t)0xcd92,(int16_t)0x75A6,
(int16_t)0x0fab,(int16_t)0x80F6,(int16_t)0x83d6,(int16_t)0xE0E6,(int16_t)0xd1ef,(int16_t)0x776C,(int16_t)0x0e1c,(int16_t)0x80C8,(int16_t)0x831c,(int16_t)0xE3F4,(int16_t)0xd65c,(int16_t)0x790A,
(int16_t)0x0c8c,(int16_t)0x809E,(int16_t)0x8276,(int16_t)0xE707,(int16_t)0xdad8,(int16_t)0x7A7D,(int16_t)0x0afb,(int16_t)0x8079,(int16_t)0x81e2,(int16_t)0xEA1E,(int16_t)0xdf61,(int16_t)0x7BC6,
(int16_t)0x096b,(int16_t)0x8059,(int16_t)0x8163,(int16_t)0xED38,(int16_t)0xe3f4,(int16_t)0x7CE4,(int16_t)0x07d9,(int16_t)0x803E,(int16_t)0x80f6,(int16_t)0xF055,(int16_t)0xe892,(int16_t)0x7DD6,
(int16_t)0x0648,(int16_t)0x8027,(int16_t)0x809e,(int16_t)0xF374,(int16_t)0xed38,(int16_t)0x7E9D,(int16_t)0x04b6,(int16_t)0x8016,(int16_t)0x8059,(int16_t)0xF695,(int16_t)0xf1e4,(int16_t)0x7F38,
(int16_t)0x0324,(int16_t)0x800A,(int16_t)0x8027,(int16_t)0xF9B8,(int16_t)0xf695,(int16_t)0x7FA7,(int16_t)0x0192,(int16_t)0x8002,(int16_t)0x800a,(int16_t)0xFCDC,(int16_t)0xfb4a,(int16_t)0x7FEA
};
static const cint32_ptr seqfft_twd512[]={(cint32_ptr)fft_twd512,(cint32_ptr)fft_twd128,(cint32_ptr)fft_twd32};
static const tFftDescr descr={  512,fft_inc512,(cint32_ptr)seqfft_twd512};
const fft_handle_t cfft16_512=(const void*)&descr;
const fft_handle_t rfft16_1024=(const void*)&descr;
#if (!XCHAL_HAVE_FUSION_16BIT_BASEBAND)
const fft_handle_t cfft16x16_512=(const void*)&descr;
const fft_handle_t rfft16x16_1024=(const void*)&descr;
#endif
