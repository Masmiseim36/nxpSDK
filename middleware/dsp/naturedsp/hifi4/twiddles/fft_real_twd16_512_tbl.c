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
#include "NatureDSP_Signal_fft.h"
#include "common.h"
#include "fft_x16_common.h"

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=512 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
    (int16_t)0x0000,(int16_t)0x7fff,
    (int16_t)0x0192,(int16_t)0x7ffe,
    (int16_t)0x0324,(int16_t)0x7ff6,
    (int16_t)0x04b6,(int16_t)0x7fea,
    (int16_t)0x0648,(int16_t)0x7fd9,
    (int16_t)0x07d9,(int16_t)0x7fc2,
    (int16_t)0x096b,(int16_t)0x7fa7,
    (int16_t)0x0afb,(int16_t)0x7f87,
    (int16_t)0x0c8c,(int16_t)0x7f62,
    (int16_t)0x0e1c,(int16_t)0x7f38,
    (int16_t)0x0fab,(int16_t)0x7f0a,
    (int16_t)0x113a,(int16_t)0x7ed6,
    (int16_t)0x12c8,(int16_t)0x7e9d,
    (int16_t)0x1455,(int16_t)0x7e60,
    (int16_t)0x15e2,(int16_t)0x7e1e,
    (int16_t)0x176e,(int16_t)0x7dd6,
    (int16_t)0x18f9,(int16_t)0x7d8a,
    (int16_t)0x1a83,(int16_t)0x7d3a,
    (int16_t)0x1c0c,(int16_t)0x7ce4,
    (int16_t)0x1d93,(int16_t)0x7c89,
    (int16_t)0x1f1a,(int16_t)0x7c2a,
    (int16_t)0x209f,(int16_t)0x7bc6,
    (int16_t)0x2224,(int16_t)0x7b5d,
    (int16_t)0x23a7,(int16_t)0x7aef,
    (int16_t)0x2528,(int16_t)0x7a7d,
    (int16_t)0x26a8,(int16_t)0x7a06,
    (int16_t)0x2827,(int16_t)0x798a,
    (int16_t)0x29a4,(int16_t)0x790a,
    (int16_t)0x2b1f,(int16_t)0x7885,
    (int16_t)0x2c99,(int16_t)0x77fb,
    (int16_t)0x2e11,(int16_t)0x776c,
    (int16_t)0x2f87,(int16_t)0x76d9,
    (int16_t)0x30fc,(int16_t)0x7642,
    (int16_t)0x326e,(int16_t)0x75a6,
    (int16_t)0x33df,(int16_t)0x7505,
    (int16_t)0x354e,(int16_t)0x7460,
    (int16_t)0x36ba,(int16_t)0x73b6,
    (int16_t)0x3825,(int16_t)0x7308,
    (int16_t)0x398d,(int16_t)0x7255,
    (int16_t)0x3af3,(int16_t)0x719e,
    (int16_t)0x3c57,(int16_t)0x70e3,
    (int16_t)0x3db8,(int16_t)0x7023,
    (int16_t)0x3f17,(int16_t)0x6f5f,
    (int16_t)0x4074,(int16_t)0x6e97,
    (int16_t)0x41ce,(int16_t)0x6dca,
    (int16_t)0x4326,(int16_t)0x6cf9,
    (int16_t)0x447b,(int16_t)0x6c24,
    (int16_t)0x45cd,(int16_t)0x6b4b,
    (int16_t)0x471d,(int16_t)0x6a6e,
    (int16_t)0x486a,(int16_t)0x698c,
    (int16_t)0x49b4,(int16_t)0x68a7,
    (int16_t)0x4afb,(int16_t)0x67bd,
    (int16_t)0x4c40,(int16_t)0x66d0,
    (int16_t)0x4d81,(int16_t)0x65de,
    (int16_t)0x4ec0,(int16_t)0x64e9,
    (int16_t)0x4ffb,(int16_t)0x63ef,
    (int16_t)0x5134,(int16_t)0x62f2,
    (int16_t)0x5269,(int16_t)0x61f1,
    (int16_t)0x539b,(int16_t)0x60ec,
    (int16_t)0x54ca,(int16_t)0x5fe4,
    (int16_t)0x55f6,(int16_t)0x5ed7,
    (int16_t)0x571e,(int16_t)0x5dc8,
    (int16_t)0x5843,(int16_t)0x5cb4,
    (int16_t)0x5964,(int16_t)0x5b9d,
    (int16_t)0x5a82,(int16_t)0x5a82,
    (int16_t)0x5b9d,(int16_t)0x5964,
    (int16_t)0x5cb4,(int16_t)0x5843,
    (int16_t)0x5dc8,(int16_t)0x571e,
    (int16_t)0x5ed7,(int16_t)0x55f6,
    (int16_t)0x5fe4,(int16_t)0x54ca,
    (int16_t)0x60ec,(int16_t)0x539b,
    (int16_t)0x61f1,(int16_t)0x5269,
    (int16_t)0x62f2,(int16_t)0x5134,
    (int16_t)0x63ef,(int16_t)0x4ffb,
    (int16_t)0x64e9,(int16_t)0x4ec0,
    (int16_t)0x65de,(int16_t)0x4d81,
    (int16_t)0x66d0,(int16_t)0x4c40,
    (int16_t)0x67bd,(int16_t)0x4afb,
    (int16_t)0x68a7,(int16_t)0x49b4,
    (int16_t)0x698c,(int16_t)0x486a,
    (int16_t)0x6a6e,(int16_t)0x471d,
    (int16_t)0x6b4b,(int16_t)0x45cd,
    (int16_t)0x6c24,(int16_t)0x447b,
    (int16_t)0x6cf9,(int16_t)0x4326,
    (int16_t)0x6dca,(int16_t)0x41ce,
    (int16_t)0x6e97,(int16_t)0x4074,
    (int16_t)0x6f5f,(int16_t)0x3f17,
    (int16_t)0x7023,(int16_t)0x3db8,
    (int16_t)0x70e3,(int16_t)0x3c57,
    (int16_t)0x719e,(int16_t)0x3af3,
    (int16_t)0x7255,(int16_t)0x398d,
    (int16_t)0x7308,(int16_t)0x3825,
    (int16_t)0x73b6,(int16_t)0x36ba,
    (int16_t)0x7460,(int16_t)0x354e,
    (int16_t)0x7505,(int16_t)0x33df,
    (int16_t)0x75a6,(int16_t)0x326e,
    (int16_t)0x7642,(int16_t)0x30fc,
    (int16_t)0x76d9,(int16_t)0x2f87,
    (int16_t)0x776c,(int16_t)0x2e11,
    (int16_t)0x77fb,(int16_t)0x2c99,
    (int16_t)0x7885,(int16_t)0x2b1f,
    (int16_t)0x790a,(int16_t)0x29a4,
    (int16_t)0x798a,(int16_t)0x2827,
    (int16_t)0x7a06,(int16_t)0x26a8,
    (int16_t)0x7a7d,(int16_t)0x2528,
    (int16_t)0x7aef,(int16_t)0x23a7,
    (int16_t)0x7b5d,(int16_t)0x2224,
    (int16_t)0x7bc6,(int16_t)0x209f,
    (int16_t)0x7c2a,(int16_t)0x1f1a,
    (int16_t)0x7c89,(int16_t)0x1d93,
    (int16_t)0x7ce4,(int16_t)0x1c0c,
    (int16_t)0x7d3a,(int16_t)0x1a83,
    (int16_t)0x7d8a,(int16_t)0x18f9,
    (int16_t)0x7dd6,(int16_t)0x176e,
    (int16_t)0x7e1e,(int16_t)0x15e2,
    (int16_t)0x7e60,(int16_t)0x1455,
    (int16_t)0x7e9d,(int16_t)0x12c8,
    (int16_t)0x7ed6,(int16_t)0x113a,
    (int16_t)0x7f0a,(int16_t)0x0fab,
    (int16_t)0x7f38,(int16_t)0x0e1c,
    (int16_t)0x7f62,(int16_t)0x0c8c,
    (int16_t)0x7f87,(int16_t)0x0afb,
    (int16_t)0x7fa7,(int16_t)0x096b,
    (int16_t)0x7fc2,(int16_t)0x07d9,
    (int16_t)0x7fd9,(int16_t)0x0648,
    (int16_t)0x7fea,(int16_t)0x04b6,
    (int16_t)0x7ff6,(int16_t)0x0324,
    (int16_t)0x7ffe,(int16_t)0x0192
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_x16_descr256,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_x16_descr256,
    __fft_real16_tw
};
const fft_handle_t rfft16_512 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rifft16_512 = (const fft_handle_t)&__rifft_descr;
