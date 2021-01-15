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

/* Twiddles tables for fft_cplx16x16, ifft_cplx16x16, N=80 */
#define N 80

/****************** stage 1 radix 5 ******************/
ALIGN(8) static const int16_t __fft16_tw1[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7f9b,(int16_t)0xf5f5,(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x79bc,(int16_t)0xd872,
(int16_t)0x7e6d,(int16_t)0xebfa,(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x678e,(int16_t)0xb4c3,
(int16_t)0x7c77,(int16_t)0xe21e,(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x4b3d,(int16_t)0x9872,
(int16_t)0x79bc,(int16_t)0xd872,(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x278e,(int16_t)0x8644,
(int16_t)0x7642,(int16_t)0xcf04,(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0x720d,(int16_t)0xc5e4,(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xd872,(int16_t)0x8644,
(int16_t)0x6d23,(int16_t)0xbd1f,(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xf5f5,(int16_t)0x8065,(int16_t)0xb4c3,(int16_t)0x9872,
(int16_t)0x678e,(int16_t)0xb4c3,(int16_t)0x278e,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x9872,(int16_t)0xb4c3,
(int16_t)0x6155,(int16_t)0xacdf,(int16_t)0x1406,(int16_t)0x8193,(int16_t)0xbd1f,(int16_t)0x92dd,(int16_t)0x8644,(int16_t)0xd872,
(int16_t)0x5a82,(int16_t)0xa57e,(int16_t)0x0000,(int16_t)0x8000,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x8000,(int16_t)0x0000,
(int16_t)0x5321,(int16_t)0x9eab,(int16_t)0xebfa,(int16_t)0x8193,(int16_t)0x92dd,(int16_t)0xbd1f,(int16_t)0x8644,(int16_t)0x278e,
(int16_t)0x4b3d,(int16_t)0x9872,(int16_t)0xd872,(int16_t)0x8644,(int16_t)0x8644,(int16_t)0xd872,(int16_t)0x9872,(int16_t)0x4b3d,
(int16_t)0x42e1,(int16_t)0x92dd,(int16_t)0xc5e4,(int16_t)0x8df3,(int16_t)0x8065,(int16_t)0xf5f5,(int16_t)0xb4c3,(int16_t)0x678e,
(int16_t)0x3a1c,(int16_t)0x8df3,(int16_t)0xb4c3,(int16_t)0x9872,(int16_t)0x8193,(int16_t)0x1406,(int16_t)0xd872,(int16_t)0x79bc,
(int16_t)0x30fc,(int16_t)0x89be,(int16_t)0xa57e,(int16_t)0xa57e,(int16_t)0x89be,(int16_t)0x30fc,(int16_t)0x0000,(int16_t)0x7fff
};
/****************** stage 2 radix 2 ******************/
ALIGN(8) static const int16_t __fft16_tw2[] = 
{
(int16_t)0x7fff,(int16_t)0x0000,
(int16_t)0x7642,(int16_t)0xcf04,
(int16_t)0x5a82,(int16_t)0xa57e,
(int16_t)0x30fc,(int16_t)0x89be,
(int16_t)0x0000,(int16_t)0x8000,
(int16_t)0xcf04,(int16_t)0x89be,
(int16_t)0xa57e,(int16_t)0xa57e,
(int16_t)0x89be,(int16_t)0xcf04
};
static const int tw_step_tab[] =
{
    1, 1, 0
}; 
static const cint16ptr_t tw_tab[] = 
{
    __fft16_tw1, __fft16_tw2, NULL
};

static const eFft_stage_type fft_cplx16x16_stg_tab[] = 
{
    fft_stage_first_DFT5,
    fft_stage_inner_DFT2,
    fft_stage_last_DFT8
};
static const eFft_stage_type ifft_cplx16x16_stg_tab[] =
{
    fft_stage_first_inv_DFT5,
    fft_stage_inner_DFT2,
    fft_stage_last_DFT8
};

static const fft_cplx_x16_descr_t __cfft_descr = 
{
    N, 
    NULL, 
    fft_cplx16x16_stg_tab, 
    NULL, 
    fft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     
static const fft_cplx_x16_descr_t __cifft_descr =
{
    N, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    NULL, 
    ifft_cplx16x16_stg_tab, 
    tw_step_tab,
    tw_tab
};     

/* Twiddles tables for fft_real32x16, ifft_real32x16, fft_real16x16, ifft_real16x16, N=160 */
ALIGN(8) static const int16_t __fft_real16_tw[] =
{
(int16_t)0x0000,(int16_t)0x7fff,
(int16_t)0x0506,(int16_t)0x7fe7,
(int16_t)0x0a0b,(int16_t)0x7f9b,
(int16_t)0x0f0b,(int16_t)0x7f1d,
(int16_t)0x1406,(int16_t)0x7e6d,
(int16_t)0x18f9,(int16_t)0x7d8a,
(int16_t)0x1de2,(int16_t)0x7c77,
(int16_t)0x22bf,(int16_t)0x7b32,
(int16_t)0x278e,(int16_t)0x79bc,
(int16_t)0x2c4e,(int16_t)0x7817,
(int16_t)0x30fc,(int16_t)0x7642,
(int16_t)0x3597,(int16_t)0x743e,
(int16_t)0x3a1c,(int16_t)0x720d,
(int16_t)0x3e8b,(int16_t)0x6fae,
(int16_t)0x42e1,(int16_t)0x6d23,
(int16_t)0x471d,(int16_t)0x6a6e,
(int16_t)0x4b3d,(int16_t)0x678e,
(int16_t)0x4f3e,(int16_t)0x6485,
(int16_t)0x5321,(int16_t)0x6155,
(int16_t)0x56e3,(int16_t)0x5dfe,
(int16_t)0x5a82,(int16_t)0x5a82,
(int16_t)0x5dfe,(int16_t)0x56e3,
(int16_t)0x6155,(int16_t)0x5321,
(int16_t)0x6485,(int16_t)0x4f3e,
(int16_t)0x678e,(int16_t)0x4b3d,
(int16_t)0x6a6e,(int16_t)0x471d,
(int16_t)0x6d23,(int16_t)0x42e1,
(int16_t)0x6fae,(int16_t)0x3e8b,
(int16_t)0x720d,(int16_t)0x3a1c,
(int16_t)0x743e,(int16_t)0x3597,
(int16_t)0x7642,(int16_t)0x30fc,
(int16_t)0x7817,(int16_t)0x2c4e,
(int16_t)0x79bc,(int16_t)0x278e,
(int16_t)0x7b32,(int16_t)0x22bf,
(int16_t)0x7c77,(int16_t)0x1de2,
(int16_t)0x7d8a,(int16_t)0x18f9,
(int16_t)0x7e6d,(int16_t)0x1406,
(int16_t)0x7f1d,(int16_t)0x0f0b,
(int16_t)0x7f9b,(int16_t)0x0a0b,
(int16_t)0x7fe7,(int16_t)0x0506
};

static const fft_real_x16_descr_t __rfft_descr =
{
    &__cfft_descr,
    __fft_real16_tw
};
static const fft_real_x16_descr_t __rifft_descr =
{
    &__cifft_descr,
    __fft_real16_tw
};
const fft_handle_t rnfft16_160 =  (const fft_handle_t)&__rfft_descr;
const fft_handle_t rinfft16_160 = (const fft_handle_t)&__rifft_descr;
